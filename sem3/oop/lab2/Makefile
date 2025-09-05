BUILD_DIR = build

SRC_DIR = src
HEAD_DIR = src/headers
TEST_DIR = test


TARGET = $(BUILD_DIR)/debug.out
LEAKS_TARGET = $(BUILD_DIR)/leaks.out

#используется для поиска
SRCS = $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
#используется для записи в строчку
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Правило для компиляции объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	g++ -Wall -Wextra -Wpedantic -Wfatal-errors -I$(HEAD_DIR) -c -o $@ $<

$(TARGET):  $(OBJS) $(SRC_DIR)/main.cpp
	g++ -g -Wall -Wextra -Wpedantic -Wfatal-errors -o $(TARGET) $(OBJS) $(SRC_DIR)/main.cpp

$(LEAKS_TARGET): $(OBJS) $(SRC_DIR)/main.cpp
	g++ -g -Wall -Wextra -Wpedantic -Wfatal-errors -fsanitize=address -o $(LEAKS_TARGET) $(OBJS) $(SRC_DIR)/main.cpp

leaks: $(LEAKS_TARGET)
	./$(LEAKS_TARGET) < $(INPUT_FILE)

debug: $(TARGET) 
	lldb $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)