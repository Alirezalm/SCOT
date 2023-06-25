//
// Created by alireza on 30/05/22.
//
#include "fstream"
#include "ScotSolver.h"
#include "../Algorithm/Dipoa.h"
#include "../Algorithm/Dihoa.h"
#include "../Tasks/TaskQueue.h"
#include "Results.h"
#include "../MIPSolver/MipSolverGurobiMultipleTree.h"
#include "MessagePassingInterface.h"
#include "Logger.h"
#include "Report.h"
#include "../ExternLibs/json/json.hpp"
#include "../Model/LogRegObjectiveFunction.h"
#include "../Model/LinRegObjectiveFunction.h"
#include "Timer.h"

namespace scot {
    ScotSolver::ScotSolver() {

        env_ = std::make_shared<Environment>();

        env_->task_queue_ptr_ = std::make_shared<TaskQueue>(env_);

        env_->results_ = std::make_shared<Results>(env_);

        env_->logger_ = std::make_shared<Logger>();

        env_->report_ = std::make_shared<Report>(env_);

        env_->settings_ = std::make_shared<Settings>();

        env_->timer_ = std::make_shared<Timer>("scot_timer");

    }

    EnvironmentPtr ScotSolver::getEnvironment() {
        return env_;
    }

    bool ScotSolver::solve() {
        if (env_->settings_->getDblSetting("verbose") == 1) {
            outputHeader();
        }
        selectAlgorithm();
        env_->timer_->start();
        return solution_algorithm_->Run();
    }

    void ScotSolver::selectAlgorithm() {

        auto alg = env_->settings_->getStrSetting("algorithm");
        if (alg == "dipoa") {

            solution_algorithm_ = std::make_shared<Dipoa>(env_);
            env_->logger_->logInfo("multi-tree algorithm, dipoa, selected.", env_->model_->getRank());

        } else if (alg == "dihoa") {
            solution_algorithm_ = std::make_shared<Dihoa>(env_);
            env_->logger_->logInfo("single-tree algorithm, dihoa, selected.", env_->model_->getRank());
        } else {
//    env_->logger_->logCritical("no algorithm provided, dipoa is selected", env_->model_->getRank());
            throw std::invalid_argument("no valid algorithm\n");

        }

    }

    void ScotSolver::outputHeader() {
        if (env_->model_->getRank() == 0) {
            env_->report_->printSolverHeader();
            env_->report_->printProblemDetails();
            env_->report_->printSolverDetails();
        }

    }

    bool ScotSolver::setProblemData(std::filesystem::path input_path) {

        std::ifstream input(input_path);

        nlohmann::json problem_json;

        input >> problem_json;

        problem_json.at("response").get_to(response_);
        problem_json.at("samples").get_to(samples_);
        problem_json.at("type").get_to(ptype_);
        if (ptype_ == "classification") {
            problem_type_ = ProbType::CLASSIFICATION;
        } else if (ptype_ == "regression") {
            problem_type_ = ProbType::REGRESSION;
        } else {
            throw std::invalid_argument("unknown problem in solver.\n");
        }
        upperbound_ = 30; //todo: for now
        env_->model_ = std::make_shared<Model>(total_nodes_, rank_, upperbound_);
        return false;
    }

    bool ScotSolver::setObjective() {
        std::shared_ptr<IObjective> obj;
        switch (problem_type_) {
            case ProbType::CLASSIFICATION:
                obj = std::make_shared<LogRegObjectiveFunction>();
                break;
            case ProbType::REGRESSION:
                obj = std::make_shared<LinRegObjectiveFunction>();
                break;
        }

        obj->setObjectiveData(samples_, response_);
        env_->model_->setObjectiveFunction(obj);
        return true;
    }

    bool ScotSolver::setSparseConstraints() {
        auto sparsity_constraint = std::make_shared<SparsityConstraint>(number_of_nonzeros_, upperbound_);
        env_->model_->setSparsityConstraint(sparsity_constraint);
        return true;
    }

    bool ScotSolver::setMpi() {
        env_->mpi_ = std::make_shared<MessagePassingInterface>(env_);
        return true;
    }

    void ScotSolver::setTotalNodes(int total_nodes) {
        total_nodes_ = total_nodes;
    }

    void ScotSolver::setRank(int rank) {
        rank_ = rank;
    }

    bool ScotSolver::setNumberOfNonzeros(int nzeros) {
        //todo: perform checks
        if (nzeros < 1) {
            throw std::invalid_argument("SCOT error: number of nonzeros must be positive.\n");
        }
        if (nzeros >= env_->model_->getNumberOfVariables()) {
            throw std::invalid_argument("SCOT error: number of nonzeros cannot be larger than variables.\n");
        }
		number_of_nonzeros_ = nzeros;
        return true;
    }

}