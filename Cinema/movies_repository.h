#pragma once
#include "db_connection.h"
#include "movie.h"

//klasa obslugujaca dane o filmie z bd
class MoviesRepository
{	
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Movie> add(const std::unique_ptr<Movie>& movie)const;//dodaje film
	std::optional<Movie> find_last()const;//wyszukuje ostatnio dodany film do bd
	std::optional<Movie> find_by_id(const int& id) const;//wyszukuje film po id
	std::optional<Movie> find_by_name(const std::string& name) const;//wyszukuje film bo nazwie 
	std::vector<std::unique_ptr<Movie>> find_all() const;//wyszukuje wszystkie filmy z bd
	std::optional<Movie> delete_one(const int& id);//usuwa film o podanym id z bd
};

