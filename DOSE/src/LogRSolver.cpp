//
// Created by alireza on 16/05/22.
//

#include "../include/LogRSolver.h"
namespace dose {
    Vec runTN(const Mat &A,
              const Vec &b,
              Vec &x,
              Vec &y,
              Vec &z,
              double &rho,
              const Vec &binVec,
              const double &M,
              const dose::TruncatedNewTonSettings
              &settings) {

        size_t iter = 0;
        double error = 1e5;
        double fval;
        double t;
        Vec g, step, h;
        Mat H;

        while ((error > settings.eps) && (iter <= settings.maxIter)) {
            ++iter;
            fval = computeObj(A, b, x, y, z, rho);
            g = computeGrad(A, b, x, y, z, rho);
            H = computeHessian(A, x, rho);
            ConjugateGradient<Mat> cg(H);
            step = cg.solve(-g);
            t = computeLineSearch(A, b, step, x, y, z, rho);
            x += t * step;
            error = g.squaredNorm();
            if (settings.verbose) {
                std::cout << "iter = " << iter << " " << "fval = " <<
                          fval << " " << "error = " << error << " t = " << t << std::endl;
            }
//            projectLogR(x, binVec, M);
            if (iter >= settings.maxIter) {
                return x;
            }
        }

//        projectLogR(x, binVec, M);
        return x;


    }

    double computeObj(const Mat &A,
                      const Vec &b,
                      const Vec &x,
                      const Vec &y,
                      const Vec &z,
                      const double &rho) {
        Vec h = computeLogistic(A, x);
        auto m = static_cast<double>(h.size()) ;
        auto f = -b.dot(h.array().log().matrix()) -
                 (1.0 - b.array()).matrix().dot((1.0 - h.array()).log().matrix());
        f /= m;
        return f + y.dot(x - z) + 0.5 * rho * (x - z).squaredNorm();


    }

    Vec computeLogistic(const Mat &A, const Vec &x) {
        Vec z = -A * x;
        Vec h = 1.0 / (1.0 + z.array().exp());
        for (int j = 0; j < h.size(); ++j) {
            if (h[j] == 1.0) {
                h[j] = 1 - 1e-8;
            } else if (h[j] == 0.0) {
                h[j] = 1e-8;
            }
        }
        return h.matrix();

    }

    Vec computeGrad(const Mat &A,
                    const Vec &b,
                    const Vec &x,
                    const Vec &y,
                    const Vec &z,
                    const double &rho) {
        Vec h = computeLogistic(A, x);
        auto m = static_cast<double>(h.size()) ;
        return (1.0 / m) * (A.transpose() * (h - b)) + y + rho * (x - z);
    }

    double computeLineSearch(const Mat &A,
                             const Vec &b,
                             const Vec &step,
                             const Vec &x,
                             const Vec &y,
                             const Vec &z,
                             double &rho) {
        double t = 1.0;
        double alpha = 1e-4;
        double beta = 0.5;
        Vec temp = x + t * step;
        double phi = computeObj(A, b, temp, y, z, rho);
        double phi_prime_0 = computeGrad(A, b, x, y, z, rho).dot(step);
        double phi0 = computeObj(A, b, x, y, z, rho);
        int iter = 0;
        const int max_iter = 500;
        while ((phi > phi0 + alpha * t * phi_prime_0) && iter < max_iter) {
            ++iter;
            t *= beta;
            temp = x + t * step;
            phi = computeObj(A, b, temp, y, z, rho);
            if (iter == max_iter - 1) {
                std::cout << "line search was not successful." << std::endl;
            }
        }
        return t;
    }

    Mat computeHessian(const Mat &A, const Vec &x, const double &rho) {
        const Vec h = computeLogistic(A, x);
//        const Array<double, Dynamic, 1> _tmpArray = h.array();
        const Vec _temp = h.array() - h.array().pow(2);
        const Ind m = A.rows(), n = A.cols();
        Mat M(m, n);
        for (int j = 0; j < m; ++j) {
            M.row(j) = A.row(j) * _temp(j);
        }

        Mat I(n, n);
 
        return (1.0 / static_cast<int>(m)) * (A.transpose() * M) + rho * I.setIdentity();


    }


    void projectLogR(Vec &x, const Vec &binVec, const double &M) {
        x = (M * binVec).cwiseMin((-M * binVec).cwiseMax(x));
    }

}