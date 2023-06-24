//
// Created by alireza on 18/05/22.
//

#include "../include/LinRegStrategy.h"
#include "../include/DistEngine.h"
#include "../include/Utilities.h"
namespace dose
{

	LinRegStrategy::LinRegStrategy(const Mat& A,
		const Vec& b,
		const int& rank,
		const int& maxNodes,
		const double& M,
		const SettingsPtr settings)
	{
		this->A = A;
		this->b = b;
		this->rank = rank;
		this->maxNodes = maxNodes;
		this->M = M;
		this->settings = settings;
		this->problemType = ProblemType::LinearRegression;

	}

	LinRegStrategy::~LinRegStrategy() = default;

	void LinRegStrategy::solve(const Vec& binvar)
	{
		SolutionPtr rhadmmSolution = runRHADMM(A, b, rank, maxNodes, M, binvar, settings, problemType);
		utilities::toDoseSolution(rhadmmSolution, doseSolution);
	}
	DoseSolution LinRegStrategy::getSolution()
	{
		return doseSolution;
	}
}