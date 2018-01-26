#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

#define SIZEY 7		// Vakioiden m‰‰rittely makroilla. V‰ltet‰‰n varsinaisten muuttujien k‰yttˆ‰.
#define SIZEX 7
#define pisinlaiva 5
#define syoteL 20

struct tiedot
{
	int laivaTaulukko[SIZEY][SIZEX];
	char ammuntaTaulukko[SIZEY][SIZEX];
	int osumat[pisinlaiva+1];
	bool complete;
	bool laivatsyotetty;
};



bool verifyShip(tiedot &, int, int, char, int);
void mainmenuPrint();
char mainmenu(tiedot &);
void enterShips(tiedot &);
void game(tiedot &);
void gameInit(tiedot &);
void hitCheck(tiedot &, char[] );
void shoot(char[],tiedot &);
void shootPrompt(tiedot &);
bool feedCheck(int, int);
void clearScreen();
void cheatMode(tiedot &);
void randomShips(tiedot &);


/*--------------------------------------------------
*
* nimi: mainmenuPrint
* toiminta: tulostaa p‰‰valikon
* parametri(t): -
* paluuarvo(t): -
*
*--------------------------------------------------*/

void mainmenuPrint() // Tulostaa p‰‰valikon
{
	clearScreen();
	std::cout << "Laivanupotuspeli\n"
		<< "================\n\n"
		<< "Valinnat:\n" << "1) Syota Laivat\n"
		<< "2) Pelaa\n"  
		<< "3) Arvo laivojen sijainti\n" << "L) Lopeta\n\n";

}

/*--------------------------------------------------
*
* nimi: enterShips
* toiminta: kysyy laivojen sijainnit ja suunnat ja asettaa laivat laivataulukkoon. Tarkistaa myˆs virhetilanteita.
* parametri(t): peli tietue
* paluuarvo(t): -
*
*--------------------------------------------------*/

void enterShips(tiedot &peli)
{
	clearScreen();
	gameInit(peli);
	char alku[syoteL];
	int kordy, kordx; //apumuuttujat taulukon k‰sittelyyn
	char suunta;
	//std::cout << "valinta 1\n";
	for (int j = pisinlaiva; j > 1;j--)
	{
		std::cout << "Anna " << j << " merkkia pitkan laivan alkupiste: ";
		std::cin >> alku;
		if (alku[2] != '\0')
		{
			std::cout << "Virheellinen syote.\n";
			j++;
		}
		else
		{
			alku[0] = toupper(alku[0]);
			kordy = static_cast<int>(alku[0]) - 65;
			kordx = alku[1] - 49;
			if (feedCheck(kordy, kordx))
			{
				std::cout << "Virheellinen syote.\n";
				j++;
			}
			else
			{
				std::cout << "Anna suunta (p(ohjoinen)/i(ta)/e(tela)/l(ansi)): ";
				std::cin >> suunta;

				if (verifyShip(peli,kordy,kordx,suunta,j))
				{
					switch (suunta) {
					case 'e':
						//std::cout << "Kordinaatit: " << alku[0] << " muunnettuna: " << static_cast<int>(alku[0]) - 97;
						for (int i = 0;i < j;i++)
						{
							peli.laivaTaulukko[kordy + i][kordx] = j;
						}
						break;
					case 'p':
						for (int i = 0;i < j;i++)
						{
							peli.laivaTaulukko[kordy - i][kordx] = j;
						}
						break;
					case 'i':
						for (int i = 0;i < j;i++)
						{
							peli.laivaTaulukko[kordy][kordx + i] = j;
						}
						break;
					case 'l':
						for (int i = 0;i < j;i++)
						{
							peli.laivaTaulukko[kordy][kordx - i] = j;
						}
						break;
					default:
						std::cout << "Virheellinen syote.\n";
						j++;
					}
				}
				else
				{
					std::cout << "Tila ei ole vapaa.\n";
					j++;
				}
			}
		}
	}
	/*for (int i = 0;i < SIZEY;i++)
	{
		for (int j = 0;j < SIZEX; j++)
		{
			std::cout << peli.laivaTaulukko[i][j];
		}
		std::cout << std::endl;
	}*/
	peli.laivatsyotetty = true;
	peli.complete = false;
}

/*--------------------------------------------------
*
* nimi: shoot
* toiminta: ammuntakordinaattien kysynt‰ ja k‰sittely
* parametri(t): ammuntakordinaatit ja peli tietue
* paluuarvo(t): muokkaa peli tietuetta
*
*--------------------------------------------------*/

void shoot(char valinta[syoteL], tiedot &peli)
{
	//std::cout << "shoot\n";
	
	shootPrompt(peli);

	std::cout << "Anna ampumiskordinaatit: ";
	std::cin >> valinta;
	valinta[0]=toupper(valinta[0]);
	if (valinta[0] == '\\' && valinta[1]=='@')
	{
		cheatMode(peli);
		std::cout << "Anna ampumiskordinaatit: ";
		std::cin >> valinta;
		valinta[0] = toupper(valinta[0]);
		if (valinta[0] != 'P')hitCheck(peli, valinta);
	}
	else if (feedCheck(valinta[0] - 65, valinta[1] - 49) || valinta[2]!='\0') std::cout << "Virheellinen syote.\n";
	else if (valinta[0] != 'P')hitCheck(peli, valinta);
	if (peli.osumat[2] == 2 && peli.osumat[3] == 3 && peli.osumat[4] == 4 && peli.osumat[5] == 5) peli.complete = true;
}

