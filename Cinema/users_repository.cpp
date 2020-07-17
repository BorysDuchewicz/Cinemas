#include "users_repository.h"

std::string UsersRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<User> UsersRepository::add(const std::unique_ptr<User>& user) const
{
	const std::string SQL = "insert into users (name,password,email,role) values(?,?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, user->name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, user->password.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, user->email.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 4, user->role);
	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<User> UsersRepository::find_last() const
{
	const std::string SQL = "select user_id,name,password,email,role from users ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(User{
		sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			 bool(sqlite3_column_int(stmt,4))
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<User> UsersRepository::find_by_email_or_name(const std::string& email) const
{
	const std::string SQL = "select user_id,name,password,email,role from users where email = ? or name =?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_STATIC);
	
	const auto results = sqlite3_step(stmt);
	
	while (results == SQLITE_ROW)
	{		
		return std::make_optional(User{
			sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			bool(sqlite3_column_int(stmt,4))
			});
	}
	
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<User>> UsersRepository::find_all() const
{
	std::vector<std::unique_ptr<User>> users;
	const std::string SQL = "select user_id,name,password,email,role from users";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		users.emplace_back(std::make_unique<User>(
			User{ sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			bool(sqlite3_column_int(stmt,4))
			}));
	}
	sqlite3_finalize(stmt);
	return users;
}
