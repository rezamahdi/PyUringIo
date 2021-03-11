
find_library(LIBURING_LIBRARIES NAMES uring)
find_path(LIBURING_INCLUDE_DIRS liburing.h)
if(LIBURING_LIBRARIES AND LIBURING_INCLUDE_DIRS)
  message(STATUS "liburing libs: ${LIBURING_LIBRARIES}")
  message(STATUS "liburing include directory: ${LIBURING_INCLUDE_DIRS}")
  set(LIBURING_FOUND TRUE CACHE BOOL "Found liburing libraries" FORCE)
  
  add_library(uring STATIC IMPORTED)
  target_link_libraries(uring INTERFACE ${LIBURING_LIBRARIES})
  target_include_directories(uring INTERFACE ${LIBURING_INCLUDE_DIRS})

  set_target_properties(uring PROPERTIES IMPORTED_LOCATION ${LIBURING_LIBRARIES})
else()
  set(LIBURING_FOUND FALSE CACHE BOOL "Found liburing libraries" FORCE)
  message(STATUS "Could NOT find liburing")
endif()