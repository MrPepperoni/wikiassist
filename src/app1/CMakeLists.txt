include_directories( BEFORE ${CMAKE_BINARY_DIR} )
file( GLOB sources ./ *.cpp )
file( GLOB headers ./ *.h )

add_executable( app1 ${sources} )
add_dependencies( app1 wikilib )
target_link_libraries( app1 wikilib )

install( TARGETS app1
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

