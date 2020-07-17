#include "registration_validator.h"

bool RegistrationValidator::is_name_validate(const std::string& name)
{
	return std::regex_match(name, std::regex("[A-Za-z0-9]+"));
}

bool RegistrationValidator::is_password_validate(const std::string& password)
{
	if (password.length() < 3 || password.length() > 30 || !std::regex_match(password, std::regex("^(?:(?=.*[0-9])(?=.*[a-z])(?=.*[<+$*)])|(?=.*[a-z])(?=.*[<+$*)])(?=.*[A-Z])|(?=.*[0-9])(?=.*[A-Z])(?=.*[<+$*)])|(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z]))(?!.*[|;{} ].*$).+$")))
	{
		return false;
	}
	return true;
}

bool RegistrationValidator::is_email_validate(const std::string& email)
{	
	if( std::regex_match(email, std::regex("[A-Za-z0-9]+\\.[A-Za-z0-9]+@[a-z]+(.pl|.com)")) ||std::regex_match(email, std::regex("[A-Za-z0-9]+@[a-z]+(.pl|.com)")))
	{
		return true;
	}
	return false;
}

bool RegistrationValidator::is_email_or_name_exist(const std::string& email) const
{
	if (users->find_by_email_or_name(email) == std::nullopt)
	{
		return true;
	}
	return false;
}

bool RegistrationValidator::is_role_validate(const int& role)
{
	if (role == 0 || role == 1)
	{
		return true;
	}
	return false;
}

std::unordered_map<std::string, std::string> RegistrationValidator::validator(const User& user)
{
	if (!is_name_validate(user.name))
	{
		errors.emplace(std::make_pair("Wrong" , " name cannot have white chars"));
	}
	if (!is_password_validate(user.password))
	{
		errors.emplace(std::make_pair("Wrong" , " Password must have at least 1 big letter 1 small letter and 1 digit"));
	}
	if (!is_email_validate(user.email))
	{
		errors.emplace(std::make_pair("Wrong ","email"));
	}
	if(!is_email_or_name_exist(user.email))
	{
		errors.emplace(std::make_pair(user.email, " exist"));
	}
	if (!is_email_or_name_exist(user.name))
	{
		errors.emplace(std::make_pair(user.name, " exist"));
	}
	return errors;
}

RegistrationValidator::RegistrationValidator(const std::shared_ptr<UsersRepository>& users)
	:users{ users } {}


std::ostream& operator<<(std::ostream& out, const RegistrationValidator& val)
{
	for (const auto& i : val.errors)
	{
		out << i.first << " " << i.second << std::endl;
	}
	return out;

}
