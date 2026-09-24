CXX = g++
CXXFLAGS = -fopenmp -Wall -Werror -Wpedantic
EXE = trabalho
SRC = trabalho.cpp

.DEFAULT_OAL := build

build: $(SRC)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(SRC)

run: $(EXE)
	./$(EXE)

clean:
	rm $(EXE)

all: clean build run
