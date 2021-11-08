#pragma once

#include <mc_connections.h>
#include <systemc.h>

#include "AccelTypes.h"
#include "ProcessingElement.h"
#include "Skewer.h"

template <typename IDTYPE, typename WDTYPE, typename ODTYPE, int NROWS,
          int NCOLS>
SC_MODULE(SystolicArray) {
 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(inputs);
  Connections::In<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(psums);
  Connections::Out<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(outputs);
  Connections::In<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(swapWeights);

  // sc_in<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(inputs);
  // sc_in<bool> CCS_INIT_S1(inputsToggle);

  sc_in<Pack1D<WDTYPE, NCOLS> > CCS_INIT_S1(weights);
  sc_in<bool> CCS_INIT_S1(weightsToggle);

  // sc_in<Pack1D<ODTYPE, NROWS> > CCS_INIT_S1(psums);
  // sc_in<bool> CCS_INIT_S1(psumsValid);

  // sc_out<Pack1D<ODTYPE, NCOLS> > CCS_INIT_S1(outputs);
  // sc_out<bool> CCS_INIT_S1(outputsValid);

  // sc_in<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(swap_weights);

  sc_signal<IDTYPE> inputConnection[NROWS][NCOLS + 1];
  sc_signal<ODTYPE> psumConnection[NROWS + 1][NCOLS];
  sc_signal<WDTYPE> weightConnection[NROWS + 1][NCOLS];
  sc_signal<ac_int<1, false> > weightSwap[NROWS][NCOLS + 1];
  sc_signal<bool> weightPush;
  sc_signal<bool> enable;
  sc_signal<bool> peToggle;
  sc_signal<bool> validOutput[NROWS][NCOLS];

  sc_fifo<Pack1D<IDTYPE, NROWS> > outputFifo;

  // ValidSkewer<IDTYPE, NROWS> inputSkewer;
  // sc_signal<Pack1D<IDTYPE, NROWS> > inputSkewerDin;
  // sc_signal<Pack1D<IDTYPE, NROWS> > inputSkewerDout;

  // ValidSkewer<IDTYPE, NROWS> psumInSkewer;
  // sc_signal<Pack1D<IDTYPE, NROWS> > psumInSkewerDin;
  // sc_signal<Pack1D<IDTYPE, NROWS> > psumInSkewerDout;

  // ValidSkewer<IDTYPE, NROWS> psumOutSkewer;
  // sc_signal<Pack1D<IDTYPE, NROWS> > psumOutSkewerDin;
  // sc_signal<Pack1D<IDTYPE, NROWS> > psumOutSkewerDout;

  // ValidSkewer<ac_int<1, false>, NROWS> weightSwapSkewer;
  // sc_signal<Pack1D<ac_int<1, false>, NROWS> > weightSwapSkewerDin;
  // sc_signal<Pack1D<ac_int<1, false>, NROWS> > weightSwapSkewerDout;

  // Skewer<Pack1D<IDTYPE, NROWS>, IDTYPE> inputSkewer;
  // Skewer<Pack1D<ODTYPE, NROWS>, ODTYPE> psumInSkewer;
  // Skewer<Pack1D<ODTYPE, NROWS>, ODTYPE> psumOutSkewer;
  // Skewer<Pack1D<ac_int<1, false>, NROWS>, ac_int<1, false> >
  // weightSwapSkewer;

  SC_CTOR(SystolicArray) {
    ProcessingElement<IDTYPE, WDTYPE, ODTYPE> *pe[NROWS][NCOLS];
    for (int i = 0; i < NROWS; i++) {
      for (int j = 0; j < NCOLS; j++) {
        pe[i][j] = new ProcessingElement<IDTYPE, WDTYPE, ODTYPE>(
            sc_gen_unique_name("pe_inst"));
        pe[i][j]->clk(clk);
        pe[i][j]->rstn(rstn);
        pe[i][j]->input_in(inputConnection[i][j]);
        pe[i][j]->weight_in(weightConnection[i][j]);
        pe[i][j]->psum_in(psumConnection[i][j]);
        pe[i][j]->swap_weights_in(weightSwap[i][j]);
        pe[i][j]->swap_weights_out(weightSwap[i][j + 1]);
        pe[i][j]->input_out(inputConnection[i][j + 1]);
        pe[i][j]->weight_out(weightConnection[i + 1][j]);
        pe[i][j]->psum_out(psumConnection[i + 1][j]);
        pe[i][j]->valid_output(validOutput[i][j]);
        // pe[i][j]->enable(enable);
        pe[i][j]->toggle(peToggle);
        pe[i][j]->push_weights(weightPush);
      }
    }

    // inputSkewer.clk(clk);
    // inputSkewer.rstn(rstn);
    // inputSkewer.din(inputSkewerDin);
    // inputSkewer.din_valid(enable);
    // inputSkewer.dout(inputSkewerDout);
    // // inputSkewer.dout_valid(inputSkewerOutputValid);

    // psumInSkewer.clk(clk);
    // psumInSkewer.rstn(rstn);
    // psumInSkewer.din(psumInSkewerDin);
    // psumInSkewer.din_valid(enable);
    // psumInSkewer.dout(psumInSkewerDout);
    // // psumInSkewer.dout_valid(psumInSkewerOutputValid);

    // weightSwapSkewer.clk(clk);
    // weightSwapSkewer.rstn(rstn);
    // weightSwapSkewer.din(weightSwapSkewerDin);
    // weightSwapSkewer.din_valid(enable);
    // weightSwapSkewer.dout(weightSwapSkewerDout);
    // // weightSwapSkewer.dout_valid(weightSwapSkewerOutputValid);

    // psumOutSkewer.clk(clk);
    // psumOutSkewer.rstn(rstn);
    // psumOutSkewer.din(psumOutSkewerDin);
    // psumOutSkewer.din_valid(enable);
    // psumOutSkewer.dout(psumOutSkewerDout);
    // // psumOutSkewer.dout_valid(psumOutSkewerOutputValid);

    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

    SC_THREAD(collect_outputs);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

    SC_THREAD(send_outputs);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

    SC_THREAD(push_weights);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);
  }

  void push_weights() {
    for (int i = 0; i < NCOLS; i++) {
      weightConnection[0][i].write(0);
    }
    weightPush.write(false);
    bool oldToggle = false;
    wait();

#pragma hls_pipeline_init_interval 1
    while (true) {
      Pack1D<WDTYPE, NCOLS> arrayWeights = weights.read();
#pragma hls_unroll yes
      for (int i = 0; i < NCOLS; i++) {
        weightConnection[0][i].write(arrayWeights[i]);
      }

      weightPush.write(weightsToggle.read() != oldToggle);
      oldToggle = weightsToggle.read();

      wait();
    }
  }

