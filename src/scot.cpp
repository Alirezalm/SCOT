#include "include/Utilities.h"
#include "argh/argh.h"
#include "Solver/MainSolver.h"
#include "spdlog/spdlog.h"
#include "Solver/Results.h"
#include "fmt/include/os.h"
#include "fmt/include/core.h"
#include "filesystem"

using namespace std;
string toLower(string txt) {

  for (char &i : txt) {
    i = static_cast<char>(tolower(i));
  }
  return txt;
}
int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);

  int rank, total_nodes;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &total_nodes);
  scot::MainSolver solver;
  argh::parser cmd;
  // command line parameters
  cmd.add_param("input");
  cmd.add_param("dir");
  cmd.add_param("nz");
  cmd.add_param("help");
  cmd.add_param("alg");
  cmd.add_param("tlim");
  cmd.add_param("rgap");

  string help_message = "usage: SCOT [OPTIONS] \n\n--dir\tdirectory of input files\n\n--input\tinput file name "
                        "without prefix and postfix\n\n--nz\tnumber of non-zeros\n\n--alg\talgorithm"
                        "\n\n--tlim\ttime limit\n\n\n--rgap\trelative gap\n";

  cmd.parse(argc, argv);
  if (cmd[{"-v", "--version"}]) {
    fmt::print("SCOT\tv0.1.0\n");
    MPI_Finalize();
    return 0;
  }

  int verbose = 0;
  if (cmd[{"--verbose"}]){
    verbose = 1;
  }

  filesystem::path dir_path;

  if (cmd("help")) {
    fmt::print(help_message);
    MPI_Finalize();
    return 0;
  }

  if (!cmd("dir") && !cmd("input") && !cmd("nz") && !cmd("alg")) {
    fmt::print(help_message);
    MPI_Finalize();
    return 0;
  }

  if ((cmd("dir"))) {
    dir_path = cmd("dir").str();
  } else {
    fmt::print("input directory is not provided.\n");
    MPI_Finalize();
    return -1;
  }
  int nnzeros;
  if ((cmd("nz"))) {
    try {

      nnzeros = std::stoi(cmd("nz").str());
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

    dir_path.append(file_name);
  } else {
    fmt::print("input file name is not provided.\n");
    MPI_Finalize();
    return -1;
  }
  string algorithm = "dipoa";
  if (cmd("alg")) {
    algorithm = toLower(cmd("alg").str());

  }

  if (!filesystem::exists(dir_path)) {
    fmt::print(fmt::format("input file path {0} does not exist.\n", dir_path.string()));
    MPI_Finalize();
    return -1;
  } else {
    fmt::print(fmt::format("{0}: {1}\n", dir_path.string(), "✓"));
  };

  double max_time = 1e10;

  if (cmd("tlim")) {
    max_time = std::stod(cmd("tlim").str());
  }

  double rgap = 1e-5; //default gap //todo: handle consts

  if (cmd("rgap")) {
    rgap = std::stod(cmd("rgap").str());
  }

  // instantiate the main solver
  try {
    solver.setRank(rank);
    solver.setTotalNodes(total_nodes);
    solver.setProblemData(dir_path);
    solver.setObjective();
    solver.setNumberOfNonzeros(nnzeros);
    solver.setSparseConstraints();
    solver.setMpi();

    // set settings
    auto settings = solver.getEnvironment()->settings_;
    settings->setStrSetting("algorithm", algorithm);
    settings->setDblSetting("verbose", verbose);
    settings->setDblSetting("max_time", max_time);
    settings->setDblSetting("rel_gap", rgap);
    settings->setDblSetting("abs_gap", rgap); //for now

    if (!solver.solve()){
      MPI_Finalize();
      return 1; //just for benchmark test todo: must be zero in final release
    }

  } catch (std::exception &e) {
    fmt::print(e.what());
    MPI_Finalize();
    return 0;
  }

  MPI_Finalize();
  return 0;

}