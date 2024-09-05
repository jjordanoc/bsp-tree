# Crea la carpeta build, ejecuta cmake y compila
all:
	mkdir -p build
	cd build && cmake .. && make

# Limpia todo el proyecto eliminando la carpeta build
clean:
	mkdir -p build
	cd build && cmake --build . --target clean-all

# Compila y ejecuta el proyecto
run: all
	./build/bin/BSPTreeProject

# Para compilar y ejecutar el proyecto:
# 	make run
#
# Para limpiar el proyecto
# 	make clean
