//
// Created by alireza on 18/05/22.
//

#ifndef DOSE_LINREGSTRATEGY_H
#define DOSE_LINREGSTRATEGY_H

#include "IAlgorithmStrategy.h"

namespace dose {
    class LinRegStrategy : public IAlgorithmStrategy{
    public:
        LinRegStrategy(const Mat &A,
                       const Vec &b,
                       const int &rank,
                       const int &maxNodes,
                       const double &M,
                       const SettingsPtr settings);

        virtual ~LinRegStrategy() override;

        void solve(const Vec &binvar) override;
		DoseSolution getSolution() override;
    };
}


#endif //DOSE_LINREGSTRATEGY_H
