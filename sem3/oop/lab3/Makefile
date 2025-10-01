BUILD_DIR = build

SRC_DIR = src
HEAD_DIR = src/headers
LAB2_DIR = ../lab2/src
TEST_DIR = test

TARGET = $(BUILD_DIR)/debug.out
LEAKS_TARGET = $(BUILD_DIR)/leaks.out

# Файлы проекта (кроме main.cpp)
SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
EXTRA_SRCS = $(LAB2_DIR)/MyString.cpp $(LAB2_DIR)/MyVector.cpp $(LAB2_DIR)/Shape.cpp

# Все объектники
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS)) \
       $(patsubst $(LAB2_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(EXTRA_SRCS))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция src/*.cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	g++ -std=c++11 -I$(HEAD_DIR) -c -o $@ $<

# Компиляция ../lab2/src/*.cpp
$(BUILD_DIR)/%.o: $(LAB2_DIR)/%.cpp | $(BUILD_DIR)
	g++ -std=c++11 -I$(HEAD_DIR) -c -o $@ $<

$(TARGET): $(OBJS) $(SRC_DIR)/main.cpp
	g++ -g -std=c++11 -o $(TARGET) $(OBJS) $(SRC_DIR)/main.cpp

$(LEAKS_TARGET): $(OBJS) $(SRC_DIR)/main.cpp
	g++ -g -std=c++11 -Wall -Wextra -Wpedantic -Wfatal-errors -fsanitize=address -o $(LEAKS_TARGET) $(OBJS) $(SRC_DIR)/main.cpp

leaks: $(LEAKS_TARGET)
	./$(LEAKS_TARGET) < $(INPUT_FILE)

debug: $(TARGET)
	lldb $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
