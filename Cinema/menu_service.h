#pragma once

#include "user_service.h"
#include  "admin_service.h"
#include "registration_validator.h"

//menu startowe ktore pozwala na zalogowanie lub rejestracje
class MenuService
{
private:
	std::shared_ptr<UsersRepository> users;
	 int chose_option()const ;
	void login() const;
	void registration() const;
	std::string encrypt(std::string password)const;
	std::string decrypt(std::string password)const;
public:
	void menu() const;
	MenuService(const std::shared_ptr<UsersRepository>& users);
	~MenuService() = default;
};

