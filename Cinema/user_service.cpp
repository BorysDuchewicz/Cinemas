#include "user_service.h"

void UserService::confirm_reservation() const
{
	std::map<int, Reservation> reservations_map;
	int i = 0;
	for (const auto& reservation : reservations->find_by_id_user(user->id))
	{
		++i;
		reservations_map.emplace(std::make_pair(i, *reservation));
	}
	int option;

	while (true) {
		option = UserDataService::get_int("Enter reservation number to confirm or press 0 to confirm all");
		if (option<0 || option>reservations_map.size())
		{
			std::cout << "Wrong option try again" << std::endl;
		}
		else
		{
			break;
		}
	}
	if (option == 0)
	{
		for (const auto& reservation : reservations->find_by_id_user(user->id))
		{
			purchases->add(std::make_unique<Purchase>(Purchase{ reservation->id_user,reservation->id_ticket,"data" }));
			reservations->delete_one(reservation->id);
		}
	}
	else
	{
		purchases->add(std::make_unique<Purchase>(Purchase{ reservations_map[option].id_user, reservations_map[option].id_ticket,"data" }));
		reservations->delete_one(reservations_map[option].id);
	}
	const int next_confirm = UserDataService::get_int("Press 1 to confirm other reservation or other to back to menu");
	if (next_confirm == 1)
	{
		confirm_reservation();
	}
}

void UserService::delete_reservation() const
{
	std::map<int, Reservation> reservations_map;
	int i = 0;
	for (const auto& reservation : reservations->find_by_id_user(user->id))
	{
		++i;
		reservations_map.emplace(std::make_pair(i, *reservation));
	}
	int option;


	while (true) {
		option = UserDataService::get_int("Enter reservation number to delete or press 0 to delete all");
		if (option<0 || option>reservations_map.size())
		{
			std::cout << "Wrong option try again" << std::endl;
		}
		else
		{
			break;
		}
	}
	if (option == 0)
	{
		for (const auto& reservation : reservations->find_by_id_user(user->id))
		{
			tickets->delete_one(reservation->id_ticket);
			reservations->delete_one(reservation->id);
		}
	}
	else
	{
		tickets->delete_one(reservations_map[option].id_ticket);
		reservations->delete_one(reservations_map[option].id);
	}
	const int next_confirm = UserDataService::get_int("Press 1 to delete other reservation");
	if (next_confirm == 1)
	{
		delete_reservation();
	}
}

int UserService::choose_option() const
{
	std::cout << "Welcome " << user->name << std::endl;
	std::cout << "Choose option" << std::endl;
	std::cout << "1 Show cinemas and buy ticket or reserve" << std::endl;
	std::cout << "2 Add movie to favorite" << std::endl;
	std::cout << "3 Show reservations" << std::endl;
	std::cout << "4 Show purchase history" << std::endl;
	std::cout << "5 Logout" << std::endl;

	const int option = UserDataService::get_int("");
	return option;
}

void UserService::option1() const
{
	for (const auto& cinema : cinemas->find_all())
	{
		std::cout << *cinema;
	}
	const int chosen_cinema = UserDataService::get_int("Choose cinema to see programme");
	const std::string search = UserDataService::get_string("Search: ");
	std::map<int, Programme> programmes;
	int number = 0;
	for (const auto& i : programme->find_all_movies_from_cinema(chosen_cinema))
	{	
		auto movie = *movies->find_by_id(i->id_movie);
		if (search.empty())
		{
			++number;
			programmes.emplace(std::make_pair(number, *i));
			std::cout << number << " " << movie.name << " " << movie.genre << " " << i->day << " " << i->hour << std::endl;
		}
		else if (movie.name == search || movie.genre == search || i->day == search)
		{
			++number;
			programmes.emplace(std::make_pair(number, *i));
			std::cout << number << " " << movie.name << " " << movie.genre << " " << i->day << " " << i->hour << std::endl;
		}
		else
		{
			std::cout << "Not found" << std::endl;
		}
	}
	const int option = UserDataService::get_int("Enter 1 to buy a ticket, 2 to reserved ticket other to back");
	int chosen_movie; 
	if (option == 1)
	{
		do {
			chosen_movie = UserDataService::get_int("Choose movie (wrote movie number): ");
		} while (chosen_movie>programmes.size() && chosen_movie<0);
		buy_ticket(chosen_cinema,programmes[chosen_movie]);
	}
	else if (option == 2)
	{
		do {
			chosen_movie = UserDataService::get_int("Choose movie (wrote movie number): ");
		} while (chosen_movie > programmes.size() && chosen_movie<0);
		reserve(chosen_cinema,programmes[chosen_movie]);
	}
}

