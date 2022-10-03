
aux_source_directory(. YSFIO_SRC)

# 添加动态库
add_library(ysfio_shared SHARED ${YSFIO_SRC})
# 添加库静态库
add_library(ysfio STATIC ${YSFIO_SRC})

