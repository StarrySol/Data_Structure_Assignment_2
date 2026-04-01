# Source and target definitions
SRCS = knapsack.cpp
TARGET = knapsack.out

# Compilation rules
$(TARGET): $(SRCS)
	g++ -Wall -o $(TARGET) $(SRCS)

# Clean up binaries
clean:
	rm -f $(TARGET)