//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_MODEL_LINREGOBJECTIVEFUNCTION_H_
#define DISCOT_SRC_MODEL_LINREGOBJECTIVEFUNCTION_H_

#include "IObjective.h"

namespace Scot {
class LinRegObjectiveFunction : public IObjective {
 public:
  LinRegObjectiveFunction();
  ~LinRegObjectiveFunction() override = default;

  int getNumberOfColumns() override;
  int getNumberOfRows() override;

  void setObjectiveData(const VectorDouble2D &samples_data, const VectorDouble &response_data) override;
  dose::ProblemType getObjectiveType() override;

  VectorDouble getResponseData() override;
  VectorDouble2D getSamplingData() override;

 private:
  VectorDouble2D samples_;
  VectorDouble response_;
};
using LinRegObjectivePtr = std::shared_ptr<LinRegObjectiveFunction>;
}

#endif //DISCOT_SRC_MODEL_LINREGOBJECTIVEFUNCTION_H_
