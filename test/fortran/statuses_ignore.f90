! RUN: %mpifort %s -g -O0 -o %s.exe
! RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 2 %s.exe 2>&1 | %filecheck %s
! REQUIRES: fortran

! CHECK-DAG: MPI_Irecv,0,0,1,MPI_INTEGER,MPI_COMM_WORLD,,,,{{.*}},{{.*}},36
! CHECK-DAG: MPI_Irecv,0,0,1,MPI_INTEGER,MPI_COMM_WORLD,,,,{{.*}},{{.*}},37
! CHECK-DAG: MPI_Isend,1,0,1,MPI_INTEGER,MPI_COMM_WORLD,,,,{{.*}},{{.*}},32
! CHECK-DAG: MPI_Isend,1,0,1,MPI_INTEGER,MPI_COMM_WORLD,,,,{{.*}},{{.*}},33

PROGRAM main
    USE mpi

    IMPLICIT NONE

    INTEGER :: ierror
    INTEGER :: size
    INTEGER :: my_rank
    INTEGER, ALLOCATABLE :: buffer(:)
    INTEGER :: buffer_length
    INTEGER :: requests(0:1)

    CALL MPI_Init(ierror)

    CALL MPI_Comm_size(MPI_COMM_WORLD, size, ierror)
    CALL MPI_Comm_rank(MPI_COMM_WORLD, my_rank, ierror)

    buffer_length = 2
    ALLOCATE(buffer(0:buffer_length-1))

    IF (my_rank .EQ. 0) THEN
        buffer = (/1, 2/)
        CALL MPI_Isend(buffer(0), 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD, requests(0), ierror)
        CALL MPI_Isend(buffer(1), 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD, requests(1), ierror)
        CALL MPI_Waitall(2, requests, MPI_STATUSES_IGNORE, ierror)
    ELSE
        CALL MPI_Irecv(buffer(0), 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, requests(0), ierror)
        CALL MPI_Irecv(buffer(1), 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, requests(1), ierror)
        CALL MPI_Waitall(2, requests, MPI_STATUSES_IGNORE, ierror)
    END IF

    DEALLOCATE(buffer)

    CALL MPI_Finalize(ierror)
END PROGRAM main
