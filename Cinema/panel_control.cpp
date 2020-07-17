#include "panel_control.h"


void PanelControl::show_cinemas() const
{
	for (const auto& cinema : cinemas->find_all())
	{
		std::cout << *cinema;
	}
}

void PanelControl::show_movies() const
{
	for (const auto& movie : movies->find_all())
	{
		std::cout << *movie << std::endl;
	}
}

void PanelControl::show_programmes(const int& id_cinema) const
{
	for (const auto& i : programme->find_all_movies_from_cinema(id_cinema))
	{
		auto movie = *movies->find_by_id(i->id_movie);
		std::cout << movie.name << " " << movie.genre << " " << i->day << " " << i->hour << std::endl;
	}
}

void PanelControl::show_rooms(const int& id_cinema) const
{
	std::vector<int> rooms;
	bool exist = false;
	for (const auto& screening_room : screening_rooms->find_cinema_rooms(id_cinema))
	{
		for(const auto& room:rooms)
		{
			if(room==screening_room->room_number)
			{
				exist = true;
			}
		}
		if (!exist)
		{
			rooms.emplace_back(screening_room->room_number);
		}
		exist = false;
	}
	for (const auto& room : rooms)
	{
		std::cout << room << std::endl;
	}
	
}

int PanelControl::chose_option() const
{
	std::cout << "1 - add cinema to the list" << std::endl;
	std::cout << "2 - add movie to the list" << std::endl;
	std::cout << "3 - add programme to the cinema" << std::endl;
	std::cout << "4 - add screening room to the cinema" << std::endl;
	std::cout << "5 - delete cinema from the list" << std::endl;
	std::cout << "6 - delete movie from the list" << std::endl;
	std::cout << "7 - delete programme from the cinema" << std::endl;
	std::cout << "8 - delete screening room from the cinema " << std::endl;
	std::cout << "9 - back to admin menu" << std::endl;
	const int chosen_option = UserDataService::get_int("Choose option: ");
	return chosen_option;
}

void PanelControl::option1() const
{
	std::string name = UserDataService::get_string("Enter cinema name: ");
	std::string location = UserDataService::get_string("Enter cinema location: ");
	double price = UserDataService::get_double("Enter ticket price: ");
	Cinema cinema_to_add{ name,location,price };
	CinemaValidator validator{ cinemas };
	if (validator.validator(cinema_to_add).empty())
	{
		cinemas->add(std::make_unique<Cinema>(cinema_to_add));
		std::cout << "Successfully added " << std::endl;
	}
	else
	{
		std::cout << validator << std::endl;
	}
}

void PanelControl::option2() const
{
	std::string name = UserDataService::get_string("Enter movie name: ");
	std::string genre = UserDataService::get_string("Enter movie genre: ");
	std::string description = UserDataService::get_string("Enter movie description: ");
	Movie movie_to_add(name, genre, description);
	MoviesValidator validator{ movies };
	if (validator.validator(movie_to_add).empty())
	{
		movies->add(std::make_unique<Movie>(movie_to_add));
		std::cout << "Successfully added " << std::endl;
	}
	else
		std::cout << validator << std::endl;
}

void PanelControl::option3() const
{
	std::cout << "Cinemas list:" << std::endl;
	show_cinemas();
	int id_cinema = UserDataService::get_int("Enter cinema number: ");
	std::cout << std::endl;

	std::cout << "Movies list:" << std::endl;
	show_movies();
	int id_movie = UserDataService::get_int("Enter movie number: ");
	std::cout << std::endl;

	std::cout << "Cinema rooms numbers:" << std::endl;
	show_rooms(id_cinema);
	int screening_room = UserDataService::get_int("Enter screening room number: ");
	std::cout << std::endl;

	std::cout << "Programme for this cinema:" << std::endl;
	show_programmes(id_cinema);
	std::string day = UserDataService::get_string("Enter day: ");
	std::string hour = UserDataService::get_string("Enter hour: ");

	Programme programme_to_add{ id_movie,id_cinema,day,hour,screening_room };
	ProgrammeValidator validator{ programme };
	
	if (validator.validator(programme_to_add).empty())
	{
		programme->add(std::make_unique<Programme>(programme_to_add));
		std::cout << "Successfully added " << std::endl;
	}
	else
		std::cout << validator << std::endl;
}

