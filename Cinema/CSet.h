#ifndef _CSET_
#define _CSET_

#include <string>

struct Charset
{
	Charset(std::string set)
	{
		this->set = set;
	}
	std::string set;
};

#endif