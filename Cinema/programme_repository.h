#pragma once
#include "db_connection.h"
#include "programme.h"

//klasa umozliwiajca obsluge informacji z bd o repertuarach
class ProgrammeRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<Programme> add(const std::unique_ptr<Programme>& programme)const;//dodaje repertuar
	std::optional<Programme> find_last()const;//wyszukuje ostatnio dodany element
	std::vector<std::unique_ptr<Programme>> find_all_movies_from_cinema(const int& id_cinema) const;//wyszukuje wszystkie filmy w podanym kinie
	std::vector<std::unique_ptr<Programme>> find_all() const;//wyszukuje wszystkie repertuary
	std::optional<Programme> find_programme(const int& id_cinema,const int& id_movie,const std::string& day,const std::string& hour) const;//wyszukuje dany repertuar 
	std::optional<Programme> find_by_id(const int& id) const;//wyszukuje repertuar po id
	std::optional<Programme> delete_one(const int& id) const;//usuwa repertuar o danym id
	
};

