#include "test/toolchain/MapOperation.h"

#include "test/toolchain/operations/Operations.h"

// create Matrix and Vector Params from SimplifiedParams
void MapOperation(const SimplifiedParams &params, MatrixParams &matrixParams,
                  bool &matrixParamsValid, VectorParams &vectorParams,
                  VectorInstructionConfig &vectorInstructionConfig,
                  bool &vectorParamsValid) {
  int X = params.loops[0][params.inputXLoopIndex[0]] *
          params.loops[1][params.inputXLoopIndex[1]];
  int Y = params.loops[0][params.inputYLoopIndex[0]] *
          params.loops[1][params.inputYLoopIndex[1]];
  int C = params.loops[1][params.reductionLoopIndex[1]] * DIMENSION;
  int K = params.loops[0][params.weightLoopIndex[0]] *
          params.loops[1][params.weightLoopIndex[1]] * DIMENSION;
  int FX = params.loops[1][params.fxIndex];
  int FY = params.loops[1][params.fyIndex];
  int STRIDE = params.STRIDE;

  if (params.SOFTMAX) {
    MapSoftmax(params, matrixParams, matrixParamsValid, vectorParams,
               vectorInstructionConfig, vectorParamsValid);
  } else if (params.SOFTMAX_GRAD) {
    MapSoftmaxGrad(params, matrixParams, matrixParamsValid, vectorParams,
                   vectorInstructionConfig, vectorParamsValid);
  } else if (params.FC_GRAD) {
    MapFCGrad(params, matrixParams, matrixParamsValid, vectorParams,
              vectorInstructionConfig, vectorParamsValid);
  } else if (params.FC) {
    MapFC(params, matrixParams, matrixParamsValid, vectorParams,
          vectorInstructionConfig, vectorParamsValid);
  } else if (params.NO_NORM) {
    MapNoNorm(params, matrixParams, matrixParamsValid, vectorParams,
              vectorInstructionConfig, vectorParamsValid);
  } else if (params.NO_NORM_GRAD) {
    MapNoNormGrad(params, matrixParams, matrixParamsValid, vectorParams,
                  vectorInstructionConfig, vectorParamsValid);
  } else if (params.MSE_GRAD || params.BCE_WITH_LOGITS_GRAD) {
    MapGenericErrorGrad(params, matrixParams, matrixParamsValid, vectorParams,
                        vectorInstructionConfig, vectorParamsValid);
  } else {
    MapMatrixOp(params, matrixParams, matrixParamsValid, vectorParams,
                vectorInstructionConfig, vectorParamsValid);
  }
}
