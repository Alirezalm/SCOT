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
vector<string> algorithms = {"dipoa", "dihoa"};

int main(int argc, char *argv[]) {

  int rank, totalNodes;

  initializeMpi(argc, argv, rank, totalNodes);

  // todo: cli only receives the input file name and setting file name
  Scot::ScotSolver solver;
  auto settings = solver.getEnvironment()->settings_;
  auto cmd = argh::parser(argc, argv);

  if (cmd[{"-v", "--version"}]) {
    fmt::print("SCOT\t0.1.0\n");
    MPI_Finalize();
    return 0;
  }

  int verbose = 0;
  if (cmd[{"--verbose"}]) {
    verbose = 1;
  }

  string helpMessage = getHelpMessage();
  if (cmd("help")) {
    fmt::print(helpMessage);
    MPI_Finalize();
    return 0;
  }

  filesystem::path dirPath;
  if (!cmd("dir") && !cmd("input") && !cmd("nz") && !cmd("alg")) {
    fmt::print(helpMessage);
    MPI_Finalize();
    return 0;
  }

  if (!(cmd("dir"))) {
    fmt::print("input directory is not provided.\n");
    MPI_Finalize();
    return 0;
  }

  dirPath = cmd("dir").str();
  int numberOfNonzeros;

  if (!(cmd("nz"))) {
    fmt::print("number of non-zeros is not provided.\n");
    MPI_Finalize();
    return 0;
  }

  try {
    numberOfNonzeros = std::stoi(cmd("nz").str());
    if (numberOfNonzeros < 1) {
      fmt::print("number of non-zeros is not provided.\n");
      MPI_Finalize();
      return 0;
    }
  } catch (std::exception &exception) {
    fmt::print(
        fmt::format("{}: nz must be a positive integer\n", exception.what()));
    MPI_Finalize();
    return 0;
  }

  if (!(cmd("input"))) {
    fmt::print("input file name is not provided.\n");
    MPI_Finalize();
    return 0;
  }
  string fileName = cmd("input").str();

  if (fileName.find(".dist.json") == string::npos) {
    fileName += ".dist.json";
  }
  fileName = fmt::format("node_{0}_", rank) + fileName;

  dirPath.append(fileName);

  string algorithm = "dipoa";
  if (cmd("alg")) {
    algorithm = cmd("alg").str();
    toLower(algorithm);
  }
  auto foundAlgorithm =
      std::find(algorithms.begin(), algorithms.end(), algorithm);

  if (foundAlgorithm == algorithms.end()) {
    fmt::print("{0} algorithm does not exit", algorithm);
    MPI_Finalize();
    return 0;
  }

  if (!filesystem::exists(dirPath)) {
    auto fse =
        fmt::format("input file path {0} does not exist.\n", dirPath.string());
    fmt::print(fse);
    MPI_Finalize();
    return 0;
  }
  fmt::print(fmt::format("{0}: {1}\n", dirPath.string(), "✓"));

  double maxTime = 1e10;

  if (cmd("tlim")) {
    maxTime = std::stod(cmd("tlim").str());
  }

  double rgap = 1e-5;

  if (cmd("rgap")) {
    rgap = std::stod(cmd("rgap").str());
  }

  double bigM = 1e3; // default Big-M parameter

  if (cmd("ub")) {
    bigM = std::stod(cmd("ub").str());
    if (bigM <= 0) {
      fmt::print("Error: Big-M value must be positive");
      MPI_Finalize();
      return 0;
    }
  }
  settings->setDblSetting("variable_bound", bigM);
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

    settings->setStrSetting("algorithm", algorithm);
    settings->setDblSetting("verbose", verbose);

    settings->setDblSetting("max_time", maxTime);
    settings->setDblSetting("rel_gap", rgap);

    settings->setDblSetting("abs_gap", rgap);

    if (solver.solve()) {
      MPI_Finalize();
      return 0;
    }
    fmt::print("SCOT did not converge");
    MPI_Finalize();
    return 0;

  } catch (std::exception &e) {
    fmt::print(e.what());
    MPI_Finalize();
    return 0;
  }
}
