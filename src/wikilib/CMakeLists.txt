include_directories( BEFORE ${CMAKE_BINARY_DIR} )
file( GLOB sources ./ *.cpp )
file( GLOB headers ./ *.h )

add_library( wikilib ${sources} )
target_link_libraries( wikilib mysqlcppconn mysqlclient m rt dl )

install( TARGETS wikilib
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )

