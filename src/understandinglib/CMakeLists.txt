include_directories( BEFORE ${CMAKE_BINARY_DIR} )
file( GLOB sources ./ *.cpp )
file( GLOB headers ./ *.h )

add_library( understandinglib ${sources} )
add_dependencies( understandinglib wikilib )
target_link_libraries( understandinglib wikilib )

install( TARGETS understandinglib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

