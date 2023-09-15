//
// Created by alireza on 26/06/22.
//

#ifndef DISCOT_SRC_TASKS_TASKPRINTITERATIONINFO_H_
#define DISCOT_SRC_TASKS_TASKPRINTITERATIONINFO_H_
#include "TaskBase.h"
namespace Scot {
/**
 * @brief This class is used to print iteration information
 */
class TaskPrintIterationInfo : public TaskBase {

 public:
  explicit TaskPrintIterationInfo(EnvironmentPtr env);
  ~TaskPrintIterationInfo() override = default;

  void initialize() override;
  void execute() override;
};

}

#endif //DISCOT_SRC_TASKS_TASKPRINTITERATIONINFO_H_
