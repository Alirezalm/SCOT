//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_ENUMS_H_
#define DISCOT_SRC_ENUMS_H_
#include "include/types.h"
namespace scot {
enum class SparsityModelingMethods {
  BIG_M,
  SOS_1,
  BIG_SOS
};

enum class AlgorithmName {
  DIPOA,
  DIHOA
};

enum class ProbType{
  CLASSIFICATION,
  REGRESSION
};

enum class VariableType {
  CONTINUOUS,
  BINARY
};

enum class CutType {
  LINEAR,
  QUADRATIC
};

using PrimalStatus = dose::SolverStatus;
}

#endif //DISCOT_SRC_ENUMS_H_