void UserService::option2() const
{
	std::cout << "All available movies in cinemas" << std::endl;
	const auto films = movies->find_all();
	for (const auto& movie : films)
	{
		std::cout << *movie << std::endl;
	}
	const int movie_number = UserDataService::get_int("Wrote movie number");
	if (movie_number > films.size() ||movie_number<0)
	{
		std::cout << "Wrong movie number try again" << std::endl;
		std::cin.get();
		system("cls");
		option2();
	}
	else if (favorite->find_movie(user->id, movie_number) != std::nullopt)
	{
		std::cout << "This movie is in your favorite" << std::endl;
		std::cin.get();
		system("cls");
		option2();
	}
	else
	{
		favorite->add(std::make_unique<Favorite>(Favorite{ user->id,movie_number }));
	}
}

void UserService::buy_ticket(const int& id_cinema, const Programme& chosen_movie) const
{
	std::vector<std::unique_ptr<Ticket>> tickets_vector;
	const int how_tickets = UserDataService::get_int("Enter how tickets you buy: ");
	
	screening_rooms->show_places(id_cinema, chosen_movie.screening_room);
	Type type;
	int place;
	bool is_ok = true;
	ScreeningRoomsValidator validator{ screening_rooms };
	for (int i = 0; i < how_tickets; ++i)
	{
		do
		{
			try
			{
				std::cout << i + 1 << " ticket" << std::endl;
				type = UserDataService::get_type("Enter ticket type{student, senior, normal, child}");
				place = UserDataService::get_int("Enter place (places with r are reserved, places with x are taken)");
				auto room = screening_rooms->find_place_status(id_cinema, chosen_movie.screening_room, place);
				if (validator.is_place_is_taken(ScreeningRoom{ room->id_cinema,room->status,room->place_number,room->room_number }))
				{
					is_ok = true;
				}
				else
				{
					std::cout << validator << std::endl;
					is_ok = false;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				is_ok = false;
			}
		} while (is_ok == false);
		tickets_vector.emplace_back(std::make_unique<Ticket>(Ticket{ chosen_movie.id,type,place,tickets->price(cinemas->find_by_id(id_cinema)->price,type) }));
	}

	double final_price=0;
	for (const auto& ticket : tickets_vector)
	{
		std::cout << "Movie: " << movies->find_by_id(chosen_movie.id_movie)->name << std::endl;
		std::cout << "Cinema: " << cinemas->find_by_id(id_cinema)->name << " " << cinemas->find_by_id(id_cinema)->location << std::endl;
		std::cout << "Screening room" << chosen_movie.screening_room << std::endl;
		std::cout << "Ticket type: " << type_to_str(ticket->type) << std::endl;
		std::cout << "Place: " << ticket->place_number << std::endl;
		std::cout << "Price: " << ticket->price << std::endl;
		final_price += ticket->price;
	}
	std::cout << final_price << std::endl;
	const int confirm= UserDataService::get_int("To confirm enter 1 to decline other");

	if (confirm == 1)
	{
		for (const auto& ticket : tickets_vector)
		{
			tickets->add(ticket);
			screening_rooms->update(id_cinema, chosen_movie.screening_room, ticket->place_number, 2);
			purchases->add(std::make_unique<Purchase>(Purchase{ user->id,tickets->find_last()->id,"10.10.2019" }));
		}
		std::cout << "Thank for transaction"<<std::endl;
	}
	
}

void UserService::reserve(const int& id_cinema,const Programme& chosen_movie) const
{
	std::vector<std::unique_ptr<Ticket>> tickets_vector;

	const int how_tickets = UserDataService::get_int("Enter how tickets you buy: ");
	screening_rooms->show_places(id_cinema, chosen_movie.screening_room);

	Type type;
	int place;
	bool is_ok = true;
	ScreeningRoomsValidator validator{ screening_rooms };
	for (int i = 0; i < how_tickets; ++i)
	{
		do
		{
			try
			{
				std::cout << i + 1 << " ticket" << std::endl;
				type = UserDataService::get_type("Enter ticket type{student, senior, normal, child}");
				place = UserDataService::get_int("Enter place (places with r are reserved, places with x are taken)");
				auto room = screening_rooms->find_place_status(id_cinema, chosen_movie.screening_room, place);
				if (validator.is_place_is_taken(ScreeningRoom{ room->id_cinema,room->status,room->place_number,room->room_number }))
				{
					is_ok = true;
				}
				else
				{
					std::cout << validator << std::endl;
					is_ok = false;
				}
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << std::endl;
				is_ok = false;
			}
		} while (is_ok == false);
		tickets_vector.emplace_back(std::make_unique<Ticket>(Ticket{ chosen_movie.id,type,place,tickets->price(cinemas->find_by_id(id_cinema)->price,type) }));
	}

	double final_price;
	for (const auto& ticket : tickets_vector)
	{
		std::cout << "Movie: " << movies->find_by_id(chosen_movie.id_movie)->name << std::endl;
		std::cout << "Cinema: " << cinemas->find_by_id(id_cinema)->name << " " << cinemas->find_by_id(id_cinema)->location << std::endl;
		std::cout << "Screening room" << chosen_movie.screening_room << std::endl;
		std::cout << "Ticket type: " << type_to_str(ticket->type) << std::endl;
		std::cout << "Place: " << ticket->place_number << std::endl;
		std::cout << "Price: " << ticket->price << std::endl;
	}

	
	const int confirm = UserDataService::get_int("To confirm reservation enter 1 to decline other");

	if (confirm == 1)
	{
		for (const auto& ticket : tickets_vector)
		{
			tickets->add(ticket);
			screening_rooms->update(id_cinema, chosen_movie.screening_room, ticket->place_number, 1);
			reservations->add(std::make_unique<Reservation>(Reservation{ user->id,tickets->find_last()->id }));
		}
	}
	
}

void UserService::option3() const
{

	int i = 0;
	for (const auto& reservation : reservations->find_by_id_user(user->id))
	{
		auto ticket = tickets->find_by_id(reservation->id_ticket);
		auto repertoire = programme->find_by_id(ticket->id_programme);
		std::cout << "Reservation number " << ++i << std::endl;
		std::cout << "Movie: " << movies->find_by_id(repertoire->id_movie)->name << std::endl;
		std::cout << "Cinema: " << cinemas->find_by_id(repertoire->id_cinema)->name << " " << cinemas->find_by_id(repertoire->id_cinema)->location << std::endl;
		std::cout << "Screening room" << repertoire->screening_room << std::endl;
		std::cout << "Ticket type: " << type_to_str(ticket->type) << std::endl;
		std::cout << "Place: " << ticket->place_number << std::endl;
		std::cout << "Price: " << ticket->price << std::endl;
	}

	const int option = UserDataService::get_int("Enter 1 to confirm, 2 to unreserve, other to back");
	if (option == 1)
	{
		confirm_reservation();
	}
	else if (option == 2)
	{
		delete_reservation();
	}
	
}

void UserService::option4() const
{
	for (const auto& purchase_history : purchases->find_by_id_user(user->id))
	{
		auto ticket = tickets->find_by_id(purchase_history->id_ticket);	
		auto repertoire = programme->find_by_id(ticket->id_programme);		
		std::cout << "Movie: " << movies->find_by_id(repertoire->id_movie)->name << std::endl;
		std::cout << "Cinema: " << cinemas->find_by_id(repertoire->id_cinema)->name << " " << cinemas->find_by_id(repertoire->id_cinema)->location << std::endl;
		std::cout << "Screening room" << repertoire->screening_room << std::endl;
		std::cout << "Ticket type: " << type_to_str(ticket->type) << std::endl;
		std::cout << "Place: " << ticket->place_number << std::endl;
		std::cout << "Price: " << ticket->price << std::endl;
		std::cout << "Date: " << purchase_history->purchase_date << std::endl;
		std::cout << std::endl;
	}
	
}

void UserService::user_menu() const
{
	try {
		while (true)
		{
			const int option = choose_option();
			switch (option)
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
				std::cout << "No option with this number" << std::endl;
				std::cin.get();
				system("cls");
				break;
			}
		}
	}
	catch (const std::exception & exception) {
		std::cout << exception.what() << std::endl;
		std::cin.get();
		system("cls");
		user_menu();
	}
}

UserService::UserService(const std::shared_ptr<MoviesRepository>& movies,
	const std::shared_ptr<CinemasRepository>& cinemas,
	const std::shared_ptr<ProgrammeRepository>& programme,
	const std::shared_ptr<FavoriteRepository>& favorite,
	const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms,
	const std::shared_ptr<PurchaseRepository>& purchases,
	const std::shared_ptr<TicketsRepository>& tickets,
	const std::shared_ptr<ReservationsRepository>& reservations,
	const std::shared_ptr<User>& user)
	:movies{ movies }, cinemas{ cinemas }, programme{ programme }, favorite{ favorite }, screening_rooms{ screening_rooms }, purchases{ purchases }, tickets{ tickets }, reservations{ reservations }, user{ user } {}
