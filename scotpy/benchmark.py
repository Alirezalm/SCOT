import math
from dataclasses import dataclass
from enum import Enum
import json
import os
from typing import Dict, List
import numpy as np
from matplotlib.ticker import MaxNLocator
from sklearn.datasets import make_classification

from scotpy import AlgorithmType, ProblemType, ScotModel, ScotPy, ScotSettings
import random
from pyomo.environ import *

import matplotlib.pyplot as plt

BIG_M = 30


class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


class Solvers(Enum):
    SCOT = "dipoa"
    SCOTH = "dihoa"
    SHOT = "shot-mt"
    SHOTH = "shot-st"
    BONMIN = "bonmin-mt"
    BONMINH = "bonmin-st"
    DICOPT = "dicopt"
    KNITRO = "knitro"


DEFAULT_SOLVER_LIST = [
    # Solvers.SHOT,
    # Solvers.BONMIN,
    Solvers.SCOT,
    # Solvers.BONMINH,
    # Solvers.SHOTH,
    Solvers.SCOTH,
    # Solvers.KNITRO,

]


class MINLP:

    def __init__(self, filepath: str, filename: str, total_size: int) -> None:
        self.filepath = filepath
        self.filename = filename
        self.total_size = total_size

        self.n_samples = None
        self.n_features = None
        self.dataset = None
        self.response = None
        self.model = None

        self.sample_list = []
        self.response_list = []

        self.__create()

    def solve(self, solver_name: str, time_limit: float, eps: float, verbose: bool):

        solver = SolverFactory('gams')

        options = [f'GAMS_MODEL.reslim = {time_limit};', f"GAMS_MODEL.optca = {eps};", f"GAMS_MODEL.optcr = {eps};",
                   'GAMS_MODEL.optfile = 1;']
        if solver_name.lower() == "shot-mt":
            shot_opts = ["$onecho > shot.opt",
                         "Dual.TreeStrategy=0",
                         "Dual.MIP.Solver=1",
                         "Model.Convexity.AssumeConvex=1",
                         "Model.BoundTightening.FeasibilityBased.Use=false",
                         "Model.BoundTightening.FeasibilityBased.UseNonlinear=false",
                         "Model.BoundTightening.InitialPOA.Use=false",
                         "$offecho"]

            options += shot_opts

            solver_str = "shot"

        elif solver_name.lower() == "shot-st":
            shot_opts = ["$onecho > shot.opt",
                         "Model.Convexity.AssumeConvex=1",
                         "Dual.TreeStrategy=1",
                         "Dual.MIP.Solver=1",
                         "Model.BoundTightening.FeasibilityBased.Use=false",
                         "Model.BoundTightening.FeasibilityBased.UseNonlinear=false",
                         "Model.BoundTightening.InitialPOA.Use=false",
                         "$offecho"]

            options += shot_opts
            solver_str = "shot"
        elif solver_name.lower() == "bonmin-mt":
            bonmin_opts = ["$onecho > bonmin.opt",
                           "bonmin.algorithm    B-OA",  # multiple tree
                           #    "bonmin.algorithm    B-QG",  # single tree
                           #    "bonmin.milp_solver   Cplex",
                           "$offecho"]

            options += bonmin_opts
            solver_str = "bonmin"

        elif solver_name.lower() == "bonmin-st":
            bonmin_opts = ["$onecho > bonmin.opt",
                           # "bonmin.algorithm    B-OA",  # multiple tree
                           "bonmin.algorithm    B-QG",  # single tree
                           #    "bonmin.milp_solver   Cplex",
                           "$offecho"]

            options += bonmin_opts
            solver_str = "bonmin"

        elif solver_name.lower() == "dicopt":
            dicopt_opts = ["$onecho > dicopt.opt",
                           "convex 1",
                           "nlpsolver ipopt",
                           # "stop  0",
                           # f"optca {eps}",
                           # f"optcr {0}",
                           f"MAXCYCLES 100",

                           "$offecho"]

            options += dicopt_opts
            solver_str = "dicopt"
        else:
            solver_str = solver_name

        try:
            res = solver.solve(self.model, solver = solver_str, tee = verbose, keepfiles = False,
                               load_solutions = False,
                               add_options = options)
            elapsed_time = res.solver.user_time
            status = res.solver.termination_condition
            gap = res.solution.gap

        except Exception as err:
            print("error: ", err)
            return -1

        # if (not math.isnan(gap) and gap <= eps) and (status == "optimal"):
        if not math.isnan(gap):
            if gap <= eps:
                print(
                    Colors.OKGREEN + Colors.BOLD + f"solver: {solver_name}, gap: {gap:6.4f}, status: {status}" + Colors.ENDC)
                return 0
            else:
                print(
                    Colors.WARNING + Colors.BOLD + f"solver: {solver_name}, gap: {gap:6.4f}, status: not optimal" + Colors.ENDC)
        else:
            print(Colors.WARNING + Colors.BOLD + f"solver: {solver_name}, status: {status}" + Colors.ENDC)
            return -1

    def create(self, kappa: int):
        self.model = ConcreteModel()

        self.model.x = Var(range(self.n_features))
        self.model.delta = Var(range(self.n_features), within = Binary)

        self.model.obj = Objective(expr = self.__objective(), sense = minimize)

        self.model.limits = ConstraintList()
        for i in range(self.n_features):
            self.model.limits.add(
                self.model.x[i] <= BIG_M * self.model.delta[i])
            self.model.limits.add(-BIG_M *
                                  self.model.delta[i] <= self.model.x[i])
        self.model.limits.add(sum([self.model.delta[i]
                                   for i in range(self.n_features)]) <= kappa)

    def __objective(self):

        assert len(self.sample_list) == len(self.response_list)

        obj = 0.0
        m = len(self.sample_list[0])
        for (X, y) in zip(self.sample_list, self.response_list):
            f = sum([-y[i] * log(self.__h(X[i])) - (1.0 - y[i])
                     * log(1 - self.__h(X[i])) for i in range(m)])
            f /= m

            obj += f
            # break  # todo: remove

        return obj

    def __h(self, sample: List[float]):
        z = sum([self.model.x[i] * sample[i] for i in range(self.n_features)])
        return 1 / (1 + exp(-z))

    def __create(self):

        filename_list = [os.path.join(
            self.filepath, f"node_{i}_{self.filename}.dist.json") for i in range(self.total_size)]

        self.n_samples = 0
        for file in filename_list:
            with open(file) as input:
                data = json.load(input)

            self.sample_list.append(data['samples'])
            self.response_list.append(data['response'])

            self.n_samples += len(data['samples'])

        self.n_features = len(self.sample_list[0][0])


