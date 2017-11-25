CC=gcc -fopenmp -g
CFLAGS=-I.
BUILD_DIR = build
IMAGES_DIR = build/images
DIFUSE_DIR = build/difuse_images

EX_2=ex_14_2
EX_3=ex_14_3

.PHONY: all
all: ex_2 ex_3

.PHONY: ex_2
ex_2: $(EX_2).c fractal/fractalfuncs.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	$(CC) fractal/fractalfuncs.c $(EX_2).c -o $(BUILD_DIR)/$(EX_2)
	chmod 755 genmovie

.PHONY: ex_3
ex_3: $(EX_3).c fractal/fractalfuncs.c
	test -d "$(BUILD_DIR)" || mkdir $(BUILD_DIR)
	test -d "$(IMAGES_DIR)" || mkdir $(IMAGES_DIR)
	test -d "$(DIFUSE_DIR)" || mkdir $(DIFUSE_DIR)
	$(CC) fractal/fractalfuncs.c $(EX_3).c -o $(BUILD_DIR)/$(EX_3)
	chmod 755 genmovie

movie:
	./genmovie

setup:
	make -C ./fractal/
	make -C ./exemplos/
	make all

clean:
	rm -r build