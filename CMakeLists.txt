cmake_minimum_required(VERSION 3.20)
project(140_p2 C)

set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Şimdi ikisini birden derlemesini söyledik, hata vermeyecektir
add_executable(140_p2 main.c pet.c)

target_link_libraries(140_p2 raylibdll opengl32 gdi32 winmm)