


BUILD_DIR = ../../build
TARGET = $(BUILD_DIR)/a.exe
OBJ_DIR = $(BUILD_DIR)/obj

LIBS = -Llib/glfw3 -lopengl32
INCLUDE = -I../../include -I../imgui

objects := $(patsubst %.cpp,$(OBJ_DIR)/%.o, $(wildcard *.cpp))

all: $(objects)
	@:

$(OBJ_DIR)/%.o: %.cpp
	g++ $(LIBS) $(INCLUDE) -o $@ -c $<
