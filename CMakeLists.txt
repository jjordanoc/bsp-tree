# Versión mínima de CMake
cmake_minimum_required(VERSION 3.10)

project(BSPTreeProject)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir los archivos fuente y cabecera
set(SOURCES
    main.cpp
    Line.cpp
    Plane.cpp
    BSPTree.cpp
)
set(HEADERS
    DataType.h
    Point.h
    Line.h
    Plane.h
    BSPTree.h
)

# Crea el ejecutable
add_executable(BSPTreeProject ${SOURCES} ${HEADERS})
target_include_directories(BSPTreeProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Ruta de salida de los binarios
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Custom target: clean-all
add_custom_target(clean-all
    COMMAND rm -rf ${CMAKE_BINARY_DIR}
    COMMENT "Limpiando el proyecto y eliminando la carpeta build..."
)

# Custom target: run
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target BSPTreeProject && ${CMAKE_BINARY_DIR}/bin/BSPTreeProject
    DEPENDS BSPTreeProject
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Ejecutando el proyecto..."
)
