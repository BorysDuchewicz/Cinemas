#pragma once
#include "libs.h"

//klasa umozliwiajca pobieranie poszczegolnych danych od uzytkownika
class UserDataService
{
public:
	UserDataService() = delete;
	~UserDataService() = delete;
	static int get_int(const std::string& message);
	static double get_double(const std::string& message);
	static std::string get_string(const std::string& message);
	static Type get_type(const std::string& message);
};

