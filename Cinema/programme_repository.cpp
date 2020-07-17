#include "programme_repository.h"

std::string ProgrammeRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Programme> ProgrammeRepository::add(const std::unique_ptr<Programme>& programme) const
{
	const std::string SQL = "insert into programme (id_movie,id_cinema,day,hour,screening_room) values(?,?,?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, programme->id_movie);
	sqlite3_bind_int(stmt, 2, programme->id_cinema);
	sqlite3_bind_text(stmt, 3, programme->day.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, programme->hour.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 5, programme->screening_room);
	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Programme> ProgrammeRepository::find_last() const
{
	const std::string SQL = "select id,id_movie,id_cinema,day,hour,screening_room from programme ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Programme{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,4)),
				sqlite3_column_int(stmt,5),
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Programme>> ProgrammeRepository::find_all_movies_from_cinema(const int& id_cinema) const
{
	std::vector<std::unique_ptr<Programme>> programme;
	const std::string SQL = "select id,id_movie,id_cinema,day,hour,screening_room from programme where id_cinema=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	auto result = 0;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		programme.emplace_back(std::make_unique<Programme>(Programme{
			sqlite3_column_int(stmt,0),
			sqlite3_column_int(stmt,1),
			sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,4)),
			sqlite3_column_int(stmt,5)		
		}));
	}
	sqlite3_finalize(stmt);
	return programme;
}

std::vector<std::unique_ptr<Programme>> ProgrammeRepository::find_all() const
{
	std::vector<std::unique_ptr<Programme>> programmes;
	const std::string SQL = "select id,id_movie,id_cinema,day,hour,screening_room from programme";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		programmes.emplace_back(std::make_unique<Programme>(
			Programme{ sqlite3_column_int(stmt,0),
			sqlite3_column_int(stmt,1),
			sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,4)),
			sqlite3_column_int(stmt,5)
			}));
	}
	sqlite3_finalize(stmt);
	return programmes;
}

std::optional<Programme> ProgrammeRepository::find_programme(const int& id_cinema, const int& id_movie,
	const std::string& day, const std::string& hour) const
{
	const std::string SQL = "select id,id_movie,id_cinema,day,hour,screening_room from programme where id_cinema=? and id_movie=? and day =? and hour =?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_cinema);
	sqlite3_bind_int(stmt, 2, id_movie);
	sqlite3_bind_text(stmt, 3, day.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, hour.c_str(), -1, SQLITE_STATIC);
	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Programme{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,4)),
				sqlite3_column_int(stmt,5),
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<Programme> ProgrammeRepository::find_by_id(const int& id) const
{
	const std::string SQL = "select id,id_movie,id_cinema,day,hour,screening_room from programme where id=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Programme{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,4)),
				sqlite3_column_int(stmt,5),
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<Programme> ProgrammeRepository::delete_one(const int& id) const
{
	const std::string SQL = "delete from programme where id = ?";
	sqlite3_stmt* stmt;

	const auto programme_to_delete = find_by_id(id);
	if (programme_to_delete.has_value()) {

		sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id);
		const auto result = sqlite3_step(stmt);

		if (result != SQLITE_DONE)
		{
			sqlite3_errmsg(connection->get_connection());
			throw std::exception(sqlite3_errmsg(connection->get_connection()));
		}
		sqlite3_finalize(stmt);
		return programme_to_delete;
	}

	return std::nullopt;
}


