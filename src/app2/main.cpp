#include "../wikilib/wiki.h"
#include "../understandinglib/understanding.h"
#include <iostream>
#include <deque>

int main( int argc, char** argv )
{
    if( argc < 3 )
    {
        std::cerr << "Usage: " << argv[0] << " start_title num_articles\n";
        return 1;
    }
    wiki::ConnectionDesc cd{ "siddhartha", "wiki", "wiki", "3e6196a6feb44d0d66a67ad27685491f" };
    wiki::Wiki w( cd );
    auto const& rv = understanding::getSuggestedTitles( w, argv[1], atoi( argv[2] ) );
    for( auto const& t : rv )
    {
        std::cout << "\t" << t << "\n";
    }
    return 0;
}
