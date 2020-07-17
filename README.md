# Cinemas
Aplikacja ta pozwala na zarządzanie siecią kin w różnych miejscowości.
Przy uruchomieniu aplikacji możemy się zalogować na konto użytkownika lub administratora (w każdym mamy inne opcje do wyboru)
Oprócz logowania można założyć konto użytkownika. Dane o użytkownikach są przechowywane w bazie danych z zaszyfrowanymi hasłami.

Aplikacja posiada własną klase wyjątków. Wyjątki nie przerywają działania programu raczej każą użytkownikowi powtórzyć czynność.
Aplikacja posiada walidacje każdego z elementów.


Funkcjonalność dla użytkownika (wybiera opcje głównie przez wybór przypisanej cyfry):
-możliwość przeglądania informacji o danym filmie i dodania wybranych do ulubionych dzieki czemu bedzie otrzymywac informacje o danym filmie 
(generowane pliki txt z tytulem nazwy uzytkowników mających ten film w ulubionych)
-możliwość zakupu biletu na podany film w podanym kinie (możliwość wyszukania interesującego nas filmu)
przy zakupie uzytkownik wybiera rodzaj biletu, ilosc, kino, miejsca. Po wprowadzeniu tych danych użytkownik może się wstrzymać z zakupem i zarezerwować na koniec dostanie
uporządkowane wszystkie bilety wraz z cena które w tym momencie zarezerwował/kupił (operacja wyboru miejsc ma swoja wizualizację)
-możliwośc zatwierdzenia lub anulowania rezerwacji
-możliwość zobaczenia swojej histori zakupów
-możliwość wylogowania się

Funkcjonalność dla administratora:
-możliwość rozsyłania wiadomości o danym filmie do wszystkich użytkowników którzy mają go w ulubionych
-możliwość wygenerowania statystyk:
  -najpopularniejszą miejscowość
  -najpopularniejszy film w każdej miejscowości
  -średnia cena biletu na osobę w każdej miejscowości
  -całkowity dochód kin z poszczególnych miejscowości
  -najczęściej kupowany rodzaj biletu w każdej miejscowości
-możliwość zmiany cen biletów dla wybranego kina
-możliwosć wejścia w panel sterujący
W panelu sterującym ma mażliwość:
-dodać/usunąć kino
-dodać/usunąć film do podanego repertuaru lub kina
-dodać/usunąć sale kinowa dla podanego kina
-dodać/usunąć repertuar dla danego kina
