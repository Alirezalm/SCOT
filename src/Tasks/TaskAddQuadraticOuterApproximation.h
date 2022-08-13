//
// Created by alireza on 01/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKADDQUADRATICOUTERAPPROXIMATION_H_
#define SCOT_SRC_TASKS_TASKADDQUADRATICOUTERAPPROXIMATION_H_

#include "TaskBase.h"
namespace scot {

class TaskAddQuadraticOuterApproximation : public TaskBase{
 public:
  explicit TaskAddQuadraticOuterApproximation(EnvironmentPtr env);
  ~TaskAddQuadraticOuterApproximation() override = default;

  void initialize() override;
  void execute() override;
};

}



#endif //SCOT_SRC_TASKS_TASKADDQUADRATICOUTERAPPROXIMATION_H_