@dataclass
class SolverSettings:
    rgap: float
    tlim: float
    verbose: bool


class Benchmark:

    def __init__(
            self,
            max_num_var: int,
            min_num_var: int,
            max_num_row: int,
            min_num_row: int,
            n_nodes: int,
            n_problems: int,
            pname: str,
            level_of_sparsity: float,
    ):
        assert min_num_row < max_num_row, "min row < max row"
        assert min_num_var < max_num_var, "min var < max var"

        self.max_num_var = max_num_var
        self.min_num_var = min_num_var
        self.max_num_row = max_num_row
        self.min_num_row = min_num_row
        self.n_nodes = n_nodes
        self.n_problems = n_problems
        self.pname = pname
        self.level_of_sparsity = level_of_sparsity
        self.kappa = None
        self.problems: List[List[ScotModel]] = []

        print(self)

    def run(self, solver: Solvers, settings: SolverSettings):
        print(f"running benchmark for {solver.value} with time limit: {settings.tlim:6.2f} ...")

        if solver == Solvers.SCOT:

            scot_settings = ScotSettings(
                relative_gap = settings.rgap,
                time_limit = settings.tlim,
                verbose = settings.verbose,
                algorithm = AlgorithmType.DIPOA
            )

            success = self.__run_scot(scot_settings)
            print(
                Colors.UNDERLINE + Colors.OKBLUE + Colors.BOLD + f"{solver.name} -> Number of problem solved: {success} / {self.n_problems}" + Colors.ENDC)
            return success

        elif solver == Solvers.SCOTH:

            scot_settings = ScotSettings(
                relative_gap = settings.rgap,
                time_limit = settings.tlim,
                verbose = settings.verbose,
                algorithm = AlgorithmType.DIHOA
            )
            success = self.__run_scot(scot_settings)
            print(
                Colors.UNDERLINE + Colors.OKBLUE + Colors.BOLD + f"{solver.name} -> Number of problem solved: {success} / {self.n_problems}" + Colors.ENDC)
            return success
        else:
            success = self.__run_gms_solver(solver = solver, settings = settings)
            print(f"{solver.value} -> Number of problem solved: {success} / {self.n_problems}")
            return success

    def __run_gms_solver(self, solver: Solvers, settings: SolverSettings):
        path = os.environ.get("HOME", "") + "/scotpy/inputs/"
        num_prob_solved = 0
        for i in range(self.n_problems):
            filename = self.pname + "_" + str(i)
            minlp_solver = MINLP(
                filepath = path, filename = filename, total_size = self.n_nodes)
            minlp_solver.create(kappa = self.kappa)
            return_code = minlp_solver.solve(solver_name = solver.value, time_limit = settings.tlim,
                                             eps = settings.rgap, verbose = settings.verbose)

            if return_code == 0:
                num_prob_solved += 1
            print(f"solver: {solver.value}, solved problem {num_prob_solved} / {self.n_problems}")
        return num_prob_solved

    def __run_scot(self, settings: ScotSettings):

        number_of_problem_solved = 0

        for problem in self.problems:
            solver = ScotPy(problem, settings)
            status_code = solver.run()
            if status_code == 0:
                number_of_problem_solved += 1
        return number_of_problem_solved

    def generate_random_problems(self):

        for i in range(self.n_problems):
            random_n_rows = random.randrange(
                self.min_num_row, self.max_num_row)

            random_n_vars = random.randrange(
                self.min_num_var, self.max_num_var)

            self.kappa = int(
                (random_n_vars * self.level_of_sparsity) / 100) + 1

            problem = Benchmark.create_instance(m = random_n_rows,
                                                n = random_n_vars,
                                                total_nodes = self.n_nodes,
                                                pname = self.pname +
                                                        "_" + str(i),
                                                kappa = self.kappa,
                                                )
            self.problems.append(problem)

    def __str__(self) -> str:

        info = f"column range: [{self.min_num_var}, {self.max_num_var}]\n" + \
               f"row range: [{self.min_num_row}, {self.max_num_row}]\n" + \
               f"number of nodes: {self.n_nodes}\n" + \
               f"benchmark name: {self.pname}\n" + \
               f"number of problems: {self.n_problems}\n" + \
               f"level of sparsity: {100 - self.level_of_sparsity}\n" + \
               "_______________________________________________________________\n"

        return info

    @staticmethod
    def create_instance(m: int, n: int, total_nodes: int, pname: str, kappa: int) -> List[ScotModel]:
        models: List[ScotModel] = []
        for rank in range(total_nodes):
            dataset, res = make_classification(
                n_samples = m, n_features = n, n_redundant = 0, n_repeated = 0)
            scp = ScotModel(problem_name = pname, rank = rank,
                            kappa = kappa, ptype = ProblemType.CLASSIFICATION)
            scp.set_data(dataset, res, normalized_data = True)
            scp.create()
            models.append(scp)

        return models


