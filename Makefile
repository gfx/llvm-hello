CLANG       := clang++
LLVM_CONFIG := llvm-config
LLVM_INTERP := lli

a.out: hello.cpp
	$(CLANG) -std=c++11 -g $< `$(LLVM_CONFIG) --cppflags --ldflags --libs` -ldl -lpthread
	./a.out 2> a.s
	$(LLVM_INTERP) a.s

clean:
	rm -rf a.*
