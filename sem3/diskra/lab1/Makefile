CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Wpedantic
INCLUDES = -Iinclude
SRC_DIR = src
BUILD_DIR = build
TARGET = $(BUILD_DIR)/diskra

SRCS = $(SRC_DIR)/main.cpp \
$(SRC_DIR)/Gray.cpp \
$(SRC_DIR)/Multiset.cpp \
$(SRC_DIR)/UI.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET)

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean 