//
// Created by alireza on 11/07/22.
//

#ifndef SCOT_SRC_ALGORITHM_DIHOA_H_
#define SCOT_SRC_ALGORITHM_DIHOA_H_
#include "IAlgorithm.h"

namespace scot {

class Dihoa: public IAlgorithm{

 public:
  explicit Dihoa(EnvironmentPtr env);
  ~Dihoa() override = default;

  bool Run() override;

  void outputIterHeader();

};

}


#endif //SCOT_SRC_ALGORITHM_DIHOA_H_
