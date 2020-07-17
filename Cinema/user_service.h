#pragma once
#include "movies_repository.h"
#include "cinemas_repository.h"
#include "programme_repository.h"
#include "favorite_repository.h"
#include "rooms_validator.h"
#include "purchase_repository.h"
#include "tickets_repository.h"
#include "reservations_repository.h"
#include "user.h"
#include "user_data_service.h"

//klasa ktora pozwala uzytkownikowi na wykonywanie operacji
class UserService
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
	std::shared_ptr<User> user;


	void confirm_reservation()const;//zatwierdzanie rezerwacji 
	void delete_reservation()const;//usuwanie rezerwacji
	int choose_option() const;//wyswietla dostepne opcje i pozwala na wybor
	void option1() const;
	void option2() const;
	void buy_ticket(const int& id_cinema, const Programme& chosen_movie) const;//pozwala na zakup biletu
	void reserve(const int& id_cinema, const Programme& chosen_movie) const;//pozwala na rezerwacje biletu 
	void option3()const;
	void option4()const;

public:
	void user_menu() const;
	UserService(
		const std::shared_ptr<MoviesRepository>& movies,
		const std::shared_ptr<CinemasRepository>& cinemas, 
		const std::shared_ptr<ProgrammeRepository>& programme,
		const std::shared_ptr<FavoriteRepository>& favorite,
		const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms,
		const std::shared_ptr<PurchaseRepository>& purchases,
		const std::shared_ptr<TicketsRepository>& tickets,
		const std::shared_ptr<ReservationsRepository>& reservations,
		const std::shared_ptr<User>& user);
	~UserService() = default;
};

