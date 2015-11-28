# Specify compiler
CC = g++ -std=c++11

# Target
Target = gen

# Source & Objects
Common = $(wildcard common/*.cpp)
Conventional = $(wildcard conventional/*.cpp)
Discrete = $(wildcard discrete/*.cpp)

Objects = ${Common: .cpp=.o} ${Conventional: .cpp=.o} ${Discrete: .cpp=.o}

# Target
.PHONY : all
all : $(Target)

$(Target) : $(Objects) main.o
	$(CC) $(Objects) main.o -o $(Target)

main.o : main.cpp
	$(CC) -c main.cpp -o main.o

# Clean target
.PHONY : clean
clean :
	rm main.o $(Target)