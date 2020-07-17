#pragma once
#include "libs.h"


//struktura przechowujaca dane o rezerwacji
struct Reservation
{

public:
	int id;
	int id_user;
	int id_ticket;

	Reservation(const int& id,const int& id_user,const int& id_ticket):id{id},id_user{id_user},id_ticket{id_ticket}{}
	Reservation(const int& id_user,const int& id_ticket):id_user{id_user},id_ticket{id_ticket}{}
	Reservation() = default;
};