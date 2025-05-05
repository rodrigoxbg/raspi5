CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lrt -lgpiod

# Paths
INCLUDE = -Isrc
LIBNAME = libraspi5.a
LIBSRC = src/raspi5.cpp
LIBOBJ = src/raspi5.o
HEADER = src/raspi5.h

# Comando para compilar y crear el ejecutable
TARGET = raspi5

# Fuentes del ejecutable
SRC = src/terminal.cpp $(LIBSRC)

# Regla principal
all: $(TARGET)

# Regla para crear el ejecutable solo de terminal.cpp
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $(TARGET) $^ $(LDFLAGS)

# Crear librería estática (si quieres seguir teniendo la librería estática)
$(LIBNAME): $(LIBOBJ)
	ar rcs $@ $^

# Instalar la librería y el ejecutable
setuplib: buildlib
	sudo cp $(LIBNAME) /usr/local/lib/
	sudo cp $(HEADER) /usr/local/include/
	@echo "Installation Complete ................... \033[1;32m[ OK ]\033[0m"

# Instalar solo el ejecutable
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	@echo "Installation Complete ................... \033[1;32m[ OK ]\033[0m"

buildlib: $(LIBNAME)
	@echo "Librería $(LIBNAME) creada."

updatelib: setuplib

# Limpiar archivos
clean:
	rm -f src/*.o $(TARGET) $(LIBNAME)

# Ejecutar el proyecto
run: $(TARGET)
	sudo ./$(TARGET)

.PHONY: all clean setuplib updatelib run