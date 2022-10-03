cmake_minimum_required(VERSION 3.0)

project("YSFIO")

add_compile_options(-std=c++11)

aux_source_directory(. ROOT_SRC)
aux_source_directory(./YSFIO YSFIO_SRC)
aux_source_directory(./example EX_SRC)

FOREACH(FILE_PATH ${EX_SRC})
    STRING(REGEX REPLACE ".+/(.+)\\..*" "\\1" FILE_NAME ${FILE_PATH})
    add_executable(${FILE_NAME} ${FILE_PATH} ${ROOT_SRC} ${YSFIO_SRC})
ENDFOREACH(FILE_PATH)

