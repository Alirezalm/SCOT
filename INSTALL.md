## Installation Guide

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

## Numerical Benchmarks

### Small scale instances

![sc I](./media/scenario_1_90.jpg)

### Medium scale instances

![sc II](./media/scenario_3_90.jpg)

### Large scale instances

![sc III](./media/scenario_3_90.jpg)

## Publications

1. A. Olama, *A Distributed Framework for Sparse Convex Optimization: Algorithms and Tools.* PhD thesis,
Federal University of Santa Catarina (UFSC), Brazil, 2023.
2. Olama, E. Camponogara, and P. R. Mendes, *Distributed primal outer approximation algorithm for sparse
convex programming with separable structures,* Journal of Global Optimization, vol. 86, no. 3, pp. 637–670, 2023.
3. A. Olama, E. Camponogara, and J. Kronqvist, *Sparse convex optimization toolkit: a mixed-integer framework,* 
Optimization Methods and Software, pp. 1–27, 2023.
4. A. Olama, N. Bastianello, P. R. Mendes, and E. Camponogara, *Relaxed hybrid consensus ADMM for distributed convex
optimisation with coupling constraints,* IET Control Theory & Applications, vol. 13, no. 17, pp. 2828–2837, 2019.