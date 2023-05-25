all: build/libPointerDeref.so

build/libPointerDeref.so: pointer-deref.cpp pointer-deref.h
	-@mkdir -p build
	cd build && cmake .. && make

test/testfile.ll: test/testfile.c
	clang -O1 -S -emit-llvm $< -o $@

test: test/testfile.ll
	${LLVM_INSTALL_DIR}/bin/opt -load-pass-plugin build/libPointerDeref.so -passes=pointer-deref -disable-output $<

clean:
	rm -rf build

.PHONY: all clean test