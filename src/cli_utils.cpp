//
// Created by alireza on 14/09/23.
//

#include "cli_utils.h"
#include "sstream"
#include "iostream"

using std::ostringstream;
using std::string;

const string OPTION_DIR = "--dir";
const string OPTION_INPUT = "--input";
const string OPTION_NZ = "--nz";
const string OPTION_ALG = "--alg";
const string OPTION_TLIM = "--tlim";
const string OPTION_RGAP = "--rgap";
const string OPTION_VERBOSE = "--verbose";
const string OPTION_BIG_M = "--ub";

const string DESCRIPTION_DIR = "Directory of input files";
const string DESCRIPTION_INPUT = "Input file name (without prefix and postfix)";
const string DESCRIPTION_NZ = "Number of non-zeros";
const string DESCRIPTION_ALG = "Algorithm to use";
const string DESCRIPTION_TLIM = "Time limit";
const string DESCRIPTION_RGAP = "Relative gap";
const string DESCRIPTION_VERBOSE = "Verbosity";
const string DESCRIPTION_BIG_M = "Big-M parameter";

void initializeMpi(int &argc, char *argv[], int &rank, int &totalNodes) {
  int initResult = MPI_Init(&argc, &argv);

  if (initResult != MPI_SUCCESS) {
    std::cerr << "MPI initialization failed. Exiting...\n";

    MPI_Abort(MPI_COMM_WORLD, initResult);
  }

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &totalNodes);
}

string getHelpMessage() {
  ostringstream message;
  message << "Usage: SCOT [OPTIONS]\n\n";
  message << "Options:\n";
  message << "  " << OPTION_DIR << "\t" << DESCRIPTION_DIR << "\n";
  message << "  " << OPTION_INPUT << "\t" << DESCRIPTION_INPUT << "\n";
  message << "  " << OPTION_NZ << "\t" << DESCRIPTION_NZ << "\n";
  message << "  " << OPTION_ALG << "\t" << DESCRIPTION_ALG << "\n";
  message << "  " << OPTION_TLIM << "\t" << DESCRIPTION_TLIM << "\n";
  message << "  " << OPTION_RGAP << "\t" << DESCRIPTION_RGAP << "\n";
  message << "  " << OPTION_VERBOSE << "\t" << DESCRIPTION_VERBOSE << "\n";
  message << "  " << OPTION_BIG_M << "\t" << DESCRIPTION_BIG_M << "\n";

  return message.str();
}
void toLower(string &txt) {
  for (char &letter : txt) {
    letter = static_cast<char>(tolower(letter));
  }
}