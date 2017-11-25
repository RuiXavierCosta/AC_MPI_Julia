CC=mpicc
RUN_CMD = mpiexec −f
CLUSTER = cluster_right
.PHONY: all_targets
all_targets: make_helloworld

.PHONY: make_helloworld
make_helloworld: helloworld.c
	$(CC) helloworld.c -o mpi_hello


run:
	$(RUN_CMD) $(CLUSTER) ./mpihello