#include <iostream>
#include<string>
#include<queue>
using namespace std;
const int MAX = 10;

struct Elem {
	
	string tekst;
	int brPotom;
	int ocenePotom[MAX]={ 0 };//ocene potomka
	Elem* nizPotom[MAX];//niz za pokazivace na potomke
	char tip;
	Elem* otac=nullptr;
	Elem(char t='P') {
		for (int i = 0; i < MAX; i++)
		{
			nizPotom[i] = nullptr;
		}
		brPotom = 0;
		tekst = "";
		tip = t;
	}
};

struct Pitanja {
	Elem* pitanje=nullptr;
	Pitanja* sled=nullptr;
};


Pitanja* kreirajPitanje()
{
	return new Pitanja();
}

void dodajPitanje(Pitanja*& prvi, Elem& pit) {
	if (prvi == nullptr)
	{
		prvi = kreirajPitanje();
	}
	if (!prvi->pitanje) {
		prvi->pitanje = new Elem(pit);
	}
	else
	{
		Pitanja* tek = prvi;
		while (tek->sled)
		{
			tek = tek->sled;
		}
		tek->sled= kreirajPitanje();
		tek->sled->pitanje = new Elem(pit);
	}
}

//Ukoliko takvo pitanje ne postoji nista ne uradi; Ne moze se pozvati kao prva naredba u proghramu
void dodajOdgovorNaPitanje(Pitanja* prvi,string pit,Elem& odg)
{
	//moze i preko funkcije pretragaPitanja
	Pitanja* tek = prvi;
	while (tek)
	{
		if (tek->pitanje->tekst == pit)
		{
			break;
		}
		tek = tek->sled;
	}
	if (tek!=nullptr)
	{
		if (tek->pitanje->brPotom < MAX)
		{
			for (int i = 0; i < MAX; i++)
			{
				if (tek->pitanje->nizPotom[i] == nullptr)
				{
					Elem* pom = new Elem(odg);
					pom->otac = tek->pitanje;
					tek->pitanje->nizPotom[i] = pom;
					tek->pitanje->brPotom++;
					break;
				}
			}
		
		}
	/*	else
		{
			cout << "Dostignut je maksimalan broj odgovora na pitanje, ne moze se dodati novi odgovor";
			exit(2);
		}*/

	}
	//else
	//{
	//	cout << "Takvo pitanje ne postoji";
	//	exit(1);
	//}
}

Elem* pretragaPitanja(Pitanja *prvi,string pit)//ukoliko ne postoji povratna vrednost je nullptr
{
	Pitanja* tek = prvi;
	while (tek)
	{
		if (tek->pitanje->tekst == pit)
		{
			return tek->pitanje;
		}
		tek = tek->sled;
	}
	return nullptr;
}

Elem* pretragaOdgovora(Pitanja* prvi, string pit, string odg)//ukoliko ne postoji povratna vrednost je nullptr;
{
	Elem* tek = pretragaPitanja(prvi,pit);
	queue <Elem*> red;
	Elem* next = tek;
	red.push(next);
	while (!red.empty())
	{
		Elem* pom = red.front();
		next = pom;
		red.pop();
		//if (next->tip != 'P') //da bi mogo da vrati i pokazivac na pitanje, potrebno za funkciju koja brise cvor da bi mogla da obrise i pitanje
		//{
			if (next->tekst == odg)
			{
				return next;
			}
		//}
		for (int i = 0; i < MAX; i++)
		{
			if (next->nizPotom[i] != nullptr)
			{
				red.push(next->nizPotom[i]);

			}
			//else break;
			
		}
	}
	return nullptr;
}
// Ukoliko takvo pitanje ne postoji nista ne uradi; Ne moze se ova funkcija pozvati kao prva jer je prvi->pitanje==nullptr
void dodajOdgovorNaOdgovor(Pitanja* prvi, string odg, Elem& kom)
{
	bool naso = false;
	Pitanja* tek = prvi;
	Pitanja* root = prvi;
	while (tek)
	{
		Elem* od = pretragaOdgovora(root, tek->pitanje->tekst,odg);
		if (od != nullptr)
		{
			if (od->brPotom < MAX)
			{
				for (int i = 0; i < MAX; i++)
				{
					if (od->nizPotom[i] == nullptr)
					{
						naso = true;
						Elem* pom = new Elem(kom);
						pom->otac = od;
						od->nizPotom[i] = pom;
						od->brPotom++;
						break;
					}
				}

			}
			//else
			//{
			//	cout << "Dostignut je maksimalan broj komentara na odgovor, ne moze se dodati novi komentar";
			//	exit(4);
			//}
		}
		tek = tek->sled;
	}
	//if (naso == false)
	//{
	//	cout << "Takav odgovor ne postoji";
	//	exit(5);
	//}
}

