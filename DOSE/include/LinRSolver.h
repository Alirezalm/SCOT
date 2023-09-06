//
// Created by alireza on 16/05/22.
//

#ifndef DOSE_LINRSOLVER_H
#define DOSE_LINRSOLVER_H

#include "Types.h"

namespace dose {


    Vec solveLinR(const Mat &A,
                  const Vec &b,
                  Vec &x,
                  Vec &y,
                  Vec &z,
                  double &rho,
                  const Vec &binVec,
                  const double &M);

    void projectLinR(Vec &x, const Vec &binVec, const double &M);
}
#endif //DOSE_LINRSOLVER_H
