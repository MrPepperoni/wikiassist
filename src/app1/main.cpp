#include "../wikilib/wiki.h"
#include <iostream>
#include <deque>
#include <chrono>

int main( int argc, char** argv )
{
    auto t1 = std::chrono::high_resolution_clock::now();
    if( argc < 2 )
    {
        std::cerr << "Usage: " << argv[0] << " start_title...\n";
        return 1;
    }
    wiki::ConnectionDesc cd{ "siddhartha.dynu.net", "wiki", "wiki", "3e6196a6feb44d0d66a67ad27685491f" };
    wiki::Wiki w( cd );
    for( int i = 1; i < argc; ++i )
    {
        std::string str( argv[i] );
        std::vector<std::string> results;
        std::deque<std::string> titles;
        std::cout << "Searching from " << str << "\n";
        titles.push_back( str );
        while ( !titles.empty() )
        {
            auto t = titles.front();
            titles.pop_front();
            if( t[0] == 'A' || t[0] == 'a' )
            {
                std::cout << "\t" << t << "\n";
                break;
            }

            if( w.getLinks( t, results ) )
            {
                titles.insert( titles.end(), results.begin(), results.end() );
            }
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "\ntook "
        << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
        << " milliseconds\n";
    return 0;
}
