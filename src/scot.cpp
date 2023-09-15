/*! \mainpage SCOT
 *
 * \section intro_sec Introduction
 *
 * SCOT is an open-source distributed solver for solving Sparse Convex
Optimization (SCO) problems over computational networks.
SCOT adopts a mixed-integer approach to find exact solutions to SCO
problems and  combines various techniques to transform the original SCO problem
into an equivalent convex Mixed-Integer Nonlinear Programming (MINLP) problem
that can benefit from high-performance and parallel computing platforms. SCOT
main algorithm, DiHOA, builds upon the LP/NLPbased branch-and-bound and is
tailored for this specific problem structure. The DiHOA algorithm combines the
so-called single- and multi-tree outer approximation, naturally integrates a
decentralized algorithm for distributed convex nonlinear subproblems, and
employs enhancement techniques such as quadratic cuts.
 *
 *
 *
 */
#include "Solver/Results.h"
#include "Solver/ScotSolver.h"
#include "argh/argh.h"
#include "cli_utils.h"
#include "filesystem"
#include "fmt/include/core.h"
#include "include/Utilities.h"

using namespace std;

int main(int argc, char *argv[]) {

  int rank, totalNodes;

  initializeMpi(argc, argv, rank, totalNodes);

  Scot::ScotSolver solver;

  // todo: cli only receives the input file name and setting file name

  auto cmd = argh::parser(argc, argv);

  string helpMessage = getHelpMessage();

  if (cmd[{"-v", "--version"}]) {
    fmt::print("SCOT\tv0.1.0\n");
    MPI_Finalize();
    return 0;
  }

  int verbose = 0;
  if (cmd[{"--verbose"}]) {
    verbose = 1;
  }

  filesystem::path dirPath;

  if (cmd("help")) {
    fmt::print(helpMessage);
    MPI_Finalize();
    return 0;
  }

  if (!cmd("dir") && !cmd("input") && !cmd("nz") && !cmd("alg")) {
    fmt::print(helpMessage);
    MPI_Finalize();
    return 0;
  }

  if ((cmd("dir"))) {
    dirPath = cmd("dir").str();
  } else {
    fmt::print("input directory is not provided.\n");
    MPI_Finalize();
    return -1;
  }
  int numberOfNonzeros;
  if ((cmd("nz"))) {
    try {

      numberOfNonzeros = std::stoi(cmd("nz").str());
    } catch (std::exception &exception) {
      fmt::print(fmt::format("{}: nz must be an integer\n", exception.what()));
      MPI_Finalize();
      return -1;
    }

  } else {
    fmt::print("number of non-zeros is not provided.\n");
    MPI_Finalize();
    return -1;
  }

  if ((cmd("input"))) {

    auto file_name = cmd("input").str();
    if (file_name.find(".dist.json") == string::npos) {
      file_name += ".dist.json";
    }
    file_name = fmt::format("node_{0}_", rank) + file_name;

    dirPath.append(file_name);
  } else {
    fmt::print("input file name is not provided.\n");
    MPI_Finalize();
    return -1;
  }
  string algorithm = "dipoa";
  if (cmd("alg")) {
    algorithm = toLower(cmd("alg").str());
  }

  if (!filesystem::exists(dirPath)) {
    fmt::print(
        fmt::format("input file path {0} does not exist.\n", dirPath.string()));
    MPI_Finalize();
    return -1;
  } else {
    fmt::print(fmt::format("{0}: {1}\n", dirPath.string(), "✓"));
  };

  double max_time = 1e10;

  if (cmd("tlim")) {
    max_time = std::stod(cmd("tlim").str());
  }

  double rgap = 1e-5; // default gap //todo: handle consts

  if (cmd("rgap")) {
    rgap = std::stod(cmd("rgap").str());
  }

  // instantiate the main solver
  try {
    solver.setRank(rank);
    solver.setTotalNodes(totalNodes);
    solver.setProblemData(dirPath);
    solver.setObjective();
    solver.setNumberOfNonzeros(numberOfNonzeros);
    solver.setSparseConstraints();
    solver.setMpi();

    // set settings
    auto settings = solver.getEnvironment()->settings_;
    settings->setStrSetting("algorithm", algorithm);
    settings->setDblSetting("verbose", verbose);
    settings->setDblSetting("max_time", max_time);
    settings->setDblSetting("rel_gap", rgap);
    settings->setDblSetting(
        "abs_gap", rgap); // todo: uniform setting handling will be implemented

    if (!solver.solve()) {
      MPI_Finalize();
      return 0;
    }

  } catch (std::exception &e) {
    fmt::print(e.what());
    MPI_Finalize();
    return 0;
  }
}
