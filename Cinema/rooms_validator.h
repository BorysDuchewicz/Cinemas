#pragma once
#include "validator.h"
#include "screening_rooms_repository.h"


//klasa sprawdzajaca czy podane dane dotyczace klasy ScreeningRoom spelniaja wymogi 
class ScreeningRoomsValidator:public Validator<ScreeningRoom>
{
private:
	std::shared_ptr<ScreeningRoomsRepository> rooms;

	bool is_room_exist(const ScreeningRoom& screening_room);
public:
	static bool is_place_is_taken(const ScreeningRoom& screening_room);
	std::unordered_map<std::string, std::string> validator(const ScreeningRoom& room)override;

	friend std::ostream& operator<<(std::ostream& out, const ScreeningRoomsValidator& val);

	ScreeningRoomsValidator(const std::shared_ptr<ScreeningRoomsRepository>& rooms);
	~ScreeningRoomsValidator() = default;
};

