//
// Created by alireza on 31/05/22.
//

#ifndef DISCOT_SRC_TASKS_TASKBASE_H_
#define DISCOT_SRC_TASKS_TASKBASE_H_
// all subclasses must implement their own constructors.

#include "../Solver/Environment.h"

namespace scot {

/**
 * @brief abstract class for all tasks.
 * @details All tasks must inherit from this class and implement initialize and execute methods.
 */
class TaskBase {
 public:

  explicit TaskBase(EnvironmentPtr env);
  virtual ~TaskBase() = default;

  /**
   * @brief checks if the task is active.
   * @details if the task is active, it will be executed in the next iteration.
   * @return true if the task is active.
   */
  virtual bool isActive();
  /**
   * @brief activates the task.
   * @details if the task is active, it will be executed in the next iteration.
   */
  virtual void activate();
  /**
   * @brief deactivates the task.
   * @details if the task is not active, it won't be executed in the next iteration.
   */
  virtual void deactivate();

  /**
   * @brief initializes the task.
   * @details this method is called before the task is executed.
   */
  virtual void initialize() = 0;
  /**
   * @brief executes the task.
   * @details this method is called after the task is initialized.
   */
  virtual void execute() = 0;

 protected:
  EnvironmentPtr env_;
  bool is_active_;
};
}
#endif //DISCOT_SRC_TASKS_TASKBASE_H_