@dataclass
class BenchmarkSettings:
    max_t: int = 100
    max_n: int = 10
    max_m: int = 100
    min_n: int = 3
    min_m: int = 20
    density_level: float = 10
    n_problems: int = 5
    n_nodes: int = 2
    name: str = "default_benchmark"
    verbose: bool = False
    solver_gap: float = 1e-4


class TimeBenchmark:
    def __init__(self, bench_settings: BenchmarkSettings = BenchmarkSettings()) -> None:
        self.bench_settings = bench_settings

        self.benchmark = Benchmark(
            max_num_var = self.bench_settings.max_n,
            min_num_var = self.bench_settings.min_n,
            max_num_row = self.bench_settings.max_m,
            min_num_row = self.bench_settings.min_m,
            n_nodes = self.bench_settings.n_nodes,
            n_problems = self.bench_settings.n_problems,
            pname = self.bench_settings.name,
            level_of_sparsity = self.bench_settings.density_level,
        )
        self.default_time = None

    def __run(self):
        max_time_lin = self.bench_settings.max_t / 3
        step = 0.5
        default_time_list_1 = np.linspace(step, max_time_lin, 10)  # todo:
        default_time_list_2 = np.linspace(
            max_time_lin + step, self.bench_settings.max_t, 20)
        self.default_time = default_time_list_1.tolist() + default_time_list_2.tolist()

        benchmark_results = {}

        for solver in DEFAULT_SOLVER_LIST:
            num_prob_solved = 0
            benchmark_results[solver.value] = []
            for time in self.default_time:
                if num_prob_solved == self.bench_settings.n_problems:
                    benchmark_results[solver.value].append(num_prob_solved)
                else:
                    solver_settings = SolverSettings(rgap = self.bench_settings.solver_gap,
                                                     tlim = time,
                                                     verbose = self.bench_settings.verbose)

                    num_prob_solved = self.benchmark.run(solver = solver, settings = solver_settings)

                    benchmark_results[solver.value].append(num_prob_solved)

            benchmark_results["time"] = self.default_time
            self.__to_json(benchmark_res = benchmark_results)
        return benchmark_results

    def __to_json(self, benchmark_res: Dict):

        json_string = json.dumps(benchmark_res,
                                 sort_keys = True,
                                 indent = 4,
                                 separators = (',', ': '))

        res_json = f"results_n_{self.bench_settings.max_n}_p_{self.bench_settings.n_problems}_m_{self.bench_settings.max_m * self.bench_settings.n_nodes}.json"

        with open(res_json, 'w') as jsonwriter:
            jsonwriter.write(json_string)

    def run(self):
        print("creating problem set ...\n")
        self.benchmark.generate_random_problems()

        res = self.__run()
        # self.__visualize_results(results = res)
        return res

    def __visualize_results(self, results: Dict[str, List[float]]):
        fig, ax = plt.subplots()

        for solver, result in results.items():
            ax.plot(self.default_time, result, label = solver)

        ax.legend()
        ax.set_xlabel("time (seconds)")
        ax.set_ylabel("number of problems")

        plt.grid()
        plt.show()


