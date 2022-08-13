//
// Created by alireza on 03/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKDISTRIBUTEDNLPSOLUTION_H_
#define DISCOT_SRC_TASKS_TASKDISTRIBUTEDNLPSOLUTION_H_

#include "TaskBase.h"
#include "../PrimalSolver/InlpSolver.h"

namespace scot {

class TaskDistributedNlpSolution : public TaskBase {

 public:
  explicit TaskDistributedNlpSolution(EnvironmentPtr env);
  ~TaskDistributedNlpSolution() override = default;

  void createNlpSolver();

  void execute() override;
  void initialize() override;

  IncumbentSolution getNlpSolution();

 private:
  InlpSolverPtr nlp_solver_;
};

}

#endif //DISCOT_SRC_TASKS_TASKDISTRIBUTEDNLPSOLUTION_H_
