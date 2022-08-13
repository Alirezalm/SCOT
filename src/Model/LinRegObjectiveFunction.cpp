//
// Created by alireza on 25/05/22.
//

#include "LinRegObjectiveFunction.h"
namespace scot {

void LinRegObjectiveFunction::setObjectiveData(const VectorDouble2D &samples_data, const VectorDouble &response_data) {
  assert(!samples_data.empty() && !response_data.empty());

  samples_ = samples_data;
  response_ = response_data;

  number_of_rows_ = static_cast<int>(samples_.size());
  number_of_columns_ = static_cast<int>(samples_.front().size());
  assert(number_of_columns_ > 0 && number_of_rows_ > 0);
  assert(number_of_rows_ > number_of_columns_);
}
VectorDouble LinRegObjectiveFunction::getResponseData() {
  assert(!samples_.empty() && "make sure data is generated");
  return response_;
}
VectorDouble2D LinRegObjectiveFunction::getSamplingData() {
  assert(!response_.empty() && "make sure data is generated");
  return samples_;
}
LinRegObjectiveFunction::LinRegObjectiveFunction() {
  number_of_columns_ = 0;
  number_of_rows_ = 0;
  ptype_ = dose::ProblemType::LinearRegression;
}
int LinRegObjectiveFunction::getNumberOfColumns() {
  return number_of_columns_;
}
int LinRegObjectiveFunction::getNumberOfRows() {
  return number_of_rows_;
}
dose::ProblemType LinRegObjectiveFunction::getObjectiveType() {
  return ptype_;
}
}