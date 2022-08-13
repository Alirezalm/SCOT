//
// Created by alireza on 18/05/22.
//

#ifndef DOSE_ALGORITHMMANAGER_H
#define DOSE_ALGORITHMMANAGER_H

#include "LinRegStrategy.h"
#include "LogRegStrategy.h"

namespace dose {
    class AlgorithmManager {

    public:
        AlgorithmManager();

        ~AlgorithmManager();

        void setAlgorithmStrategy(AlgorithmPtr algorithm);

        DoseSolution runAlgorithm(Vec &binvec);

    private:
        AlgorithmPtr solutionAlgorithm;

    };
    using AlgManagerPtr = std::shared_ptr<AlgorithmManager>;
}



#endif //DOSE_ALGORITHMMANAGER_H
