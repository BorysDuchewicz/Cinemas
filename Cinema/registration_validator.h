#pragma once
#include "validator.h"
#include "users_repository.h"

//klasa sprawdzajaca czy podane dane dotyczace klasy User spelniaja wymogi 
class RegistrationValidator :public Validator<User>
{
private:
	std::shared_ptr<UsersRepository> users;
	 bool is_name_validate(const std::string& name);
	 bool is_password_validate(const std::string& password);
	 bool is_email_validate(const std::string& email);
	 bool is_email_or_name_exist(const std::string& email) const;
	 bool is_role_validate(const int& role);
public:

	std::unordered_map<std::string, std::string> validator(const User& user)override;

	friend std::ostream& operator<<(std::ostream& out, const RegistrationValidator& val);

	RegistrationValidator(const std::shared_ptr<UsersRepository>& users);
	~RegistrationValidator() = default;
};






