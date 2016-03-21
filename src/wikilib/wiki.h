#pragma once
#ifndef INCLUDED_WIKI_H
#define INCLUDED_WIKI_H

#include <string>
#include <vector>
#include <memory>

namespace wiki {
struct ConnectionDesc
{
    std::string host; // hostname[:port]
    std::string db;
    std::string username;
    std::string password;
};
class Wiki
{
public:
    explicit Wiki( ConnectionDesc const& connection );
    virtual ~Wiki();
    /// \brief collects the in-site links referenced from sourceTitle in linkedTitles
    /// \return true on success, false on failure
    bool getLinks( std::string const& sourceTitle, std::vector< ::std::string >& linkedTitles );
    static std::string normalizeTitle( std::string const& str );
private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};
} // namespace wiki

#endif // INCLUDED_WIKI_H

