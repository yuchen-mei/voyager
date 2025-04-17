set block "VectorUnit"
set full_block_name "VectorUnit<$VECTOR_DATATYPE, $ACCUM_BUFFER_DATATYPE, $SCALE_DATATYPE, $OC_DIMENSION>"

proc pre_libraries {} {
  solution library add {[Block] VectorFetchUnit.v1}
  solution library add {[Block] VectorOpUnit.v1}
  solution library add {[Block] OutputController.v1}
  solution library add {[Block] VectorParamsDeserializer.v1}
}

proc pre_assembly {} {
  global full_block_name
  set full_block_name_stripped [string map {" " ""} $full_block_name]

  global VECTOR_DATATYPE ACCUM_BUFFER_DATATYPE OC_DIMENSION VU_INPUT_TYPES SCALE_DATATYPE OUTPUT_DATATYPES
  set vector_fetch_name "VectorFetchUnit<$VECTOR_DATATYPE,$ACCUM_BUFFER_DATATYPE,$OC_DIMENSION,$VU_INPUT_TYPES>"
  set vector_fetch_name_stripped [string map {" " ""} $vector_fetch_name]
  set vector_op_name "VectorOpUnit<$VECTOR_DATATYPE,$ACCUM_BUFFER_DATATYPE,$SCALE_DATATYPE,$OC_DIMENSION>"
  set vector_op_name_stripped [string map {" " ""} $vector_op_name]
  set vector_output_name "OutputController<$VECTOR_DATATYPE,$SCALE_DATATYPE,$OC_DIMENSION,$OUTPUT_DATATYPES>"
  set vector_output_name_stripped [string map {" " ""} $vector_output_name]

  directive set /$full_block_name_stripped/$vector_fetch_name_stripped -MAP_TO_MODULE {[Block] VectorFetchUnit.v1}
  directive set /$full_block_name_stripped/$vector_op_name_stripped -MAP_TO_MODULE {[Block] VectorOpUnit.v1}
  directive set /$full_block_name_stripped/$vector_output_name_stripped -MAP_TO_MODULE {[Block] OutputController.v1}
  directive set /$full_block_name_stripped/VectorParamsDeserializer -MAP_TO_MODULE {[Block] VectorParamsDeserializer.v1}
}