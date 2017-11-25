CC_MPI=mpicc
CC_OMP=gcc -fopenmp -g
CFLAGS=-I.
BUILD_DIR = build
IMAGES_DIR = build/images
DIFUSE_DIR = build/difuse_images

.PHONY: fractal_MPI
fractal_MPI: fractal.c fractal/fractalfuncs.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	test -d "$(DIFUSE_DIR)" || mkdir $(DIFUSE_DIR)
	$(CC_MPI) fractal/fractalfuncs.c fractal.c -o $(BUILD_DIR)/fractal
	chmod 755 genmovie

.PHONY: fractal_OMP
fractal_OMP: fractal.c fractal/fractalfuncs.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	test -d "$(DIFUSE_DIR)" || mkdir $(DIFUSE_DIR)
	$(CC_OMP) fractal/fractalfuncs.c fractal.c -o $(BUILD_DIR)/fractal
	chmod 755 genmovie

movie:
	./genmovie

setup:
	make -C ./fractal/
	make -C ./exemplos/
	make all

clean:
	rm -r build