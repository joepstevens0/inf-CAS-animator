
CFLAGS = 

BUILD_DIR = ../../build
OBJ_DIR = $(BUILD_DIR)/obj

LIBS = -Llib/glfw3 -lopengl32
INCLUDE = -I../../include -I../util -I..
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)

OBJECTS := $(notdir $(patsubst %.cpp, %.o, $(SOURCES)))

all: $(patsubst %, $(OBJ_DIR)/% ,$(OBJECTS))
	@:

$(OBJ_DIR)/%.o: %.cpp
	g++ $(CFLAGS) $(LIBS) $(INCLUDE) -o $@ -c $<

$(OBJ_DIR)/%.o: */%.cpp
	g++ $(CFLAGS) $(LIBS) $(INCLUDE) -o $@ -c $<
