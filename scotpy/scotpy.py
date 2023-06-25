import json
import os
import pathlib
import random
from re import T
import shutil
import subprocess
from dataclasses import dataclass
from enum import Enum
from tabnanny import verbose
from typing import List

from numpy import ndarray
from sklearn.datasets import make_classification
from sklearn.preprocessing import normalize

HOME = os.environ.get("HOME", "")
ROOT = "scotpy"
INPUT = "inputs"
WORKING_DIR = os.path.join(HOME, ROOT)
pathlib.Path(WORKING_DIR).mkdir(exist_ok=True)


class ProblemType(Enum):
    CLASSIFICATION = 0
    REGRESSION = 1


class AlgorithmType(Enum):
    DIPOA = 0
    DIHOA = 1


class ScotModel:

    def __init__(self, problem_name: str, rank: int, kappa: int, ptype: ProblemType = ProblemType.CLASSIFICATION):
        self.name = problem_name
        self.rank = rank
        self.ptype = ptype

        self.__samples = None
        self.__response = None
        self._n_samples = None  # rows
        self._n_features = None  # cols
        self.kappa = kappa
        self.problem_dict = {}

    def set_data(self, samples: ndarray, response: ndarray, normalized_data: bool = True):
        self.__samples = samples
        self.__response = response

        if normalized_data:
            self.__normalize_data()

        self._n_samples, self._n_features = self.__samples.shape
        self.__response.reshape(self._n_samples, 1)

    def create(self):
        self.__build()
        self.__save_to_json_file()

    def __build(self):
        self.problem_dict["name"] = self.name
        self.problem_dict["version"] = "1.0.0"

        if self.ptype == ProblemType.CLASSIFICATION:
            self.problem_dict["type"] = "classification"
        elif self.ptype == ProblemType.REGRESSION:
            self.problem_dict["type"] = "regression"
        else:
            raise ValueError("unknown problem")

        self.problem_dict["response"] = [float(x) for x in self.__response]

        self.problem_dict['samples'] = []
        for row_index, row_value in enumerate(self.__samples):
            self.problem_dict["samples"].append([float(x) for x in row_value])

    def __save_to_json_file(self):
        to_json = json.dumps(self.problem_dict,
                             sort_keys=True,
                             indent=4,
                             separators=(',', ': '))

        file_name = f"node_{self.rank}_{self.name}.dist.json"
        pathlib.Path(os.path.join(WORKING_DIR, INPUT)).mkdir(exist_ok=True)
        target = os.path.join(WORKING_DIR, INPUT, file_name)
        with open(target, 'w') as jsonwriter:
            jsonwriter.write(to_json)
        print(target, "created.")

    def __normalize_data(self):
        self.__samples = normalize(self.__samples, norm='l2')
        self.__response = self.__response / max(self.__response)


@dataclass
class ScotSettings:
    algorithm: AlgorithmType = AlgorithmType.DIPOA
    time_limit: float = 1e10
    relative_gap: float = 1e-5
    verbose: bool = True


class ScotPy:

    def __init__(self, models: List[ScotModel], settings: ScotSettings):
        self.models = models
        self.settings = settings
        if os.environ.get("SCOT_HOME", "") == "":
            raise ScotPyException(
                "Solver path is unknown. make sure the executable is in the system path.")
        self.scot_path = os.environ.get("SCOT_HOME", "")
        self.total_size = len(models)
        self.cmd_args = []
        self.__generate_mpi_cmd()

    def run(self):
        print(" ".join(self.cmd_args))
        command_return = subprocess.run(self.cmd_args)
        return command_return.returncode

    def __generate_mpi_cmd(self):
        mpi_path = None
        if shutil.which("mpiexec") is not None:
            mpi_path = shutil.which("mpiexec")

        elif shutil.which("mpirun") is not None:
            mpi_path = shutil.which("mpirun")
        else:
            raise ScotPyException("Make sure MPI binaries are installed.")

        input_path = os.path.join(WORKING_DIR, INPUT)
        input_name = self.models[0].name
        n_nonzeros = self.models[0].kappa
        algorithm = self.settings.algorithm
        alg = ""
        if algorithm == AlgorithmType.DIPOA:
            alg = "dipoa"
        elif algorithm == AlgorithmType.DIHOA:
            alg = "dihoa"
        else:
            raise ScotPyException("Unknown algorithm.")
        time_limit = self.settings.time_limit
        r_gap = self.settings.relative_gap
        if self.settings.verbose:
            verbose = "--verbose"

            self.cmd_args = [
                mpi_path,
                "-n",
                str(self.total_size),
                str(os.path.join(self.scot_path, "scot")),
                f"--dir={input_path}",
                f"--input={input_name}",
                f"--nz={n_nonzeros}",
                f"--alg={alg}",
                f"--tlim={time_limit}",
                f"--rgap={r_gap}",
                verbose
            ]
        else:
            self.cmd_args = [
                mpi_path,
                "-n",
                str(self.total_size),
                str(os.path.join(self.scot_path, "scot")),
                f"--dir={input_path}",
                f"--input={input_name}",
                f"--nz={n_nonzeros}",
                f"--alg={alg}",
                f"--tlim={time_limit}",
                f"--rgap={r_gap}",
            ]


class ScotPyException(Exception):
    pass
