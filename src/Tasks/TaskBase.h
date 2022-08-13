//
// Created by alireza on 31/05/22.
//

#ifndef DISCOT_SRC_TASKS_TASKBASE_H_
#define DISCOT_SRC_TASKS_TASKBASE_H_
// all subclasses must implement their own constructors.

#include "../Solver/Environment.h"

namespace scot {
class TaskBase {
 public:
  explicit TaskBase(EnvironmentPtr env);
  virtual ~TaskBase() = default;
  virtual bool isActive();
  virtual void activate();
  virtual void deactivate();
  virtual void initialize() = 0;
  virtual void execute() = 0;

 protected:
  EnvironmentPtr env_;
  bool is_active_;
};
}
#endif //DISCOT_SRC_TASKS_TASKBASE_H_
