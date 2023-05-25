all: testfile.elf testfile.ll output.ll

testfile.ll: testfile.c
	${LLVM_INSTALL_DIR}/bin/clang -O0 -S -emit-llvm $< -o $@

output.ll: testfile.ll ../build/libPointerDeref.so
	${LLVM_INSTALL_DIR}/bin/opt -load-pass-plugin ../build/libPointerDeref.so -passes=pointer-deref $< -S -o $@

output.bc: output.ll
	${LLVM_INSTALL_DIR}/bin/llvm-as $< -o $@

testfile.elf: output.bc
	${LLVM_INSTALL_DIR}/bin/llc -filetype=obj $< -o testfile.o
	${LLVM_INSTALL_DIR}/bin/clang -no-pie testfile.o -o $@

run:
	./testfile.elf

clean:
	-@rm *.ll *.bc *.o *.elf

.PHONY: all clean run