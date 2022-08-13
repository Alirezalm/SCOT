//
// Created by alireza on 06/06/22.
//

#include "TaskInitializeNewIteration.h"
#include "../Solver/Results.h"

namespace scot {

void TaskInitializeNewIteration::initialize() {

}

void TaskInitializeNewIteration::execute() {
  env_->results_->makeIteration();
}
}