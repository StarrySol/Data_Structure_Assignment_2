CXX = g++
CXXFLAGS = -std=c++20 -pedantic-errors -Wall -Wextra -Werror

TARGET = simplify

SRCS = main.cpp \
       CSVReader.cpp \
       Math.cpp \
       Polygon.cpp \
       Simplifier.cpp

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)