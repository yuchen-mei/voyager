#pragma once

#include <mc_connections.h>
#include <systemc.h>

#include "AccelTypes.h"
#include "ProcessingElement.h"
#include "Tieoff.h"
#include "mc_scverify.h"

template <typename Input, typename Weight, typename Psum, int NCols>
SC_MODULE(SystolicArrayRow) {
 private:
  Connections::Combinational<PEInput<Input>> input_wires[NCols];

#ifdef __SYNTHESIS__
  ProcessingElement<Input, Weight, Psum> pe[NCols];
  Tieoff<PEInput<Input>> input_tieoff;
#else
  ProcessingElement<Input, Weight, Psum> *pe_ptr[NCols];
  Tieoff<PEInput<Input>> *input_tieoff_ptr;
#endif

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<PEInput<Input>> input;
  Connections::In<PEWeight<Weight>> weights_in[NCols];
  Connections::Out<PEWeight<Weight>> weights_out[NCols];
  Connections::In<Psum> psums_in[NCols];
  Connections::Out<Psum> psums_out[NCols];

#ifdef __SYNTHESIS__
  SC_HAS_PROCESS(SystolicArrayRow);
  SystolicArrayRow()
      : sc_module(sc_gen_unique_name("SystolicArrayRow"))
#else
  SC_CTOR(SystolicArrayRow)
#endif
  {
    for (int j = 0; j < NCols; j++) {
#ifdef __SYNTHESIS__
      pe[j].clk(clk);
      pe[j].rstn(rstn);

      if (j == 0) {
        pe[j].input_in(input);
      } else {
        pe[j].input_in(input_wires[j - 1]);
      }
      pe[j].input_out(input_wires[j]);

      pe[j].weight_in(weights_in[j]);
      pe[j].weight_out(weights_out[j]);

      pe[j].psum_in(psums_in[j]);
      pe[j].psum_out(psums_out[j]);
#else
      pe_ptr[j] =
          new ProcessingElement<Input, Weight, Psum>(sc_gen_unique_name("pe"));

      pe_ptr[j]->clk(clk);
      pe_ptr[j]->rstn(rstn);

      if (j == 0) {
        pe_ptr[j]->input_in(input);
      } else {
        pe_ptr[j]->input_in(input_wires[j - 1]);
      }
      pe_ptr[j]->input_out(input_wires[j]);

      pe_ptr[j]->weight_in(weights_in[j]);
      pe_ptr[j]->weight_out(weights_out[j]);

      pe_ptr[j]->psum_in(psums_in[j]);
      pe_ptr[j]->psum_out(psums_out[j]);
#endif
    }

    // tie off unused connection for last input
#ifdef __SYNTHESIS__
#ifdef CONNECTIONS_FAST_SIM
    input_tieoff.clk(clk);
    input_tieoff.rstn(rstn);
#endif
    input_tieoff.in(input_wires[NCols - 1]);
#else
    input_tieoff_ptr =
        new Tieoff<PEInput<Input>>(sc_gen_unique_name("input_tieoff"));
#ifdef CONNECTIONS_FAST_SIM
    input_tieoff_ptr->clk(clk);
    input_tieoff_ptr->rstn(rstn);
#endif
    input_tieoff_ptr->in(input_wires[NCols - 1]);
#endif
  };
};

