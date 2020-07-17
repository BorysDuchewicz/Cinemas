#include "menu_service.h"



int main()
{
	
		UsersRepository users;
		MenuService menu{ std::make_shared<UsersRepository>(users) };

		//Przyk³adowa podstawowa baza danych

		/*CinemasRepository cinemas;
		MoviesRepository movies;
		FavoriteRepository favorite;
		PurchaseRepository purchase;
		ProgrammeRepository programme;
		ScreeningRoomsRepository rooms;
		TicketsRepository tickets;
		ReservationsRepository reservations;

		cinemas.add(std::make_unique<Cinema>(Cinema{ "CinemaCity","Zoliborz",29.99 }));
		cinemas.add(std::make_unique<Cinema>(Cinema{ "CinemaCity","Bemowo",25.50 }));
		movies.add(std::make_unique<Movie>(Movie{ "Jhony Wick","Action","Jakis opis" }));
		movies.add(std::make_unique<Movie>(Movie{ "Lsnienie","Thriller","Jakis opis" }));
		programme.add(std::make_unique<Programme>(Programme{ 1,2,"10-12-1980","10:40",1}));
		programme.add(std::make_unique<Programme>(Programme{ 2,2,"10-12-1980","12:30",2 }));
		programme.add(std::make_unique<Programme>(Programme{ 1,1,"10-12-1980","10:30",1 }));
		programme.add(std::make_unique<Programme>(Programme{ 2,1,"10-12-1980","12:30",2 }));

		Cryptor cryptor;
		cryptor.addCharset(Charset("&FOXYMA#NRSVT$UEGBZ%W+HIKLJQCDP1234567890"));
		cryptor.addCharset(Charset("fo.xym!anrsv_tu:egbz/wh)ikl=jqcd?p"));
		cryptor.setAdditionalCryptions(10);
		std::string password = cryptor.encrypt("Admin1!");
		users.add(std::make_unique<User>(User{ "Borys",password,"borys.duhewicz@op.pl",true }));
		for(int i=1;i<=30;++i)
		{
			rooms.add(std::make_unique<ScreeningRoom>(ScreeningRoom{ 1,0,i,1 }));
			rooms.add(std::make_unique<ScreeningRoom>(ScreeningRoom{ 1,0,i,2 }));
			rooms.add(std::make_unique<ScreeningRoom>(ScreeningRoom{ 2,0,i,1 }));
			rooms.add(std::make_unique<ScreeningRoom>(ScreeningRoom{ 2,0,i,2 }));
		}*/

		menu.menu();

	return 0;
}