#include "purchase_repository.h"

std::string PurchaseRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Purchase> PurchaseRepository::add(const std::unique_ptr<Purchase>& purchase) const
{
	const std::string SQL = "insert into purchase_history (id_user,id_ticket,purchase_date) values(?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, purchase->id_user);
	sqlite3_bind_int(stmt, 2, purchase->id_ticket);
	sqlite3_bind_text(stmt, 3, purchase->purchase_date.c_str(), -1, SQLITE_STATIC);

	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Purchase> PurchaseRepository::find_last() const
{
	const std::string SQL = "select id,id_user,id_ticket,purchase_date from purchase_history ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Purchase{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3))
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Purchase>> PurchaseRepository::find_by_id_user(const int& id_user) const
{
	std::vector<std::unique_ptr<Purchase>> purchase_history;
	const std::string SQL = "select id_user,id_ticket,purchase_date from purchase_history where id_user =?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_user);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		purchase_history.emplace_back(std::make_unique<Purchase>(
			Purchase{ sqlite3_column_int(stmt,0),
			 sqlite3_column_int(stmt,1),
				from_unsigned_char_to_string(sqlite3_column_text(stmt,2))
			}));
	}
	sqlite3_finalize(stmt);
	return purchase_history;
}
