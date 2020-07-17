#include "reservations_repository.h"

std::string ReservationsRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Reservation> ReservationsRepository::add(const std::unique_ptr<Reservation>& reservations) const
{
	const std::string SQL = "insert into reservations (id_user,id_ticket) values(?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, reservations->id_user);
	sqlite3_bind_int(stmt, 2, reservations->id_ticket);

	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Reservation> ReservationsRepository::find_last() const
{
	const std::string SQL = "select id,id_user,id_ticket from reservations ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Reservation{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Reservation>> ReservationsRepository::find_by_id_user(const int& id_user) const
{
	std::vector<std::unique_ptr<Reservation>> reservations;
	const std::string SQL = "select id,id_user,id_ticket from reservations where id_user =?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_user);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		reservations.emplace_back(std::make_unique<Reservation>(
			Reservation{ sqlite3_column_int(stmt,0),
			 sqlite3_column_int(stmt,1),
				 sqlite3_column_int(stmt,2)
			}));
	}
	sqlite3_finalize(stmt);
	return reservations;
}

std::optional<Reservation> ReservationsRepository::find_by_id(const int& id) const
{
	const std::string SQL = "select id,id_user,id_ticket from reservations where id =?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{
		return std::make_optional(Reservation{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1)
			});
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}

void ReservationsRepository::delete_one(const int& id) const
{
	const std::string SQL = "delete from reservations where id=?";
	sqlite3_stmt* stmt;

	const auto reservation_to_delete = find_by_id(id);
	if (reservation_to_delete.has_value()) {

		sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id);
		const auto result = sqlite3_step(stmt);

		if (result != SQLITE_DONE)
		{
			sqlite3_errmsg(connection->get_connection());
			throw std::exception(sqlite3_errmsg(connection->get_connection()));
		}
	}
}
