#pragma once
#include "db_connection.h"
#include "ticket.h"

//klasa do obslugi danych z bd dotyczacych biletow
class TicketsRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Ticket> add(const std::unique_ptr<Ticket>& ticket)const;//dodaje bilet
	std::optional<Ticket> find_last()const;//wyszukuje ostatnio dodany bilet
	double price(const double& price,const Type& type);//ustala cene po uzwglednieniu znizek
	std::optional<Ticket> find_by_id(const int& id)const;//wyszukuje biletu po id
	void delete_one(const int& id) const;//usuwa bilet o podanym id
	int count_tickets(const int& id)const;//liczy ilosc biletow
	std::vector<double> ticket_price(const int& id)const;//zwraca ceny biletow
	std::vector<std::string> find_type(const int& id)const;//zwraca typy biletow
};