void pisiPitanja(Pitanja* prvi)
{
	if (prvi == nullptr)//za slucaj da se pozove kao prva naredba
	{
		cout << "Ne postoji ni jedno pitanje." << endl;
		exit(4);
	}
	Pitanja* tek = prvi;
	int i = 1;
	while (tek)
	{
		cout <<i<<". "<< tek->pitanje->tekst << endl;
		i++;
		tek = tek->sled;
	}
}

void povecajOcenu(Pitanja* prvi, string odg)//ako nema takav odgovor nista se nece desiti, samo ce se ispisati poruka
{
	if (prvi->pitanje == nullptr)//za slucaj da se pozove kao prva funkcija
	{
		cout << "Ne postoji ni jedno pitanje." << endl;
		exit(4);
	}
	bool povecano = false;
	Pitanja* tek = prvi;
	Pitanja* root = prvi;
	while (tek)
	{
		Elem* od = pretragaOdgovora(root, tek->pitanje->tekst, odg);
		if (od != nullptr)
		{
			Elem* otac = od->otac;
			for (int i = 0; i < MAX; i++)
			{
				if (otac->nizPotom[i] == od)
				{
					otac->ocenePotom[i]++;
					povecano = true;
					break;
				}
			}
			
		}
		if (povecano)
		{
			break;
		}
		tek = tek->sled;
		
	}
	if (!povecano)
	{
		cout << "Takvo pitanje ne postoji." << endl;
	}
}

void urediCvor(Elem* &cvor)
{
	for (int i = 0; i < MAX-1; i++)
	{
		for (int j = i+1; j < MAX; j++)
		{
			if (cvor->nizPotom[i] != nullptr && cvor->nizPotom[j] != nullptr)
			{
				if (cvor->ocenePotom[j] > cvor->ocenePotom[i])
				{
					//obratiti paznju na mogucu gresku kad je nizpot[i]==nullptr
					Elem* pom = cvor->nizPotom[i];
					cvor->nizPotom[i] = cvor->nizPotom[j];
					cvor->nizPotom[j] = pom;
					int p = cvor->ocenePotom[i];
					cvor->ocenePotom[i] = cvor->ocenePotom[j];
					cvor->ocenePotom[j] = p;
				}
			}
			
		}
	}
}

void urediOdgovore(Pitanja* prvi) {
	if (prvi->pitanje == nullptr)//za slucaj ako se pozove kao prva komanda
	{
		cout << "Ne postoji nijedno pitanje." << endl;
		exit(4);
	}
	Pitanja* tek = prvi;
	while (tek)
	{
		Elem* next = tek->pitanje;
		queue<Elem*> red;
		red.push(next);
		while (!red.empty())
		{
			Elem* pom = red.front();
			next = pom;
			red.pop();
			urediCvor(next);
			for (int i = 0; i < MAX; i++)
			{
				if (next->nizPotom[i] != nullptr)
				{
					red.push(next->nizPotom[i]);

				}
				else
					break;
			}


		}



		tek = tek->sled;
	}
}

