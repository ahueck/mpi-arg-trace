# mpi-arg-trace
mpi-arg-trace uses PMPI to intercept MPI calls and write calls and key parameters to a stream (console/file).

### Build
mpi-arg-trace uses CMake to build, and is tested against Clang/LLVM 12.
Further requirements are (Open)MPI and Python 3. 


```sh
$> git clone https://github.com/ahueck/mpi-arg-trace
$> cd mpi-arg-trace
$> CC=clang CXX=clang++ cmake -B build -DCMAKE_BUILD_TYPE=Release
$> cmake --build build --target mpitracer_mpitracer
```

The shared library will be in the folder `build/lib`.

#### Build options

| Option                        | Default | Description                                    |
|-------------------------------|:-------:|------------------------------------------------|
| `MPITRACER_PRINT_TO_CONSOLE`  |  `OFF`  | Print trace to console, instead of file.       |
| `MPITRACER_TEST`              |  `OFF`  | Build with tests. Needs `lit` and `FileCheck`. |


#### Testing
Enable `MPITRACER_TEST` and `MPITRACER_PRINT_TO_CONSOLE` in CMake.
This needs `llvm-lit` and `FileCheck` for testing the output.

### Intercepting MPI

First compile your target with MPI and Clang as the underlying compiler.
Preload the mpi-arg-trace library when executing with `mpiexec`.

```sh
OMPI_CC=clang mpicc mpi_comm.c -g -o mpi_comm.bin
export MPI_ARG_TRACE_FILE_TARGET=/directory/path-to/trace-files
export MPI_ARG_TRACE_SKIP_HEADER=1
LD_PRELOAD=/path-to/libmpitracer.so mpiexec -n 2 mpi_comm.bin
```

#### Environment variables

These influence the trace generation. The directory target for the trace must be set.

| Option                      |                                   Description                                   |
|-----------------------------|:-------------------------------------------------------------------------------:|
| `MPI_ARG_TRACE_DIR`         |           A directory where the trace files per rank are written to.            |
| `MPI_ARG_TRACE_SKIP_HEADER` |                If set, do not print CSV-like data column header.                |
| `MPI_ARG_TRACE_FILE_TARGET` | Explicitly set the name of the translation unit, instead of parsing debug data. |
