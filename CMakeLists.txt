cmake_minimum_required(VERSION 3.10)
project(lite-http)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -pthread -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# project files
file(GLOB_RECURSE header_files src/*.h)
message("${header_files}")
file(GLOB_RECURSE source_files src/*.cpp)
file(GLOB_RECURSE main_files src/main/*.cpp)
list(REMOVE_ITEM source_files ${main_files})

# get header directories based on location of headers
macro(get_header_directories return_list)
    file(GLOB_RECURSE new_list src/*.h)
    set(dir_list "")
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    set(${return_list} ${dir_list})
endmacro()

# static library
set(library_target lite-http)
add_library(${library_target} STATIC ${source_files} ${header_files})
target_link_libraries(${library_target} pthread)

install(TARGETS ${library_target} DESTINATION lib)
install(FILES ${header_files} DESTINATION include)

include_directories("src")
# include header directories
# get_header_directories(include_dirs)
# include_directories(${include_dirs})
# foreach(dir IN LISTS include_dirs)
#   message("${dir}")
# endforeach()

# build main files
foreach(main_program ${main_files})
  # NAME_WE stands for "name without extension"
  get_filename_component(main_target ${main_program} NAME_WE)
  add_executable(${main_target} ${main_program})
  target_link_libraries(${main_target} ${library_target})
  set_target_properties(${main_target}
    PROPERTIES
    PROJECT_LABEL "${main_target}"
    FOLDER "main")
  install(TARGETS ${main_target} DESTINATION bin)
endforeach()
