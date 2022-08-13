//
// Created by alireza on 16/05/22.
//

#ifndef DOSE_DISTENGINE_H
#define DOSE_DISTENGINE_H

#include "LocalNLPSolver.h"


namespace dose
{


/// Runs RHADMM for Logreg or linreg problem in the following form
/// \obj
/// min_x f(x)
/// \constraints
/// s.t. -M delta <= x <= M delta
/// \param A Dataset
/// \param b Response vector, binary for logreg and real for linreg
/// \param rank MPI rank
/// \param maxNodes MPI total nodes
/// \param M Big-M parameter for bounded constraints
/// \param binVec Binary Vector
/// \param settings
/// \param problemType type of problem
	SolutionPtr runRHADMM(const Mat& A,
		const Vec& b,
		const int& rank,
		const int& maxNodes,
		const double& M,
		const Vec& binVec,
		const SettingsPtr settings,
		const ProblemType& problemType);

/// cold starts params
/// \param x admm current solution
/// \param y admm dual var
/// \param z admm consensus var
/// \param zOld last consensus var
/// \param n number of variables
	void coldStart(Vec& x, Vec& y, Vec& z, Vec& zOld, const Ind& n);

	/// Updates consensus variables
	void updateZ(Vec& x,
		Vec& y,
		Vec& z,
		double& rho,
		const int& n,
		const int& maxNodes);

/// updates primal variable according to problem type
	void updateX(const Mat& A,
		const Vec& b,
		Vec& x,
		Vec& y,
		Vec& z,
		double& rho,
		const Vec& binVec,
		const double& M,
		double& total_f,
		const ProblemType& problemType);

/// updates dual variables
	void updateY(Vec& x,
		Vec& y,
		Vec& z,
		double& rho);

/// updates residualts
	void updateResiduals(double& pres,
		double& dres,
		double& t,
		Vec& x,
		Vec& y,
		Vec& z,
		Vec& z_old,
		double& rho,
		int maxNodes);

/// applies adaptive rho strategy
	void updateRho(double& rho, const double& t, const double& dres);

/// updates total objective value
	void updateTotalObjVal(double& local_f, double& total_f);

/// formats output
	void outputLog(InfoPtr info);

	void printHeader();

	void computeLinRObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f);

	void computeLogRObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f);

	Vec computeLogRGrad(const Mat& A,
		const Vec& b,
		const Vec& x
	);

	Vec computeLinRGrad(const Mat& A,
		const Vec& b,
		const Vec& x
	);

	double computeLogRMinEig(const Mat& A,
		const Vec& x
	);

	double computeLinRMinEig(const Mat& A,
		const Vec& x
	);

	void updateLocalObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f,
		const ProblemType& ptype);
	void projectOnBinaryBoundedSpace(Vec& x, const Vec& binVec, const double& M, ProblemType ptype);
}
#endif //DOSE_DISTENGINE_H
