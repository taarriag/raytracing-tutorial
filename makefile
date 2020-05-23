CXX = clang++
SDL = -F/Library/Frameworks -framework SDL2
CXXFLAGS = -Wall -c -g -std=c++17 -Ithird_party/stb -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks
LDFLAGS = $(SDL)
SRC_DIR = src
OUT_DIR = out
EXE = main

SOURCES = main.cc
OBJECTS = $(SOURCES:%.cc=$(OUT_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(OUT_DIR)/$@

# Use an order-only prerequisite to always rebuild the folder, but do
# not rebuild .o files if not needed (e.g. when changes in an unrelated 
# file inside OUT_DIR changes the directory timestamp).
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cc | $(OUT_DIR)/
	$(CXX) $(CXXFLAGS) $< -o $@

$(OUT_DIR)/:
	mkdir -p $@

clean:
	rm -rf $(OUT_DIR)/*
