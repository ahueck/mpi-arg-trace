! RUN: %mpifort %s -g -O0 -o %s.exe
! RUN: LD_PRELOAD=%mpitracer %mpi-exec -n 2 %s.exe 2>&1 | %filecheck %s

! CHECK-DAG: MPI_Recv,0,1,1,MPI_INT,MPI_COMM_WORLD,,,,{{.*}}/send_recv.f90,{{.*}},21
! CHECK-DAG: MPI_Send,1,1,1,MPI_INT,MPI_COMM_WORLD,,,,{{.*}}/send_recv.f90,{{.*}},18

PROGRAM send_recv_mpi
include 'mpif.h'

integer rank, size_world, ierror, buffer

call MPI_INIT(ierror)
call MPI_COMM_SIZE(MPI_COMM_WORLD, size_world, ierror)
call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierror)

IF(rank == 0) THEN
    buffer = 42
    call MPI_SEND(buffer, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, ierror)
    print *, "Send message: ", buffer
ELSE IF(rank == 1) THEN
    call MPI_RECV(buffer, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE, ierror)
    print *, "Received message: ", buffer
END IF

call MPI_FINALIZE(ierror)
END PROGRAM
