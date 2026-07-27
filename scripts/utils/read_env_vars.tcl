# Read and set relevant environment variables
set ENV_VARS {BLOCK CLOCK_PERIOD DATATYPE IC_DIMENSION OC_DIMENSION INPUT_BUFFER_SIZE WEIGHT_BUFFER_SIZE ACCUM_BUFFER_SIZE TECHNOLOGY CATAPULT_BUILD_DIR DOUBLE_BUFFERED_ACCUM_BUFFER SUPPORT_MVM SUPPORT_SPMM SUPPORT_DWC}

foreach var $ENV_VARS {
  set $var [exec echo $::env($var)]
}

# Accumulator (PSUM) datatype name forwarded from the build flow (bare type such
# as "int24"). architecture.tcl and setup_project.tcl qualify it as
# DataTypes::<name>. Defaults to int24 when unset (standalone builds).
if {[info exists ::env(ACCUM_DATATYPE)]} {
  set ACCUM_DATATYPE_NAME [exec echo $::env(ACCUM_DATATYPE)]
} else {
  set ACCUM_DATATYPE_NAME int24
}

# Optional fetcher FIFO depths, forwarded to the Catapult HLS compile in
# setup_project.tcl. Default to 16 when unset.
set ENV_VARS_OPTIONAL {VOYAGER_WEIGHT_FETCHER_FIFO_DEPTH VOYAGER_INPUT_FETCHER_FIFO_DEPTH}
foreach var $ENV_VARS_OPTIONAL {
  if {[info exists ::env($var)]} {
    set $var [exec echo $::env($var)]
  } else {
    set $var 16
  }
}

set ROOT [file normalize $::env(PROJ_ROOT)]
