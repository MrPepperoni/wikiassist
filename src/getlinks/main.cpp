#include "../wikilib/wiki.h"
#include <iostream>
#include <deque>

int main( int argc, char** argv )
{
    if( argc < 2 )
    {
        std::cerr << "Usage: " << argv[0] << " start_title\n";
        return 1;
    }
    wiki::ConnectionDesc cd{ "siddhartha", "wiki", "wiki", "3e6196a6feb44d0d66a67ad27685491f" };
    wiki::Wiki w( cd );
    std::vector<std::string> results;
    if( w.getLinks( argv[1], results ) )
    {
        for( auto const& t : results )
        {
            std::cout << "\t" << t << "\n";
        }
    }
    return 0;
}
