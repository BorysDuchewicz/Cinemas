#pragma once
#include "libs.h"

//struktura przechowujaca dane o kinie 
struct Cinema
{
public:
	int id;
	std::string name;
	std::string location;
	double price;
	
	Cinema(const int& id, const std::string& name, const std::string& location, const double& price) :id{ id }, name{ name }, location{ location }, price{ price } {}
	Cinema(const std::string& name, const std::string& location, const double& price) :name{ name }, location{ location }, price{ price } {}

	friend std::ostream& operator<<(std::ostream& out,const Cinema& cinema)
	{
		return out << cinema.id << " " << cinema.name << " " << cinema.location << std::endl;
	}
	
};
