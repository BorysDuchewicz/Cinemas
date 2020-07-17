#pragma once
#include "db_connection.h"
#include "screening_room.h"

//klasa do obslugi danych z bd dotyczacych sal kinowych
class ScreeningRoomsRepository
{
private:
	DbConnection* connection = DbConnection::get_instance();
	static std::string from_unsigned_char_to_string(const unsigned char* value);
public:
	std::optional<ScreeningRoom> add(const std::unique_ptr<ScreeningRoom>& screening_room)const;//dodaje sale kinowa
	std::optional<ScreeningRoom> find_last()const;//wyszukuje ostatnio dodany element
	std::optional<ScreeningRoom> find_by_cinema_and_room(const int& id_cinema, const int& room_number)const;//wyszkuje danej sali w danym kinie
	std::vector<std::unique_ptr<ScreeningRoom>> find_room_in_cinema(const int& id_cinema,const int& room_number) const;//wyszukuje sale w danym kinie
	std::vector<std::unique_ptr<ScreeningRoom>> find_cinema_rooms(const int& id_cinema) const;//wyszukuje wszystkie sale w danym kinie
	std::optional<ScreeningRoom> find_place_status(const int& id_cinema, const int& room_number, const int& place_number)const;//wyszukuje i sprawdza dostenosc danego miejsca 
	std::optional<ScreeningRoom> delete_one(const int& id_cinema, const int& room_number)const;//usuwa podana sale z kina

	void update(const int& id_cinema,const int& room_number,const int& place_number,const int& status) const;//aktualizuje dostepnosc miejsca
	void show_places(const int& id_cinema,const int& room_number) const;//pokazuje miejsca w danej sali
};

