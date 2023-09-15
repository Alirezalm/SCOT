//
// Created by alireza on 07/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKCHECKSOCEVENT_H_
#define SCOT_SRC_TASKS_TASKCHECKSOCEVENT_H_

#include "TaskBase.h"
namespace Scot {

class TaskCheckSocEvent : public TaskBase {

 public:
  explicit TaskCheckSocEvent(EnvironmentPtr env);
  void initialize() override;
  void execute() override;


};

}

#endif //SCOT_SRC_TASKS_TASKCHECKSOCEVENT_H_
