CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = simplify
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(wildcard *.h)

all: $(TARGET)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

.PHONY: all clean run

# usage:
# make run FILE=input_rectangle_with_two_holes.csv VERTICES=7
run: $(TARGET)
	@if [ -z "$(FILE)" ]; then \
		echo "Error: Please specify a CSV file with FILE=<input.csv>"; \
		exit 1; \
	fi
	@if [ -z "$(VERTICES)" ]; then \
		echo "Error: Please specify target vertices with VERTICES=<number>"; \
		exit 1; \
	fi
	./$(TARGET) $(FILE) $(VERTICES)

clean:
	rm -f $(TARGET) $(OBJS)