#pragma once

#include <boost/preprocessor/repetition/repeat.hpp>

#include "AccelTypes.h"
#include "ArchitectureParams.h"

#define REPEAT(x) BOOST_PP_REPEAT(DIMENSION, x, 0)

template <typename T, int NUM_REGS>
class Fifo {
 public:
  Fifo() {}

#pragma hls_design interface ccore
  void run(T input, T &output) {
  SHIFT:
    for (int i = NUM_REGS - 1; i >= 0; i--) {
      if (i == 0) {
        regs[i] = input;
      } else {
        regs[i] = regs[i - 1];
      }

      output = regs[NUM_REGS - 1];
    }
  }

 private:
  T regs[NUM_REGS];
};

template <typename DTYPE, int SIZE>
SC_MODULE(Skewer) {
 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<Pack1D<DTYPE, SIZE> > CCS_INIT_S1(din);
  Connections::Out<Pack1D<DTYPE, SIZE> > CCS_INIT_S1(dout);

#define INIT_FIFOS(z, i, unused) Fifo<DTYPE, i + 1> BOOST_PP_CAT(fifo_, i);
  REPEAT(INIT_FIFOS)
#undef INIT_FIFOS

  SC_CTOR(Skewer) {
    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);
  }

  void run() {
    dout.Reset();
    din.Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<DTYPE, SIZE> input = din.Pop();

      Pack1D<DTYPE, SIZE> fifoInput;
      fifoInput = input;

      Pack1D<DTYPE, SIZE> fifoOutput;
#define INPUT_FIFO_BODY(z, i, unused)                        \
  DTYPE BOOST_PP_CAT(fifo_output_, i);                       \
  DTYPE BOOST_PP_CAT(fifo_input_, i);                        \
  BOOST_PP_CAT(fifo_input_, i) = fifoInput[i];               \
  BOOST_PP_CAT(fifo_, i).run(BOOST_PP_CAT(fifo_input_, i),   \
                             BOOST_PP_CAT(fifo_output_, i)); \
  fifoOutput[i] = BOOST_PP_CAT(fifo_output_, i);

      REPEAT(INPUT_FIFO_BODY)
#undef INPUT_FIFO_BODY

      Pack1D<DTYPE, SIZE> output;
      output = fifoOutput;

      dout.Push(output);
    }
  }
};
