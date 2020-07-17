#pragma once
#include "libs.h"
//strtuktura przechowujaca informacje o ulubionym filmie uzytkownika
struct Favorite
{
public:
	int id;
	int id_user;
	int id_movie;

	Favorite(const int& id, const int& id_user, const int& id_movie) :id{ id }, id_user{ id_user }, id_movie{ id_movie }{}
	Favorite(const int& id_user, const int& id_movie) : id_user{ id_user }, id_movie{ id_movie }{}
};