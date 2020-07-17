#include "admin_service.h"



std::multimap<int, std::string,std::greater<>> AdminService::most_popular_location() const
{
	std::map<std::string, int> most_popular_location;
	int i = 0;
	for (const auto& cinema : cinemas->find_all())
	{
		i = 0;
		for (const auto& repertoire : programme->find_all_movies_from_cinema(cinema->id))
		{
			i += tickets->count_tickets(repertoire->id);
		}
		most_popular_location.emplace(std::make_pair(cinema->location, i));
	}
	auto locations = flip_map(most_popular_location);
	return locations;
}

std::map<std::string, std::string> AdminService::most_popular_movie_in_location() const
{
	std::map<std::string, std::string> most_popular_movie_in_location;
	int most_popular_movie = 0;
	int count;
	std::string most_popular_movie_name;
	for (const auto& cinema : cinemas->find_all())
	{
		most_popular_movie_name = "don't have";
		most_popular_movie = 0;
			for (const auto& repertoire : programme->find_all_movies_from_cinema(cinema->id))
			{
				count = tickets->count_tickets(repertoire->id);
				if (most_popular_movie < count)
				{
					most_popular_movie_name=movies->find_by_id(repertoire->id_movie)->name;
					most_popular_movie = count;
				}
			}		
			most_popular_movie_in_location.emplace(std::make_pair(cinema->location,most_popular_movie_name));		
	}
	return most_popular_movie_in_location;
}

std::map<std::string, double> AdminService::average_ticket_price_on_location() const
{
	std::map<std::string, double>average_ticket_price_on_location;
	double sum = 0;
	int how = 0;
	for (const auto& cinema : cinemas->find_all())
	{
		how = 0;
		sum = 0;
		for (const auto& repertoire : programme->find_all_movies_from_cinema(cinema->id))
		{
			for (const auto& i : tickets->ticket_price(repertoire->id))
			{
				sum += i;
			}
			how += tickets->ticket_price(repertoire->id).size();

		}
		if (how == 0)
		{
			++how;
		}
		average_ticket_price_on_location.emplace(std::make_pair(cinema->location,sum / how));
	}
	return average_ticket_price_on_location;
}

std::map<std::string, double> AdminService::profit_on_location() const
{
	std::map<std::string, double> profit_on_location;
	double sum = 0;
	for (const auto& cinema : cinemas->find_all())
	{		
		sum = 0;
		for (const auto& repertoire : programme->find_all_movies_from_cinema(cinema->id))
		{
			for (const auto& i : tickets->ticket_price(repertoire->id))
			{
				sum += i;
			}
		}
		profit_on_location.emplace(std::make_pair(cinema->location, sum));
	}
	return profit_on_location;
}

std::map<std::string, std::string> AdminService::most_popular_type_on_location() const
{
	std::map<std::string, std::string> most_popular_type_on_location;
	std::vector<std::string> type_vector;
	for (const auto& cinema : cinemas->find_all())
	{
		type_vector.clear();
		for (const auto& repertoire : programme->find_all_movies_from_cinema(cinema->id))
		{
			for (const auto& i : tickets->find_type(repertoire->id))
			{
				type_vector.emplace_back(i);
			}
		}
		if (most_common_element_in_vector(type_vector).empty())
		{
			most_popular_type_on_location.emplace(std::make_pair(cinema->location, "don't have"));
		}
		else
		{
			most_popular_type_on_location.emplace(std::make_pair(cinema->location, most_common_element_in_vector(type_vector).begin()->second));
		}
		
	}
	return most_popular_type_on_location;
}


std::multimap<int, std::string,std::greater<>> AdminService::most_common_element_in_vector(const std::vector<std::string>& v) const
{
	std::map<std::string, int> counters;

	for (const auto& i : v)
	{
		++counters[i];
	}
	auto counters1 = flip_map(counters);
	return counters1;
}


void AdminService::send_email(const std::string& name, const int& id_movie, const std::string& message) const
{	
	std::ofstream send;
	send.open(name + ".txt");

	if (send.is_open())
	{
		send << movies->find_by_id(id_movie)->name << " " << message << std::endl;
	}
	else
	{
		std::cout << "Path error!" << std::endl;
	}
}

