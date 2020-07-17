#pragma once
#include "movies_repository.h"
#include "cinemas_repository.h"
#include "programme_repository.h"
#include "favorite_repository.h"
#include "rooms_validator.h"
#include "purchase_repository.h"
#include "tickets_repository.h"
#include "reservations_repository.h"
#include "users_repository.h"
#include "user_data_service.h"
#include "panel_control.h"


//klasa pozwalajaca administratorowi obslugiwac aplikacje
class AdminService
{
private:
	std::shared_ptr<MoviesRepository> movies;
	std::shared_ptr<CinemasRepository> cinemas;
	std::shared_ptr<ProgrammeRepository> programme;
	std::shared_ptr<FavoriteRepository> favorite;
	std::shared_ptr<ScreeningRoomsRepository> screening_rooms;
	std::shared_ptr<PurchaseRepository> purchases;
	std::shared_ptr<TicketsRepository> tickets;
	std::shared_ptr<ReservationsRepository> reservations;
	std::shared_ptr<UsersRepository> users;

	std::multimap<int, std::string,std::greater<>> most_popular_location() const;//zwraca najpopularniejsza lokacje
	std::map<std::string, std::string> most_popular_movie_in_location() const;//zwraca najpopularniejszy film dla kazdej lokacji
	std::map<std::string, double> average_ticket_price_on_location()const;//zwraca srednia cene biletu dla kazdej lokacji 
	std::map<std::string, double> profit_on_location()const;//zwraca zarobki kazdej lokacji
	std::map<std::string, std::string> most_popular_type_on_location() const;//zwraca najpopularniejszy gatunek filmu dla kazdej lokacji

	 std::multimap<int, std::string,std::greater<>> most_common_element_in_vector(const std::vector<std::string>& v)const;//zwraca najczesciej wystepujacy element w podanym wektorze
	
	void send_email(const std::string& name, const int& id_movie, const std::string& message) const;//generuje plik txt dla uzytkownikow ktorzy dodali film ktorego wiadomosc sie tyczy do ulubionych
	int chose_option()const;//przedstawia opcje do wyboru i umozliwia wybor
	void option1() const;
	void option2()const;
	void option3() const;
	void option4()const;
public:
	void admin_menu()const;
	AdminService(
		const std::shared_ptr<MoviesRepository>& movies,
		const std::shared_ptr<CinemasRepository>& cinemas,
		const std::shared_ptr<ProgrammeRepository>& programme,
		const std::shared_ptr<FavoriteRepository>& favorite,
		const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms,
		const std::shared_ptr<PurchaseRepository>& purchases,
		const std::shared_ptr<TicketsRepository>& tickets,
		const std::shared_ptr<ReservationsRepository>& reservations,
		const std::shared_ptr<UsersRepository>& users);

	~AdminService() = default;
};