#pragma design modulario < in>
  void run() {
    bool toggle = false;
    bool oldToggle = false;
    // outputs.write(Pack1D<ODTYPE, NCOLS>());
    // outputs.Reset();
    enable.write(false);
    peToggle.write(false);

    inputs.Reset();
    psums.Reset();
    swapWeights.Reset();

    for (int i = 0; i < NCOLS; i++) {
      psumConnection[0][i].write(0);
    }

    for (int i = 0; i < NROWS; i++) {
      inputConnection[i][0].write(0);
      weightSwap[i][0].write(0);
    }

    // outputsValid.write(0);

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<IDTYPE, NROWS> arrayInput = inputs.Pop();
      Pack1D<ODTYPE, NCOLS> arrayPsum = psums.Pop();
      Pack1D<ac_int<1, false>, NROWS> arraySwap = swapWeights.Pop();

      toggle = !toggle;
      peToggle.write(toggle);

#pragma hls_unroll yes
      for (int i = 0; i < NROWS; i++) {
        inputConnection[i][0].write(arrayInput[i]);
        weightSwap[i][0].write(arraySwap[i]);
      }

#pragma hls_unroll yes
      for (int i = 0; i < NCOLS; i++) {
        psumConnection[0][i].write(arrayPsum[i]);
      }

      // outputsValid.write(inputsToggle != oldToggle);
      // oldToggle = inputsToggle;
      oldToggle = toggle;
      wait();

      // oldToggle = toggle;
      // wait();
    }
  }

#pragma design modulario < in>
  void collect_outputs() {
    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      Pack1D<ODTYPE, NCOLS> finalOutputs;
#pragma hls_unroll yes
      for (int i = 0; i < NCOLS; i++) {
        finalOutputs[i] = psumConnection[NCOLS][NCOLS - 1 - i];
      }
      if (validOutput[NROWS - 1][NCOLS - 1]) {
        outputFifo.write(finalOutputs);
      }

      wait();
    }
  }

#pragma design modulario < in>
  void send_outputs() {
    outputs.Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      outputs.Push(outputFifo.read());
      wait();
    }
  }
};

// #pragma design modulario < out>
//   void run() {
//     bool oldToggle = false;
//     outputs.write(Pack1D<ODTYPE, NCOLS>());
//     enable.write(false);

//     for (int i = 0; i < NCOLS; i++) {
//       psumConnection[0][i].write(0);
//     }

//     for (int i = 0; i < NROWS; i++) {
//       inputConnection[i][0].write(0);
//       weightSwap[i][0].write(0);
//     }

//     outputsValid.write(0);

//     wait();

// #pragma hls_pipeline_init_interval 1
//     while (true) {
//       Pack1D<IDTYPE, NROWS> arrayInput = inputs.read();
//       Pack1D<ODTYPE, NCOLS> arrayPsum = psums.read();
//       Pack1D<ac_int<1, false>, NROWS> arraySwap = swap_weights.read();

//       //  = inputs.read();
//       //  = psums.read();
//       //  = swap_weights.read();

// #pragma hls_unroll yes
//       for (int i = 0; i < NROWS; i++) {
//         inputConnection[i][0].write(arrayInput[i]);
//         weightSwap[i][0].write(arraySwap[i]);
//       }

// #pragma hls_unroll yes
//       for (int i = 0; i < NCOLS; i++) {
//         psumConnection[0][i].write(arrayPsum[i]);
//       }

//       Pack1D<ODTYPE, NCOLS> finalOutputs;
// #pragma hls_unroll yes
//       for (int i = 0; i < NCOLS; i++) {
//         finalOutputs[i] = psumConnection[NCOLS][NCOLS - 1 - i];
//       }

//       //       if (inputsToggle != oldToggle) {
//       //         Pack1D<ODTYPE, NCOLS> flippedOutputs;
//       // #pragma hls_unroll yes
//       //         for (int i = 0; i < NCOLS; i++) {
//       //           flippedOutputs[i] = psumConnection[NCOLS][NCOLS - 1 - i];
//       //         }

//       //         Pack1D<ODTYPE, NCOLS> flippedOutputsSkewed;
//       //         psumOutSkewer.run(flippedOutputs, flippedOutputsSkewed);

//       // #pragma hls_unroll yes
//       //         for (int i = 0; i < NCOLS; i++) {
//       //           finalOutputs[i] = flippedOutputsSkewed[NCOLS - 1 - i];
//       //         }
//       //       }

//       enable.write(inputsToggle != oldToggle);
//       CCS_LOG("enable: " << enable);
//       outputs.write(finalOutputs);
//       // outputsValid.write(inputsToggle != oldToggle);
//       oldToggle = inputsToggle;

//       wait();
//     }
//   }
// };
