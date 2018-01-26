#include "Laivanupotus.h"

/*--------------------------------------------------
*
* nimi: mainmenu
* toiminta: k‰sittelee p‰‰valikon valinnat ja ohjelman kulun
* parametri(t): peli tietue
* paluuarvo(t): valinta
*
*--------------------------------------------------*/

char mainmenu(tiedot &peli)
{
	char valinta;
	mainmenuPrint();

	std::cout << "Valintasi: ";
	std::cin >> valinta;

	switch (valinta)
	{
	case '1':
		enterShips(peli);
		break;
	case '2':
		if (peli.laivatsyotetty == true)
		{
			game(peli);
		}
		else std::cout << "Syota ensin laivat.\n";

		break;
	case '3':
		randomShips(peli);
		break;
	case 'l':
	case 'L':
		break;
	default:
		std::cout << "Virheellinen syote";
	}

	return valinta;
}

int main()
{
	srand(time(NULL));
	tiedot peli;
	char valinta;

	do
	{
		valinta= mainmenu(peli);
	}while(toupper(valinta) != 'L');

	return 0;
	 
}