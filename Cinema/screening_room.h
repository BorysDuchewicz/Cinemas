#pragma once
#include "libs.h"


//struktura przechowujaca informacje o miejscu w kinie
struct ScreeningRoom
{
public:
	int id;
	int id_cinema;
	int status;
	int place_number;
	int room_number;

	ScreeningRoom(const int& id, const int& id_cinema, const int& status, const int& place_number, const int& room_number )
		:id{id},id_cinema{id_cinema},status{status},place_number{place_number},room_number{room_number}{}
	ScreeningRoom( const int& id_cinema, const int& status, const int& place_number, const int& room_number)
		:id_cinema{ id_cinema }, status{ status }, place_number{ place_number }, room_number{ room_number }{}
	ScreeningRoom(const int& status, const int& place_number)
		: status{ status }, place_number{ place_number }{}
};