CC=gcc -fopenmp -g
CFLAGS=-I.
BUILD_DIR = build
IMAGES_DIR = build/images
DIFUSE_DIR = build/difuse_images

.PHONY: fractal
ex_3: fractal.c fractal/fractalfuncs.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	test -d "$(DIFUSE_DIR)" || mkdir $(DIFUSE_DIR)
	$(CC) fractal/fractalfuncs.c fractal.c -o $(BUILD_DIR)/fractal
	chmod 755 genmovie

movie:
	./genmovie

setup:
	make -C ./fractal/
	make -C ./exemplos/
	make all

clean:
	rm -r build