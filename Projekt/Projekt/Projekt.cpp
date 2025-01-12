#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>

// wynik gracza
struct Wynik {
    int uniknieteAsteroidy; 
    float czas;
    std::string data; 
};

// zwracanie aktualnej daty
std::string pobierzAktualnaDate() {
    std::time_t teraz = std::time(nullptr); 
    std::tm* lokalnyCzas = std::localtime(&teraz); // konwersja na lokalny czas

    char bufor[11]; // przechowywanie sformatowanej daty
    std::strftime(bufor, sizeof(bufor), "%Y-%m-%d", lokalnyCzas); // formatowanie daty 
    return std::string(bufor); // konwersja w string
}

// potwierdzenie wyjscia z gry
bool potwierdzWyjscie(sf::RenderWindow& okno, sf::Font& czcionka) {
    // tekst potwierdzajacy wyjscie
    sf::Text tekstPotwierdzenia("Czy na pewno chcesz wyjsc? [T/N]", czcionka, 20);
    tekstPotwierdzenia.setFillColor(sf::Color::White); 
    tekstPotwierdzenia.setPosition(200, 300); 

    bool czekamyNaOdpowiedz = true; // oczekiwanie na odpowiedź
    while (czekamyNaOdpowiedz) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            // zamykanie okna
            if (zdarzenie.type == sf::Event::Closed) {
                okno.close(); 
                return false; 
            }
            // naciśnięcie klawisza
            if (zdarzenie.type == sf::Event::KeyPressed) {
                if (zdarzenie.key.code == sf::Keyboard::T) { 
                    return true; 
                }
                if (zdarzenie.key.code == sf::Keyboard::N) {
                    return false; 
                }
            }
        }

        okno.clear(); 
        okno.draw(tekstPotwierdzenia);
        okno.display(); 
    }

    return false; 
}


// zapisywanie wyniku do pliku rankingu
void zapiszRankingDoPliku(const std::string& nazwaPliku, const std::vector<Wynik>& ranking) {
    std::ofstream plik(nazwaPliku); // otwieranie pliku do zapisu
    if (!plik.is_open()) {
        
        std::cout << "Błąd podczas otwierania pliku do zapisu: " << nazwaPliku << "\n";
        return;
    }

    // zapisywanie pozycji w pliku
    for (const auto& wynik : ranking) {
        plik << wynik.uniknieteAsteroidy << " "
            << wynik.czas << " "
            << wynik.data << "\n"; //liczba asteroid,czas,data
    }

    plik.close();
}

// wczytywanie rankingu
void wczytajRankingZPliku(const std::string& nazwaPliku, std::vector<Wynik>& ranking) {
    std::ifstream plik(nazwaPliku); //otwieranie pliku do odczytu
    if (!plik.is_open()) {
        
        std::cout << "Błąd podczas otwierania pliku do odczytu: " << nazwaPliku << "\n";
        return;
    }

    Wynik tymczasowyWynik; // tymczasowe wczytywanie danych
    while (plik >> tymczasowyWynik.uniknieteAsteroidy
        >> tymczasowyWynik.czas
        >> tymczasowyWynik.data) {
        ranking.push_back(tymczasowyWynik); // dodanie wyniku do pliku
    }

    plik.close(); 
}

