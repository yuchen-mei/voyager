set block "VectorAccumulator"
set full_block_name "VectorAccumulator<$VECTOR_DATATYPE, $ACCUMULATOR_WIDTH>"

proc pre_architect {} {
  global full_block_name
  set vector_accumulator_stripped [string map {" " ""} $full_block_name]
}
