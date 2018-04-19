OBJ=BestFirstSearch.o
SRC=BestFirstSearch.cpp
EXEC=BestFirstSearch
CXX=g++
FLAGS= -c -Wall -I=.

#+==============================================================================

all: link
link: compile $(OBJ)
	$(CXX) -o $(EXEC) $(OBJ)
compile:
	$(CXX) $(FLAGS) $(SRC)
clean:
	rm -rf $(EXEC) $(OBJ)
test:
	./$(EXEC) input.txt output.txt
	cat output.txt
