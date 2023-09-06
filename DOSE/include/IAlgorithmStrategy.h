//
// Created by alireza on 18/05/22.
//

#ifndef DOSE_IALGORITHMSTRATEGY_H
#define DOSE_IALGORITHMSTRATEGY_H

#include "Types.h"

namespace dose {
    class IAlgorithmStrategy {
    public:
        virtual ~IAlgorithmStrategy() = default;

        virtual void solve(const Vec &binvar) = 0;
		virtual DoseSolution getSolution() = 0;
    protected:
        DoseSolution doseSolution;
        Mat A;
        Vec b;
        int rank;
        int maxNodes;
        double M;
        SettingsPtr settings;
        ProblemType problemType;

    };
}


#endif //DOSE_IALGORITHMSTRATEGY_H
