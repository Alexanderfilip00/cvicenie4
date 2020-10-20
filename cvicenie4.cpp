#include <iostream> //vlozenie hlavickového súboru
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

class Customer {
private:
	string name, surname;
	float budget;
	int* cart;
public:
	Customer();
	Customer(string I_name, string I_surname, float I_budget, int n );
	void SetCart(int id, int n);
	void SetBudget(float n) { budget += n; };

	string Name() { return name; };
	string Surname() { return surname; };
	float Budget() { return budget; };
	int Cart(int I_id) { return cart[I_id]; };	
};

class Product {
private:
	int id;
	string name;
	string producer;
	int stock;
	float price;
public:
	Product();
	Product(int I_id, string I_name, string I_producer, int I_stock, float I_price);
	void Set(int I_id, string I_name, string I_producer, int I_stock, float I_price);
	void SetId(int n) { id = n; };
	void SetStock(int n);
	void PInfo();		//info o produkte

	int Id() { return id; };
	string Name() { return name; };
	string Producer() { return producer; };
	int Stock() { return stock; };
	float Price() { return price; };
};

class Eshop {
private:
	Product* List;
public:
	Eshop(string name);
	bool ReadFile(string name);
	Product* ByName(string Pname, Customer* osoba);
	Product* ByProducer(string producer, Customer* osoba);
	bool Exit(string output, float zaplatit, Customer* osoba);
	void Info(int id) { List[id].PInfo(); };
	int Choice(Customer* osoba, int vysledky[]);
	Product* GiveProduct(int id) { return &List[id]; };		
	void Intro();										//zaciatok nakupu
	void Shopping(Customer* osoba);						//hlavne menu obchodu
	void Inventory(Product* P, Customer* osoba);		//proc. na upravenie skladovych zasob

};



Eshop::Eshop(string name)
{
	fstream zoznam(name, ios::in);
	int i;
	short int pocet;
	int id, stock;
	float price;
	string I_name, I_producer;

	if (!zoznam.is_open()) {		//test existencie suboru
		cout << "Subor sa neda otvorit. \n";
		exit(1);
	}

	zoznam >> pocet;

	List = new Product[pocet + 1];	//dynamicka alokacia pola produktov   ....vytvoreny jeden sample produkt, kde si ulozim pocet produktov		
	for (i = 1; i <= pocet; i++) {
		zoznam >> id;
		zoznam >> I_name;
		zoznam >> I_producer;
		zoznam >> stock;
		zoznam >> price;

		List[i].Set(id, I_name, I_producer, stock, price);
	}

	zoznam.close();
	List[0].SetId(pocet);		//pocitadlo mnozstva produktov
}

bool Eshop::ReadFile(string name)
{
	fstream zoznam(name, ios::in);
	int i;
	short int pocet;
	int id, stock;
	float price;
	string I_name, I_producer;

	if (!zoznam.is_open()) {		//test existencie suboru
		cout << "Subor sa neda otvorit. \n";
		return false;
	}

	zoznam >> pocet;

	List = new Product[pocet + 1];	//dynamicka alokacia pola produktov   ....vytvoreny jeden sample produkt, kde si ulozim pocet produktov		
	for (i = 1; i <= pocet; i++) {
		zoznam >> id;
		zoznam >> I_name;
		zoznam >> I_producer;
		zoznam >> stock;
		zoznam >> price;

		List[i].Set(id, I_name, I_producer, stock, price);
	}

	zoznam.close();
	List[0].SetId(pocet);		//pocitadlo mnozstva produktov
	return true;
}

Product* Eshop::ByName(string Pname, Customer* osoba)
{
	int i, n;
	int* vysledky = new int[List[0].Id() + 1];
	vysledky[0] = 0;			//deklaracia pocitadla spravnych vysledkov


	cout << "\nid" << string(10, ' ') << "nazov" << string(5, ' ') << "vyrobca" << string(5, ' ') << "zasoba" << string(5, ' ') << "cena\n";

	for (i = 1; i <= List[0].Id(); i++) {


		if (List[i].Name().find(Pname,0) != string::npos) {							//porovnanie hladaneho a vysledkov
			List[i].PInfo();
			vysledky[0]++;										//pocet spravnych vysledkov sa zvacsi o 1
			vysledky[vysledky[0]] = i;							//na dane miesto sa zapise id najdeneho produktu

		}
	}

	if (vysledky[0] == 0) {
		cout << "Hladany produkt nebol najdeny. Navrat do hlavnej ponuky. \n";
		Shopping(osoba);
		delete[] vysledky;
	}

	n = Choice(osoba, vysledky);
	return &List[n];
}

