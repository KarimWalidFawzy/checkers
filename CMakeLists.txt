cmake_minimum_required(VERSION 3.23)
project(MySDL3Project)

# Find the SDL3 package. 'REQUIRED' makes the build fail if not found.
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
# Add your executable target
add_executable(checkers main.cc)

# Link the imported target provided by the SDL3 package.
# The `SDL3::SDL3` target automatically handles linking the correct library
# (shared or static) and setting include paths.
target_link_libraries(checkers PRIVATE SDL3::SDL3)
