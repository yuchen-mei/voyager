#pragma once

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/stringize.hpp>

#include "AccelTypes.h"

#ifndef PE_LATENCY
#define PE_LATENCY 2
#endif

#define REPEAT_IC(x) BOOST_PP_REPEAT(IC_DIMENSION, x, 0)
#define REPEAT_OC(x) BOOST_PP_REPEAT(OC_DIMENSION, x, 0)

template <typename T, int Size>
SC_MODULE(InputSerializedSkewer) {
 private:
#define DECL_FIFOS(z, i, unused) sc_fifo<PEInput<T>> BOOST_PP_CAT(fifo, i);
  REPEAT_IC(DECL_FIFOS)
#undef DECL_FIFOS
  int dummy;

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<Pack1D<PEInput<T>, Size>> CCS_INIT_S1(din);
  Connections::Out<PEInput<T>> dout[Size];

#define FIFO_SIZE_INIT(z, i, unused) BOOST_PP_CAT(fifo, i)(i * PE_LATENCY + 1),

  SC_CTOR(InputSerializedSkewer) : REPEAT_IC(FIFO_SIZE_INIT) dummy(0) {
#undef FIFO_SIZE_INIT
    SC_THREAD(write_fifo);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

#define DECL_THREADS(z, i, unused)                                          \
  declare_thread_process(BOOST_PP_CAT(BOOST_PP_CAT(read_fifo, i), _handle), \
                         BOOST_PP_STRINGIZE(BOOST_PP_CAT(read_fifo, i)),     \
                                            SC_CURRENT_USER_MODULE,         \
                                            BOOST_PP_CAT(read_fifo, i));    \
  sensitive << clk.pos();                                                   \
  async_reset_signal_is(rstn, false);

    REPEAT_IC(DECL_THREADS)
#undef DECL_THREADS
  }

  void write_fifo() {
    din.Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<PEInput<T>, Size> input = din.Pop();

#define FIFO_WRITE(z, i, unused) BOOST_PP_CAT(fifo, i).write(input[i]);
      REPEAT_IC(FIFO_WRITE)
#undef FIFO_WRITE
    }
  }

#define DECL_FUNCS(z, i, unused)                                \
  void BOOST_PP_CAT(read_fifo, i)() {                           \
    dout[i].Reset();                                            \
    wait();                                                     \
    _Pragma("hls_pipeline_init_interval 1")                     \
        _Pragma("hls_pipeline_stall_mode flush") while (true) { \
      PEInput<T> pe_in = BOOST_PP_CAT(fifo, i).read();          \
      PEInput<T> pe_out;                                        \
      pe_out.data = pe_in.data;                                 \
      pe_out.swap_weights = pe_in.swap_weights;                 \
      dout[i].Push(pe_out);                                     \
    }                                                           \
  }

  REPEAT_IC(DECL_FUNCS)
#undef DECL_FUNCS
};

template <typename T, int Size>
SC_MODULE(WeightSerializedSkewer) {
 private:
#define DECL_FIFOS(z, i, unused) sc_fifo<PEWeight<T>> BOOST_PP_CAT(fifo, i);
  REPEAT_OC(DECL_FIFOS)
#undef DECL_FIFOS
  int dummy;

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<Pack1D<PEWeight<T>, Size>> CCS_INIT_S1(din);
  Connections::Out<PEWeight<T>> dout[Size];

#define FIFO_SIZE_INIT(z, i, unused) BOOST_PP_CAT(fifo, i)(i * 1 + 1),

  SC_CTOR(WeightSerializedSkewer) : REPEAT_OC(FIFO_SIZE_INIT) dummy(0) {
#undef FIFO_SIZE_INIT
    SC_THREAD(write_fifo);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

#define DECL_THREADS(z, i, unused)                                          \
  declare_thread_process(BOOST_PP_CAT(BOOST_PP_CAT(read_fifo, i), _handle), \
                         BOOST_PP_STRINGIZE(BOOST_PP_CAT(read_fifo, i)),     \
                                            SC_CURRENT_USER_MODULE,         \
                                            BOOST_PP_CAT(read_fifo, i));    \
  sensitive << clk.pos();                                                   \
  async_reset_signal_is(rstn, false);

    REPEAT_OC(DECL_THREADS)
#undef DECL_THREADS
  }

  void write_fifo() {
    din.Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<PEWeight<T>, Size> input = din.Pop();

#define FIFO_WRITE(z, i, unused) BOOST_PP_CAT(fifo, i).write(input[i]);
      REPEAT_OC(FIFO_WRITE)
#undef FIFO_WRITE
    }
  }

#define DECL_FUNCS(z, i, unused)                                \
  void BOOST_PP_CAT(read_fifo, i)() {                           \
    dout[i].Reset();                                            \
    wait();                                                     \
    _Pragma("hls_pipeline_init_interval 1")                     \
        _Pragma("hls_pipeline_stall_mode flush") while (true) { \
      PEWeight<T> pe_in = BOOST_PP_CAT(fifo, i).read();         \
      PEWeight<T> pe_out;                                       \
      pe_out.data = pe_in.data;                                 \
      pe_out.tag = pe_in.tag;                                   \
      dout[i].Push(pe_out);                                     \
    }                                                           \
  }

  REPEAT_OC(DECL_FUNCS)
#undef DECL_FUNCS
};

template <typename T, int Size>
SC_MODULE(DeserializedSkewer) {
 private:
#define DECL_FIFOS(z, i, unused) sc_fifo<T> BOOST_PP_CAT(fifo, i);
  REPEAT_OC(DECL_FIFOS)
#undef DECL_FIFOS

  int dummy;

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<T> din[Size];
  Connections::Out<Pack1D<T, Size>> CCS_INIT_S1(dout);

#define FIFO_SIZE_INIT(z, i, unused) \
  BOOST_PP_CAT(fifo, i)(1 * (OC_DIMENSION - i + 1)),

  SC_CTOR(DeserializedSkewer) : REPEAT_OC(FIFO_SIZE_INIT) dummy(0) {
#undef FIFO_SIZE_INIT
    SC_THREAD(read_fifo);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

#define DECL_THREADS(z, i, unused)                                           \
  declare_thread_process(BOOST_PP_CAT(BOOST_PP_CAT(write_fifo, i), _handle), \
                         BOOST_PP_STRINGIZE(BOOST_PP_CAT(write_fifo, i)),     \
                                            SC_CURRENT_USER_MODULE,          \
                                            BOOST_PP_CAT(write_fifo, i));    \
  sensitive << clk.pos();                                                    \
  async_reset_signal_is(rstn, false);

    REPEAT_OC(DECL_THREADS)
#undef DECL_THREADS
  }

  void read_fifo() {
    dout.Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<T, Size> output;

#define FIFO_READ(z, i, unused) output[i] = BOOST_PP_CAT(fifo, i).read();
      REPEAT_OC(FIFO_READ)
#undef FIFO_READ
      dout.Push(output);
    }
  }

#define DECL_FUNCS(z, i, unused)                                \
  void BOOST_PP_CAT(write_fifo, i)() {                          \
    din[i].Reset();                                             \
    wait();                                                     \
    _Pragma("hls_pipeline_init_interval 1")                     \
        _Pragma("hls_pipeline_stall_mode flush") while (true) { \
      BOOST_PP_CAT(fifo, i).write(din[i].Pop());                \
    }                                                           \
  }

  REPEAT_OC(DECL_FUNCS)
#undef DECL_FUNCS
};
