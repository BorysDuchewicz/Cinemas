#pragma once
#include "libs.h"
class DbConnection
{
private:
	static DbConnection* instance;
	const std::string database_name = "cinema";
	DbConnection();
	sqlite3* connection;
	void create_table()const;
public:
	static DbConnection* get_instance();
	sqlite3* get_connection();
	~DbConnection();
};

