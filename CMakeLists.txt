cmake_minimum_required(VERSION 3.12)

project(Relaxed-Lenses VERSION 1.0.0 LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/Arco.cpp
        src/Curva.cpp
        src/Polinomio.cpp
        src/Punti.cpp
        src/Raggio.cpp
        src/Sistema.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

# Optionally set compiler options and features
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)

# Optional: Install the library
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
 )
install(DIRECTORY include/ DESTINATION include)