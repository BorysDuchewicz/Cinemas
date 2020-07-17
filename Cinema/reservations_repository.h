#pragma once
#include "db_connection.h"
#include "reservation.h"

//klasa do obslugi danych z bd dotyczacych rezerwacji
class ReservationsRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Reservation> add(const std::unique_ptr<Reservation>& reservations)const;//dodaje elemnt
	std::optional<Reservation> find_last()const;//wyszukuje ostatnio dodany element
	std::vector<std::unique_ptr<Reservation>> find_by_id_user(const int& id_user) const;//wyszukuje rezerwacji podanego uzytkownika
	std::optional<Reservation> find_by_id(const int& id) const;//wyszukuje rezerwacji o podanym id
	void delete_one(const int& id) const;//usuwa rezerwacje o danym id
};

