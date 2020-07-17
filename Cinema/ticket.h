#pragma once

#include "libs.h"


//struktura przechowujaca dane o biletach
struct Ticket
{
public:
	int id;
	int id_programme;
	Type type;
	int place_number;
	double price;

	Ticket(const int& id, const int& id_programme, const Type& type, const int& place_number, const double& price)
		:id{ id }, id_programme{ id_programme }, type{ type }, place_number{ place_number }, price{ price } {}

	Ticket(const int& id_programme, const Type& type, const int& place_number, const double& price)
		: id_programme{ id_programme }, type{ type }, place_number{ place_number }, price{ price }{}
};