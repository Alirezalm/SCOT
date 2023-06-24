//
// Created by alireza on 16/05/22.
//

#include "../include/DistEngine.h"

namespace dose
{
	const double ad_mu = 10;

	const double ad_t = 2;

	const int space = 5;

	const int data = 10;

	SolutionPtr runRHADMM(const Mat& A,
		const Vec& b,
		const int& rank,
		const int& maxNodes,
		const double& M,
		const Vec& binVec,
		const SettingsPtr settings,
		const ProblemType& problemType)
	{

		const int n = static_cast<int>(A.cols());

		// initialize x, y, z
		Vec x(n, 1);
		Vec y(n, 1);
		Vec z(n, 1);
		Vec z_old(n, 1);

		coldStart(x, y, z, z_old, n);

		// default rho
		double rho = settings->rho;
		double t;
		double pres;
		double dres;
		double total_f;
		double local_f;

		InfoPtr info = std::make_shared<RHADMMInfo>();
		SolutionPtr solution = std::make_shared<RHADMMSolution>();

		if ((settings->verbose) && (rank == 0)) printHeader();
		for (int i = 0; i < settings->maxIter; ++i)
		{
			z_old = z;
			updateX(A, b, x, y, z, rho, binVec, M, total_f, problemType);
			updateZ(x, y, z, rho, n, maxNodes);
			projectOnBinaryBoundedSpace(z, binVec, M, problemType);
			updateY(x, y, z, rho);
			updateLocalObjVal(A, b, x, local_f, problemType);
			updateTotalObjVal(local_f, total_f);
			updateResiduals(pres, dres, t, x, y, z, z_old, rho, maxNodes);
			if (settings->adaptive) updateRho(rho, t, dres);
			if ((settings->verbose) && (rank == 0))
			{
				info->iter = i;
				info->t = t;
				info->dres = dres;
				info->total_f = total_f;
				info->rho = rho;
				outputLog(info);
			}

			if ((t <= settings->eps) && dres <= settings->eps)
			{
				solution->x = x;
				solution->total_f = total_f;
				solution->local_f = local_f;
				solution->status = SolverStatus::OPTIMAL;
				switch (problemType)
				{
				case LogisticRegression:
					solution->minEig = computeLogRMinEig(A, x);
					solution->gx = computeLogRGrad(A, b, x);
					break;
				case LinearRegression:
					solution->minEig = computeLinRMinEig(A, x);
					solution->gx = computeLinRGrad(A, b, x);
					break;
				};
				return solution;
			}
		}
		solution->x = x;
		solution->total_f = total_f;
		solution->local_f = local_f;
		solution->status = SolverStatus::MAXITER;
		return solution;

	}

	void coldStart(Vec& x, Vec& y, Vec& z, Vec& zOld, const Ind& n)
	{
		x.setZero();
		y.setZero();
		z.setZero();
		zOld.setZero();
	}

	void updateZ(Vec& x, Vec& y, Vec& z, double& rho, const int& n, const int& maxNodes)
	{
		Vec sum = x + (1.0 / rho) * y;
		Vec z_rcv(n, 1);
		MPI_Allreduce(sum.data(),
			z_rcv.data(),
			n,
			MPI_DOUBLE,
			MPI_SUM,
			MPI_COMM_WORLD);

		z_rcv.resize(n, 1);
		z = (1.0 / maxNodes) * z_rcv;

	}

	void updateX(const Mat& A,
		const Vec& b,
		Vec& x,
		Vec& y,
		Vec& z,
		double& rho,
		const Vec& binVec,
		const double& M,
		double& total_f,
		const ProblemType& problemType)
	{


		if (problemType == ProblemType::LogisticRegression)
		{
			x = runTN(A, b, x, y, z, rho, binVec, M);
		}
		else
		{
			x = solveLinR(A, b, x, y, z, rho, binVec, M);
		}

	}

	void updateY(Vec& x, Vec& y, Vec& z, double& rho)
	{
		y += rho * (x - z);
	}

