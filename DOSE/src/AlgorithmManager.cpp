//
// Created by alireza on 18/05/22.
//

#include "../include/AlgorithmManager.h"

namespace dose
{

	AlgorithmManager::AlgorithmManager() = default;

	void AlgorithmManager::setAlgorithmStrategy(AlgorithmPtr algorithm)
	{
		solutionAlgorithm = algorithm;
	}

	DoseSolution AlgorithmManager::runAlgorithm(Vec& binvec)
	{
		solutionAlgorithm->solve(binvec);
		return solutionAlgorithm->getSolution();
	}

	AlgorithmManager::~AlgorithmManager() = default;
}