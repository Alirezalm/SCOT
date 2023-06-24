//
// Created by alireza on 21/05/22.
//

#ifndef DOSE_RANDOMDATAGENERATOR_H
#define DOSE_RANDOMDATAGENERATOR_H

#include "random"
#include "types.h"

namespace dose {
    class RandomDataGenerator {
    public:
        RandomDataGenerator();

        RandomDataGenerator(int nrows, int ncols, ProblemType ptype);

        ~RandomDataGenerator();

        void generate();

        VectorDouble getRandomInteger(int size);

        const VectorDouble &getResponse() const;

        const VectorDouble2D &getDataSet() const;

    private:
        void randomResponseLogRegGenerator();

        void randomResponseLinRegGenerator();

        void randomDataSetGenerator();
        int nrows;
        int ncols;
        ProblemType ptype;
        std::default_random_engine generator;
        std::normal_distribution<double> normal_distribution_doubles;
        std::uniform_int_distribution<int> uniform_distribution_ints;
        VectorDouble response;
        VectorDouble2D dataSet;

    };

}


#endif //DOSE_RANDOMDATAGENERATOR_H
