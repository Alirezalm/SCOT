//
// Created by alireza on 17/05/22.
//
#ifndef DOSE_EXCEPTIONS_H
#define DOSE_EXCEPTIONS_H

#include "exception"

namespace dose {
    class InvalidProblemSize : public std::exception {
    private:
        const char *err;
    public:
        InvalidProblemSize(){
            err = "invalid problem size";
        };

        InvalidProblemSize(const char *err):err(err) {};

        const char * what() const noexcept override{
            return err;
        }
    };
}
#endif //DOSE_EXCEPTIONS_H
