CC=gcc -fopenmp -g
CFLAGS=-I.

TEST_DATA = "10 2\n0 0 0 10\n5 0 0 20\n0 5 5 10\n10 5 0 20"
BUILD_DIR= build
IMG_DIR= build/images
HELPER_FUNCS= helpers/file_functions.c helpers/algebra_functions.c helpers/physics_functions.c


.PHONY: all
all: main

.PHONY: main
main: main.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMG_DIR)" || mkdir $(IMG_DIR)
	$(CC) $(HELPER_FUNCS) main.c -o $(BUILD_DIR)/main
	chmod 755 doplot2movie

setup:
	make generate_test
	make -C ./exemplos/
	make all

clean:
	rm -r build

generate_test:
	echo $(TEST_DATA) > test_particles.dat