//
// Created by alireza on 21/05/22.
//

#include "../include/RandomDataGenerator.h"
// TODO: IMPLEMENT with different seed

namespace dose {

    RandomDataGenerator::RandomDataGenerator() {
        ncols = 1;
        nrows = 1;
        ptype = ProblemType::LinearRegression;
    }

    RandomDataGenerator::RandomDataGenerator(int nrows,
                                             int ncols,
                                             ProblemType ptype) : nrows(nrows),
                                                                  ncols(ncols),
                                                                  ptype(ptype) {}

    void RandomDataGenerator::randomResponseLogRegGenerator() {
        response.resize(nrows);
        uniform_distribution_ints = std::uniform_int_distribution<int>(0, 1);
        for (int i = 0; i < nrows; ++i) {
            response.at(i) = uniform_distribution_ints(generator);
        }
    }

    void RandomDataGenerator::randomResponseLinRegGenerator() {
        response.resize(nrows);
        normal_distribution_doubles = std::normal_distribution<double>(-1.0, 1.0);
        for (int i = 0; i < nrows; ++i) {
            response.at(i) = normal_distribution_doubles(generator);
        }
    }

    void RandomDataGenerator::randomDataSetGenerator() {
        dataSet.resize(nrows);
        for (int i = 0; i < nrows; ++i) {
            dataSet.at(i).resize(ncols);
            for (int j = 0; j < ncols; ++j) {
                dataSet.at(i).at(j) = normal_distribution_doubles(generator);
            }
        }
    }

    void RandomDataGenerator::generate() {
        switch (ptype) {
            case ProblemType::LogisticRegression:
                randomResponseLogRegGenerator();
                break;
            case ProblemType::LinearRegression:
                randomResponseLinRegGenerator();
                break;
        }
        randomDataSetGenerator();

    }

    const VectorDouble &RandomDataGenerator::getResponse() const {
        assert(response.size() == nrows);
        return response;
    }

    const VectorDouble2D &RandomDataGenerator::getDataSet() const {
        assert(dataSet.front().size() == ncols);
        assert(dataSet.size() == nrows);
        return dataSet;
    }

    VectorDouble RandomDataGenerator::getRandomInteger(int size) {
        VectorDouble v(size);
        uniform_distribution_ints = std::uniform_int_distribution<int>(0, 1);
        for (int i = 0; i < size; ++i) {
            v.at(i) = uniform_distribution_ints(generator);
        }
        return v;
    }

    RandomDataGenerator::~RandomDataGenerator() = default;
}