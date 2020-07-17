#pragma once
#include<iostream>
#include<memory>
#include<vector>
#include<string>
#include<fstream>
#include <optional>
#include <regex>
#include <algorithm>


#include "json.hpp"
#include "sqlite3.h"
#include "Cryptor.h"



using json = nlohmann::json;

enum class Type{student,senior,child,normal};

static std::vector<std::string> TypeValues = { "student","senior","child","normal" };

inline Type str_to_type(const std::string& txt)
{
	if(txt=="student" || txt=="Student")
	{
		return Type::student;
	}
	if (txt == "senior"||txt=="Senior")
	{
		return Type::senior;
	}
	if (txt == "child"||txt=="Child")
	{
		return Type::child;
	}
	if (txt == "normal"|| txt=="Normal")
	{
		return Type::normal;
	}
	throw std::exception("No type");
}

inline std::string type_to_str(const Type& type)
{
	return TypeValues[static_cast<int>(type)];
}

template <typename A, typename B>
std::multimap<B, A,std::greater<>> flip_map(std::map<A, B>& src) {

	std::multimap<B, A,std::greater<>> dst;

	for (typename std::map<A, B>::const_iterator it = src.begin(); it != src.end(); ++it)
		dst.insert(std::pair<B, A>(it->second, it->first));

	return dst;
}