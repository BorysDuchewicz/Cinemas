#include "cinemas_repository.h"

std::string CinemasRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Cinema> CinemasRepository::add(const std::unique_ptr<Cinema>& cinema) const
{
	const std::string SQL = "insert into cinemas (name,location,price) values(?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, cinema->name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, cinema->location.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_double(stmt, 3, cinema->price);
	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Cinema> CinemasRepository::find_last() const
{
	const std::string SQL = "select id,name,location,price from cinemas ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Cinema{
		sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			 sqlite3_column_double(stmt,3)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<Cinema> CinemasRepository::find_by_id(const int& id) const
{
	const std::string SQL = "select id,name,location,price from cinemas where id=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);


	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{
		return std::make_optional(Cinema{
			sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			sqlite3_column_double(stmt,3)
			});
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Cinema>> CinemasRepository::find_all() const
{
	std::vector<std::unique_ptr<Cinema>> cinemas;
	const std::string SQL = "select id,name,location,price from cinemas";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		cinemas.emplace_back(std::make_unique<Cinema>(
			Cinema{ sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			sqlite3_column_double(stmt,3)
			}));
	}
	sqlite3_finalize(stmt);
	return cinemas;
}

std::optional<Cinema> CinemasRepository::delete_one(const int& id) const
{
	const std::string SQL = "delete from cinemas where id = ?1";
	sqlite3_stmt* stmt;

	const auto cinema_to_delete = find_by_id(id);
	if (cinema_to_delete.has_value()) {

		sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id);
		const auto result = sqlite3_step(stmt);

		if (result != SQLITE_DONE)
		{
			sqlite3_errmsg(connection->get_connection());
			throw std::exception(sqlite3_errmsg(connection->get_connection()));
		}
		sqlite3_finalize(stmt);
		return cinema_to_delete;
	}

	return std::nullopt;
}

void CinemasRepository::update_price(const int& id,const double& price) const
{
	const std::string SQL = "update cinemas set price = ? where id = ?";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	std::cout << price << std::endl;
	sqlite3_bind_double(stmt, 1, price);
	sqlite3_bind_int(stmt, 2, id);

	const auto result = sqlite3_step(stmt);

	if (result != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
}
