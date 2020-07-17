#include "db_connection.h"

DbConnection::DbConnection()
{
	const auto results = sqlite3_open(database_name.c_str(), &connection);
	if (SQLITE_OK != results)
	{
		throw std::exception(sqlite3_errmsg(connection));
	}
	create_table();
}

void DbConnection::create_table() const
{
	const std::string sql_users =
		"create table if not exists users("
		"user_id integer primary key autoincrement,"
		"name varchar(30) not null,"
		"password varchar(30) not null,"
		"email varchar(30) not null,"
		"role boolean not null); ";
	char** error_message = nullptr;
	auto results = sqlite3_exec(connection, sql_users.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("table with users creation failed");
	}

	const std::string sql_movies =
		"create table if not exists movies("
		"id integer primary key autoincrement,"
		"name varchar(30) not null,"
		"genre varchar(30) not null,"
		"description varchar(256));";

	results = sqlite3_exec(connection, sql_movies.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("movies table creation failed");
	}

	const std::string sql_cinemas =
		"create table if not exists cinemas("
		"id integer primary key autoincrement,"
		"name varchar(30) not null,"
		"location varchar(30) not null,"
		"price double not null);";

	results = sqlite3_exec(connection, sql_cinemas.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("cinemas table creation failed");
	}

	const std::string sql_programme =
		"create table if not exists programme("
		"id integer primary key autoincrement,"
		"id_movie integer not null,"
		"id_cinema integer not null,"
		"day varchar(10) not null,"
		"hour varchar(5) not null,"
		"screening_room integer not null,"
		"foreign key (id_movie) references movies(id) on delete cascade on update cascade,"
		"foreign key (id_cinema) references cinemas(id) on delete cascade on update cascade);";

	results = sqlite3_exec(connection, sql_programme.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("programme table creation failed");
	}

	const std::string sql_favorite =
		"create table if not exists favorite("
		"id integer primary key autoincrement,"
		"id_user integer not null,"
		"id_movie integer not null,"
		"foreign key (id_user) references users(id) on delete cascade on update cascade,"
		"foreign key (id_movie) references movies(id) on delete cascade on update cascade);";

	results = sqlite3_exec(connection, sql_favorite.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("favorite table creation failed");
	}

	const std::string sql_tickets =
		"create table if not exists tickets("
		"id integer primary key autoincrement,"
		"id_programme integer not null,"
		"type varchar(10) not null,"
		"place_number integer not null,"
		"price double not null,"
		"foreign key (id_programme) references programme(id) on delete cascade on update cascade);";
		

	results = sqlite3_exec(connection, sql_tickets.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("tickets table creation failed");
	}

	const std::string sql_purchase_history =
		"create table if not exists purchase_history("
		"id integer primary key autoincrement,"
		"id_user integer not null,"
		"id_ticket integer not null,"
		"purchase_date varchar(10) not null,"
		"foreign key (id_user) references users(id) on delete cascade on update cascade,"
		"foreign key (id_ticket) references tickets(id) on delete cascade on update cascade);";

	results = sqlite3_exec(connection, sql_purchase_history.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("purchase history table creation failed");
	}

	const std::string sql_screening_room =
		"create table if not exists screening_room("
		"id integer primary key autoincrement,"
		"id_cinema integer not null,"
		"status integer not null,"
		"place_number integer not null,"
		"room_number integer not null,"
		"foreign key (id_cinema) references cinemas(id) on delete cascade on update cascade);";

	results = sqlite3_exec(connection, sql_screening_room.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("screening room table creation failed");
	}

	const std::string sql_reservations =
		"create table if not exists reservations("
		"id integer primary key autoincrement,"
		"id_user integer not null,"
		"id_ticket integer not null,"
		"foreign key (id_user) references users(id) on delete cascade on update cascade,"
		"foreign key (id_ticket) references tickets(id) on delete cascade on update cascade);";

	results = sqlite3_exec(connection, sql_reservations.c_str(), nullptr, nullptr, error_message);
	if (SQLITE_OK != results)
	{
		std::cout << sqlite3_errmsg(connection) << std::endl;
		sqlite3_free(error_message);
		throw std::exception("purchase history table creation failed");
	}
}

DbConnection* DbConnection::get_instance()
{
	return instance;
}

sqlite3* DbConnection::get_connection()
{
	return connection;
}

DbConnection::~DbConnection()
{
	free(connection);
}
DbConnection* DbConnection::instance = new DbConnection();