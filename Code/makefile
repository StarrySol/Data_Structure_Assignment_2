CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic-errors

TARGET = testcsv

SRCS = main.cpp CSVReader.cpp Polygon.cpp Math.cpp
OBJS = $(SRCS:.cpp=.o)

TEST_DIR = ../test_cases
OUT_DIR = ../testoutput

CSV_FILES = $(wildcard $(TEST_DIR)/*.csv)

.PHONY: all clean test dirs

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

dirs:
	mkdir -p $(OUT_DIR)

test: $(TARGET) dirs
	for file in $(CSV_FILES); do \
		name=$$(basename $$file .csv); \
		./$(TARGET) $$file > $(OUT_DIR)/$$name.txt; \
	done

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(OUT_DIR)