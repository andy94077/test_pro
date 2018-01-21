CC=g++
CXXFLAGS=-O2 -std=c++11
SRC=test_pro.cpp
OBJ=$(SRC:.cpp=.o)
EXE=test_pro

all:$(EXE)
$(EXE):$(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o $(EXE)
clean:
	-@rm -f $(OBJ) $(EXE)
