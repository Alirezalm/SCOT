//
// Created by alireza on 17/05/22.
//
// TODO: fix compiler warning
// TODO: check clang-tidy warnings
// TODO: export to library

#include "../include/DOSE.h"
#include "../include/Utilities.h"
#include "../include/AlgorithmManager.h"

namespace dose {


    DOSE::DOSE(const VectorDouble2D &pdataSet, const VectorDouble &pdataRes, ProblemType ptype, SettingsPtr settings,
               int rank, int totalNodes, double M) : pdataSet(pdataSet),
                                                       pdataRes(pdataRes),
                                                       ptype(ptype),
                                                       settings(settings),
                                                       rank(rank),
                                                       totalNodes(totalNodes),
                                                       M(M) {
        validateData();
        toEigen();


    }

    DOSE::DOSE(const VectorDouble2D &pdataSet, const VectorDouble &pdataRes, ProblemType ptype, int rank,
               int totalNodes, double M) : pdataSet(pdataSet),
                                            pdataRes(pdataRes),
                                            ptype(ptype),
                                            rank(rank),
                                            totalNodes(totalNodes),
                                            M(M) {
        validateData();
        toEigen();

        settings = std::make_shared<RHADMMSettings>();  //default settings
    }

    DOSE::DOSE(const VectorDouble2D &pdataSet, const VectorDouble &pdataRes, int rank,
               int totalNodes, double M) : pdataSet(pdataSet),
                                            pdataRes(pdataRes),
                                            rank(rank),
                                            totalNodes(totalNodes), M(M) {
        validateData();
        toEigen();

        settings = std::make_shared<RHADMMSettings>();  //default settings
        ptype = ProblemType::LOGISTIC_REGRESSION;    // default problem type
    }

    void DOSE::toEigen() {
        pdataSetMat.setZero(rows, cols);
        pdataResVec.setZero(rows, 1);
        utilities::vec2Vec(pdataRes, pdataResVec);
        utilities::mat2Mat(pdataSet, pdataSetMat);

    }

    void DOSE::validateData() {
        utilities::checkVec2Mat(pdataSet);
        rows = static_cast<int>(pdataSet.size());
        cols = static_cast<int>(pdataSet.front().size());
        utilities::checkVec2Vec(pdataRes, rows);
    }

    void DOSE::solve(const VectorDouble &binvec) {
        assert(binvec.size() == cols);
        Vec binvecEigen(cols, 1);
        utilities::vec2Vec(binvec, binvecEigen);
        AlgManagerPtr algorithmManager = std::make_shared<AlgorithmManager>();
        AlgorithmPtr algorithm;
        switch (ptype) {
            case ProblemType::LINEAR_REGRESSION:
                algorithm = std::make_shared<LinRegStrategy>(pdataSetMat, pdataResVec, rank, totalNodes, M, settings);
                algorithmManager->setAlgorithmStrategy(algorithm);
                solution =  algorithmManager->runAlgorithm(binvecEigen);
                break;
            case ProblemType::LOGISTIC_REGRESSION:
                algorithm = std::make_shared<LogRegStrategy>(pdataSetMat, pdataResVec, rank, totalNodes, M, settings);
                algorithmManager->setAlgorithmStrategy(algorithm);
                solution = algorithmManager->runAlgorithm(binvecEigen);
                break;
        }

    }



	const DoseSolution & DOSE::GetSolution() const
	{
		return solution;
	}

    void DOSE::setPdataSet(const VectorDouble2D &pdataSetNew) {
        DOSE::pdataSet = pdataSetNew;
    }

    void DOSE::setPdataRes(const VectorDouble &pdataResNew) {
        DOSE::pdataRes = pdataResNew;
    }

    void DOSE::setPtype(ProblemType ptypeNew) {
        DOSE::ptype = ptypeNew;
    }
	void DOSE::setSettings(const SettingsPtr settingsNew) {
        DOSE::settings = settingsNew;
    }

}