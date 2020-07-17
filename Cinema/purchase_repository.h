#pragma once
#include "db_connection.h"
#include "purchase.h"

//klasa do obslugi danych z bd dotyczacych historii zakupow
class PurchaseRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Purchase> add(const std::unique_ptr<Purchase>& purchase)const;//dodaje nowy element
	std::optional<Purchase> find_last()const;//wyszukuje ostatnio dodany element
	std::vector<std::unique_ptr<Purchase>> find_by_id_user(const int& id_user) const;//wyszukuje wszystkie zakupy uzytkownika o podanym id
};

