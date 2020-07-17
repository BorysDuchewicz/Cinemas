#pragma once

#include "libs.h"
//struktura przechowujaca informacje o filmie
struct Movie
{
public:
	int id;
	std::string name;
	std::string genre;
	std::string description;
	

	Movie(const int& id, const std::string& name, const std::string& genre, const std::string& description)
	:id{ id }, name{ name }, genre{ genre },description{description}{}
	Movie(const std::string& name, const std::string& genre, const std::string& description) : name{ name }, genre{ genre }, description{description} {}

	friend std::ostream& operator<<(std::ostream& out, const Movie& movie)
	{
		return out<<movie.id<<" "<< movie.name << " " << movie.genre<<" "<<movie.description;
	}

};