CXX = g++
CXXFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -Werror

TARGET = simplify

SRCS = main.cpp Code/Ring.cpp Code/FileReader.cpp Code/Collapse.cpp

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)