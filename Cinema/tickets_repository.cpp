#include "tickets_repository.h"

std::string TicketsRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Ticket> TicketsRepository::add(const std::unique_ptr<Ticket>& ticket) const
{
	const std::string SQL = "insert into tickets (id_programme,type,place_number,price) values(?,?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, ticket->id_programme);
	const auto type_str = type_to_str(ticket->type);
	sqlite3_bind_text(stmt, 2, type_str.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 3, ticket->place_number);
	sqlite3_bind_double(stmt, 4, ticket->price);

	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Ticket> TicketsRepository::find_last() const
{
	const std::string SQL = "select id,id_programme,type,place_number,price from tickets order by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{

		return std::make_optional(Ticket{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
			str_to_type(from_unsigned_char_to_string(sqlite3_column_text(stmt,2))),
			sqlite3_column_int(stmt,3),
			sqlite3_column_double(stmt,4)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

double TicketsRepository::price(const double& price, const Type& type)
{
	if (type_to_str(type) == "Senior" || type_to_str(type) == "senior")
	{
		return 0.7 * price;
	}
	if (type_to_str(type) == "Student" || type_to_str(type) == "student" || type_to_str(type) == "Child" || type_to_str(type) == "child")
	{
		return 0.5 * price;
	}
	if (type_to_str(type) == "Normal" || type_to_str(type) == "normal")
	{
		return price;
	}
	return price;
}

std::optional<Ticket> TicketsRepository::find_by_id(const int& id) const
{
	const std::string SQL = "select id,id_programme,type,place_number,price from tickets where id = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{
		return std::make_optional(Ticket{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
			str_to_type(from_unsigned_char_to_string(sqlite3_column_text(stmt,2))),
			sqlite3_column_int(stmt,3),
			sqlite3_column_double(stmt,4)
			});
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}

void TicketsRepository::delete_one(const int& id) const
{
	const std::string SQL = "delete from tickets where id=?";
	sqlite3_stmt* stmt;

	const auto ticket_to_delete = find_by_id(id);
	if (ticket_to_delete.has_value()) {

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

int TicketsRepository::count_tickets(const int& id) const
{
	int how = 0;
	const std::string SQL = "select count(id_programme) from tickets where id_programme = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	const auto results = sqlite3_step(stmt);

	if (results == SQLITE_ROW)
	{
		how = sqlite3_column_int(stmt, 0);
	}
	sqlite3_finalize(stmt);
	return how;
}

std::vector<double> TicketsRepository::ticket_price(const int& id) const
{
	std::vector<double> price;
	const std::string SQL = "select price from tickets where id_programme = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	auto results = 0;

	while ((results = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		price.emplace_back(sqlite3_column_double(stmt, 0));
	}
	sqlite3_finalize(stmt);
	return price;
}

std::vector<std::string> TicketsRepository::find_type(const int& id) const
{
	std::vector<std::string> type;
	const std::string SQL = "select type from tickets where id_programme = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id);

	auto results = 0;
	while ((results = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		type.emplace_back(from_unsigned_char_to_string(sqlite3_column_text(stmt, 0)));
	}
	sqlite3_finalize(stmt);
	return type;
}
