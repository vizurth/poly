BUILD_DIR = build
TARGET = $(BUILD_DIR)/main
SRC = lab1.cpp
CXXFLAGS = -Wall -Wextra -pedantic -Wno-unused-but-set-variable
#-Wconversion
.PHONY: build run clean

build:
	mkdir -p $(BUILD_DIR)
	g++ $(CXXFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
