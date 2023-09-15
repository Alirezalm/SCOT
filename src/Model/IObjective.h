//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_IOBJECTIVE_H
#define DISCOT_IOBJECTIVE_H

#include "../Solver/Structs.h"
#include "memory"
#include "../../DOSE/include/DOSE.h"

namespace Scot {
class IObjective {
 public:

  virtual ~IObjective() = default;

  virtual void setObjectiveData(const VectorDouble2D &samples_data, const VectorDouble &response_data) = 0;

  virtual VectorDouble getResponseData() = 0;;
  virtual VectorDouble2D getSamplingData() = 0;

  virtual int getNumberOfColumns() = 0;
  virtual int getNumberOfRows() = 0;
  virtual dose::ProblemType getObjectiveType() = 0;

 protected:
  int number_of_columns_ = 0;
  int number_of_rows_ = 0;
  dose::ProblemType ptype_ = dose::LOGISTIC_REGRESSION;

};
using IObjectivePtr = std::shared_ptr<IObjective>;
}

#endif //DISCOT_IOBJECTIVE_H
