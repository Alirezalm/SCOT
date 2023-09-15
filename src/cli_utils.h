//
// Created by alireza on 14/09/23.
//

#ifndef SCOT_SRC_CLI_UTILS_H_
#define SCOT_SRC_CLI_UTILS_H_
#include "string"
#include "mpi.h"
using std::string;

/// \brief get cli help message
/// \return help message
string getHelpMessage();

/// \brief initialize MPI communicator
/// \param argc  number of arguments
/// \param argv  argument vector
/// \param rank  rank of the process
/// \param totalNodes  total number of nodes
void initializeMpi(int &argc, char *argv[], int &rank, int &totalNodes);

string toLower(string txt);
#endif // SCOT_SRC_CLI_UTILS_H_
