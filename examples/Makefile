CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lrt -lraspi5 -lgpiod 

# Variable de target (subcarpeta con main.cpp)
TARGET ?= digital

# Paths
INCLUDE = -Isrc

# Ruta al archivo fuente
SRC = $(TARGET)/main.cpp

# Nombre del ejecutable (lo dejamos como 'main' por consistencia)
OUT = ./main

# Regla principal
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDFLAGS)


# Ejecutar directamente el binario
run: 
	sudo $(OUT)

# Limpiar ejecutables
clean:
	rm -f $(OUT);

.PHONY: all clean run