#pragma once

#include <mc_connections.h>
// #include <nvhls_connections.h>
#include <systemc.h>

#include "SystolicArray.h"

template <typename IDTYPE, typename WDTYPE, typename ODTYPE, int NROWS,
          int NCOLS, int BUFFER_SIZE>
SC_MODULE(MatrixProcessor) {
 private:
  Connections::SyncChannel CCS_INIT_S1(weightSync);
  sc_signal<bool> CCS_INIT_S1(waitForWeight);
  sc_signal<bool> CCS_INIT_S1(weightReady);
  sc_signal<bool> CCS_INIT_S1(weightFill);

  Pack1D<ODTYPE, NCOLS> accumulation_buffer[BUFFER_SIZE];

  sc_signal<bool> CCS_INIT_S1(paramsReady);
  // Connections::SyncChannel CCS_INIT_S1(outputsDone);

  sc_signal<bool> CCS_INIT_S1(toggleOut);
  sc_signal<bool> CCS_INIT_S1(outputsValid);

  // // ToggleSkewer<IDTYPE, NROWS> CCS_INIT_S1(inputSkewer);
  // sc_signal<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(inputSkewerOutput);
  // // sc_signal<bool> CCS_INIT_S1(inputSkewerOutputValid);

  // // ToggleSkewer<ODTYPE, NROWS> CCS_INIT_S1(psumInSkewer);
  // sc_signal<Pack1D<IDTYPE, NCOLS> > CCS_INIT_S1(psumInSkewerOutput);
  // // sc_signal<bool> CCS_INIT_S1(psumInSkewerOutputValid);

  // // ToggleSkewer<ac_int<1, false>, NROWS> CCS_INIT_S1(weightSwapSkewer);
  // sc_signal<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(
  //     weightSwapSkewerOutput);
  // // sc_signal<bool> CCS_INIT_S1(weightSwapSkewerOutputValid);

  // // ValidSkewer<ODTYPE, NROWS, true> CCS_INIT_S1(psumOutSkewer);
  // sc_signal<Pack1D<IDTYPE, NCOLS> > CCS_INIT_S1(psumOutSkewerOutput);
  // // sc_signal<bool> CCS_INIT_S1(psumOutSkewerOutputValid);

  Skewer<IDTYPE, NROWS> CCS_INIT_S1(inputSkewer);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      inputSkewerDin);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      inputSkewerDout);

  Skewer<IDTYPE, NROWS> CCS_INIT_S1(psumInSkewer);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      psumInSkewerDin);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      psumInSkewerDout);

  Skewer<IDTYPE, NROWS> CCS_INIT_S1(psumOutSkewer);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      psumOutSkewerDin);
  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      psumOutSkewerDout);

  Skewer<ac_int<1, false>, NROWS> CCS_INIT_S1(weightSwapSkewer);
  Connections::Combinational<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(
      weightSwapSkewerDin);
  Connections::Combinational<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(
      weightSwapSkewerDout);

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(inputsChannel);
  Connections::In<Pack1D<WDTYPE, NROWS> > CCS_INIT_S1(weightsChannel);
  Connections::Out<Pack1D<ODTYPE, NROWS> > CCS_INIT_S1(outputsChannel);

  Connections::In<Params> CCS_INIT_S1(paramsIn);

  SystolicArray<IDTYPE, WDTYPE, ODTYPE, NROWS, NCOLS> CCS_INIT_S1(
      systolicArray);

  Connections::Combinational<Pack1D<IDTYPE, NROWS> > CCS_INIT_S1(
      inputsToSystolicArray);
  Connections::Combinational<Pack1D<ODTYPE, NCOLS> > CCS_INIT_S1(
      psumsToSystolicArray);
  Connections::Combinational<Pack1D<ODTYPE, NCOLS> > CCS_INIT_S1(
      outputsFromSystolicArray);
  Connections::Combinational<Pack1D<ac_int<1, false>, NROWS> > CCS_INIT_S1(
      weightSwapToSystolicArray);

  // Connections::Buffer<Pack1D<ODTYPE, NCOLS>, 2> CCS_INIT_S1(outputsBuffer);
  // Connections::Combinational<Pack1D<ODTYPE, NCOLS> > CCS_INIT_S1(
  //     outputsFromBuffer);

  sc_signal<Pack1D<WDTYPE, NCOLS> > CCS_INIT_S1(weightsToSystolicArray);
  sc_signal<bool> CCS_INIT_S1(weightsToggle);

  sc_signal<bool> CCS_INIT_S1(outputsDone);

  SC_CTOR(MatrixProcessor) {
    systolicArray.clk(clk);
    systolicArray.rstn(rstn);

    systolicArray.inputs(inputsToSystolicArray);
    systolicArray.weights(weightsToSystolicArray);
    systolicArray.weightsToggle(weightsToggle);
    systolicArray.psums(psumsToSystolicArray);
    systolicArray.outputs(outputsFromSystolicArray);
    systolicArray.swapWeights(weightSwapToSystolicArray);

    // outputsBuffer.clk(clk);
    // outputsBuffer.rst(rstn);
    // outputsBuffer.enq(psumOutSkewerDout);
    // outputsBuffer.deq(outputsFromBuffer);

    // inputSkewer.clk(clk);
    // inputSkewer.rstn(rstn);
    // inputSkewer.din(inputsToSkewer);
    // inputSkewer.din_toggle(toggleOut);
    // inputSkewer.dout(inputSkewerOutput);
    // inputSkewer.dout_valid(inputSkewerOutputValid);

    // psumInSkewer.clk(clk);
    // psumInSkewer.rstn(rstn);
    // psumInSkewer.din(psumsToSkewer);
    // psumInSkewer.din_toggle(toggleOut);
    // psumInSkewer.dout(psumInSkewerOutput);
    // psumInSkewer.dout_valid(psumInSkewerOutputValid);

    // psumOutSkewer.clk(clk);
    // psumOutSkewer.rstn(rstn);
    // psumOutSkewer.din(outputsToSkewer);
    // psumOutSkewer.din_valid(outputsValid);
    // psumOutSkewer.dout(psumOutSkewerOutput);
    // psumOutSkewer.dout_valid(psumOutSkewerOutputValid);

    // weightSwapSkewer.clk(clk);
    // weightSwapSkewer.rstn(rstn);
    // weightSwapSkewer.din(weightSwapToSkewer);
    // weightSwapSkewer.din_toggle(toggleOut);
    // weightSwapSkewer.dout(weightSwapSkewerOutput);
    // weightSwapSkewer.dout_valid(weightSwapSkewerOutputValid);

    inputSkewer.clk(clk);
    inputSkewer.rstn(rstn);
    inputSkewer.din(inputSkewerDin);
    inputSkewer.dout(inputsToSystolicArray);
    // inputSkewer.dout_valid(inputSkewerOutputValid);

    psumInSkewer.clk(clk);
    psumInSkewer.rstn(rstn);
    psumInSkewer.din(psumInSkewerDin);
    psumInSkewer.dout(psumsToSystolicArray);
    // psumInSkewer.dout_valid(psumInSkewerOutputValid);

    weightSwapSkewer.clk(clk);
    weightSwapSkewer.rstn(rstn);
    weightSwapSkewer.din(weightSwapSkewerDin);
    weightSwapSkewer.dout(weightSwapToSystolicArray);
    // weightSwapSkewer.dout_valid(weightSwapSkewerOutputValid);

    psumOutSkewer.clk(clk);
    psumOutSkewer.rstn(rstn);
    psumOutSkewer.din(outputsFromSystolicArray);
    psumOutSkewer.dout(psumOutSkewerDout);
    // psumOutSkewer.dout_valid(psumOutSkewerOutputValid);

    SC_THREAD(process_weights);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);

    SC_THREAD(run);
    sensitive << clk.pos();
    async_reset_signal_is(rstn, false);
  }

  void process_weights() {
    weightsChannel.Reset();
    weightsToSystolicArray.write(Pack1D<WDTYPE, NCOLS>());
    weightReady.write(false);
    weightSync.ResetRead();
    weightsToggle.write(false);

    bool toggle = false;
    bool weightFillToggle = false;

    wait();

    while (true) {
#pragma hls_pipeline_init_interval 1
      for (int weight_count = 0; weight_count < NROWS; weight_count++) {
        Pack1D<WDTYPE, NCOLS> arrayWeights = weightsChannel.Pop();
        std::cout << "Weights: " << arrayWeights << std::endl;
        weightsToSystolicArray.write(arrayWeights);
        toggle = !toggle;
        weightsToggle.write(toggle);
        wait();
      }

      weightReady.write(true);

      while (weightFillToggle == weightFill.read()) {
        wait();
      }
      weightFillToggle = weightFill.read();
      weightReady.write(false);

      // wait for swap signal to propagate throughout the entire array
      for (int i = 0; i < NROWS + NCOLS; i++) {
        wait();
      }
    }
  }

  //   void run() {
  //     paramsIn.Reset();

  //     inputSkewerDin.ResetWrite();
  //     toggleOut.write(false);
  //     inputsChannel.Reset();
  //     weightSync.ResetWrite();
  //     psumInSkewerDin.ResetWrite();
  //     outputsChannel.Reset();
  //     weightFill.write(false);
  //     weightSwapSkewerDin.ResetWrite();

  //     bool toggle = false;
  //     bool weightFillToggle = false;

  //     wait();

  //     while (true) {
  //       Params params = paramsIn.Pop();

  //       int loop_counters[2][3];
  //       int loop_counters_out[2][3];

  // #pragma hls_unroll yes
  //       for (int i = 0; i < 2; i++) {
  // #pragma hls_unroll yes
  //         for (int j = 0; j < 3; j++) {
  //           loop_counters[i][j] = 0;
  //           loop_counters_out[i][j] = 0;
  //         }
  //       }

  //       int totalOps = 1;
  // #pragma hls_unroll yes
  //       for (int i = 0; i < 2; i++) {
  // #pragma hls_unroll yes
  //         for (int j = 0; j < 3; j++) {
  //           totalOps *= params.loops[i][j];
  //         }
  //       }

  // #pragma hls_pipeline_init_interval 1
  //       for (int step = 0; step < totalOps + (NROWS - 1) + (NCOLS - 1) + 0;
  //            step++) {
  //         CCS_LOG(step);
  //         Pack1D<ac_int<1, false>, NROWS> weightSwap;
  //         if (loop_counters[1][2] == 0 && step < totalOps) {
  //           while (!weightReady) {
  //             wait();
  //           }
  //           CCS_LOG("weight ready");

  // #pragma hls_unroll yes
  //           for (int i = 0; i < NROWS; i++) {
  //             weightSwap.value[i] = true;
  //           }

  //           weightFillToggle = !weightFillToggle;
  //         } else {
  // #pragma hls_unroll yes
  //           for (int i = 0; i < NROWS; i++) {
  //             weightSwap.value[i] = false;
  //           }
  //         }

  //         Pack1D<IDTYPE, NROWS> inputs;
  //         if (step < totalOps) {
  //           inputs = inputsChannel.Pop();
  //           CCS_LOG("input: " << inputs);
  //         }
  //         toggle = !toggle;
  //         toggleOut.write(toggle);

  //         inputSkewerDin.Push(inputs);
  //         weightSwapSkewerDin.Push(weightSwap);

  //         // Pack1D<IDTYPE, NROWS> skewedInputs;
  //         // inputSkewer.run(inputs, skewedInputs);
  //         // inputSkewerOutput.write(skewedInputs);

  //         // Pack1D<ac_int<1, false>, NROWS> skewedWeightSwap;
  //         // weightSwapSkewer.run(weightSwap, skewedWeightSwap);
  //         // weightSwapSkewerOutput.write(skewedWeightSwap);

  //         weightFill.write(weightFillToggle);

  //         Pack1D<ODTYPE, NCOLS> psum;
  // #pragma hls_unroll yes
  //         for (int i = 0; i < NCOLS; i++) {
  //           psum.value[i] = 0;
  //         }

  //         if (loop_counters[1][params.reductionLoopIndex[1]] != 0 &&
  //             step < totalOps) {
  //           int readAddress = loop_counters[0][params.weightLoopIndex[0]] *
  //                                 params.loops[1][params.inputLoopIndex[1]] +
  //                             loop_counters[1][params.inputLoopIndex[1]];
  // #ifdef __SYNTHESIS__
  //         READ_ACC_BUFFER:
  // #endif
  //           psum = accumulation_buffer[readAddress];
  //         }

  //         psumInSkewerDin.Push(psum);

  //         // Pack1D<ODTYPE, NCOLS> psumSkewed;
  //         // psumInSkewer.run(psum, psumSkewed);
  //         // psumInSkewerOutput.write(psumSkewed);
  //         // psumsToSkewer.write(psum);

  //         // #ifndef __SYNTHESIS__
  //         // wait();
  //         // #endif

  //         Pack1D<ODTYPE, NCOLS> outputs;
  //         if (step >= 2) {
  //           psumOutSkewerDout.PopNB(outputs);
  //           // outputs = psumOutSkewerDout.Pop();
  //         }

  //         Pack1D<ODTYPE, NCOLS> flippedOutputs;

  // #pragma hls_unroll yes
  //         for (int i = 0; i < NCOLS; i++) {
  //           flippedOutputs[i] = outputs[NCOLS - 1 - i];
  //         }

  //         //         Pack1D<ODTYPE, NCOLS> flippedOutputsSkewed;
  //         //         psumOutSkewer.run(flippedOutputs, flippedOutputsSkewed);

  //         //         Pack1D<ODTYPE, NCOLS> finalOutputs;
  //         // #pragma hls_unroll yes
  //         //         for (int i = 0; i < NCOLS; i++) {
  //         //           finalOutputs[i] = flippedOutputsSkewed[NCOLS - 1 - i];
  //         //         }

  //         if (step >= (NCOLS - 1) + (NROWS - 1) + 0) {
  //           if (loop_counters_out[1][params.reductionLoopIndex[1]] ==
  //               params.loops[1][params.reductionLoopIndex[1]] - 1) {
  //             outputsChannel.Push(flippedOutputs);
  //             std::cout << "Output: " << flippedOutputs << std::endl;
  //           } else {
  //             int writeAddress =
  //             loop_counters_out[0][params.weightLoopIndex[0]] *
  //                                    params.loops[1][params.inputLoopIndex[1]]
  //                                    +
  //                                loop_counters_out[1][params.inputLoopIndex[1]];

  // #ifdef __SYNTHESIS__
  //           WRITE_ACC_BUFFER:
  // #endif
  //             accumulation_buffer[writeAddress] = flippedOutputs;
  //           }
  //         }

  //         loop_counters[1][2]++;
  // #pragma hls_unroll yes
  //         for (int i = 1; i >= 0; i--) {
  // #pragma hls_unroll yes
  //           for (int j = 2; j >= 0; j--) {
  //             if (loop_counters[i][j] == params.loops[i][j]) {
  //               loop_counters[i][j] = 0;
  //               if (j > 0) {
  //                 loop_counters[i][j - 1]++;
  //               } else {
  //                 if (i > 0) {
  //                   loop_counters[i - 1][2]++;
  //                 }
  //               }
  //             }
  //           }
  //         }

  //         if (step >= (NCOLS - 1) + (NROWS - 1) + 0) {
  //           loop_counters_out[1][2]++;
  // #pragma hls_unroll yes
  //           for (int i = 1; i >= 0; i--) {
  // #pragma hls_unroll yes
  //             for (int j = 2; j >= 0; j--) {
  //               if (loop_counters_out[i][j] == params.loops[i][j]) {
  //                 loop_counters_out[i][j] = 0;
  //                 if (j > 0) {
  //                   loop_counters_out[i][j - 1]++;
  //                 } else {
  //                   if (i > 0) {
  //                     loop_counters_out[i - 1][2]++;
  //                   }
  //                 }
  //               }
  //             }
  //           }
  //         }

  //         // wait();
  //       }
  //     }
  //   }

  //   void sendInputs() {
  //     inputParams.Reset();
  //     inputSkewerDin.ResetWrite();

  //     wait();

  //     while (true) {
  //       Params params = inputParams.Pop();
  //       int totalOps = 1;
  // #pragma hls_unroll yes
  //       for (int i = 0; i < 2; i++) {
  // #pragma hls_unroll yes
  //         for (int j = 0; j < 3; j++) {
  //           totalOps *= params.loops[i][j];
  //         }
  //       }

  //       int step = 0;

  //       while (!outputsDone) {

  //         step++;
  //       }
  //     }
  //   }

  void run() {
    paramsIn.Reset();

    inputSkewerDin.ResetWrite();
    inputSkewerDout.ResetRead();
    toggleOut.write(false);
    inputsChannel.Reset();
    weightSync.ResetWrite();
    psumInSkewerDin.ResetWrite();
    psumInSkewerDout.ResetRead();
    outputsChannel.Reset();
    weightFill.write(false);
    weightSwapSkewerDin.ResetWrite();
    weightSwapSkewerDout.ResetRead();

    bool toggle = false;
    bool weightFillToggle = false;

    wait();

    while (true) {
      Params params = paramsIn.Pop();

      int loop_counters[2][3];
      int loop_counters_out[2][3];

#pragma hls_unroll yes
      for (int i = 0; i < 2; i++) {
#pragma hls_unroll yes
        for (int j = 0; j < 3; j++) {
          loop_counters[i][j] = 0;
          loop_counters_out[i][j] = 0;
        }
      }

      int totalOps = 1;
#pragma hls_unroll yes
      for (int i = 0; i < 2; i++) {
#pragma hls_unroll yes
        for (int j = 0; j < 3; j++) {
          totalOps *= params.loops[i][j];
        }
      }

      int step = 0;
      int outputCounter = 0;

// TODO: figure out why this is the case
#ifdef __SYNTHESIS__
      const int latency = 7;
#else
      const int latency = 2;
#endif

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
      while (step < totalOps + (NROWS - 1) + (NCOLS - 1) + latency) {
        // while (!outputsDone) {
        CCS_LOG("step " << step << " out of " << totalOps);
        CCS_LOG("inner: " << loop_counters[1][2]);

        Pack1D<ac_int<1, false>, NROWS> weightSwap;
        if (loop_counters[1][2] == 0 && step < totalOps) {
          CCS_LOG("*****");
          while (!weightReady) {
            wait();
          }
          CCS_LOG("weight ready");

#pragma hls_unroll yes
          for (int i = 0; i < NROWS; i++) {
            weightSwap.value[i] = true;
          }

          weightFillToggle = !weightFillToggle;
        } else {
          CCS_LOG("skip");
#pragma hls_unroll yes
          for (int i = 0; i < NROWS; i++) {
            weightSwap.value[i] = false;
          }
        }

        Pack1D<IDTYPE, NROWS> inputs;
        if (step < totalOps) {
          inputs = inputsChannel.Pop();
          CCS_LOG("input: " << inputs);
        }
        toggle = !toggle;
        toggleOut.write(toggle);

        inputSkewerDin.Push(inputs);
        weightSwapSkewerDin.Push(weightSwap);

        weightFill.write(weightFillToggle);

        Pack1D<ODTYPE, NCOLS> psum;
#pragma hls_unroll yes
        for (int i = 0; i < NCOLS; i++) {
          psum.value[i] = 0;
        }

        if (loop_counters[1][params.reductionLoopIndex[1]] != 0 &&
            step < totalOps) {
          int readAddress = loop_counters[1][params.weightLoopIndex[1]] *
                                params.loops[1][params.inputLoopIndex[1]] +
                            loop_counters[1][params.inputLoopIndex[1]];
#ifdef __SYNTHESIS__
        READ_ACC_BUFFER:
#endif
          psum = accumulation_buffer[readAddress];
          CCS_LOG("readAddress: " << readAddress << " psum " << psum);
        }

        psumInSkewerDin.Push(psum);

        // Pack1D<ODTYPE, NCOLS> psumSkewed;
        // psumInSkewer.run(psum, psumSkewed);
        // psumInSkewerOutput.write(psumSkewed);
        // psumsToSkewer.write(psum);

        // #ifndef __SYNTHESIS__
        // if (step == 0 || step == 1) wait();
        // #endif

        wait();

        Pack1D<ODTYPE, NCOLS> outputs;
        psumOutSkewerDout.PopNB(outputs);
        Pack1D<ODTYPE, NCOLS> flippedOutputs;

#pragma hls_unroll yes
        for (int i = 0; i < NCOLS; i++) {
          flippedOutputs[i] = outputs[NCOLS - 1 - i];
        }

        //         Pack1D<ODTYPE, NCOLS> flippedOutputsSkewed;
        //         psumOutSkewer.run(flippedOutputs, flippedOutputsSkewed);

        //         Pack1D<ODTYPE, NCOLS> finalOutputs;
        // #pragma hls_unroll yes
        //         for (int i = 0; i < NCOLS; i++) {
        //           finalOutputs[i] = flippedOutputsSkewed[NCOLS - 1 - i];
        //         }

        if (step >= (NCOLS - 1) + (NROWS - 1) + latency) {
          if (loop_counters_out[1][params.reductionLoopIndex[1]] ==
              params.loops[1][params.reductionLoopIndex[1]] - 1) {
            outputsChannel.Push(flippedOutputs);
            std::cout << "Output: " << flippedOutputs << std::endl;
          } else {
            int writeAddress = loop_counters_out[1][params.weightLoopIndex[1]] *
                                   params.loops[1][params.inputLoopIndex[1]] +
                               loop_counters_out[1][params.inputLoopIndex[1]];

#ifdef __SYNTHESIS__
          WRITE_ACC_BUFFER:
#endif
            accumulation_buffer[writeAddress] = flippedOutputs;
            CCS_LOG("writeAddress: " << writeAddress << " val "
                                     << flippedOutputs);
          }
        }

        if (step >= (NCOLS - 1) + (NROWS - 1) + latency) {
          loop_counters_out[1][2]++;
#pragma hls_unroll yes
          for (int i = 1; i >= 0; i--) {
#pragma hls_unroll yes
            for (int j = 2; j >= 0; j--) {
              if (loop_counters_out[i][j] == params.loops[i][j]) {
                loop_counters_out[i][j] = 0;
                if (j > 0) {
                  loop_counters_out[i][j - 1]++;
                } else {
                  if (i > 0) {
                    loop_counters_out[i - 1][2]++;
                  }
                }
              }
            }
          }
        }

        step++;
        loop_counters[1][2]++;
#pragma hls_unroll yes
        for (int i = 1; i >= 0; i--) {
#pragma hls_unroll yes
          for (int j = 2; j >= 0; j--) {
            if (loop_counters[i][j] == params.loops[i][j]) {
              loop_counters[i][j] = 0;
              if (j > 0) {
                loop_counters[i][j - 1]++;
              } else {
                if (i > 0) {
                  loop_counters[i - 1][2]++;
                }
              }
            }
          }
        }

        // wait();
      }
    }
  }

  // void read_params() {
  //   paramsIn.Reset();
  //   psumParams.ResetWrite();
  //   inputParams.ResetWrite();

  //   wait();

  //   while (true) {
  //     Params params = paramsIn.Pop();

  //     psumParams.Push(params);
  //     inputParams.Push(params);
  //   }
  // }
};
