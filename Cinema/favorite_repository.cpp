#include "favorite_repository.h"

std::string FavoriteRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Favorite> FavoriteRepository::add(const std::unique_ptr<Favorite>& favorite) const
{
	const std::string SQL = "insert into favorite (id_user,id_movie) values(?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, favorite->id_user);
	sqlite3_bind_int(stmt, 2, favorite->id_movie);

	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Favorite> FavoriteRepository::find_last() const
{
	const std::string SQL = "select id,id_user,id_movie from favorite ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Favorite{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2)
		});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<Favorite> FavoriteRepository::find_movie(const int& id_user, const int& id_movie) const
{
	const std::string SQL = "select id,id_user,id_movie from favorite where id_user=? and  id_movie=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt,1, id_user);
	sqlite3_bind_int(stmt,2, id_movie);
	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Favorite{
		sqlite3_column_int(stmt,0),
		sqlite3_column_int(stmt,1),
		sqlite3_column_int(stmt,2)
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Favorite>> FavoriteRepository::find_all() const
{
	std::vector<std::unique_ptr<Favorite>> favorite;
	const std::string SQL = "select id,id_user,id_movie from favorite";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		favorite.emplace_back(std::make_unique<Favorite>(
			Favorite{ sqlite3_column_int(stmt,0),
			sqlite3_column_int(stmt,1),
			sqlite3_column_int(stmt,2)
			}));
	}
	sqlite3_finalize(stmt);
	return favorite;
}

std::vector<int> FavoriteRepository::find_all_favorite_user_movies(const int& id_user) const
{
	std::vector<int> favorite;
	const std::string SQL = "select id_movie from favorite where id_user=?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_int(stmt, 1, id_user);
	auto result = 0;

	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		favorite.emplace_back(sqlite3_column_int(stmt,2));
	}
	sqlite3_finalize(stmt);
	return favorite;
}
