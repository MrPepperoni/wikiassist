include_directories( BEFORE ${CMAKE_BINARY_DIR} )
file( GLOB sources ./ *.cpp )
file( GLOB headers ./ *.h )

add_executable( app2 ${sources} )
add_dependencies( app2 wikilib understandinglib )
target_link_libraries( app2 wikilib understandinglib )

install( TARGETS app2
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

