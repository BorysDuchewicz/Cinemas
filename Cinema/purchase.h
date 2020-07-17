#pragma once
#include "libs.h"


//struktura przechowujaca dane o zakupie biletu 
struct Purchase
{
public:
	int id;
	int id_user;
	int id_ticket;
	std::string purchase_date;

	Purchase(const int& id, const int& id_user, const int& id_ticket, const std::string& purchase_date)
		:id{id},id_user{id_user},id_ticket{id_ticket},purchase_date{purchase_date}{}
	Purchase(const int& id_user, const int& id_ticket, const std::string& purchase_date)
		: id_user{ id_user }, id_ticket{ id_ticket }, purchase_date{ purchase_date }{}
};