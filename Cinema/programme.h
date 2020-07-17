#pragma once
#include "libs.h"


//struktura przechowujaca dane o repertuarze 
struct Programme
{
public:
	int id;
	int id_movie;
	int id_cinema;
	std::string day;
	std::string hour;
	int screening_room;

	Programme() = default;
	Programme(const int& id, const int& id_movie, const int& id_cinema, const std::string& day, const std::string& hour, const int& screening_room)
		:id{ id }, id_movie{ id_movie }, id_cinema{ id_cinema}, day{ day }, hour{ hour },screening_room{screening_room}{}
	Programme(const int& id_movie, const int& id_cinema, const std::string& day, const std::string& hour,const int& screening_room)
		:id_movie{ id_movie }, id_cinema{ id_cinema }, day{ day }, hour{ hour }, screening_room{screening_room}{}

	bool operator== (const Programme& val) const
	{
		return val.id_cinema == this->id_cinema && val.day == this->day && val.hour == this->hour && val.screening_room==this->screening_room;
	}
	friend std::ostream& operator<<(std::ostream& out, const Programme& programme)
	{
		return out << programme.id_movie << " " << programme.id_cinema << " " << programme.day << " " << programme.hour <<" "<<programme.screening_room<< std::endl;
	}
};




