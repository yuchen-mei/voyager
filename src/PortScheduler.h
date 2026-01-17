#pragma once

#include <systemc.h>

#include "AccelTypes.h"
#include "ArchitectureParams.h"

template <int N_MASTERS, int N_PORTS>
SC_MODULE(PortScheduler) {
  struct MasterState {
    int port_id;
    ac_int<16, false> num_requests = 0;
  };

  struct RouterState {
    int master_id;
    ac_int<16, false> num_responses = 0;
  }

  struct RouterParams {
    MasterState masters[N_MASTERS];
    RouterState ports[N_PORTS];
  };

  Connections::In<RouterParams> scheduler_params;
  Connections::Combinational<RouterParams> router_params;

  // From logical fetch engines
  Connections::In<MemoryRequest> master_req[N_MASTERS];
  Connections::Out<ac_int<OC_PORT_WIDTH, false>> master_resp[N_MASTERS];

#define DECLARE_ROUTER(m, p)                              \
  Connections::Combinational<MemoryRequest> BOOST_PP_CAT( \
      router_, BOOST_PP_CAT(m, BOOST_PP_CAT(_, p)));

#define DECLARE_ROUTER_PORT(z, p, m) DECLARE_ROUTER(m, p)

#define DECLARE_ROUTER_MASTER(z, m, unused) \
  BOOST_PP_REPEAT(N_PORTS, DECLARE_ROUTER_PORT, m)

  BOOST_PP_REPEAT(N_MASTERS, DECLARE_ROUTER_MASTER, ~)

  // From physical memory ports
  Connections::Out<MemoryRequest> port_req[N_PORTS];
  Connections::In<ac_int<OC_PORT_WIDTH, false>> port_resp[N_PORTS];

  sc_in<bool> clk;
  sc_in<bool> rstn;

  SC_CTOR(PortScheduler) {
#define INSTANTIATE_SEND_REQUESTS(z, m, unused) \
  SC_THREAD(send_requests<m>);                  \
  sensitive << clk.pos();                       \
  async_reset_signal_is(rstn, false);
    BOOST_PP_REPEAT(N_MASTERS, INSTANTIATE_SEND_REQUESTS, ~)

#define INSTANTIATE_PROCESS_RESPONSES(z, p, unused) \
  SC_THREAD(process_responses<p>);                  \
  sensitive << clk.pos();                           \
  async_reset_signal_is(rstn, false);
    BOOST_PP_REPEAT(N_PORTS, INSTANTIATE_PROCESS_RESPONSES, ~)
  }

  template <int master_id>
  void send_requests() {
    master_req[master_id].Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      const auto params = scheduler_params.Pop();

      const auto state = params.masters[master_id];
      const auto port_id = state.port_id;

      if (state.num_requests > 0) {
        for (ac_int<16, false> i = 0;; i++) {
          const auto req = master_req[master_id].Pop();
          BOOST_PP_CAT(router_,
                       BOOST_PP_CAT(master_id, BOOST_PP_CAT(_, port_id)))
              .Push(req);
          if (i == state.num_requests - 1) break;
        }
      }
    }
  }

  template <int port_id>
  void process_responses() {
    port_resp[port_id].Reset();

    wait();

#pragma hls_pipeline_init_interval 1
#pragma hls_pipeline_stall_mode flush
    while (true) {
      const auto params = router_params.Peek();

      const auto state = params.ports[port_id];
      const auto master_id = state.master_id;

      if (params.ports[port_id].num_responses) {
        for (ac_int<16, false> i = 0;; i++) {
          const auto resp = port_resp[port_id].Pop();
          master_resp[master_id].Push(resp);
          if (i == state.num_responses - 1) break;
        }
      }
    }
  }
};
