#include "menu_service.h"

int MenuService::chose_option()const
{
	std::cout << "Menu" << std::endl;
	std::cout << "1 - Login" << std::endl;
	std::cout << "2 - Registration" << std::endl;
	std::cout << "3 - Exit" << std::endl;
	const int choose = UserDataService::get_int("Choose option: ");
	return choose;
}

void MenuService::login() const
{
	const std::string email = UserDataService::get_string("Enter email or nick: ");
	const std::string password = UserDataService::get_string("Enter password: ");
	
	const auto user = users->find_by_email_or_name(email);
	
	if (decrypt(user->password) == password &&user!=std::nullopt)
	{
		if (user->role)
		{
			system("cls");
			MoviesRepository movies;
			CinemasRepository cinemas;
			ProgrammeRepository programme;
			FavoriteRepository favorite;
			ScreeningRoomsRepository screening_rooms;
			PurchaseRepository purchase;
			TicketsRepository tickets;
			ReservationsRepository reservations;
			UsersRepository users;
			AdminService admin_service{
				std::make_shared<MoviesRepository>(movies),
				std::make_shared<CinemasRepository>(cinemas),
				std::make_shared<ProgrammeRepository>(programme),
				std::make_shared<FavoriteRepository>(favorite),
				std::make_shared<ScreeningRoomsRepository>(screening_rooms),
				std::make_shared<PurchaseRepository>(purchase),
				std::make_shared<TicketsRepository>(tickets),
				std::make_shared<ReservationsRepository>(reservations),
				std::make_shared<UsersRepository>(users) };
			admin_service.admin_menu();
		
		}
		else if (!user->role)
		{
			system("cls");
			MoviesRepository movies;
			CinemasRepository cinemas;
			ProgrammeRepository programme;
			FavoriteRepository favorite;
			ScreeningRoomsRepository screening_rooms;
			PurchaseRepository purchase;
			TicketsRepository tickets;
			ReservationsRepository reservations;
			UserService user_service{
				std::make_shared<MoviesRepository>(movies),
				std::make_shared<CinemasRepository>(cinemas),
				std::make_shared<ProgrammeRepository>(programme),
				std::make_shared<FavoriteRepository>(favorite),
				std::make_shared<ScreeningRoomsRepository>(screening_rooms),
				std::make_shared<PurchaseRepository>(purchase),
				std::make_shared<TicketsRepository>(tickets),
				std::make_shared<ReservationsRepository>(reservations),
				std::make_shared<User>(User{user->id,user->name,user->password,user->email,user->role}) };
			user_service.user_menu();
		}
	}
	else
	{
		std::cout << "Incorrect email or password! Pres enter to try again" << std::endl;
		std::cin.get();
		system("cls");
		login();
	}
}

void MenuService::registration() const
{	
	RegistrationValidator validator{users};

	const std::string name=UserDataService::get_string("Enter username: ");
	const std::string password = UserDataService::get_string("Enter password: ");
	const std::string email = UserDataService::get_string("Enter email: ");
	
	User u{name,password,email,false };
	
	if (validator.validator(u).empty())
	{
		users->add(std::make_unique<User>(u.name,encrypt(u.password),u.email,false));
		std::cin.get();
		system("cls");
	}
	else
	{
		std::cout << validator << std::endl;
		std::cout << "Press enter to try again" << std::endl;
		std::cin.get();
		system("cls");
		registration();
	}
}

std::string MenuService::encrypt(std::string password)const
{
	Cryptor cryptor;
	cryptor.addCharset(Charset("&FOXYMA#NRSVT$UEGBZ%W+HIKLJQCDP1234567890"));
	cryptor.addCharset(Charset("fo.xym!anrsv_tu:egbz/wh)ikl=jqcd?p"));
	cryptor.setAdditionalCryptions(10);
	return cryptor.encrypt(password);
}

std::string MenuService::decrypt(std::string password)const
{
	Cryptor cryptor;
	cryptor.addCharset(Charset("&FOXYMA#NRSVT$UEGBZ%W+HIKLJQCDP1234567890"));
	cryptor.addCharset(Charset("fo.xym!anrsv_tu:egbz/wh)ikl=jqcd?p"));
	cryptor.setAdditionalCryptions(10);
	return cryptor.decrypt(password);
}

void MenuService::menu() const
{
	try
	{
		while (true)
		{
			switch (chose_option())
			{
			case 1:
				system("cls");
				login();
				break;
			case 2:
				system("cls");
				registration();
				break;
			case 3:
				std::cout << "Have a nice day!" << std::endl;
				std::cin.get();
				return;
			default:
				std::cout << "No option with this number" << std::endl;
				std::cin.get();
				system("cls");
			}
		}
	}
	catch (const std::exception& exception) {
		std::cout << exception.what() << std::endl;
		std::cin.get();
		system("cls");
		menu();
	}
}
MenuService::MenuService(const std::shared_ptr<UsersRepository>& users) :users{ users } {}
