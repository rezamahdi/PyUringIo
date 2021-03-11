
Python3_add_library (_uring_io SHARED main.c ring.c sqe.c cqe.c)
target_link_libraries(_uring_io PUBLIC uring)
set_target_properties(_uring_io PROPERTIES SUFFIX ${PYTHON_MODULE_EXTENSION})
set_target_properties(_uring_io PROPERTIES PREFIX "")
# install pyd files one directory structure UP in the destination as 
# CMAKE_CURRENT_SOURCE_DIR is holding the source files of the MODULE
# which matches the name of CMAKE_CURRENT_SOURCE_DIR
get_filename_component(CURRENT_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
if(${SRC_DIR} STREQUAL ${CURRENT_SRC_DIR}) 
    set(DST_DIR ".") # if parent is the base folder
else()
    file(RELATIVE_PATH DST_DIR ${SRC_DIR} ${CURRENT_SRC_DIR})
endif()
install(TARGETS _uring_io DESTINATION ${DST_DIR} COMPONENT "EXT")
