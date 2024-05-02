# CMakeList.txt : projet CMake pour Template, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Ajoutez une source à l'exécutable de ce projet.
add_executable (Template "main.cpp" "application.h" "application.cpp" "imageManager.cpp" "imageManager.h" "size.h" "buttonManager.h" "fontManager.h")

target_link_libraries(Template SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Template PROPERTY CXX_STANDARD 20)
endif()
