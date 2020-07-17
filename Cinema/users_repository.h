#pragma once
#include  "db_connection.h"
#include "user.h"

//klasa do obslugi danych z bd dotyczacych uzytkownikow
class UsersRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char*value);
public:
	std::optional<User> add(const std::unique_ptr<User>& user)const;//dodaje uzytkownika
	std::optional<User> find_last()const;//wyszukuje ostatnio dodanego
	std::optional<User> find_by_email_or_name(const std::string& email)const;//wyszukuje uzytkownika po emailu lub nicku
	std::vector<std::unique_ptr<User>> find_all() const;//wyszukuje wszystkich uzytkownikow
};

