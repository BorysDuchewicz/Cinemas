#pragma once

#include "libs.h"

//struktura przechowujaca dane o uzytkowniku
struct User
{
public:
	int id;
	std::string name;
	std::string password;
	std::string email;
	bool role;

	User(const int& id, const std::string& name, const std::string& password, const std::string& email, const bool& role) :id{ id }, name{ name }, password{ password }, email{ email }, role{role} {}
	User(const std::string& name, const std::string& password, const std::string& email, const bool& role) : name{ name }, password{ password }, email{ email }, role{role} {}


	friend std::ostream& operator<<(std::ostream& out, const User& user)
	{
		return out << user.id << " " << user.name << " " << user.password << " " << user.email << std::endl;
	}

};