/*--------------------------------------------------
*
* nimi: shootPrompt
* toiminta: n‰ytt‰‰ ammuntataulukon
* parametri(t): peli tietue
* paluuarvo(t): -
*
*--------------------------------------------------*/

void shootPrompt(tiedot &peli)
{
	//std::cout << "shootPrompt\n";
	std::cout << "Pelitilanne on seuraava:\n";

	std::cout << std::setw(5) << ' ' << "1 2 3 4 5 6 7 \n";
	std::cout << "  ------------------  \n";

	for (int i = 0;i < SIZEY;i++)
	{
		std::cout << static_cast<char>('A' + i) << " | ";

		for (int j = 0;j < SIZEX;j++)
		{
			std::cout << std::setw(2) << peli.ammuntaTaulukko[i][j];
			
		}

		std::cout << " |" << static_cast<char>('A' + i) << std::endl;
	}
	std::cout << "  ------------------  \n";
	std::cout << std::setw(5) << ' ' << "1 2 3 4 5 6 7 \n";
}

/*--------------------------------------------------
*
* nimi: game
* toiminta: yleinen pelinkulku. Jos peli on p‰‰ttynyt est‰‰ jatkamisen.
* parametri(t): peli tietue
* paluuarvo(t): -
*
*--------------------------------------------------*/

void game(tiedot &peli)
{
	clearScreen();
	char valinta[syoteL];
	do
	{
		//std::cout << "Valinta 2\n";
		if (peli.complete == true)
		{
			std::cout << "Kaikki laivat upotettu. Peli paattyi.\n";
			break;
		}
		shoot(valinta, peli);
	} while (toupper(valinta[0] != 'P'));
}

/*--------------------------------------------------
*
* nimi: hitCheck
* toiminta: Tarkistaa tuliko osumaa ja tekee merkinn‰t ammuntataulukkoon
* parametri(t): peli tietue ja ammunta kordinaatit
* paluuarvo(t): muokkaa ammuntataulukkoa
*
*--------------------------------------------------*/

void hitCheck(tiedot &peli, char valinta[syoteL])
{
	clearScreen();
	if (peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1]-49] <= pisinlaiva
		&& peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1]-49] > 0)
	{
		peli.osumat[peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1]-49]]++;

		if (peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1]-49] 
			== peli.osumat[peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1]-49]])
		{
			std::cout << "Laukaus kohtaan " << valinta << " upotti laivan\n";
			for (int i = 0;i < SIZEY;i++)
			{
				for (int j = 0;j < SIZEX;j++)
				{
					if (peli.laivaTaulukko[i][j] == peli.laivaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1] - 49])
					{
						peli.ammuntaTaulukko[i][j] = '#';
					}
				}
			}
		}
		else
		{
			peli.ammuntaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1] - 49] = '*';
			std::cout << "Laukaus kohtaan " << valinta << " osui laivaan\n";
		}
	}
	else
	{
		peli.ammuntaTaulukko[static_cast<int>(valinta[0]) - 65][valinta[1] - 49] = 'x';
		std::cout << "Laukaus kohtaan " << valinta << " ei osunut\n";
	}
}

/*--------------------------------------------------
*
* nimi: gameInit
* toiminta: alustaa peli tietueen taulukot
* parametri(t): peli tietue
* paluuarvo(t): taulukot alustettuina
*
*--------------------------------------------------*/

void gameInit(tiedot &peli) //Tietojen alustus
{
	for (int i = 0;i < pisinlaiva + 1;i++)
	{
		peli.osumat[i] = 0;
	}
	for (int i = 0;i < SIZEY; i++)
	{
		for (int j = 0;j < SIZEX; j++)
		{
			peli.laivaTaulukko[i][j] = 0;
		}
	}
	for (int i = 0;i < SIZEY; i++)
	{
		for (int j = 0;j < SIZEX; j++)
		{
			peli.ammuntaTaulukko[i][j] = ' ';
		}
	}

}

/*--------------------------------------------------
*
* nimi: feedCheck
* toiminta: tarkistaa ett‰ syˆteen kordinaatit ovat oikealta alueelta
* parametri(t): y- ja x-kordinaatit
* paluuarvo(t): 1 = jos virheellinen syˆte, 0 jos oikea
*
*--------------------------------------------------*/

bool feedCheck(int kordy, int kordx)
{
	return (kordy >= SIZEY || kordx >= SIZEX || kordy < 0 || kordx < 0);
}

/*--------------------------------------------------
*
* nimi: clearScreen
* toiminta: tulostaa tyhj‰‰ ruudun siistimiseksi
* parametri(t): -
* paluuarvo(t): -
*
*--------------------------------------------------*/