void brisiOdgovor(Pitanja* &prvi, string odg)
{
	if (prvi->pitanje == nullptr)//ukoliko se kao prva naredba unese brisi odgovor
	{
		cout << "Ne postoji nijedno pitanje." << endl;
		exit(4);
	}
	bool izbrisan = false;
	Pitanja* tek = prvi;
	while (tek)
	{
		Elem* pom = pretragaOdgovora(prvi, tek->pitanje->tekst, odg);
		if (pom != nullptr)
		{
			//postavljanje nullptr u cvoru oca na mestu cvora koji se brise samo ukoliko taj cvor nije pitanje jer pitanje nema oca
			if (pom->tekst != tek->pitanje->tekst) {
				Elem* otac = pom->otac;
				for (int i = 0; i < MAX; i++)
				{
					if (otac->nizPotom[i] == pom)
					{
						otac->nizPotom[i] = nullptr;
						otac->ocenePotom[i] = 0;
						break;
					}
				}
				otac->brPotom--;

			}
			
			Elem* next = pom;
			queue<Elem*>red;
			red.push(next);
			while (!red.empty())
			{
				Elem* pom = red.front();
				next = pom;
				red.pop();
				for (int i = 0; i < MAX; i++)
				{
					if (next->nizPotom[i] != nullptr)
					{
						red.push(next->nizPotom[i]);

					}
				}
				delete next;
				next = nullptr;


			}
			izbrisan = true;
			break;

		}


		tek = tek->sled;
	}
	if (!izbrisan)
	{
		cout << "Takav odgovor ne postoji." << endl;
	}
}

void brisiPitanje(Pitanja* &prvi, string pit)
{
	if (prvi->pitanje == nullptr)//ukoliko se kao prva naredba unese brisi pitanje
	{
		cout << "Ne postoji ni jedno pitanje." << endl;
		exit(4);
	}
	Elem* stari = pretragaPitanja(prvi, pit);
	if (stari != nullptr)
	{
		if (prvi->pitanje == stari)
		{
			Pitanja* tek = prvi;
			while (tek)
			{
				if (tek->pitanje == stari)
				{
					break;
				}
				tek = tek->sled;
			}
			brisiOdgovor(prvi, stari->tekst);
			tek->pitanje = nullptr;
			prvi = prvi->sled;
			delete tek;
		}
		else
		{
			Pitanja* tek = prvi;
			while (tek)
			{
				if (tek->sled->pitanje == stari)
				{
					Pitanja *st = tek->sled;
					brisiOdgovor(prvi, stari->tekst);
					st->pitanje = nullptr;
					tek->sled = st->sled;
					delete st;
					break;
				}

				tek = tek->sled;
			}
		}
	}
	else
	{
		cout << "Takvo pitanje ne postoji." << endl;
	}
	
}

void pisiPitanje(Pitanja* prvi, string pit) 
{
	Elem* otac = nullptr;
	bool j = false;
	Elem* pitanje = pretragaPitanja(prvi, pit);
	if (pitanje == nullptr)
	{
		cout << "Takvo pitanje ne postoji."<<endl;

	}
	else
	{
		queue<Elem*> red;
		Elem* next = pitanje;
		red.push(next);
		while (!red.empty())
		{
			Elem* pom = red.front();
			next = pom;
			red.pop();
			if (next == nullptr)
			{
				if (!red.empty())
				{
					red.push(nullptr);
					cout << endl;
					cout << endl;
					j = false;
					continue;
				}
				else
				{
					break;
				}

			}
			else
			{
				if (next->tip == 'P')
				{
					cout << "P: " << next->tekst << endl;
					cout << endl;
				}
				else
				{
					if (!j)
					{
						otac = next->otac;
						j = true;
						cout << "(" << next->otac->tekst << ")" << next->tip << ": " << next->tekst << '\t';
					}
					else
					{
						if (next->otac == otac)
						{
							cout << "(" << next->otac->tekst << ")" << next->tip << ": " << next->tekst << '\t';
						}
						else
						{
							otac = next->otac;
							cout << "|*****|\t";
							cout << "(" << next->otac->tekst << ")" << next->tip << ": " << next->tekst << '\t';
						}
					}


				}
			}
			for (int i = 0; i < MAX; i++)
			{
				if (next->nizPotom[i] != nullptr)
				{
					red.push(next->nizPotom[i]);
				}


			}
			if (next->tip == 'P')
			{
				red.push(nullptr);
			}

		}
	}
	
}
//proveriti funkciju
Elem* pitanjeSaNajviseOdgIKom(Pitanja* prvi)
{
	Elem* max = prvi->pitanje;
	if (max == nullptr)
	{
		cout << "Ne postoji ni jedno pitanje."<<endl;
		exit(4);
	}
	int m = 0;
	int p = 0;
	Pitanja* tek = prvi;
	while (tek)
	{
		p = 0;
		queue<Elem*> red;
		Elem* next = tek->pitanje;
		red.push(next);
		while (!red.empty()) 
		{
			Elem* pom = red.front();
			next = pom;
			red.pop();
			p += next->brPotom;
			for (int i = 0; i < MAX; i++)
			{
				if (next->nizPotom[i] != nullptr)
				{
					red.push(next->nizPotom[i]);

				}

			}


		}
		if (p > m)
		{
			m = p;
			max = tek->pitanje;
			
		}

		tek = tek->sled;
	}
	return max;
}

