#include <iostream>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>


using namespace std;
#define MAXKOLUMN 40
#define MAXWIERSZY 40
#define MAXMIN 100
#define strzalka_lewo 0x25
#define strzalka_prawo 0x27
#define strzalka_dol 0x28
#define strzalka_gora 0x26
#define enter 0x0D

int poz_x = 0, poz_y = 0, o_poz_x = 1, o_poz_y = 1;
int koniec = 0;



struct pole {
	int punkty;
	bool otwartosc;
};

pole plansza[MAXKOLUMN][MAXWIERSZY];

bool mina(int wsp_x, int wsp_y,int a)	 // ustalenie nadania po 1 punkcie na polach dookola miny
{
	if (plansza[wsp_x][wsp_y].punkty != 9)
	{
		plansza[wsp_x][wsp_y].punkty = 9;
		for (int k = -1; k < 2; k++)
			for (int x = -1; x < 2; x++)
			{
				if ((wsp_x + x) < 0 || (wsp_y + k) < 0) continue;
				if ((wsp_x + x) > a || (wsp_y + k) > a) continue;

				if (plansza[wsp_x + x][wsp_y + k].punkty == 9) continue;
				plansza[wsp_x + x][wsp_y + k].punkty+=1;
			}	
	}
	return true;
}


bool tworzenie_planszy(int a, int b)  //generowanie planszy do gry
{	
	for(int i=0; i<a; i++)
		for (int j = 0; j < b; j++)
		{
			plansza[i][j].punkty = 0;
			plansza[i][j].otwartosc = false;
			
		}
	return true;
}

void losowanie_min(int a, int b, int c) //losowanie i ustawianie min na pozycjach
{
	int wsp_x, wsp_y;
	int liczba = a;
	srand(time(NULL));

	while (liczba > 0)
	{
		wsp_x = rand() % b;
		wsp_y = rand() % c;

		if (plansza[wsp_x][wsp_y].punkty != 9)
		{
			mina(wsp_x, wsp_y, a);
			liczba--;
		}
	}

}

void pokaz_plansze(int a, int b)
{
	system("cls"); 

	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (j == poz_x && i == poz_y) 
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x01);	//kolor niebieski dla wskaznika
				cout << "*";
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);	//kolor bialy
				if (plansza[j][i].otwartosc == true) 
				{
					if (plansza[j][i].punkty == 0)   
						cout << " ";                
					else
						cout << plansza[j][i].punkty; 

				}
				if (plansza[j][i].otwartosc == false) 
					cout << "#"; 
			}
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07);	 //kolor bialy
		cout << endl;
	}

	cout << "\npozycja kursora:\n";  
	cout << "X: " << poz_x << endl;  
	cout << "Y: " << poz_y << endl;  
}

void odkryj_plansze(int x, int y, int a)
{
	if (x < 0 || x>a) return; 
	if (y < 0 || y>a) return; 
	if (plansza[x][y].otwartosc == true) return;  

	if (plansza[x][y].punkty != 9 && plansza[x][y].otwartosc == false)
		plansza[x][y].otwartosc = true;   

	if (plansza[x][y].punkty != 0) return; 

	
	odkryj_plansze(x - 1, y - 1,a);
	odkryj_plansze(x - 1, y,a);
	odkryj_plansze(x - 1, y + 1,a);
	odkryj_plansze(x + 1, y - 1,a);
	odkryj_plansze(x + 1, y,a);
	odkryj_plansze(x + 1, y + 1,a);
	odkryj_plansze(x, y - 1,a);
	odkryj_plansze(x, y,a);
	odkryj_plansze(x, y + 1,a);
}

void sterowanie(int a, int b)
{
	if ((GetKeyState(enter) & 0x8000))
	{
		if (plansza[poz_x][poz_y].punkty == 9) 
			koniec = 2;

		odkryj_plansze(poz_x, poz_y,a); 
		pokaz_plansze(a,b);
	}

	if ((GetKeyState(strzalka_prawo) & 0x8000) && poz_x < a) poz_x++;
	if ((GetKeyState(strzalka_lewo) & 0x8000) && poz_x > 0) poz_x--;
	if ((GetKeyState(strzalka_dol) & 0x8000) && poz_y < a) poz_y++;
	if ((GetKeyState(strzalka_gora) & 0x8000) && poz_y > 0) poz_y--;

	if (o_poz_y == poz_y && o_poz_x == poz_x) return;

	o_poz_y = poz_y; 
	o_poz_x = poz_x;

	pokaz_plansze(a,b); 
}

