#pragma once
#include "libs.h"

//szablon dla walidacji
template <class T>
class Validator
{
protected:
	std::unordered_map<std::string, std::string> errors;
public:
	virtual std::unordered_map<std::string, std::string> validator(const T& t) = 0;

	Validator() = default;
	virtual ~Validator() = default;
};