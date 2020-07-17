#pragma once
#include "movies_repository.h"
#include "validator.h"

//klasa sprawdzajaca czy podane dane dotyczace klasy Movie spelniaja wymogi 
class MoviesValidator:public Validator<Movie>
{
private:
	std::shared_ptr<MoviesRepository> movies;
	bool is_movie_exist(const Movie& movie) const;
public:
	std::unordered_map<std::string, std::string> validator(const Movie& movie) override;

	friend std::ostream& operator<<(std::ostream& out, const MoviesValidator& val);

	MoviesValidator(const std::shared_ptr<MoviesRepository>& movies);
	~MoviesValidator() = default;
};

