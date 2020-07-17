#include "rooms_validator.h"


std::ostream& operator<<(std::ostream& out, const ScreeningRoomsValidator& val)
{
	for (const auto& i : val.errors)
	{
		out << i.first << " " << i.second << std::endl;
	}
	return out;

}

bool ScreeningRoomsValidator::is_place_is_taken(const ScreeningRoom& screening_room)
{
	
	if (screening_room.status == 1 || screening_room.status == 2)
	{
		return false;
	}
	return true;
}

bool ScreeningRoomsValidator::is_room_exist(const ScreeningRoom& screening_room)
{
	for(const auto& ele:rooms->find_cinema_rooms(screening_room.id_cinema))
	{
		if(ele->room_number==screening_room.room_number)
		{
			return false;
		}
	}
	return true;
}

std::unordered_map<std::string, std::string> ScreeningRoomsValidator::validator(const ScreeningRoom& room)
{
	if(!is_place_is_taken(room))
	{
		errors.emplace(std::make_pair("This place", "is take"));
	}
	if(!is_room_exist(room))
	{
		errors.emplace(std::make_pair("This screening room ", "exist in this cinema"));
	}
	return errors;
}

ScreeningRoomsValidator::ScreeningRoomsValidator(const std::shared_ptr<ScreeningRoomsRepository>& rooms)
:rooms{rooms}{}