// wyswietlanie rankingu
void pokazRanking(sf::RenderWindow& okno, sf::Font& czcionka, std::vector<Wynik>& ranking) {
    bool wRankingu = true;

    // sortowanie rankingu malejace
    std::sort(ranking.begin(), ranking.end(), [](const Wynik& a, const Wynik& b) {
        return a.uniknieteAsteroidy > b.uniknieteAsteroidy; 
        });

    while (wRankingu) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            
            if (zdarzenie.type == sf::Event::Closed) {
                okno.close();
                return;
            }

            
            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) {
                wRankingu = false;
            }
        }

        okno.clear();

        
        sf::Text tytulRankingu("Ranking", czcionka, 30);
        tytulRankingu.setFillColor(sf::Color::White);
        tytulRankingu.setPosition(300, 50);
        okno.draw(tytulRankingu);

        if (ranking.empty()) {
            
            sf::Text brakWynikow("Brak wyników do wyświetlenia.", czcionka, 20);
            brakWynikow.setFillColor(sf::Color::White);
            brakWynikow.setPosition(200, 300);
            okno.draw(brakWynikow);
        }
        else {
            // wyswietlanie rankingu
            int pozycjaY = 100;
            for (size_t i = 0; i < ranking.size(); ++i) {
                std::string wpis = std::to_string(i + 1) + ". Asteroidy: " + std::to_string(ranking[i].uniknieteAsteroidy) +
                    " | Czas: " + std::to_string(static_cast<int>(ranking[i].czas)) + "s | Data: " + ranking[i].data;

                sf::Text tekstWyniku(wpis, czcionka, 20);
                tekstWyniku.setFillColor(sf::Color::White);
                tekstWyniku.setPosition(50, pozycjaY);
                okno.draw(tekstWyniku);
                pozycjaY += 30; 
            }
        }

        
        sf::Text tekstPowrotu("ESC - Powrót", czcionka, 20);
        tekstPowrotu.setFillColor(sf::Color::White);
        tekstPowrotu.setPosition(10, 550);
        okno.draw(tekstPowrotu);

        okno.display();
    }
}

// ekran pomocy
void pokazEkranPomocy(sf::RenderWindow& okno, sf::Font& czcionka, sf::Sprite& tlo) {
    bool wPomocy = true; // potwierdzenie czy gracz znajduje sie w ekranie pomocy
    while (wPomocy) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            
            if (zdarzenie.type == sf::Event::Closed) {
                okno.close();
                return; 
            }
            
            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) {
                wPomocy = false; 
            }
        }

        okno.clear(); 
        okno.draw(tlo); 

        
        sf::Text tekstPomocy("Sterowanie:\nRuch strzalkami lewo/prawo\nUnikaj kolizji z przeszkodami\n\nESC - Powrot", czcionka, 20);
        tekstPomocy.setPosition(100, 200); 
        tekstPomocy.setFillColor(sf::Color::White); 
        okno.draw(tekstPomocy); 
        okno.display(); 
    }
}

class Asteroida {
private:
    sf::Sprite sprite;
    float predkosc;

public:
    Asteroida(const sf::Texture& tekstura, float startX, float startY, float szybkoscRuchu) : predkosc(szybkoscRuchu) {
        sprite.setTexture(tekstura);
        sprite.setPosition(startX, startY);
    }

    // aktualizacja pozycji asteroidy
    void aktualizuj(float deltaTime, float wysokoscOkna) {
        sprite.move(0, predkosc * deltaTime); // przesuniecie asteroidy w dol na podstawie czasu

        // sprawdzenie czy asteroida opuściła okno
        if (sprite.getPosition().y > wysokoscOkna) {
            resetujPozycje(); 
        }
    }

    
    void rysuj(sf::RenderWindow& okno) {
        okno.draw(sprite); 
    }

    // granice asteroidy
    sf::FloatRect pobierzGranice() const {
        return sprite.getGlobalBounds(); 
    }

    // resetowanie pozycji asteroidy
    void resetujPozycje() {
        //ustawienie poczatkowe
        sprite.setPosition(static_cast<float>(std::rand() % 748), -sprite.getGlobalBounds().height);

        // predkosc poczatkowa
        predkosc = static_cast<float>(std::rand() % 100 + 200);
    }

    // zwiekszenie predkosci
    void zwiekszPredkosc(float przyrost) {
        predkosc += przyrost; 
    }
};



class Gracz {
private:
    sf::Texture tekstura; 
    sf::Sprite sprite; 
    float predkosc;

public:
    Gracz(const std::string& sciezkaTekstury, float startX, float startY, float szybkoscRuchu) : predkosc(szybkoscRuchu) {
        if (!tekstura.loadFromFile(sciezkaTekstury)) {
            std::cout << "Błąd podczas ładowania obrazu gracza: " << sciezkaTekstury << "\n";
            exit(1);
        }

        sprite.setTexture(tekstura);
        sprite.setPosition(startX, startY);
    }

