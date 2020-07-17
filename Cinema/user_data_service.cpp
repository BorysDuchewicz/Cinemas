
#include "user_data_service.h"

int UserDataService::get_int(const std::string& message)
{
	std::cout << message << std::endl;
	std::string txt;
	std::getline(std::cin, txt);
	if(!std::regex_match(txt,std::regex("\\d+")))
	{
		throw std::exception("its not number value");
	}
	return stoi(txt);
}

double UserDataService::get_double(const std::string& message)
{
	std::cout << message << std::endl;
	std::string txt;
	std::getline(std::cin, txt);
	if (!std::regex_match(txt, std::regex("\\d+")) && !std::regex_match(txt, std::regex("[0-9]+(\\.)[0-9]{0,2}")))
	{
		throw std::exception("wrong number value");
	}
	return stod(txt);
}

std::string UserDataService::get_string(const std::string& message)
{
	std::string txt;
	std::cout << message << std::endl;
	std::getline(std::cin, txt);
	return txt;

}
Type UserDataService::get_type(const std::string& message)
{
	std::cout << message << std::endl;
	std::string txt;
	std::getline(std::cin, txt);
	return  str_to_type(txt);
}
