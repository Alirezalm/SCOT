//
// Created by alireza on 16/05/22.
//

#ifndef DOSE_LOGRSOLVER_H
#define DOSE_LOGRSOLVER_H

#include "Types.h"

namespace dose {


    /// runTN solves the local logistic regression problem.
    /// \param A logistic regression dataset
    /// \param b logistic regression response vector
    /// \param x initial condition for parameters
    /// \param y initial condition for ADMM
    /// \param z initial condition for ADMM
    /// \param rho ADMM penalty parameter
    /// \param settings local NLP settings
    /// \return solution of local NLP problem for logistic regression problem.
    Vec runTN(const Mat &A,
              const Vec &b,
              Vec &x,
              Vec &y,
              Vec &z,
              double &rho,
              const Vec &binVec,
              const double &M,
              const TruncatedNewTonSettings &settings = {});

///
/// Computes the logistic regression cost function for a given param x and ADMM params.
///
    double computeObj(const Mat &A,
                      const Vec &b,
                      const Vec &x,
                      const Vec &y,
                      const Vec &z,
                      const double &rho);

///
/// Computes the logistic regression gradient for a given param x and ADMM params.
///
    Vec computeGrad(const Mat &A,
                    const Vec &b,
                    const Vec &x,
                    const Vec &y,
                    const Vec &z,
                    const double &rho);

///
/// Computes the logistic regression hessian for a given param x and ADMM params.
///
    Mat computeHessian(const Mat &A, const Vec &x, const double &rho);

///
/// Computes the logistic function for a given param x.
///
    Vec computeLogistic(const Mat &A, const Vec &x);

///
/// Computes the line search parameter for TN algorithm
///
    double computeLineSearch(const Mat &A,
                             const Vec &b,
                             const Vec &step,
                             const Vec &x,
                             const Vec &y,
                             const Vec &z,
                             double &rho);

    void projectLogR(Vec &x, const Vec &binVec, const double &M);
}
#endif //DOSE_LOGRSOLVER_H
