MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

CXX = g++
RAYLIB_PATH = /home/rafael/src/lib/raylib
CXXFLAGS = -DPLATFORM_DESKTOP -std=c++20 -Wall -Werror

CXXFLAGS += -ggdb3

INCLUDE_PATHS = -I. -I$(RAYLIB_PATH)/src -I$(RAYLIB_PATH)/src/external -I/usr/local/include -I$(RAYLIB_PATH)/src/external/glfw/include
LDFLAGS = -L. -L$(RAYLIB_PATH)/src -L$(RAYLIB_PATH)/src -L/usr/local/lib -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -latomic 

SRCDIR = src
OBJDIR = obj

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
HEADERS = $(wildcard $(SRCDIR)/*.hpp)
OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))

EXECUTABLE = raycast

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(INCLUDE_PATHS) $(LDFLAGS)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_PATHS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean
