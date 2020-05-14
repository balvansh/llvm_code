INC=-I/usr/local/include/
all: dominators.so dead-code-elimination.so loop_invariant_code_motion.so

CXXFLAGS = -rdynamic $(shell llvm-config --cxxflags) $(INC) -g -O0 -fPIC

dataflow.o: dataflow.cpp dataflow.h

dominators.o : dominators.cpp dominators.h 

loop_invariant_code_motion.so: loop_invariant_code_motion.o dominators.o dataflow.o
        #$(CXX) -shared $^ -o $@

available-support.o: available-support.cpp available-support.h	

%.so: %.o dataflow.o 
	$(CXX) -dylib -shared $^ -o $@

clean:
	rm -f *.o *~ *.so

.PHONY: clean all
