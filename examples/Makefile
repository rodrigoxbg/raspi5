CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lrt -lraspi5 -lgpiod


TARGET ?= digital
OUT = main
LAST_TARGET_FILE = .last_target

# Archivos fuente: main.cpp + todos los .cpp en subdirectorios
SRC = $(wildcard $(TARGET)/main.cpp) $(wildcard $(TARGET)/src/*.cpp) $(wildcard $(TARGET)/lib/*.cpp)

# Incluye las carpetas src y lib si existen
INCLUDE = -Isrc -I$(TARGET) -I$(TARGET)/src -I$(TARGET)/lib

all: check_target $(OUT)

check_target:
	@echo $(TARGET) > $(LAST_TARGET_FILE).tmp
	@if [ ! -f $(LAST_TARGET_FILE) ] || ! cmp -s $(LAST_TARGET_FILE) $(LAST_TARGET_FILE).tmp; then \
		cp $(LAST_TARGET_FILE).tmp $(LAST_TARGET_FILE); \
		echo "Target changed, forcing rebuild..."; \
		rm -f $(OUT); \
	fi
	@rm -f $(LAST_TARGET_FILE).tmp

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)

run: $(OUT)
	sudo ./$(OUT)

clean:
	find -name main -type f -exec rm -f {} \;
	rm -f $(LAST_TARGET_FILE)

.PHONY: all clean run check_target
