#pragma once

#include "src/AccelTypes.h"
#include "src/ArchitectureParams.h"
#include "src/Params.h"
#include "test/common/VerificationTypes.h"

void MapSoftmax(const SimplifiedParams &params, MatrixParams &matrixParams,
                bool &matrixParamsValid, VectorParams &vectorParams,
                VectorInstructionConfig &vectorInstructionConfig,
                bool &vectorParamsValid);

void MapSoftmaxGrad(const SimplifiedParams &params, MatrixParams &matrixParams,
                    bool &matrixParamsValid, VectorParams &vectorParams,
                    VectorInstructionConfig &vectorInstructionConfig,
                    bool &vectorParamsValid);

void MapFCGrad(const SimplifiedParams &params, MatrixParams &matrixParams,
               bool &matrixParamsValid, VectorParams &vectorParams,
               VectorInstructionConfig &vectorInstructionConfig,
               bool &vectorParamsValid);

void MapFC(const SimplifiedParams &params, MatrixParams &matrixParams,
           bool &matrixParamsValid, VectorParams &vectorParams,
           VectorInstructionConfig &vectorInstructionConfig,
           bool &vectorParamsValid);

void MapNoNorm(const SimplifiedParams &params, MatrixParams &matrixParams,
               bool &matrixParamsValid, VectorParams &vectorParams,
               VectorInstructionConfig &vectorInstructionConfig,
               bool &vectorParamsValid);

void MapNoNormGrad(const SimplifiedParams &params, MatrixParams &matrixParams,
                   bool &matrixParamsValid, VectorParams &vectorParams,
                   VectorInstructionConfig &vectorInstructionConfig,
                   bool &vectorParamsValid);

void MapGenericErrorGrad(const SimplifiedParams &params,
                         MatrixParams &matrixParams, bool &matrixParamsValid,
                         VectorParams &vectorParams,
                         VectorInstructionConfig &vectorInstructionConfig,
                         bool &vectorParamsValid);

void MapMatrixOp(const SimplifiedParams &params, MatrixParams &matrixParams,
                 bool &matrixParamsValid, VectorParams &vectorParams,
                 VectorInstructionConfig &vectorInstructionConfig,
                 bool &vectorParamsValid);
