CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0
TARGET   = btree_app


MAIN_SRC = example.cpp
OBJ      = $(MAIN_SRC:.cpp=.o)


DEPS     = Btree.hpp Btree.cpp BtreeNode.hpp BtreeNode.cpp



all: $(TARGET)


$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)
	@echo "Build completata: ./$(TARGET)"


$(OBJ): $(MAIN_SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) -c $(MAIN_SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

check: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

.PHONY: all clean check