#pragma once
#include "validator.h"
#include "programme_repository.h"

//klasa sprawdzajaca czy podane dane dotyczace klasy Programme spelniaja wymogi 
class ProgrammeValidator:public Validator<Programme> 
{
private:
	std::shared_ptr<ProgrammeRepository> programmes;
	bool is_programme_exist(const Programme& programme) const;
public:
	std::unordered_map<std::string, std::string> validator(const Programme& programme) override;

	friend std::ostream& operator<<(std::ostream& out, const ProgrammeValidator& val);

	ProgrammeValidator(const std::shared_ptr<ProgrammeRepository>& programmes);
	~ProgrammeValidator() = default;
};

