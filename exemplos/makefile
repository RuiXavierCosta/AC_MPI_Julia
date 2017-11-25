CC=mpicc
RUN_CMD = mpiexec −f
CLUSTER = cluster_right
.PHONY: all_targets
all_targets: make_helloworld make_exemplo2

.PHONY: make_helloworld
make_helloworld: helloworld.c
	$(CC) helloworld.c -o mpi_hello

.PHONY: make_exemplo2
make_exemplo2: exemplo2.c
	$(CC) exemplo2.c -o mpi_exemplo2


run:
	$(RUN_CMD) $(CLUSTER) ./mpihello