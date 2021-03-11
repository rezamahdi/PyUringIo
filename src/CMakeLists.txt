# root source directory should only contain this file (unless creating a root-package Python module)

# variable to hold the path to src folder to compute relative path of its decendents
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(RELATIVE_PATH DST_DIR ${SRC_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PYFILES "${CMAKE_CURRENT_SOURCE_DIR}/*.py")
install(FILES ${PYFILES} DESTINATION . COMPONENT "PY")

# traverse the tree
add_subdirectory("native")