int AdminService::chose_option() const
{
	std::cout << "Admin menu chose option" << std::endl;
	std::cout << "1 - open control panel" << std::endl;
	std::cout << "2 - send messages" << std::endl;
	std::cout << "3 - show statistics" << std::endl;
	std::cout << "4 - change price for the cinema" << std::endl;
	std::cout << "5 - logout" << std::endl;

	const int chosen_option = UserDataService::get_int("Choose option: ");
	return chosen_option;
}


void AdminService::option1() const
{
	PanelControl panel_control{ movies,cinemas,programme,favorite,screening_rooms,purchases,tickets,reservations,users };
	panel_control.panel_control();
}

void AdminService::option2() const
{

	for (const auto& movie : movies->find_all())
	{
		std::cout << *movie << std::endl;
	}
	const int id_movie = UserDataService::get_int("Enter movie number");
	const std::string message = UserDataService::get_string("Enter message to: ");
	std::cout << std::endl;
	for (const auto& user : users->find_all())
	{
		if (favorite->find_movie(user->id, id_movie) != std::nullopt)
		{
			send_email(user->email, id_movie, message);
		}
	}
	std::cout << "Successfully send" << std::endl;
}

void AdminService::option3 () const
{

	std::ofstream save;
	save.open("Statistics.txt");
	if (save.is_open())
	{
		save << "Most popular location: " << most_popular_location().begin()->second<< std::endl;
		save << std::endl;
		save << "Most popular movies on location: " << std::endl;
		for(const auto& movie:most_popular_movie_in_location())
		{
			save << movie.first << ": " << movie.second << std::endl;
		}
		save << std::endl;
		save << "Average ticket price on location: " << std::endl;
		for(const auto& price:average_ticket_price_on_location())
		{
			save << price.first << ": " << price.second << std::endl;
		}
		save << std::endl;
		save << "Cinema profit on location: " << std::endl;
		for (const auto& price : profit_on_location())
		{
			save << price.first << ": " <<  price.second << std::endl;
		}
		save << std::endl;
		save << "Most popular ticket type on location: " << std::endl;
		for (const auto& type : most_popular_type_on_location())
		{
			save << type.first << ": " << type.second << std::endl;
		}
	}
	else
	{		
		std::cout << "Path error" << std::endl;
	}	
	
}

void AdminService::option4() const
{
	for (const auto& cinema : cinemas->find_all())
	{
		std::cout << *cinema << std::endl;
	}
	const int id_cinema = UserDataService::get_int("Enter cinema to edit price: ");
	const double price = UserDataService::get_double("Enter new price: ");
	cinemas->update_price(id_cinema, price);
	std::cout << "Successfully update" << std::endl;
}

void AdminService::admin_menu() const
{
	try
	{
		while (true)
		{
			switch (chose_option())
			{
			case 1:
				system("cls");
				option1();
				std::cin.get();
				system("cls");
				break;
			case 2:
				system("cls");
				option2();
				std::cin.get();
				system("cls");
				break;
			case 3:
				system("cls");
				option3();
				std::cin.get();
				system("cls");
				break;
			case 4:
				system("cls");
				option4();
				std::cin.get();
				system("cls");
				break;
			case 5:
				system("cls");
				return;
			default:
				std::cout << "Wrong option" << std::endl;
				std::cin.get();
				system("cls");
			}
		}
	}
	catch (const std::exception & e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		system("cls");
		admin_menu();
	}
}

AdminService::AdminService(const std::shared_ptr<MoviesRepository>& movies, const std::shared_ptr<CinemasRepository>& cinemas,
	const std::shared_ptr<ProgrammeRepository>& programme, const std::shared_ptr<FavoriteRepository>& favorite,
	const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms, const std::shared_ptr<PurchaseRepository>& purchases,
	const std::shared_ptr<TicketsRepository>& tickets, const std::shared_ptr<ReservationsRepository>& reservations,
	const std::shared_ptr<UsersRepository>& users) :
	movies{ movies }, cinemas{ cinemas }, programme{ programme }, favorite{ favorite }, screening_rooms{ screening_rooms }, purchases{ purchases }, tickets{ tickets }, reservations{ reservations }, users{ users }{}
