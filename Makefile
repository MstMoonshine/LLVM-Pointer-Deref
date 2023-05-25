all: build/libPointerDeref.so

build/libPointerDeref.so: pointer-deref.cpp pointer-deref.h
	-@mkdir -p build
	cd build && cmake .. && make

test:
	make -C test

run:
	make run -C test

clean:
	rm -rf build
	make clean -C test

.PHONY: all clean test