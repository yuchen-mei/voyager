import datetime
import multiprocessing as mp
import os
import struct
import subprocess

import argparse
import numpy as np

mobilebert_layers = [
  "bottleneck_input_dense",
  "bottleneck_input_LayerNorm",
  "bottleneck_attention_dense",
  "bottleneck_attention_LayerNorm",
  "attention_self_query_layer",
  "attention_self_key_layer",
  "attention_self_value_layer",
  "attention_self_attention_scores_0",
  "attention_self_attention_probs_0",
  "attention_self_context_layer_0",
  "attention_output_dense",
  "attention_output_LayerNorm",
  "ffn_0_intermediate_dense",
  "ffn_0_output_dense",
  "ffn_0_output_LayerNorm",
  "ffn_1_intermediate_dense",
  "ffn_1_output_dense",
  "ffn_1_output_LayerNorm",
  "ffn_2_intermediate_dense",
  "ffn_2_output_dense",
  "ffn_2_output_LayerNorm",
  "intermediate_dense",
  "output_dense",
  "output_LayerNorm",
  "output_bottleneck_dense",
  "output_bottleneck_LayerNorm"
]

def run_test(results_folder, test):
    env_vars = os.environ.copy()
    env_vars['GROUP'] = "mobilebert"
    env_vars['SIMS'] = "fp32,hlsposit,accelerator"
    env_vars['TASK'] = "inference"
    env_vars['TESTS'] = test

    stdout_file = open(f'{results_folder}/{test}.log', 'w')
    stderr_file = open(f'{results_folder}/{test}.out', 'w')

    sim_process = subprocess.Popen(
        './build/TestRunner',
        env=env_vars,
        stdout=stdout_file,
        stderr=stderr_file
    )

    sim_process.communicate()


if __name__ == '__main__':    
    # Create directory with current time
    results_folder = 'logs/' + datetime.datetime.now().strftime("%Y_%m_%d_%H_%M_%S")
    os.makedirs(results_folder)

    # Create pool of multiprocessors
    pool = mp.Pool(32)

    for layer in mobilebert_layers:
        pool.apply_async(run_test, (results_folder, layer))

    pool.close()
    pool.join()
