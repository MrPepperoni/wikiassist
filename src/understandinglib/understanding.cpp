#include "understanding.h"

#include <math.h>
#include <map>
#include <iostream>
#include <algorithm>
#include <list>

using namespace wiki;
namespace understanding {
namespace {
static int const maxresults = 10;
typedef std::map<std::string, double> wmap_t;
typedef std::map<std::string, wmap_t> wmat_t;

double coeff( int i, int n )
{
    int m = std::min<int>( n, maxresults );
    double wm = 0.25 / ( pow( 1.5, m ) - 1.0 );
    double wi = i > m ? 0.0 : ( pow( 1.5, m - i ) * wm );
    return wi;
}
struct Worker
{
    Wiki& wiki;
    int const numArticles;
    wmat_t weights;
    std::map<std::string, std::vector<std::string> > links;
    std::set<std::string> reachedLinks;
    wmap_t nodeWeights;
    std::set<std::string> selection;

    Worker( Wiki& w, int n ) : wiki( w ), numArticles( n ) {}
    std::vector<std::string> const& getLinks( std::string const& sourceTitle )
    {
        std::string str = Wiki::normalizeTitle( sourceTitle );
        auto it = links.find( str );
        if( it != links.end() )
        {
            return it->second;
        }
        std::vector<std::string>& rv = links[ str ];
        wiki.getLinks( str, rv );
        if( rv.size() > maxresults )
        {
            rv.resize( maxresults );
        }
        return rv;
    }
    int getNumRefs( std::string const& sourceTitle )
    {
        return getLinks( sourceTitle ).size();
    }
    void branchFromTitle( std::string const& title, std::string const& from, double titleWeight, int level )
    {
        std::string ntitle = Wiki::normalizeTitle( title );
        std::string nfrom = Wiki::normalizeTitle( from );
        reachedLinks.insert( ntitle );
        weights[ nfrom ][ ntitle ] += titleWeight;
        if( level >= numArticles )
        {
            return;
        }
        std::vector<std::string> const& links = getLinks( ntitle );
        int linkindex = 0;
        for( auto const& link : links )
        {
            ++linkindex;
            branchFromTitle( Wiki::normalizeTitle( link ), ntitle, titleWeight * coeff( linkindex, links.size() ), level + 1 );
        }
    }
    double nodeWeight( std::string const& str )
    {
        double rv = 0.0;
        for( auto const& wm : weights )
        {
            auto it = wm.second.find( str );
            if( wm.second.end() != it )
            {
                rv += it->second;
            }
        }
        if( !getLinks( str ).empty() )
        {
            rv *= 0.5;
        }
        return rv;
    }
    bool compareReachedLinks( std::string const& a, std::string const& b ) const
    {
        return nodeWeights.at( a ) > nodeWeights.at( b );
    }
    bool isReachableFromSelection( std::string const& str )
    {
        for( auto const& s : selection )
        {
            auto const& l = getLinks( s );
            if( std::find( l.begin(), l.end(), str ) != l.end() )
            {
                return true;
            }
        }
        return false;
    }
    void work( std::string const& sourceTitle )
    {
        if( numArticles < 1 )
        {
            return;
        }
        branchFromTitle( sourceTitle, "", 1.0, 1 );
        for( auto const& i : reachedLinks )
        {
            nodeWeights[ i ] = nodeWeight( i );
        }
        // select from root node greedily
        selection.insert( Wiki::normalizeTitle( sourceTitle ) );
        std::vector<std::string> reachedLinksVec( reachedLinks.begin(), reachedLinks.end() );
        using namespace std::placeholders;
        std::sort( reachedLinksVec.begin(), reachedLinksVec.end(), std::bind( &Worker::compareReachedLinks, this, _1, _2 )  );
        std::list<std::string> reachedLinksList( reachedLinksVec.begin(), reachedLinksVec.end() );
        while( selection.size() < numArticles && !reachedLinksList.empty() )
        {
            for( auto const& title : reachedLinksList )
            {
                if( isReachableFromSelection( title ) )
                {
                    selection.insert( title );
                    reachedLinksList.remove( title );
                    break;
                }
            }
        }
    }
};
}


std::set<std::string> getSuggestedTitles( wiki::Wiki& wiki, std::string const& sourceTitle, int numArticles )
{
    Worker wk( wiki, numArticles );
    wk.work( sourceTitle );
    return wk.selection;
}
}


