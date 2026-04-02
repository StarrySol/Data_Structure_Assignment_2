CXX = g++
CXXFLAGS = -std=c++17 -pedantic-errors -Wall -Wextra -Werror

TARGET = simplify

SRCS = main.cpp Code/Ring.cpp Code/FileReader.cpp Code/Collaspe.cpp
OBJS = $(SRCS:.cpp=.o)

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)