cmake_minimum_required(VERSION 3.0)

project("YSFIO")

add_compile_options(-std=c++11)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(./YSFIO)

aux_source_directory(. ROOT_SRC)
aux_source_directory(./example EX_SRC)

 FOREACH(FILE_PATH ${EX_SRC})
     STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE_PATH})
     add_executable(${FILE_NAME} ${FILE_PATH} ${ROOT_SRC})
     target_link_libraries(${FILE_NAME} ysfio_shared)
 ENDFOREACH(FILE_PATH)
 