void clearScreen()
{
	std::cout << '\r';

	for (int i = 0;i < 30;i++)
	{
		std::cout << std::endl;
	}

	std::cout << '\r';
}

/*--------------------------------------------------
*
* nimi: verifyShip
* toiminta: tarkistaa, ett‰ tila on vapaa laivan syˆttˆˆn
* parametri(t): peli tietue, valintakordinaatit ja suunta, sek‰ laivan koko.
* paluuarvo(t): 1 = jos tila on vapaa
*
*--------------------------------------------------*/

bool verifyShip(tiedot &peli, int kordy, int kordx, char suunta,int laivan_koko)
{
	int virhe = 0;
	switch (suunta)
	{
	case 'e':
		if (kordy + laivan_koko < SIZEY)
		{
			for (int i = 0;i < laivan_koko;i++)
			{
				if (peli.laivaTaulukko[kordy + i][kordx] != 0) virhe++;
			}
			return (virhe == 0);
		}
		else return 0;
		break;
	case 'p':
		if (kordy - laivan_koko >= 0)
		{
			for (int i = 0;i < laivan_koko;i++)
			{
				if (peli.laivaTaulukko[kordy - i][kordx] != 0) virhe++;
			}
			return (virhe == 0);
		}
		else return 0;
		break;
	case 'i':
		if (kordx + laivan_koko < SIZEX)
		{
			for (int i = 0;i < laivan_koko;i++)
			{
				if (peli.laivaTaulukko[kordy][kordx+i] != 0) virhe++;
			}
			return (virhe == 0);
		}
		else return 0;
		break;
	case 'l':
		if (kordx - laivan_koko >= 0)
		{
			for (int i = 0;i < laivan_koko;i++)
			{
				if (peli.laivaTaulukko[kordy][kordx-i] != 0) virhe++;
			}
			return (virhe == 0);
		}
		else return 0;
		break;
	default:
		return 0;
		break;

	}
}

/*--------------------------------------------------
*
* nimi: randomShips
* toiminta: Arpoo laivat laivataulukkoon
* parametri(t): peli tietue
* paluuarvo(t): -
*
*--------------------------------------------------*/

void randomShips(tiedot &peli)
{
	gameInit(peli);
	for (int i = 5;i > 1;)
	{
		int kordy = rand() % 7;
		int kordx = rand() % 7;
		int suuntarand = rand() % 4;
		char suunta = 'e';

		switch (suuntarand)
		{
		case 1:
			suunta = 'e';
			break;
		case 2:
			suunta = 'p';
			break;
		case 3:
			suunta = 'i';
			break;
		case 4:
			suunta = 'l';
			break;
		}

		if (verifyShip(peli, kordy, kordx, suunta, i))
		{
			switch (suunta) {
			case 'e':
				//std::cout << "Kordinaatit: " << alku[0] << " muunnettuna: " << static_cast<int>(alku[0]) - 97;
				for (int j = 0;j < i;j++)
				{
					peli.laivaTaulukko[kordy + j][kordx] = i;
				}
				break;
			case 'p':
				for (int j = 0;j < i;j++)
				{
					peli.laivaTaulukko[kordy - j][kordx] = i;
				}
				break;
			case 'i':
				for (int j = 0;j < i;j++)
				{
					peli.laivaTaulukko[kordy][kordx + j] = i;
				}
				break;
			case 'l':
				for (int j = 0;j < i;j++)
				{
					peli.laivaTaulukko[kordy][kordx - j] = i;
				}
				break;
			}
			i--;
		}

	}
	std::cout << "Laivat arvottu.\n";
	for (int i = 0;i < SIZEY;i++)
	{
		for (int j = 0;j < SIZEX; j++)
		{
			std::cout << peli.laivaTaulukko[i][j];
		}
		std::cout << std::endl;
	}
	peli.laivatsyotetty = true;
	peli.complete = false;
}

/*--------------------------------------------------
*
* nimi: cheatMode
* toiminta: n‰ytt‰‰ laivojen sijainnin
* parametri(t): peli tietue
* paluuarvo(t): -
*
*--------------------------------------------------*/

void cheatMode(tiedot &peli)
{
	clearScreen();
	std::cout << "Laivojen sijainnit ovat seuraavat: \n";

	std::cout << std::setw(5) << ' ' << "1 2 3 4 5 6 7 \n";
	std::cout << "  ------------------  \n";

	for (int i = 0;i < SIZEY;i++)
	{
		std::cout << static_cast<char>('A' + i) << " | ";

		for (int j = 0;j < SIZEX;j++)
		{
			if (peli.laivaTaulukko[i][j] != 0) std::cout << std::setw(2) << peli.laivaTaulukko[i][j];
			else std::cout << std::setw(2) << peli.ammuntaTaulukko[i][j];

		}

		std::cout << " |" << static_cast<char>('A' + i) << std::endl;
	}
	std::cout << "  ------------------  \n";
	std::cout << std::setw(5) << ' ' << "1 2 3 4 5 6 7 \n";
}