Product* Eshop::ByProducer(string producer, Customer* osoba)
{
	int i, n;
	int* vysledky = new int[List[0].Id() + 1];
	vysledky[0] = 0;			//deklaracia pocitadla spravnych vysledkov


	cout << "\nid" << string(10, ' ') << "nazov" << string(5, ' ') << "vyrobca" << string(5, ' ') << "zasoba" << string(5, ' ') << "cena\n";

	for (i = 1; i <= List[0].Id(); i++) {


		if (List[i].Producer().find(producer,0) == 0) {							//porovnanie hladaneho a vysledkov
			List[i].PInfo();
			vysledky[0]++;										//pocet spravnych vysledkov sa zvacsi o 1
			vysledky[vysledky[0]] = i;							//na dane miesto sa zapise id najdeneho produktu

		}
	}

	if (vysledky[0] == 0) {
		cout << "Hladany vyrobca nebol najdeny. Navrat do hlavnej ponuky. \n";
		delete[] vysledky;
		Shopping(osoba);

	}

	n = Choice(osoba, vysledky);
	return &List[n];
}

int Eshop::Choice(Customer* osoba, int vysledky[])
{
	int id, i;


	while (true) {
		cout << "Zadajte id ziadaneho tovaru alebo 0 pre hlavnu ponuku: ";
		cin >> id;

		if (id == 0) {
			delete[] vysledky;
			Shopping(osoba);
		}
		else {
			for (i = 1; i <= vysledky[0]; i++) {		//prehladava id najdenych produktov
				if (id == vysledky[i]) {
					delete[] vysledky;
					return id;
				}
			}
		}
	}
}

void Eshop::Inventory(Product* P, Customer* osoba)
{
	int kusy = -1;
	if (P->Stock() == 0) {				//kontrola, ci nie je vypredane
		cout << "Tovar je vypredany. Navrat do hlavnej ponuky. \n";

		Shopping(osoba);
	}

	do {		//opyta pocet kusov na kupenie a kontroluje zle hodnoty
		cout << "Zadajte ziadany pocet kusov. Pre navrat do hlavnej ponuky napiste 0: ";
		cin >> kusy;

		if (kusy == 0) {
			Shopping(osoba);
		}

		else if (P->Stock() < kusy) {	//kontrola, ci je na sklade dost kusov
			kusy = -1;
			cout << "Nedostatok produktov na sklade, zostalo uz len " << P->Stock() << " ks.\n";
		}

		else if ((P->Price() * kusy) > osoba->Budget()) {			//kontrola rozpoctu
			cout << "Nedostatocny rozpocet, zostava vam uz len " << osoba->Budget() << ". Pre navrat do hlavnej ponuky napiste 0:\n";
			kusy = -1;
		}

	} while (kusy < 0);

	osoba->SetBudget(-(P->Price() * kusy));
	P->SetStock(-kusy);
	osoba->SetCart(0, 1);		//ukazujem, ze kosik nie je prazdny
	osoba->SetCart(P->Id(), kusy);
	cout << "Uspesne kupenych " << kusy << "ks produktu " << P->Name() << " od vyrobcu " << P->Producer() << ".\n";

	Shopping(osoba);
}

void Eshop::Intro()
{
	int pyt;		//premenna na kontrolu zberu informacii
	string meno, priezvisko;
	float rozpocet = 0;

	cout << "Vitajte v obchode.\nTento program o vas zbiera informacie, prajete si pokracovat?\n 1 = ano \t 0 = nie \n";
	cin >> pyt;
	if (pyt == 0) {
		cout << "KONIEC\n";
		exit(1);
	}
	else if (pyt == 1) {
	}
	else {
		cout << "Nevyjadrenie suhlasu povazujeme za nesuhlas.\n";
		exit(1);
	}

	cout << "Zadajte vase krstne meno: \n";
	cin >> meno;

	cout << "Zadajte vase priezvisko: \n";
	cin >> priezvisko;

	cout << "Zadajte vas rozpocet: " << endl;

	do {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin >> rozpocet;
		if (rozpocet <= 0) {
			cout << "Neplatny vstup. Zadajte kladny rozpocet. \n";
		}

	} while (rozpocet <= 0);

	Customer* osoba = new Customer(meno, priezvisko, rozpocet, List[0].Id());

	Shopping(osoba);
}
	
