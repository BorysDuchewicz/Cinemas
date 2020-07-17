#include "movies_repository.h"

std::string MoviesRepository::from_unsigned_char_to_string(const unsigned char* value)
{
	std::string result;
	for (int i = 0; i < strlen(reinterpret_cast<const char*>(value)); ++i)
	{
		result += value[i];
	}
	return result;
}

std::optional<Movie> MoviesRepository::add(const std::unique_ptr<Movie>& movie) const
{
	const std::string SQL = "insert into movies (name,genre,description) values(?,?,?);";
	sqlite3_stmt* stmt = nullptr;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, movie->name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, movie->genre.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, movie->description.c_str(), -1, SQLITE_STATIC);
	const auto results = sqlite3_step(stmt);

	if (results != SQLITE_DONE)
	{
		sqlite3_errmsg(connection->get_connection());
		throw std::exception(sqlite3_errmsg(connection->get_connection()));
	}
	sqlite3_finalize(stmt);
	return find_last();
}

std::optional<Movie> MoviesRepository::find_last() const
{
	const std::string SQL = "select id,name,genre,description from movies ordered by id desc limit 1";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);

	const auto results = sqlite3_step(stmt);
	while (results == SQLITE_ROW)
	{
		return std::make_optional(Movie{
		sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			});
	}
	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::optional<Movie> MoviesRepository::find_by_id(const int& id) const
		{
			const std::string SQL = "select id,name,genre,description from movies where id = ?";
			sqlite3_stmt* stmt;
			sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
			sqlite3_bind_int(stmt, 1, id);

			const auto results = sqlite3_step(stmt);

			while (results == SQLITE_ROW)
			{
				return Movie{
					sqlite3_column_int(stmt,0),
					from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
					from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
					from_unsigned_char_to_string(sqlite3_column_text(stmt,3))
				};
			}

			sqlite3_finalize(stmt);
			return std::nullopt;
}

std::optional<Movie> MoviesRepository::find_by_name(const std::string& name) const
{
	const std::string SQL = "select id,name,genre,description from movies where name = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_text(stmt, 1, name.c_str(),-1,SQLITE_STATIC);

	const auto results = sqlite3_step(stmt);

	while (results == SQLITE_ROW)
	{
		return Movie{
			sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3))
		};
	}

	sqlite3_finalize(stmt);
	return std::nullopt;
}

std::vector<std::unique_ptr<Movie>> MoviesRepository::find_all() const
{
	std::vector<std::unique_ptr<Movie>> movies;
	const std::string SQL = "select id,name,genre,description from movies";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
	auto result = 0;
	while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		movies.emplace_back(std::make_unique<Movie>(
			Movie{ sqlite3_column_int(stmt,0),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,1)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,2)),
			from_unsigned_char_to_string(sqlite3_column_text(stmt,3)),
			}));
	}
	sqlite3_finalize(stmt);
	return movies;
}

std::optional<Movie> MoviesRepository::delete_one(const int& id)
{
	const std::string SQL = "delete from movies where id = ?1";
	sqlite3_stmt* stmt;

	const auto movies_to_delete = find_by_id(id);
	if (movies_to_delete.has_value()) {

		sqlite3_prepare_v2(connection->get_connection(), SQL.c_str(), -1, &stmt, nullptr);
		sqlite3_bind_int(stmt, 1, id);
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