def plot_results_from_file(filename: str):
    with open(filename, 'r') as reader:
        results = json.load(reader)
    fs = 10
    markers = ["o", "s", "*", "h", "+", "x", "D"]
    colors = ['midnightblue', 'mediumslateblue', 'mediumorchid', 'mediumseagreen', 'slategray', 'olive', 'red']
    fig, ax = plt.subplots(figsize = (5, 4))
    i = 0
    for key, res in results.items():
        if key != "time":
            ax.step(results["time"], res, label = key, marker = markers[i], markersize = 4, color = colors[i])
            # ax.yaxis.set_major_locator(MaxNLocator(integer = True))
        ax.set_yticks(range(0, 20 + 1, 2))
        i += 1
    plt.legend(bbox_to_anchor = (0, 1.02, 1, 0.2), loc = "lower left",
               mode = "expand", borderaxespad = 0, ncol = 4, fontsize = fs)
    ax.set_xlabel("time (seconds)", fontsize = fs)
    ax.set_ylabel("number of problems", fontsize = fs)
    plt.grid()
    figname = filename.split(".")[0] + ".pdf"
    plt.savefig(f"./figs/{figname}", format = "pdf")

    # plt.show()


if __name__ == '__main__':
    bs = BenchmarkSettings(
        max_t = 50,
        max_n = 100,
        max_m = 75000,
        min_n = 20,
        min_m = 1000,
        density_level = 20,
        n_problems = 20,
        n_nodes = 4,
        name = "benchmark_2",
        verbose = False,
        solver_gap = 1e-3
    )
    tm = TimeBenchmark(bs)
    result = tm.run()
    print(result)
