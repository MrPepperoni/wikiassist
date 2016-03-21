#pragma once
#ifndef INCLUDED_UNDERSTANDING_H
#define INCLUDED_UNDERSTANDING_H

#include "../wikilib/wiki.h"
#include <set>

namespace understanding {
std::set<std::string> getSuggestedTitles( wiki::Wiki& wiki, std::string const& sourceTitle, int numArticles );
}

#endif // INCLUDED_UNDERSTANDING_H