bool sprawdz_czy_wygrana(int a, int b, int c)
{
	int miny = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (plansza[j][i].otwartosc == false)
				miny++;
		}
	}
	if (miny == c) return true;
	return false;
}


int liczenie_pkt(int a, int b)
{
	int suma_pkt=0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			plansza[i][j];
			if (plansza[i][j].punkty == 1 && plansza[i][j].otwartosc==true)
				suma_pkt++;
			else if (plansza[i][j].punkty == 2 && plansza[i][j].otwartosc==true)
				suma_pkt += 2;
			else if (plansza[i][j].punkty == 3 && plansza[i][j].otwartosc==true)
				suma_pkt += 3;
			else if (plansza[i][j].punkty == 4 && plansza[i][j].otwartosc == true)
				suma_pkt += 4;
			else if (plansza[i][j].punkty == 5 && plansza[i][j].otwartosc == true)
				suma_pkt += 5;
			else if (plansza[i][j].punkty == 6 && plansza[i][j].otwartosc == true)
				suma_pkt += 6;
			else if (plansza[i][j].punkty == 7 && plansza[i][j].otwartosc == true)
				suma_pkt += 7;
			else if (plansza[i][j].punkty == 8 && plansza[i][j].otwartosc == true)
				suma_pkt += 8;
			else
				suma_pkt;
			
		}
	}
	cout << "Ilosc zdobytych punktow to: " << suma_pkt << endl;
	return suma_pkt;
}

int ostatnie_pkt(int suma_pkt,unsigned c)
{

	if (c > 0 && c < 6000)
		suma_pkt = (suma_pkt * 5);
	else if (c > 6000 && c < 15000)
		suma_pkt = (suma_pkt * 4);
	else if (c > 15000 && c < 30000)
		suma_pkt = (suma_pkt * 2);
	else
		suma_pkt;
	return suma_pkt;
}



int main()
{
	int i, j, k, suma, suma_koniec;
	unsigned czas;
	string nazwa;
	cout << "Witaj w grze SAPER" << endl;
	//cout << "Jesli zdolasz wygrac w conajwyzej 6000ms liczba punktow zostanie pomnozona przez 5" << endl << "conajwyzej 15000ms daje promocje o 4 razy wiecej punktow" << endl << "30000ms to ilosc punktow razy 2" << endl;
	cout << "Podaj nazwe gracza: ";
	cin >> nazwa;
	cout << endl;
	cout << "Podaj wilkosc planszy na ktorej chcesz grac(liczba kolum jak i wierszy nie moze byc wieksza niz 40): " << endl;
	cout << "Liczba kolumn: " << endl;
	cin >> i;
	if (i > MAXKOLUMN)
	{
		cout << "Zbyt duza liczba kolumn!";
		system("pause");
	}
	cout<<"Liczba wierszy : " << endl;
	cin >> j;
	if (j > MAXWIERSZY)
	{
		cout << "Zbyt duza liczba wierszy!";
		system("pause");
	}
	cout << "Podaj ilosc min: " << endl;
	cin >> k;
	if (k > MAXMIN)
	{
		cout << "Zbyt duzo min!";
		system("pause");
	}

	
	clock_t start = ((1000 * clock()) / CLOCKS_PER_SEC);
	tworzenie_planszy(i,j);
	losowanie_min(k,i,j);
	Sleep(2000);
	
	while (koniec == 0)
	{
		Sleep(60);
		sterowanie(i,j);
		if (sprawdz_czy_wygrana(i,j,k) == true) koniec = 1;
	}
	if (koniec == 1)
	{
		cout << endl << "WYGRANA!";
	}
	if (koniec == 2)
	{
		cout << "WYBUCH!" << endl << "PRZEGRANA!";
	}
	cout << endl;
	czas = (clock() - start);
	printf("Czas gry: %lu ms\n", clock() - start);
	suma = liczenie_pkt(i, j);
	ostatnie_pkt(suma, czas);
	suma_koniec = ostatnie_pkt(suma, czas);
	fstream Text;
	Text.open("Text.txt");
	if (Text.good() == true)
	{
		Text << nazwa << " " << "ilosc zdobytych punktow: " << suma_koniec << endl;
		

		Text.close();
	}

	


	system("pause >nul");
	return 0;

	
}