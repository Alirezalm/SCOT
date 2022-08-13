//
// Created by alireza on 26/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKCOMPUTEOBJECTIVEGAP_H_
#define DISCOT_SRC_TASKS_TASKCOMPUTEOBJECTIVEGAP_H_
#include "TaskBase.h"
namespace scot {

class TaskComputeObjectiveGap: public TaskBase{

 public:
    explicit TaskComputeObjectiveGap(EnvironmentPtr env);
    ~TaskComputeObjectiveGap() override = default;

  void initialize() override;
  void execute() override;

 private:
  double incumbent_;
  double dual_;
  double abs_gap_;
  double rel_gap_;
};

}


#endif //DISCOT_SRC_TASKS_TASKCOMPUTEOBJECTIVEGAP_H_
