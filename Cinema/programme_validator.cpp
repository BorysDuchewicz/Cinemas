#include "programme_validator.h"

bool ProgrammeValidator::is_programme_exist(const Programme& programme) const
{
	for(const auto& ele: programmes->find_all())
	{
		if(*ele==programme)
		{
			return false;
		}
	}
	return true;
}

std::unordered_map<std::string, std::string> ProgrammeValidator::validator(const Programme& programme)
{
	if(!is_programme_exist(programme))
	{
		errors.emplace(std::make_pair("In this room is ", "reserved for movie at this time"));
	}
	return errors;
}

std::ostream& operator<<(std::ostream& out, const ProgrammeValidator& val)
{
	for (const auto& i : val.errors)
	{
		out << i.first << " " << i.second << std::endl;
	}
	return out;
}

ProgrammeValidator::ProgrammeValidator(const std::shared_ptr<ProgrammeRepository>& programmes)
	:programmes{programmes} {}