void Eshop::Shopping(Customer* osoba)
{
	int i;
	int volba = 0;
	string input;

	cout << "\n################################################################ \n";
	cout << "Vazeny/a " << osoba->Name() << " " << osoba->Surname() << ", vitajte v obchode. Vas zvysny rozpocet je: " << osoba->Budget() << endl << "Ponuka sluzieb: \n";
	cout << "  1. Vyhladavanie produktov podla nazvu \n  2. Vyhladavanie produktov podla vyrobcu \n  3. Ukoncit nakup a opustit obchod \n";

	if (osoba->Budget() == 0) {				//kontrola vycerpania rozpoctu
		volba = 3;
		cout << "Rozpocet bol vycerpany. Ukoncenie nakupu.\n";
	}

	while (volba < 1 || volba > 3) {		//kontrola vstupu
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Zadajte vasu volbu: ";
		cin >> volba;
	}

	if (volba == 1) {
		cout << "Zadajte nazov hladaneho produktu: \n";
		cin >> input;
		Inventory(ByName(input, osoba), osoba);
	}
	else if (volba == 2) {
		cout << "Zadajte nazov hladaneho vyrobcu: \n";
		cin >> input;
		Inventory(ByProducer(input, osoba), osoba);
	}
	else if (volba == 3) {
		float zaplatit = 0;
		for (i = 1; i <= List[0].Id(); i++) {
			zaplatit += (osoba->Cart(i) * List[i].Price());
		}

		if (!Exit("blocek.txt", zaplatit, osoba)) {
			cout << "Chyba ukladania suboru. Rusim nakup.\n";
			exit(1);
		}
		else {
			cout << "Dakujeme za nakup. Nakupili ste produkty v hodnote " << zaplatit << "\n";
			exit(0);
		}
	}
}

bool Eshop::Exit(string output, float zaplatit, Customer* osoba)
{

	int i;
	fstream blok(output, ios::out);

	if (!blok.is_open()) {		//kontrola vytvorenia suboru
		cout << "Chyba ukladania blocka. \n";
		return false;
	}

	blok << "meno: " << osoba->Name() << " \npriezvisko: " << osoba->Surname() << " \n\n";
	blok << "Nakupene produkty: \nid" << string(10, ' ') << "nazov" << string(5, ' ') << "vyrobca" << string(5, ' ') << "zasoba" << string(5, ' ') << "cena\n";


	for (i = 1; i <= List[0].Id(); i++) {
		if (osoba->Cart(i) != 0) {
			blok << setw(2) << List[i].Id() << setw(15) << List[i].Name() << setw(12) << List[i].Producer() << setw(11) << osoba->Cart(i) << setw(9) << List[i].Price() << endl;
		}
	}

	blok << "\ncena nakupu: " << fixed << setprecision(2) << zaplatit << "\ncena bez DPH: " << fixed << setprecision(2) << zaplatit * 0.8 << endl;
	blok.close();
	return true;
}


Product::Product()
{
	id = 0;
	name = "";
	producer = "";
	stock = 0;
	price = 0.0;
}

Product::Product(int I_id, string I_name, string I_producer, int I_stock, float I_price)
{
	id = I_id;
	name = I_name;
	producer = I_producer;
	stock = I_stock;
	price = I_price;
}

void Product::Set(int I_id, string I_name, string I_producer, int I_stock, float I_price)
{
	id = I_id;
	name = I_name;
	producer = I_producer;
	stock = I_stock;
	price = I_price;
}

void Product::PInfo()
{
	cout << setw(2) << id << setw(15) << name << setw(12) << producer << setw(11) << stock << setw(9) << price << endl;
}

void Product::SetStock(int n)
{
	stock += n;
}


void Customer::SetCart(int id, int n)
{
	cart[id] += n;
}

Customer::Customer()
{
	name = "";
	surname = "";
	budget = 0;
	cart = new int[1];
	cart[0] = 0;
}

Customer::Customer(string I_name, string I_surname, float I_budget, int n)
{
	int i;
	name = I_name;
	surname = I_surname;
	budget = I_budget;
	cart = new int[n + 1];
	for (i = 0; i < n + 1; i++) {
		cart[i] = 0;		//nastavi prazdny kosik
	}
}


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//

int main() {
	Eshop Obchodik("produkty.txt");
	Obchodik.Intro();

	return 0;
}