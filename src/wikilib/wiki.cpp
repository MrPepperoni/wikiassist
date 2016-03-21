#include "wiki.h"


#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cctype>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace wiki {

class Wiki::Impl
{
    sql::Driver* mDriver;
    std::unique_ptr<sql::Connection> mConnection;
    std::unique_ptr<sql::PreparedStatement> mStatement;
    ConnectionDesc const mConnectionDesc;
public:
    bool getLinks( std::string const& sourceTitle, std::vector< ::std::string >& linkedTitles );
    Impl( ConnectionDesc const& c );
    ~Impl();
};

Wiki::Impl::Impl( ConnectionDesc const& c )
    : mConnectionDesc( c )
{
    mDriver = get_driver_instance();
    mConnection.reset( mDriver->connect( mConnectionDesc.host,
                mConnectionDesc.username,
                mConnectionDesc.password ) );
    mConnection->setSchema( mConnectionDesc.db );

    mStatement.reset( mConnection->prepareStatement("SELECT pagelinks.pl_title FROM page INNER JOIN pagelinks ON pagelinks.pl_from=page.page_id AND pagelinks.pl_from_namespace=page.page_namespace WHERE page.page_namespace=0 AND page.page_title=? ORDER BY pagelinks.number_in_page") );
}

Wiki::Impl::~Impl()
{
}


std::string Wiki::normalizeTitle( std::string const& str )
{
    auto rv = str;
    if( !rv.empty() )
    {
        rv[0] = toupper( rv[0] );
    }
    return rv;
}

bool Wiki::Impl::getLinks( std::string const& sourceTitle, std::vector< ::std::string >& linkedTitles )
{
    std::string sourceTitleNormalized = Wiki::normalizeTitle( sourceTitle );
    try
    {
        std::vector< std::string > rv;
        mStatement->setString( 1, sourceTitleNormalized );
        std::unique_ptr<sql::ResultSet> res( mStatement->executeQuery() );
        for(;;)
        {
            while ( res->next() )
            {
                rv.push_back( res->getString( 1 ) );
            }

            if (mStatement->getMoreResults())
            {
                res.reset(mStatement->getResultSet());
                continue;
            }
            break;
        }
        std::swap( rv, linkedTitles );
        return true;
    }
    catch( ... )
    {
        return false;
    }
}
Wiki::Wiki( ConnectionDesc const& desc )
    : mImpl( new Impl( desc ) )
{
}

Wiki::~Wiki()
{
}

bool Wiki::getLinks( std::string const& sourceTitle, std::vector< ::std::string >& linkedTitles )
{
    return mImpl->getLinks( sourceTitle, linkedTitles );
}

} // namespace wiki
