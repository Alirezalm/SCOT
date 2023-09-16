# Sparse Convex Optimization Toolkit (SCOT)

**SCOT** is an open-source distributed optimization solver designed to tackle Sparse Convex Optimization (SCO) problems 
within computational networks.

## Overview

**SCOT** can be employed both as a standalone executable application and as a  Python API known as SCOTPY, offering flexibility in integrating it into your projects.

## Key Features

- Solve Sparse Convex Optimization problems with ease.
- Utilize **SCOT** via its Python API, SCOTPY.
- Distribute optimization tasks efficiently across computational networks.
- Compatible with various optimization algorithms for diverse problem domains.
- Easily customizable and extensible for specific use cases.



## Installation

### Dependencies

To set up **SCOT**, you need the following prerequisites:

1. **gcc Compiler**: Ensure you have a C++17-compatible compiler.
2. **Gurobi Optimizer**: Version 10 or higher is required. [Quick Start Guide](https://www.gurobi.com/documentation/quickstart.html).
3. **Message Passing Interface (MPI)**:
   - [OpenMPI](https://www.open-mpi.org/)
   - [MPICH](https://www.mpich.org/)
   - [Microsoft MPI (for Windows)](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi)
4. **CMake**

### Supported Platforms

**SCOT** is compatible with the following platforms:

1. Ubuntu 21.10 or WSL2.
2. macOS


## Installation on Ubuntu OS

### Build with CMake

Clone this repository
```commandline
git clone https://github.com/Alirezalm/scot.git
```
Ensure that the ```GUROBI_HOME``` environment variable is defined, pointing to your Gurobi installation:
```commandline
export GUROBI_HOME=/path/to/gurobi
```
Navigate to the cloned repository and execute the following commands:

```commandline
mkdir build
cd build
cmake ../
make install
```
Upon successful installation, you will find the ```scot_framework``` folder in the project's root directory,
containing the following structure:

```
├── bin
│   └── scot
└── lib
    ├── libalgorithms.so
    ├── libdose.so
    ├── libmip.so
    ├── libmodel.so
    ├── libprimal.so
    ├── libsolver.so
    └── libtasks.so
```
To ensure the shared libraries in the ```lib``` folder are found by the OS, you can use the following command:
```commandline
export LD_LIBRARY_PATH=./lib:$GUROBI_HOME/lib
```
Now, you can execute ```SCOT``` help as follows to make sure the application is compiled correctly:
```commandline
 ./bin/scot --help
```
```commandline
Usage: SCOT [OPTIONS]

Options:
  --dir         Directory of input files
  --input       Input file name (without prefix and postfix)
  --nz          Number of non-zeros
  --alg         Algorithm to use
  --tlim        Time limit
  --rgap        Relative gap
  --verbose     Verbosity
  --ub          Big-M parameter
```
## Usage
SCOT MPI Command-Line Interface (SMCLI) can be employed to utilize SCOT as a solver, provided appropriate input files 
in ```.dist.json``` format are available.

### Example: Distributed Sparse Logistic Regression (DSLogR)
Here's how to solve a DSLogR problem using the ```SMCLI``` interface:

1. Prepare problem data for each node in ```JSON``` format. Assuming two nodes, name the JSON files following this
convention:
```commandline
node_{mpi_rank}_{problem_name}.dist.json
```
For two nodes and the problem name ```logistic_regression```, the files should be named as:
```commandline
1. node_{0}_{logistic_regression}.dist.json
2. node_{1}_{logistic_regression}.dist.json
```
The exact ```JSON``` format for these files can be found in the ``data`` folder.
2. Execute ```SCOT``` with default settings:
```commandline
mpirun -n 2 ./bin/scot --dir=/path/to/.dist.json/files --input=logistic_regression --nz=2 
```
3. After successful execution, two output files will be created in the ```scot_framework``` folder:
```commandline
1. rank_0_output.json
2. rank_1_output.json
```
These files contain essential information about the solution to the given problem.