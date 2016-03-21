#include "../wikilib/wiki.h"
#include "../understandinglib/understanding.h"
#include <iostream>
#include <deque>
#include <chrono>

int main( int argc, char** argv )
{
    auto t1 = std::chrono::high_resolution_clock::now();
    if( argc < 3 || argc % 2 == 0 )
    {
        std::cerr << "Usage: " << argv[0] << " [start_title num_articles]...\n";
        return 1;
    }
    wiki::ConnectionDesc cd{ "siddhartha.dynu.net", "wiki", "wiki", "3e6196a6feb44d0d66a67ad27685491f" };
    wiki::Wiki w( cd );
    for( int i = 1; i < argc; i += 2 )
    {
        std::string title = argv[ i ];
        std::cout << "Suggestions for " << title << ":\n";
        auto const& rv = understanding::getSuggestedTitles( w, title, atoi( argv[i+1] ) );
        for( auto const& t : rv )
        {
            std::cout << "\t" << t << "\n";
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\ntook "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << " milliseconds\n";
    return 0;
}
