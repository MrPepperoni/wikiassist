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
    std::deque<std::string> titles;
    titles.push_back( argv[ 1 ] );
    while ( !titles.empty() )
    {
        auto t = titles.front();
        titles.pop_front();
        if( t[0] == 'A' || t[0] == 'a' )
        {
            std::cout << t << "\n";
            return 0;
        }

        if( w.getLinks( t, results ) )
        {
            titles.insert( titles.end(), results.begin(), results.end() );
        }
    }
    return 0;
}
