# Specify compiler
CC = g++ -std=c++11 -DNDEBUG -O2

# Target
Target = DPSO

# Source & Objects
Common = $(wildcard common/*.cpp)
CommonIO = $(wildcard common/io/*.cpp)
PSO = $(wildcard pso/*.cpp)
Variants = $(wildcard pso/variants/*.cpp)

Objects = Sovler.o ${Common: .cpp=.o} ${CommonIO: .cpp=.o} ${PSO: .cpp=.o} \
          ${Variants: .cpp=.o}

# Target
.PHONY : all
all : $(Target)

Sovler.o : ./minisat/solver/Solver.C
	$(CC) $(CFLAGS)  -I ./minisat/include -c -o $@ $^

$(Target) : $(Objects) main.o
	$(CC) $(Objects) main.o -o $(Target)

main.o : main.cpp
	$(CC) -c main.cpp -o main.o

# Clean target
.PHONY : clean
clean :
	rm main.o $(Target)