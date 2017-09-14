CC = g++
CFLAGS = -std=c++11 -O3 

all: auto_diff_tests function_minimizer_tests

auto_diff_tests: AutoDiffTests.cpp
	$(CC) $(CFLAGS) -o auto_diff_tests AutoDiffTests.cpp

function_minimizer_tests: FunctionMinimizerTests.cpp
	$(CC) $(CFLAGS) -o function_minimizer_tests FunctionMinimizerTests.cpp

clean:
	rm auto_diff_tests
	rm function_minimizer_tests

run:
	./auto_diff_tests
	./function_minimizer_tests

