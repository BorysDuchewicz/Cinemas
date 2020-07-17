#pragma once
#include "db_connection.h"
#include "favorite.h"

//klasa laczca sie z bd i obslugujaca dane o ulubionych filmach uzytkownikow
class FavoriteRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Favorite> add(const std::unique_ptr<Favorite>& favorite)const;//dodaje ulubiony film
	std::optional<Favorite> find_last()const;//wyszukuje ostatnio dodany element do bd
	std::optional<Favorite> find_movie(const int& id_user,const int& id_movie)const;//wyszukuje filmu po id uzytkownika i filmu
	std::vector<std::unique_ptr<Favorite>>find_all() const;//wyszukuje wszystkie filmy ktore ktos dodal do ulubionych
	std::vector<int> find_all_favorite_user_movies(const int& id_user) const;//wyszukuje ile polubien ma poszczegolny film 

};