    // ruch w dowolnym kierunku
    void ruch(float deltaTime, float szerokoscOkna, int kierunek) {
        float przesuniecie = predkosc * deltaTime * kierunek; // obliczone przesuniecie na podstawie kierunku -1 lewo +1 prawo
        sprite.move(przesuniecie, 0);

        // ograniczenie ruchu
        if (sprite.getPosition().x < 0) {
            sprite.setPosition(0, sprite.getPosition().y);
        }
        else if (sprite.getPosition().x + sprite.getGlobalBounds().width > szerokoscOkna) {
            sprite.setPosition(szerokoscOkna - sprite.getGlobalBounds().width, sprite.getPosition().y);
        }
    }

    
    void rysuj(sf::RenderWindow& okno) {
        okno.draw(sprite);
    }

    // granice gracza
    sf::FloatRect pobierzGranice() const {
        return sprite.getGlobalBounds();
    }
};

class Menu {
private:
    sf::Font czcionka;
    sf::Text opcjeMenu[4];
    int wybranaOpcja;

public:
    Menu() : wybranaOpcja(0) {
        if (!czcionka.loadFromFile("arial.ttf")) {
            std::cout << "Błąd podczas ładowania czcionki\n";
            exit(1);
        }

        // konfiguracja menu
        opcjeMenu[0].setFont(czcionka);
        opcjeMenu[0].setString("Start");
        opcjeMenu[0].setPosition(300, 200);
        opcjeMenu[0].setFillColor(sf::Color::Red);

        opcjeMenu[1].setFont(czcionka);
        opcjeMenu[1].setString("Pomoc");
        opcjeMenu[1].setPosition(300, 300);
        opcjeMenu[1].setFillColor(sf::Color::White);

        opcjeMenu[2].setFont(czcionka);
        opcjeMenu[2].setString("Ranking");
        opcjeMenu[2].setPosition(300, 400);
        opcjeMenu[2].setFillColor(sf::Color::White);

        opcjeMenu[3].setFont(czcionka);
        opcjeMenu[3].setString("Wyjscie");
        opcjeMenu[3].setPosition(300, 500);
        opcjeMenu[3].setFillColor(sf::Color::White);
    }

    // zmiana pozycji menu
    void zmienOpcje(int kierunek) {
        opcjeMenu[wybranaOpcja].setFillColor(sf::Color::White); 

        // zmiana opcji zalezna od kierunku -1 gora +1 dol
        wybranaOpcja += kierunek;


        if (wybranaOpcja < 0) {
            wybranaOpcja = 0;
        }
        else if (wybranaOpcja >= 4) {
            wybranaOpcja = 3;
        }

        opcjeMenu[wybranaOpcja].setFillColor(sf::Color::Red);
    }

    
    void rysuj(sf::RenderWindow& okno) {
        for (const auto& opcja : opcjeMenu) {
            okno.draw(opcja);
        }
    }

    // pobieranie indeksu opcji
    int pobierzWybranaOpcje() const {
        return wybranaOpcja;
    }

    
    void resetujMenu() {
        for (int i = 0; i < 4; ++i) {
            opcjeMenu[i].setFillColor(sf::Color::White);
        }
        wybranaOpcja = 0;
        opcjeMenu[0].setFillColor(sf::Color::Red);
    }
};

