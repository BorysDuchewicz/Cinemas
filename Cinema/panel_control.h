#pragma once
#include "admin_service.h"
#include "cinema_validator.h"
#include "movies_validator.h"
#include "programme_validator.h"

//panel administracyjny ktory umozliwa obsluge wszystkich elementow kina
class PanelControl
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

	void show_cinemas()const;
	void show_movies()const;
	void show_programmes(const int& id_cinema)const;
	void show_rooms(const int& id_cinema)const;
	int chose_option() const;
	void option1()const;
	void option2()const;
	void option3()const;
	void option4()const;
	void option5()const;
	void option6()const;
	void option7()const;
	void option8()const;
public:
	void panel_control()const;
	PanelControl(
		const std::shared_ptr<MoviesRepository>& movies,
		const std::shared_ptr<CinemasRepository>& cinemas,
		const std::shared_ptr<ProgrammeRepository>& programme,
		const std::shared_ptr<FavoriteRepository>& favorite,
		const std::shared_ptr<ScreeningRoomsRepository>& screening_rooms,
		const std::shared_ptr<PurchaseRepository>& purchases,
		const std::shared_ptr<TicketsRepository>& tickets,
		const std::shared_ptr<ReservationsRepository>& reservations,
		const std::shared_ptr<UsersRepository>& users);

	~PanelControl() = default;
};