void dealociraj(Pitanja*& prvi)
{
	while (prvi)
	{
		if (prvi != nullptr)
		{
			brisiPitanje(prvi, prvi->pitanje->tekst);
		}
	}
}




int main() {

	Pitanja *prvi = kreirajPitanje();
	int x;
	
	
	do
	{
		cout << "==========================" << endl;
		cout << "\t***MENI***" << endl;
		cout << "1. Dodaj pitanje" << endl;
		cout << "2. Dodaj odgovor na pitanje" << endl;
		cout << "3. Dodaj odgovor na odgovor" << endl;
		cout << "4. Nadji pitanje i ispisi" << endl;
		cout << "5. Povecaj ocenu nekom odgovoru" << endl;
		cout << "6. Uredi odgovore prema oceni" << endl;
		cout << "7. Ispisi sva pitanja" << endl;
		cout << "8. Obrisi odgovor" << endl;
		cout << "9. Obrisi pitanje" << endl;
		cout << "10.Ispisi pitanje sa najvise odgovora i komentara" << endl;
		cout << "0. Prekid programa" << endl;
		cout << "\t ******" << endl;
		cout << "***MAKSIMALAN BROJ ODGOVORA NA NEKO PITANJE JE 10***"<<endl;
		cout << "Unesite zeljenu opciju" << endl;
		cin >> x;
		cout << "==========================" << endl;
		if (x == 1)
		{
			string g;
			getline(cin, g);
			cout << "Unesite pitanje: ";
			string pom;
			getline(cin, pom);
			Elem pit('P');
			pit.tekst = pom;
			dodajPitanje(prvi, pit);
		}
		else if (x == 2)
		{
			string g;
			getline(cin, g);
			cout << "Unesite pitanje na koje zelite da date odgovor: ";
			string pi;
			getline(cin, pi);
			cout << "Unesite zeljeni odgovor: ";
			string odg;
			getline(cin, odg);
			Elem o('O');
			o.tekst = odg;
			dodajOdgovorNaPitanje(prvi, pi, o);

		}
		else if (x == 3)
		{
			string g;
			getline(cin, g);
			cout << "Unesite odgovor na koji zelite da date odgovor: ";
			string od;
			getline(cin, od);
			cout << "Unesite zeljeni odgovor: ";
			string odg;
			getline(cin, odg);
			Elem k('K');
			k.tekst = odg;
			dodajOdgovorNaOdgovor(prvi, od, k);

		}
		else if (x == 4)
		{
			string g;
			getline(cin, g);
			cout << "Unesite pitanje koje zelite da se ispise: ";
			string pit;
			getline(cin, pit);
			cout << endl;
			pisiPitanje(prvi, pit);
			cout << endl;
			cout << endl;
		}
		else if (x == 5)
		{
			string g;
			getline(cin, g);
			cout << "Unesite odgovor kome zelite da povecate ocenu: ";
			string od;
			getline(cin, od);
			povecajOcenu(prvi, od);
		}
		else if (x == 6)
		{
			urediOdgovore(prvi);
		}
		else if (x == 7)
		{
			cout << endl;
			pisiPitanja(prvi);
			cout << endl;
		}
		else if (x == 8)
		{
			string g;
			getline(cin, g);
			cout << "Unesite odgovor koji zelite da izbrisete: ";
			string od;
			getline(cin, od);
			brisiOdgovor(prvi, od);
		}
		else if (x == 9)
		{
			string g;
			getline(cin, g);
			cout << "Unesite pitanje koje zelite da izbrisete: ";
			string pi;
			getline(cin, pi);
			brisiPitanje(prvi, pi);
		}
		else if (x == 10)
		{
			Elem* s = pitanjeSaNajviseOdgIKom(prvi);
			cout << endl;
			pisiPitanje(prvi, s->tekst);
			cout << endl;
			cout << endl;
		}
	} while (x != 0);

	dealociraj(prvi);


	return 0;
}