// rozpoczecie gry 
void rozpocznijGre(sf::RenderWindow& okno, sf::Sprite& tlo, sf::Font& czcionka, std::vector<Wynik>& ranking) {
    Gracz gracz("statek.png", 358, 500, 300.0f); //pozycja poczatkowa gracza
    sf::Clock zegar; // delta czasu
    sf::Clock zegarGry; // calkowity czas
    sf::Clock zegarAnimacji; // przelaczanie animacji

    
    sf::Texture teksturaAsteroidy;
    if (!teksturaAsteroidy.loadFromFile("asteroida.png")) {
        std::cout << "Błąd podczas ładowania tekstury asteroidy\n";
        return;
    }

    
    sf::Texture teksturaTlo2;
    if (!teksturaTlo2.loadFromFile("tlo2.png")) {
        std::cout << "Błąd podczas ładowania tekstury tlo2.png\n";
    }

    // tworzenie listy asteroid
    std::vector<Asteroida> asteroidy;
    const int liczbaAsteroid = 5;
    for (int i = 0; i < liczbaAsteroid; ++i) {
        asteroidy.emplace_back(teksturaAsteroidy, static_cast<float>(std::rand() % 748), 0.0f, static_cast<float>(std::rand() % 100 + 100));
    }

    int uniknieteAsteroidy = 0;

    
    sf::Text tekstWyniku("Wynik: 0", czcionka, 30);
    tekstWyniku.setFillColor(sf::Color::White);
    tekstWyniku.setPosition(350, 10);

    sf::Text tekstPoziomu("Poziom: 1", czcionka, 20);
    tekstPoziomu.setFillColor(sf::Color::White);
    tekstPoziomu.setPosition(350, 50);

    sf::Text tekstCzasu;
    tekstCzasu.setFont(czcionka);
    tekstCzasu.setCharacterSize(20);
    tekstCzasu.setFillColor(sf::Color::White);
    tekstCzasu.setPosition(350, 50);

    sf::Text tekstESC("ESC - Menu", czcionka, 20);
    tekstESC.setPosition(10, 10);
    tekstESC.setFillColor(sf::Color::White);

    sf::Text tekstF1("F1 - Pomoc", czcionka, 20);
    tekstF1.setPosition(10, 40);
    tekstF1.setFillColor(sf::Color::White);

    sf::Text tekstKoniecGry("Game Over", czcionka, 50);
    tekstKoniecGry.setFillColor(sf::Color::Red);
    tekstKoniecGry.setPosition(250, 250);

    bool graTrwa = true;
    bool koniecGry = false;
    sf::Clock zegarKoniecGry;

    int aktualnyPoziom = 1;
    bool widocznyTekstPoziomu = true;

    // glowna petla gry obsluga ruchu gracza
    while (graTrwa) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed) {
                okno.close();
                return;
            }

            if (!koniecGry && zdarzenie.type == sf::Event::KeyPressed) {
                if (zdarzenie.key.code == sf::Keyboard::Escape) {
                    graTrwa = false;
                }
                if (zdarzenie.key.code == sf::Keyboard::F1) {
                    pokazEkranPomocy(okno, czcionka, tlo);
                }
            }
        }

        if (!koniecGry) {
            float deltaTime = zegar.restart().asSeconds();

            // obsluga ruchu gracza
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                gracz.ruch(deltaTime, okno.getSize().x, -1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                gracz.ruch(deltaTime, okno.getSize().x, 1);
            }


            // akualizacja asteroid
            for (auto& asteroida : asteroidy) {
                asteroida.aktualizuj(deltaTime, okno.getSize().y);

                if (asteroida.pobierzGranice().top + asteroida.pobierzGranice().height > okno.getSize().y) {
                    asteroida.resetujPozycje();
                    uniknieteAsteroidy++;
                    tekstWyniku.setString("Wynik: " + std::to_string(uniknieteAsteroidy));

                    
                    if (uniknieteAsteroidy >= 30 && aktualnyPoziom == 1) {
                        tlo.setTexture(teksturaTlo2);
                        aktualnyPoziom = 2;
                        tekstPoziomu.setString("Poziom: 2");
                        for (auto& asteroida : asteroidy) {
                            asteroida.zwiekszPredkosc(300.0f);
                        }
                    }
                }

                // detekcja kolizji
                if (gracz.pobierzGranice().intersects(asteroida.pobierzGranice())) {
                    koniecGry = true;
                    zegarKoniecGry.restart();
                }
            }

            // akutalizacja czasu gry
            float calkowityCzas = zegarGry.getElapsedTime().asSeconds();
            tekstCzasu.setString("Czas: " + std::to_string(static_cast<int>(calkowityCzas)) + "s");

            // animacja czasu
            sf::Time czasAnimacji = zegarAnimacji.getElapsedTime();
            if (czasAnimacji.asSeconds() >= 1.5f) {
                widocznyTekstPoziomu = !widocznyTekstPoziomu;
                zegarAnimacji.restart();
            }
        }
        else {
            if (zegarKoniecGry.getElapsedTime().asSeconds() > 3.0f) {
                graTrwa = false;
            }
        }

        
        okno.clear();
        okno.draw(tlo);
        gracz.rysuj(okno);
        for (auto& asteroida : asteroidy) {
            asteroida.rysuj(okno);
        }
        okno.draw(tekstESC);
        okno.draw(tekstF1);
        okno.draw(tekstWyniku);

        if (widocznyTekstPoziomu) {
            okno.draw(tekstPoziomu);
        }
        else {
            okno.draw(tekstCzasu);
        }

        if (koniecGry) {
            okno.draw(tekstKoniecGry);
        }

        okno.display();
    }

    // zapisywanie wyniku
    if (koniecGry) {
        float calkowityCzas = zegarGry.getElapsedTime().asSeconds();
        std::string obecnaData = pobierzAktualnaDate();
        ranking.push_back({ uniknieteAsteroidy, calkowityCzas, obecnaData });
    }


    koniecGry = false;
}