void PanelControl::option4() const
{
	std::cout << "Cinemas list:" << std::endl;
	show_cinemas();
	int id_cinema = UserDataService::get_int("Enter cinema number: ");
	std::cout << std::endl;

	std::cout << "Cinema rooms: " << std::endl;
	show_rooms(id_cinema);
	int room_nubmer = UserDataService::get_int("Enter room number: ");
	int places = UserDataService::get_int("Enter places number: ");
	
	ScreeningRoom room_to_add{ id_cinema,0,0,room_nubmer };
	ScreeningRoomsValidator validator{ screening_rooms };
	if (validator.validator(room_to_add).empty())
	{
		for (int i = 1; i <= places; ++i)
		{
			screening_rooms->add(std::make_unique<ScreeningRoom>(ScreeningRoom{ id_cinema,0,i,room_nubmer }));
		}
		std::cout << "Successfully added " << std::endl;
	}
	else
	{
		std::cout << validator << std::endl;
	}
}

void PanelControl::option5() const
{
	show_cinemas();
	const int id = UserDataService::get_int("Enter cinema number to delete: ");
	cinemas->delete_one(id);
	std::cout << "Successfully deleted " << std::endl;
}

void PanelControl::option6() const
{
	show_movies();
	const int id = UserDataService::get_int("Enter movie number to delete: ");
	movies->delete_one(id);
	std::cout << "Successfully deleted " << std::endl;
}

void PanelControl::option7() const
{
	show_cinemas();
	const int id_cinema = UserDataService::get_int("Enter cinema number: ");
	std::map<int, Programme> programme_map;
	int i = 0;
	for (const auto& repertoire : programme->find_all_movies_from_cinema(id_cinema))
	{
		++i;
		programme_map.emplace(std::make_pair(i, *repertoire));
		auto movie = *movies->find_by_id(repertoire->id_movie);
		std::cout << i << " " << movie.name << " " << movie.genre << " " << repertoire->day << " " << repertoire->hour << std::endl;
	}
	const int id_programme = UserDataService::get_int("Enter programme number to delete: ");
	programme->delete_one(programme_map[id_programme].id);
	std::cout << "Successfully deleted " << std::endl;
}


void PanelControl::option8() const
{
	show_cinemas();
	const int id_cinema = UserDataService::get_int("Enter cinema number: ");
	show_rooms(id_cinema);
	const int id_rooms = UserDataService::get_int("Enter room number: ");
	screening_rooms->delete_one(id_cinema, id_rooms);
	std::cout << "Successfully deleted " << std::endl;
}

void PanelControl::panel_control() const
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
				option5();
				std::cin.get();
				system("cls");
				break;
			case 6:
				system("cls");
				option6();
				std::cin.get();
				system("cls");
				break;
			case 7:
				system("cls");
				option7();
				std::cin.get();
				system("cls");
				break;
			case 8:
				system("cls");
				option8();
				std::cin.get();
				system("cls");
				break;
			case 9:
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
		panel_control();
	}
}

PanelControl::PanelControl(const std::shared_ptr<MoviesRepository>& movies,
	const std::shared_ptr<CinemasRepository>& cinemas, const std::shared_ptr<ProgrammeRepository>& programme,
	const std::shared_ptr<FavoriteRepository>& favorite,
	const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms,
	const std::shared_ptr<PurchaseRepository>& purchases, const std::shared_ptr<TicketsRepository>& tickets,
	const std::shared_ptr<ReservationsRepository>& reservations, const std::shared_ptr<UsersRepository>& users)
	:movies{ movies }, cinemas{ cinemas }, programme{ programme }, favorite{ favorite }, screening_rooms{ screening_rooms }, purchases{ purchases }, tickets{ tickets }, reservations{ reservations }, users{ users }{}

