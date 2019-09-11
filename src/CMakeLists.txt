find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GSTREAMER_PLUGINS_BASE REQUIRED gstreamer-plugins-base-1.0)

set(HEADERS buffer-meta.h tags-meta.h)

add_library(depth-meta SHARED buffer-meta.c tags-meta.c)
target_link_libraries(depth-meta ${GSTREAMER_LDFLAGS} ${GSTREAMER_PLUGINS_BASE_LDFLAGS})
include_directories(depth-meta ${GSTREAMER_INCLUDE_DIRS} ${GSTREAMER_PLUGINS_BASE_INCLUDE_DIRS})

find_program(INTROSPECTION_SCANNER "g-ir-scanner")
find_program(INTROSPECTION_COMPILER "g-ir-compiler")

set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(GIT_PKG_VERSION "$ENV{GIT_PKG_VER}")
configure_file("depth-meta.pc.in" "depth-meta.pc" @ONLY)

set(GIR_PREFIX "Depth-meta-$ENV{GIT_PKG_VER}")
set(GIR_XML "${GIR_PREFIX}.gir")
set(GIR_TYPELIB "${GIR_PREFIX}.typelib")

add_custom_command(OUTPUT ${GIR_XML}
      COMMAND ${INTROSPECTION_SCANNER}
              --namespace=Depth-meta
              --nsversio=$ENV{GIT_PKG_VER}
              --library=depth-meta
              --no-libtool
              --include=GObject-2.0
              --include=GModule-2.0
              --output ${GIR_XML}
              --warn-all
              ${HEADERS}
      DEPENDS ${HEADERS}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT ${GIR_TYPELIB}
      COMMAND ${INTROSPECTION_COMPILER}
              -o ${GIR_TYPELIB}
              ${GIR_XML}
      DEPENDS ${GIR_XML}
      WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR})

add_custom_target(gir ALL DEPENDS ${GIR_XML} ${GIR_TYPELIB})
add_dependencies(gir depth-meta)

install(TARGETS depth-meta
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(FILES ${HEADERS} DESTINATION include)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/depth-meta.pc" DESTINATION lib/pkgconfig)