int main() {
    
    sf::RenderWindow okno(sf::VideoMode(800, 600), "Glowne Menu");
    sf::Font czcionka;

    
    if (!czcionka.loadFromFile("arial.ttf")) {
        std::cout << "Błąd podczas ładowania czcionki\n";
        return 1;
    }

    
    sf::Texture teksturaTla;
    if (!teksturaTla.loadFromFile("tlo.png")) {
        std::cout << "Błąd podczas ładowania obrazu tła\n";
        return 1;
    }
    sf::Sprite tlo(teksturaTla);

    Menu menu;
    bool wMenu = true;
    std::vector<Wynik> ranking;

   
    const std::string plikRankingu = "ranking.txt";
    wczytajRankingZPliku(plikRankingu, ranking);

    // glowna petla obsluga menu
    while (okno.isOpen()) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {

            if (zdarzenie.type == sf::Event::Closed) {

                zapiszRankingDoPliku(plikRankingu, ranking);
                okno.close();
            }

            
            if (wMenu) {
                if (zdarzenie.type == sf::Event::KeyPressed) {
                    if (zdarzenie.key.code == sf::Keyboard::Up) {
                        menu.zmienOpcje(-1);
                    }
                    else if (zdarzenie.key.code == sf::Keyboard::Down) {
                        menu.zmienOpcje(1);
                    }
                    else if (zdarzenie.key.code == sf::Keyboard::Enter) {
                        int wybranaOpcja = menu.pobierzWybranaOpcje();
                        if (wybranaOpcja == 0) {
                            wMenu = false;
                            rozpocznijGre(okno, tlo, czcionka, ranking);
                            wMenu = true;
                            menu.resetujMenu();
                            tlo.setTexture(teksturaTla);
                        }
                        else if (wybranaOpcja == 1) {
                            pokazEkranPomocy(okno, czcionka, tlo);
                        }
                        else if (wybranaOpcja == 2) {
                            pokazRanking(okno, czcionka, ranking);
                            menu.resetujMenu();
                        }
                        else if (wybranaOpcja == 3) {
                            if (potwierdzWyjscie(okno, czcionka)) {
                                zapiszRankingDoPliku(plikRankingu, ranking);
                                okno.close();
                            }
                        }
                    }
                }
            }
        }

        
        if (wMenu) {
            okno.clear();
            okno.draw(tlo);
            menu.rysuj(okno);
            okno.display();
        }
    }

    return 0;
}