	void updateResiduals(double& pres,
		double& dres,
		double& t,
		Vec& x,
		Vec& y,
		Vec& z,
		Vec& z_old,
		double& rho,
		const int maxNodes)
	{

		pres = (x - z).squaredNorm();
		dres = rho * sqrt(maxNodes) * (z_old - z).squaredNorm();
		MPI_Allreduce(&pres, &t, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	}

	void updateRho(double& rho, const double& t, const double& dres)
	{
		if (sqrt(t) > ad_mu * sqrt(dres))
		{
			rho *= ad_t;
		}
		else if (sqrt(dres) > ad_mu * sqrt(t))
		{
			rho /= ad_t;
		}
	}

	void updateTotalObjVal(double& local_f, double& total_f)
	{
		MPI_Allreduce(&local_f, &total_f, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
	}

	void outputLog(InfoPtr info)
	{
		std::cout << std::setw(space) << std::scientific << std::setprecision(3) << info->iter;
		std::cout << std::setw(space) << " ";
		std::cout << std::setw(data) << std::scientific << std::setprecision(3) << info->t;
		std::cout << std::setw(space) << " ";
		std::cout << std::setw(data) << std::scientific << std::setprecision(3) << info->dres;
		std::cout << std::setw(space) << " ";
		std::cout << std::setw(data) << std::scientific << std::setprecision(3) << info->total_f;
		std::cout << std::setw(space) << " ";
		std::cout << std::setw(space) << std::scientific << std::setprecision(3) << info->rho << std::endl;
	}

	void printHeader()
	{
		std::cout << std::setw(space * 2) << std::left << "\niter";
		std::cout << std::setw(data + space) << std::left << "pres";
		std::cout << std::setw(data + space) << std::left << "dres";
		std::cout << std::setw(data + space) << std::left << "fval";
		std::cout << std::setw(data + space) << std::left << "rho" << std::endl;
	}

	void computeLinRObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f)
	{
        auto m = static_cast<double>(b.size()) ;
		local_f = 0.5  * (A * x - b).squaredNorm();
        local_f = local_f / m;
	}

	void computeLogRObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f)
	{
		Vec h = computeLogistic(A, x);
        auto m = static_cast<double>(h.size()) ;
		local_f = -b.dot(h.array().log().matrix()) -
			(1.0 - b.array()).matrix().dot((1.0 - h.array()).log().matrix());
        local_f = local_f / m;
	}

	void updateLocalObjVal(const Mat& A,
		const Vec& b,
		const Vec& x,
		double& local_f,
		const ProblemType& ptype)
	{
		if (ptype == LogisticRegression)
		{
			computeLogRObjVal(A, b, x, local_f);
		}
		else if (ptype == LinearRegression)
		{
			computeLinRObjVal(A, b, x, local_f);
		}
	}

	void projectOnBinaryBoundedSpace(Vec& x, const Vec& binVec, const double& M, ProblemType ptype)
	{
		switch (ptype)
		{
		case ProblemType::LogisticRegression:
			projectLogR(x, binVec, M);
			break;
		case ProblemType::LinearRegression:
			projectLinR(x, binVec, M);
			break;
		}
	}
	Vec computeLogRGrad(const Mat& A, const Vec& b, const Vec& x)
	{
		Vec h = computeLogistic(A, x);
        auto m = static_cast<double>(h.size()) ;
		return (1.0 / m) *( A.transpose() * (h - b));
	}
	Vec computeLinRGrad(const Mat& A, const Vec& b, const Vec& x)
	{
		Mat Q = A.transpose() * A;
		Vec q = -A.transpose() * b;
        auto m = static_cast<double>(b.size()) ;
		return (1.0 / m) * (Q * x + q);
	}
	double computeLogRMinEig(const Mat& A, const Vec& x)
	{
		const Vec h = computeLogistic(A, x);
//		const Array<double, Dynamic, 1> _tmpArray = h.array();
		const Vec _temp = h.array() - h.array().pow(2);
		const Ind m = A.rows(), n = A.cols();
		Mat M(m, n);
		for (int j = 0; j < m; ++j)
		{
			M.row(j) = A.row(j) * _temp(j);
		}

		Mat Hessian = (1/static_cast<double>(m)) * A.transpose() * M;
		return Hessian.eigenvalues().real().minCoeff();
	}
	double computeLinRMinEig(const Mat& A, const Vec& x)
	{
        auto m = static_cast<double>(A.rows()) ;
		Mat Hessian = (1.0 / m) * ( A.transpose() * A);
		return Hessian.eigenvalues().real().minCoeff();
	}

}