template <typename Input, typename Weight, typename Psum, int NRows, int NCols>
SC_MODULE(SystolicArray) {
 private:
  Connections::Combinational<Psum> psum_wires[NRows][NCols];
  Connections::Combinational<PEWeight<Weight>> weight_wires[NRows][NCols];

// To speed up HLS synthesis, we instantiate arrays of SC_MODULE on
// the stack. However, for simulation, we will run into stack overflow issues,
// so we have to instantiate them on the heap.
#ifdef __SYNTHESIS__
  SystolicArrayRow<Input, Weight, Psum, NCols> sa_rows[NRows];
  Tieoff<PEWeight<Input>> weight_wires_tieoff[NCols];
  ZeroTieoff<Psum> psum_wires_tieoff[NCols];
#else
  SystolicArrayRow<Input, Weight, Psum, NCols> *sa_rows_ptr[NRows];
  Tieoff<PEWeight<Input>> *weight_wires_tieoff_pt[NCols];
  ZeroTieoff<Psum> *psum_wires_tieoff_ptr[NCols];
#endif

 public:
  sc_in<bool> CCS_INIT_S1(clk);
  sc_in<bool> CCS_INIT_S1(rstn);

  Connections::In<PEInput<Input>> inputs[NRows];
  Connections::In<PEWeight<Weight>> weights[NCols];
  Connections::Out<Psum> outputs[NCols];

  SC_CTOR(SystolicArray) {
    for (int i = 0; i < NRows; i++) {
#ifdef __SYNTHESIS__
      sa_rows[i].clk(clk);
      sa_rows[i].rstn(rstn);
      sa_rows[i].input(inputs[i]);

      for (int j = 0; j < NCols; j++) {
        if (i == 0) {
          sa_rows[i].weights_in[j](weights[j]);
        } else {
          sa_rows[i].weights_in[j](weight_wires[i - 1][j]);
        }

        sa_rows[i].weights_out[j](weight_wires[i][j]);

        sa_rows[i].psums_in[j](psum_wires[i][j]);

        if (i == NRows - 1) {
          sa_rows[i].psums_out[j](outputs[j]);
        } else {
          sa_rows[i].psums_out[j](psum_wires[i + 1][j]);
        }
      }
#else
      sa_rows_ptr[i] = new SystolicArrayRow<Input, Weight, Psum, NCols>(
          sc_gen_unique_name("row"));

      sa_rows_ptr[i]->clk(clk);
      sa_rows_ptr[i]->rstn(rstn);
      sa_rows_ptr[i]->input(inputs[i]);

      for (int j = 0; j < NCols; j++) {
        if (i == 0) {
          sa_rows_ptr[i]->weights_in[j](weights[j]);
        } else {
          sa_rows_ptr[i]->weights_in[j](weight_wires[i - 1][j]);
        }

        sa_rows_ptr[i]->weights_out[j](weight_wires[i][j]);

        sa_rows_ptr[i]->psums_in[j](psum_wires[i][j]);

        if (i == NRows - 1) {
          sa_rows_ptr[i]->psums_out[j](outputs[j]);
        } else {
          sa_rows_ptr[i]->psums_out[j](psum_wires[i + 1][j]);
        }
      }
#endif
    }

    // Tie the first row of psum wires to zero
    for (int i = 0; i < NCols; i++) {
#ifdef __SYNTHESIS__
#ifdef CONNECTIONS_FAST_SIM
      psum_wires_tieoff[i].clk(clk);
      psum_wires_tieoff[i].rstn(rstn);
#endif
      psum_wires_tieoff[i].out(psum_wires[0][i]);
#else
      psum_wires_tieoff_ptr[i] =
          new ZeroTieoff<Psum>(sc_gen_unique_name("tieoff"));
#ifdef CONNECTIONS_FAST_SIM
      // we need to connect clock and reset if using fast sim
      psum_wires_tieoff_ptr[i]->clk(clk);
      psum_wires_tieoff_ptr[i]->rstn(rstn);
#endif
      psum_wires_tieoff_ptr[i]->out(psum_wires[0][i]);
#endif
    }

    // Tie the last row of weight wires
    for (int i = 0; i < NCols; i++) {
#ifdef __SYNTHESIS__
#ifdef CONNECTIONS_FAST_SIM
      weight_wires_tieoff[i].clk(clk);
      weight_wires_tieoff[i].rstn(rstn);
#endif
      weight_wires_tieoff[i].in(weight_wires[NRows - 1][i]);
#else
      weight_wires_tieoff_pt[i] =
          new Tieoff<PEWeight<Input>>(sc_gen_unique_name("tieoff"));
      weight_wires_tieoff_pt[i]->in(weight_wires[NRows - 1][i]);
#ifdef CONNECTIONS_FAST_SIM
      // we need to connect clock and reset if using fast sim
      weight_wires_tieoff_pt[i]->clk(clk);
      weight_wires_tieoff_pt[i]->rstn(rstn);
#endif
#endif
    }
  }
};
