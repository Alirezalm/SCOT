//
// Created by alireza on 12/07/22.
//

#ifndef SCOT_SRC_TASKS_TASKCHECKHYBRIDEVENT_H_
#define SCOT_SRC_TASKS_TASKCHECKHYBRIDEVENT_H_

#include "TaskBase.h"
namespace scot {

class TaskCheckHybridEvent : public TaskBase {
 public:
  explicit TaskCheckHybridEvent(EnvironmentPtr env);
  ~TaskCheckHybridEvent() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //SCOT_SRC_TASKS_TASKCHECKHYBRIDEVENT_H_
