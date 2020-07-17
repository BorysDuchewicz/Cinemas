#pragma once
#include "validator.h"
#include "cinemas_repository.h"

//klasa sprawdzajaca czy podane dane dotyczace klasy Cinema spelniaja wymogi 
class CinemaValidator: public Validator<Cinema>
{
private:
	std::shared_ptr<CinemasRepository> cinemas;
	bool is_cinema_exist(const Cinema& cinema) const;
public:
	std::unordered_map<std::string, std::string> validator (const Cinema& cinema) override;

	friend std::ostream& operator<<(std::ostream& out, const CinemaValidator& val);

	CinemaValidator(const std::shared_ptr<CinemasRepository>& cinemas);
	~CinemaValidator() = default;
};

