#pragma once
#include  "db_connection.h"
#include "cinema.h"

//klasa umozliwiajaca polaczenie sie z baza danych i obslugi elementow dotyczacych kin
class CinemasRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Cinema> add(const std::unique_ptr<Cinema>& cinema)const;//umozliwia dodanie kina do bd
	std::optional<Cinema> find_last()const;//wyszukuje ostatnio dodane kino
	std::optional<Cinema> find_by_id(const int& id)const;//wyszukuje kino po id
	std::vector<std::unique_ptr<Cinema>> find_all()const;//znajduje wszystkie kina w bd
	std::optional<Cinema> delete_one(const int& id) const;//usuwa kino o podanych id
	void update_price(const int& id,const double& price)const;//zmienia cene biletu dla danego kina
};

