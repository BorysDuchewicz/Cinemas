#include "movies_validator.h"



std::ostream& operator<<(std::ostream& out, const MoviesValidator& val)
{
	for (const auto& i : val.errors)
	{
		out << i.first << " " << i.second << std::endl;
	}
	return out;
}

bool MoviesValidator::is_movie_exist(const Movie& movie) const
{
	for(const auto& ele:movies->find_all())
	{
		if(movie.name==ele->name && movie.genre==ele->genre)
		{
			return false;
		}
	}
	return true;
}

std::unordered_map<std::string, std::string> MoviesValidator::validator(const Movie& movie)
{
	if(!is_movie_exist(movie))
	{
		errors.emplace(std::make_pair("This movie ", "exist"));
	}
	return errors;
}

MoviesValidator::MoviesValidator(const std::shared_ptr<MoviesRepository>& movies)
	:movies{movies} {}
