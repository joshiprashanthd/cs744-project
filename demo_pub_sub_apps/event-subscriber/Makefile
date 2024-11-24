DEPENDENCIES := ./dependencies
CIVETWEB_LIB := libcivetweb.a

CXX         := g++
CXX_FLAGS   := -Wall -Wextra -std=c++17 -ggdb $(COPT) -DNO_FILES -DMG_EXPERIMENTAL_INTERFACES

BIN         := bin
SRC_DIR     := src
OBJ_DIR     := obj
SRC         := $(shell find $(SRC_DIR) -type f -name '*.cpp')  # Recursively find all .cpp files
OBJS        := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
INCLUDE     := -Iinclude -Ilib \
 -I$(DEPENDENCIES)/civetweb/include \
 -I$(DEPENDENCIES)/json/include \
 -I$(DEPENDENCIES)/HTTPRequest/include
LIB         := lib

LIBRARIES   := -lpthread
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE)


$(BIN)/$(EXECUTABLE): $(OBJS) $(LIB)/$(CIVETWEB_LIB) | $(BIN)
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)

# Compile each .cpp file into an .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)  # Ensure subdirectories in obj/ exist
	$(CXX) $(CXX_FLAGS) $(INCLUDE) -c $< -o $@


$(LIB)/$(CIVETWEB_LIB): 
	$(MAKE) -C $(DEPENDENCIES)/civetweb WITH_IPV6=1 WITH_WEBSOCKET=1 COPT='-DNO_SSL -DMG_EXPERIMENTAL_INTERFACES' clean lib
	cp $(DEPENDENCIES)/civetweb/$(CIVETWEB_LIB) $(LIB)

# Ensure the bin and obj directories exist
$(BIN) $(OBJ_DIR):
	mkdir -p $@

# Clean target
clean:
	-rm -rf $(BIN)/*
	-rm -rf $(OBJ_DIR)/*
	-rm -f $(LIB)/$(CIVETWEB_LIB) 

.PHONY: all clean run
