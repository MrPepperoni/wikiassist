include_directories( BEFORE ${CMAKE_BINARY_DIR} )
file( GLOB sources ./ *.cpp )
file( GLOB headers ./ *.h )

add_executable( getlinks ${sources} )
add_dependencies( getlinks wikilib )
target_link_libraries( getlinks wikilib )

install( TARGETS getlinks
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

