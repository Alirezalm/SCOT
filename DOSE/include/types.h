//
// Created by alireza on 16/05/22.
//

#ifndef DOSE_TYPES_H
#define DOSE_TYPES_H

#include "iostream"
#include <iomanip>
#include "chrono"
#include "memory"
#include "mpi/mpi.h"
#include <vector>
#include "../ThirdParty/Eigen/Dense"
#include "../ThirdParty/Eigen/IterativeLinearSolvers"

namespace dose
{

	using namespace Eigen;
	using namespace std::chrono;
	using Scalar = typename Eigen::MatrixXd::Scalar;
	using Mat = Matrix<Scalar, Dynamic, Dynamic, RowMajor>;
	using Vec = Matrix<Scalar, Dynamic, 1, ColMajor>;
	using Ind = typename Mat::Index;
	using VectorDouble = std::vector<double>;
	using VectorDouble2D = std::vector<VectorDouble>;
	struct TruncatedNewTonSettings
	{
		int maxIter = 1000;
		double eps = 1e-5;
		bool verbose = false;
	};

	struct RHADMMSettings
	{
		double rho = 1.0;
		double maxIter = 1000;
		double eps = 1e-8;
		bool adaptive = true;
		bool verbose = true;
	};

	struct RHADMMInfo
	{
		int iter;
		double t;
		double dres;
		double total_f;
		double rho;
	};

	enum class SolverStatus
	{
		OPTIMAL,
		SUBOPTIMAL,
		MAXITER,
		INFEASIBLE
	};

	struct RHADMMSolution
	{
		Vec x;
		Vec gx;
		double local_f;
		double total_f;
		SolverStatus status;
		double minEig;
	};

	struct DoseSolution
	{
		double local_f;
		VectorDouble x;
		VectorDouble gx;
		double minEig;
		double total_f;
		SolverStatus status;
	};

	enum ProblemType
	{
		LinearRegression,
		LogisticRegression
	};
	class LinRegStrategy;
	class LogRegStrategy;
	class IAlgorithmStrategy;
	using SettingsPtr = std::shared_ptr<RHADMMSettings>;
	using SolutionPtr = std::shared_ptr<RHADMMSolution>;

	using InfoPtr = std::shared_ptr<RHADMMInfo>;
	using LinRegPtr = std::shared_ptr<LinRegStrategy>;
	using LogRegPtr = std::shared_ptr<LogRegStrategy>;
	using AlgorithmPtr = std::shared_ptr<IAlgorithmStrategy>;

}
#endif //DOSE_TYPES_H
