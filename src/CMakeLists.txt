find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_PLUGINS_BASE REQUIRED gstreamer-plugins-base-1.0)

set(HEADERS depth-meta-buffer.h depth-meta-tags.h)

add_library(depth-meta SHARED depth-meta-buffer.c depth-meta-tags.c)
target_link_libraries(depth-meta ${GSTREAMER_LDFLAGS} ${GSTREAMER_PLUGINS_BASE_LDFLAGS})
include_directories(depth-meta ${GSTREAMER_INCLUDE_DIRS} ${GSTREAMER_PLUGINS_BASE_INCLUDE_DIRS})

find_program(INTROSPECTION_SCANNER "g-ir-scanner")
find_program(INTROSPECTION_COMPILER "g-ir-compiler")

set(GIR_NAMESPACE "DepthMeta")
set(GIR_VERSION "1.0")
set(GIR_NAME "${GIR_NAMESPACE}-${GIR_VERSION}.gir")
set(GIR_TYPELIB_NAME "${GIR_NAMESPACE}-${GIR_VERSION}.typelib")
set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
configure_file("depth-meta.pc.in" "depth-meta.pc" @ONLY)

function(prepend VAR)
   set(RES "")
   foreach(I ${${VAR}})
      list(APPEND RES -I${I})
   endforeach(I)
   set(${VAR}_ARGS "${RES}" PARENT_SCOPE)
endfunction(prepend)
get_target_property(INCLUDE_DIRS depth-meta INCLUDE_DIRECTORIES)
prepend(INCLUDE_DIRS)

add_custom_command(OUTPUT ${GIR_NAME}
      COMMAND ${INTROSPECTION_SCANNER}
              --namespace=${GIR_NAMESPACE}
              --nsversio=${GIR_VERSION}
              --library=depth-meta
              -I${INCLUDE_DIRS_ARGS}
              --output ${GIR_NAME}
              --warn-all
              ${HEADERS}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${GIR_TYPELIB_NAME}
      COMMAND ${INTROSPECTION_COMPILER}
              -o ${GIR_TYPELIB_NAME}
              ${GIR_NAME}
      DEPENDS ${GIR_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(gir ALL DEPENDS ${GIR_NAME} ${GIR_TYPELIB_NAME})
add_dependencies(gir depth-meta)

install(TARGETS depth-meta
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/depth-meta.pc" DESTINATION lib/pkgconfig)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIR_NAME}" DESTINATION share/gir-1.0)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${GIR_TYPELIB_NAME}" DESTINATION lib/girepository-1.0)
