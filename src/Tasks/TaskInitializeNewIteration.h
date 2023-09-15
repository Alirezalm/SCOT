//
// Created by alireza on 06/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKINITIALIZENEWITERATION_H_
#define DISCOT_SRC_TASKS_TASKINITIALIZENEWITERATION_H_

#include "TaskBase.h"

namespace Scot {
/**
 * @brief This class is responsible for initializing the new iteration.
 * @details This class is responsible for initializing the new iteration.
 * It is called at the beginning of each iteration.
 */
class TaskInitializeNewIteration : public TaskBase {

 public:

  explicit TaskInitializeNewIteration(EnvironmentPtr env) : TaskBase(env) {}

  ~TaskInitializeNewIteration() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //DISCOT_SRC_TASKS_TASKINITIALIZENEWITERATION_H_
