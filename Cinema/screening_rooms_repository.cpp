#include "screening_rooms_repository.h"

#include "programme.h"

std::string ScreeningRoomsRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<ScreeningRoom> ScreeningRoomsRepository::add(const std::unique_ptr<ScreeningRoom>& screening_room) const
{
	const std::string SQL = "insert into screening_room (id_cinema,status,place_number,room_number) values(?,?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, screening_room->id_cinema);
	sqlite3_bind_int(stmt, 2, screening_room->status);
	sqlite3_bind_int(stmt, 3, screening_room->place_number);
	sqlite3_bind_int(stmt, 4, screening_room->room_number);
	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<ScreeningRoom> ScreeningRoomsRepository::find_last() const
{
	const std::string SQL = "select id,id_cinema,status,place_number,room_number from screening_room ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(ScreeningRoom{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
		sqlite3_column_int(stmt,3),
		sqlite3_column_int(stmt,4)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<ScreeningRoom> ScreeningRoomsRepository::find_by_cinema_and_room(const int& id_cinema, const int& room_number) const
{
	const std::string SQL = "select id,id_cinema,status,place_number,room_number from screening_room where id_cinema=? and room_number=?";
	sqlite3_stmt* stmt;
	
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	sqlite3_bind_int(stmt, 2, room_number);
	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(ScreeningRoom{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
		sqlite3_column_int(stmt,3),
		sqlite3_column_int(stmt,4)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<ScreeningRoom>> ScreeningRoomsRepository::find_room_in_cinema(const int& id_cinema,
	const int& room_number) const
{
	std::vector<std::unique_ptr<ScreeningRoom>> places;
	const std::string SQL = "select status,place_number from screening_room where id_cinema=? and room_number=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	sqlite3_bind_int(stmt, 2, room_number);
	auto result = 0;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		places.emplace_back(std::make_unique<ScreeningRoom>(ScreeningRoom{
			sqlite3_column_int(stmt,0),
			sqlite3_column_int(stmt,1),
			}));
	}
	sqlite3_finalize(stmt);
	return places;
}

std::vector<std::unique_ptr<ScreeningRoom>> ScreeningRoomsRepository::find_cinema_rooms(const int& id_cinema) const
{
	std::vector<std::unique_ptr<ScreeningRoom>> rooms;
	const std::string SQL = "select id,id_cinema,status,place_number,room_number from screening_room where id_cinema=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	
	auto result = 0;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		rooms.emplace_back(std::make_unique<ScreeningRoom>(ScreeningRoom{
			sqlite3_column_int(stmt,1),
			sqlite3_column_int(stmt,2),
			sqlite3_column_int(stmt,3),
			sqlite3_column_int(stmt,4),
			}));
	}
	sqlite3_finalize(stmt);
	return rooms;
}

std::optional<ScreeningRoom> ScreeningRoomsRepository::find_place_status(const int& id_cinema, const int& room_number,
	const int& place_number) const
{
	const std::string SQL = "select id_cinema,status,place_number,room_number from screening_room where id_cinema=? and place_number =? and room_number=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	sqlite3_bind_int(stmt, 2, place_number);
	sqlite3_bind_int(stmt, 3, room_number);


	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{
		return std::make_optional(ScreeningRoom{
			sqlite3_column_int(stmt,0),
			sqlite3_column_int(stmt,1),
			sqlite3_column_int(stmt,2),			
			sqlite3_column_int(stmt,3)
			});
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<ScreeningRoom> ScreeningRoomsRepository::delete_one(const int& id_cinema, const int& room_number) const
{
	const std::string SQL = "delete from movies where id_cinema = ? and room_number=?";
	sqlite3_stmt* stmt;

	const auto movies_to_delete = find_by_cinema_and_room(id_cinema,room_number);
	if (movies_to_delete.has_value()) {

		sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id_cinema);
		sqlite3_bind_int(stmt, 2, room_number);
		const auto result = sqlite3_step(stmt);

		if (result != SQLITE_DONE)
		{
			sqlite3_errmsg(connection->get_connection());
			throw std::exception(sqlite3_errmsg(connection->get_connection()));
		}
		sqlite3_finalize(stmt);
		return movies_to_delete;
	}

	return std::nullopt;
}


void ScreeningRoomsRepository::update(const int& id_cinema, const int& room_number, const int& place_number, const int& status) const
{
	const std::string SQL = "update screening_room set status = ? where id_cinema = ? and room_number = ? and place_number = ?";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	sqlite3_bind_int(stmt, 1, status);
	sqlite3_bind_int(stmt, 2, id_cinema);
	sqlite3_bind_int(stmt, 3, room_number);
	sqlite3_bind_int(stmt, 4, place_number);

	const auto result = sqlite3_step(stmt);

	if (result != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
}

void ScreeningRoomsRepository::show_places(const int& id_cinema, const int& room_number) const
{
	int i = 0;
	for (const auto& room : find_room_in_cinema(id_cinema,room_number))
	{
		i++;
		if (room->status == 0)
		{
			std::cout << "[ " << i << " ] ";
		}
		else if (room->status == 1)
		{
			std::cout << "[ r ] ";
		}
		else if (room->status == 2)
		{
			std::cout << "[ x ] ";
		}
		if (i % 5 == 0)
		{
			std::cout << std::endl;
		}
	}
}

