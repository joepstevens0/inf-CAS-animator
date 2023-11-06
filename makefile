
CXX = g++
CFLAGS = -O3 -static -static-libstdc++ -static-libgcc

BIN = bin
SRC = src
INCLUDE = -Iinclude -Isrc/util -Isrc/imgui -Isrc
LIBS = -Llib/glfw3 -lopengl32

BUILD_DIR = build
TARGET = $(BUILD_DIR)/a.exe
OBJ_DIR = $(BUILD_DIR)\obj

run: build
	$(TARGET)

build: clean build_dir subfiles $(OBJ_DIR)/main.o
	$(CXX) $(CFLAGS) $(LIBS) $(INCLUDE) -o $(TARGET) glfw3.dll glad.c $(OBJ_DIR)/*.o

build_dir:
	if not exist "$(BUILD_DIR)" mkdir $(BUILD_DIR)
	if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
subfiles:
	@cd $(SRC)/imgui && $(MAKE) --no-print-directory
	@cd $(SRC)/render && $(MAKE) --no-print-directory
	@cd $(SRC)/channels && $(MAKE) --no-print-directory
	@cd $(SRC)/app && $(MAKE) --no-print-directory

$(OBJ_DIR)/main.o: $(SRC)/main.cpp
	$(CXX) $(CFLAGS) $(LIBS) $(INCLUDE) -c $^ -o $@

clean:
	if exist $(BUILD_DIR) rmdir $(BUILD_DIR) /S/Q