#include "cinema_validator.h"

bool CinemaValidator::is_cinema_exist(const Cinema& cinema) const
{
	for (const auto& ele:cinemas->find_all())
	{
		if(ele->name==cinema.name && ele->location==cinema.location)
		{
			return false;
		}
	}
	return true;
}

std::unordered_map<std::string, std::string> CinemaValidator::validator(const Cinema& cinema)
{
	if(!is_cinema_exist(cinema))
	{
		errors.emplace(std::make_pair("That cinema", "exist"));
	}
	return errors;
}

CinemaValidator::CinemaValidator(const std::shared_ptr<CinemasRepository>& cinemas)
:cinemas{cinemas}{}

std::ostream& operator<<(std::ostream& out, const CinemaValidator& val)
{
	for (const auto& i : val.errors)
	{
		out << i.first << " " << i.second << std::endl;
	}
	return out;
}