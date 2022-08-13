//
// Created by alireza on 26/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKCHECKTERMINATIONGAP_H_
#define DISCOT_SRC_TASKS_TASKCHECKTERMINATIONGAP_H_
#include "TaskBase.h"
namespace scot{

class TaskCheckTerminationGap: public TaskBase{

 public:
 explicit TaskCheckTerminationGap(EnvironmentPtr env);
  ~TaskCheckTerminationGap() override = default;

  void initialize() override;
  void execute() override;
 private:
  double abs_gap_;
  double rel_gap_;

  double eps_abs_;
  double eps_rel_;
};

}


#endif //DISCOT_SRC_TASKS_TASKCHECKTERMINATIONGAP_H_
