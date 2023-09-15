//
// Created by alireza on 25/05/22.
//

#ifndef DISCOT_SRC_SOLVER_STRUCTS_H_
#define DISCOT_SRC_SOLVER_STRUCTS_H_

#include <vector>
#include "memory"
#include "Enums.h"

#include "include/DOSE.h"

namespace Scot {

using VectorDouble = std::vector<double>;
using VectorDouble2D = std::vector<VectorDouble>;

using PairDouble = std::pair<double, double>;

struct IncumbentSolution {
  VectorDouble consensus_solution;
  VectorDouble local_gradient;
  double min_eig_val;
  double local_obj_value;
  double total_obj_value;
  PrimalStatus status;

};
struct DualSolution {
  VectorDouble binary_vector;
  VectorDouble real_vector;
  double objective_value;
  // TODO: solver status should be here

};

struct LinearOuterApproximation {
  VectorDouble consensus_feasible_point;
  VectorDouble local_gradient_at_feasible_point;
  double local_objective_value_at_feasible_point;
};

struct QuadraticOuterApproximation {
  VectorDouble consensus_feasible_point;
  VectorDouble local_gradient_at_feasible_point;
  double local_objective_value_at_feasible_point;
  double local_min_eig_at_feasible_point;
};

// forward declarations
class Results;
class TaskBase;
class TaskQueue;
class IMipSolver;
class MessagePassingInterface;
class Logger;
class Report;
class Settings;
class Timer;
using DosePtr = std::shared_ptr<dose::DOSE>;
using ResultsPtr = std::shared_ptr<Results>;
using TaskQueuePtr = std::shared_ptr<TaskQueue>;
using TaskBasePtr = std::shared_ptr<TaskBase>;
using PairTaskDescription = std::pair<TaskBasePtr, std::string>;
using MipSolverPtr = std::shared_ptr<IMipSolver>;
using MessagePassingInterfacePtr = std::shared_ptr<MessagePassingInterface>;
using LoggerPtr = std::shared_ptr<Logger>;
using ReportPtr = std::shared_ptr<Report>;
using SettingsPtr = std::shared_ptr<Settings>;
using TimerPtr = std::shared_ptr<Timer>;
}
#endif //DISCOT_SRC_SOLVER_STRUCTS_H_
