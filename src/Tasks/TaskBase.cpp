//
// Created by alireza on 03/06/22.
//

#include "TaskBase.h"

namespace Scot {

TaskBase::TaskBase(EnvironmentPtr env) {
  env_ = env;
  is_active_ = true;
}
bool Scot::TaskBase::isActive() {
  return is_active_;
}
void TaskBase::activate() {
  is_active_ = true;
}
void TaskBase::deactivate() {
  is_active_ = false;
}
}