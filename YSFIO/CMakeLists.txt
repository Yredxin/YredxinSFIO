aux_source_directory(. YSFIO_SRC)

set(MY_PROJECT_NAME "ysfio")
set(YSFIO_STATIC_LIB_NAME ${MY_PROJECT_NAME}-static)

# 添加动态库
add_library(${MY_PROJECT_NAME} SHARED ${YSFIO_SRC})
# 添加库静态库
add_library(${YSFIO_STATIC_LIB_NAME} STATIC ${YSFIO_SRC})
# 将静态库重命名
set_target_properties(${YSFIO_STATIC_LIB_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

