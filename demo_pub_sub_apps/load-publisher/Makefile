DEPENDENCIES := ./dependencies

CXX         := g++
CXX_FLAGS   := -Wall -Wextra -std=c++17 -ggdb $(COPT)

BIN         := bin
SRC_DIR     := src
OBJ_DIR     := obj
SRC         := $(shell find $(SRC_DIR) -type f -name '*.cpp')  # Recursively find all .cpp files
OBJS        := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDE     := -Iinclude -Ilib \
 -I$(DEPENDENCIES)/json/include \
 -I$(DEPENDENCIES)/HTTPRequest/include
LIB         := lib

LIBRARIES   := -lpthread
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(OBJS) | $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

# Compile each .cpp file into an .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@

# Ensure the bin and obj directories exist
$(BIN) $(OBJ_DIR):
	mkdir -p $@

# Clean target
clean:
	-rm -rf $(BIN)/*
	-rm -rf $(OBJ_DIR)/*

.PHONY: all clean run
