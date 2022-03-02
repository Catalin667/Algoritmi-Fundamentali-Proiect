/// Lacatus Catalin-Petru, grupa 2341
#include<iostream>
#include<vector>
#include <queue>
#include<stack>
#include <utility>
#include<algorithm>
#include<fstream>
#include<tuple>

using namespace std;
const int oo = numeric_limits<int>::max();
class Graf { ///clasa abstracta graf
protected:
	int Nr_noduri, Nr_muchii;
public:
	Graf(int, int);
	~Graf();
	virtual void citire(istream& in) = 0;
	virtual void afisare(ostream& out) = 0;
};
Graf::Graf(int Nr_noduri = 0, int Nr_muchii = 0) {
	this->Nr_noduri = Nr_noduri;
	this->Nr_muchii = Nr_muchii;
}
Graf::~Graf() {
	this->Nr_noduri = 0;
	this->Nr_muchii = 0;
}
/// ------------------------------------------------------------------------
class Graf_orientat : public Graf ///graf orientat
{
private:
	vector<vector<int>>Lista_de_adiacenta;
	vector<vector<int>> Costuri;
public:
	Graf_orientat(int, int, vector<vector<int>>, vector<vector<int>>, int ok);
	~Graf_orientat();
	void citire(istream& in);
	void afisare(ostream& out);
	friend istream& operator >>(istream&, Graf_orientat&);
	friend ostream& operator <<(ostream&, Graf_orientat&);
	void Adauga_in_lista_de_adiacenta(const int&, const int&, const int&);
	vector<int> BFS(int) const;
    vector<int> Sortare_topologica() const;
	vector<vector<int>> CTC(int&);
	vector<int> Dijkstra_infoarena();
	vector<int> Bellman_ford_infoarena();
	void Roy_Floyd(vector<vector<long long>>&);
	int Flux_maxim();
	void DFS(int, vector<int>&, stack<int>&); /// pentru ctc
private:
	void Transpusa();
	void DFS_transpusa(int, vector<int>&, int, vector<vector<int>>&);
	void Dijkstra(int, vector<int>&);
	void Bellman_ford(int, vector<int>&, bool&);
	int Bfs_flux(const int&, const int&, vector<int>, const vector<vector<int>>&, vector<int>&);
};
void Graf_orientat::Adauga_in_lista_de_adiacenta(const int& nod_st, const int& nod_dr, const int& val_cost_indice) {
	this->Lista_de_adiacenta[nod_st].push_back(nod_dr);
	this->Costuri[nod_st].push_back(val_cost_indice);
}
Graf_orientat::Graf_orientat(int Nr_noduri = 0, int Nr_muchii = 0, vector<vector<int>>Lista_de_adiacenta = {}, vector<vector<int>>Costuri = {}, int ok = 0) :Graf(Nr_noduri, Nr_muchii) {

	try {
		if (Costuri.size() == 0 && Lista_de_adiacenta.size() == 0 && Nr_noduri != 0 && Nr_muchii != 0)
			throw 4;
		if (Costuri.size() == 0 || Nr_muchii == 0)
			throw 1;
		if (ok == 1)
			throw 3;
		this->Lista_de_adiacenta.resize(Nr_noduri + 1);
		this->Costuri.resize(Nr_noduri + 1);
		for (int i = 0; i < Lista_de_adiacenta.size(); i++)
			for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
			{
				this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta[i][j]);
				this->Costuri[i].push_back(Costuri[i][j]);
			}
	}
	catch (int x) {
		if (x == 1)
		{
			this->Lista_de_adiacenta.resize(Nr_noduri + 1);
			this->Costuri.resize(Nr_noduri + 1);
			for (int i = 0; i < Lista_de_adiacenta.size(); i++)
				for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
					this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta[i][j]);
		}
		if (x == 3)
		{
			this->Lista_de_adiacenta.resize(Nr_noduri + 1);
			this->Costuri.resize(Nr_noduri + 1, vector<int>(Nr_noduri, 0));
			for (int i = 1; i < Lista_de_adiacenta.size(); i++)
				for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
				{
					this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta[i][j]);
					this->Costuri[i][Lista_de_adiacenta[i][j] - 1] = Costuri[i][Lista_de_adiacenta[i][j] - 1];
				}
		}
		if (x == 4)
		{
			this->Lista_de_adiacenta.resize(Nr_noduri + 1);
			this->Costuri.resize(Nr_noduri + 1);
		}
	}
}

void Graf_orientat::citire(istream& in) {

	in >> Nr_noduri;
	in >> Nr_muchii;
	this->Lista_de_adiacenta.resize(Nr_noduri + 1);
	int stanga, dreapta;
	for (int i = 1; i <= Nr_muchii; i++)
	{
		in >> stanga;
		in >> dreapta;
		Lista_de_adiacenta[stanga].push_back(dreapta);
	}
}

void Graf_orientat::afisare(ostream& out) {
	out << Nr_noduri << " ";
	out << Nr_muchii << endl;
	for (int i = 1; i < Lista_de_adiacenta.size(); i++) {
		cout << endl;
		for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
			out << i << ":" << Lista_de_adiacenta[i][j] << " ";
	}
}

istream& operator>>(istream& in, Graf_orientat& g)
{
	g.citire(in);
	return in;
}

ostream& operator<<(ostream& out, Graf_orientat& g)
{
	g.afisare(out);
	return out;
}

Graf_orientat::~Graf_orientat() {
	this->Lista_de_adiacenta.clear();
}

/// ------------------------------------------------------------------------
class Graf_neorientat : public Graf   ///graf neorientat
{
private:
	vector<vector<int>>Lista_de_adiacenta;
	vector<vector<int>> Costuri;
public:
	Graf_neorientat(int, int, vector<vector<int>>, vector<vector<int>>);
	~Graf_neorientat();
	int get_nr_noduri();
	void Adauga_in_lista_de_adiacenta(const int&, const int&, const int&);
	void citire(istream& in);
	void afisare(ostream& out);
	friend istream& operator >>(istream&, Graf_neorientat&);
	friend ostream& operator <<(ostream&, Graf_neorientat&);
	void DFS(int, vector<int>&) const; ///dfs
	int Nr_componente_conexe(); ///afla componente biconexe
	vector<int> ANC() const;  ///afla noduri critice
	vector<vector<pair<int, int>>> Componente_biconexe(int&);
	bool Havel_Hakimi_algoritm(vector<int>& Secventa_de_grade);
	vector<pair<int, int>> Afla_muchii_critice_rezolvare();
	vector<string> Pad_de_mult_dis_infoarena(vector<tuple<int, int, int>>);
	int Diametru_arbore();
	vector<int> Arbore_partial_de_cost_minim(int&, int&) const;
	vector<int> Ciclu_eulerian(const int&);
private:
	void Afla_muchii_critice(int, vector<int>&, vector<int>&, vector<int>&, vector<int>&, vector<pair<int, int>>&);
	vector<pair<int, int>> AMC();
	void Afla_noduri_critice(int, vector<int>&, vector<int>&, vector<int>&, vector<int>&, vector<int>&) const;
	void Parcurgere_comp_biconexe(int, vector<int>&, vector<int>&, vector<int>&, vector<int>&, stack<pair<int, int>>&, vector<vector<pair<int, int>>>&, int&);
	int Gaseste_tata_arbore(int, vector<int>&);
	void Reuniune_de_multimi(int, int, vector<int>&);
	void Afla_distante_dfs(int, vector<int>&, vector<int>&);
};

void Graf_neorientat::Adauga_in_lista_de_adiacenta(const int& nod_st, const int& nod_dr, const int& val_cost_indice) {
	this->Lista_de_adiacenta[nod_st].push_back(nod_dr);
	this->Lista_de_adiacenta[nod_dr].push_back(nod_st);
	this->Costuri[nod_st].push_back(val_cost_indice);
	this->Costuri[nod_dr].push_back(val_cost_indice);
}
Graf_neorientat::Graf_neorientat(int Nr_noduri = 0, int Nr_muchii = 0, vector<vector<int>>Lista_de_adiacenta = {}, vector<vector<int>>Costuri = {}) :Graf(Nr_noduri, Nr_muchii) {
	try {
		if (Lista_de_adiacenta.size() == 0 || Costuri.size() == 0)
			throw 1;
		if (Costuri.size() == 0)
			throw 2;

		this->Lista_de_adiacenta.resize(Nr_noduri + 1);
		this->Costuri.resize(Nr_noduri + 1);
		for (int i = 0; i < Lista_de_adiacenta.size(); i++)
			for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
			{
				this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta[i][j]);
				this->Costuri[i].push_back(Costuri[i][j]);
			}

	}
	catch (int x) {
		if (x == 2)
		{
			this->Lista_de_adiacenta.resize(Nr_noduri + 1);
			for (int i = 1; i < Lista_de_adiacenta.size(); i++)
				for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
				{
					this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta[i][j]);
				}
		}
		else if (x == 1 && Nr_noduri != 0 && Nr_muchii != 0)
		{
			this->Lista_de_adiacenta.resize(Nr_noduri + 1);
			this->Costuri.resize(Nr_noduri + 1);
		}

	}

}

int Graf_neorientat::get_nr_noduri() {
	return Nr_noduri;
}

void Graf_neorientat::citire(istream& in) {

	in >> Nr_noduri;
	in >> Nr_muchii;
	this->Lista_de_adiacenta.resize(Nr_noduri + 1);
	this->Costuri.resize(Nr_noduri + 1);
	int stanga, dreapta;
	for (int i = 1; i <= Nr_muchii; i++)
	{
		in >> stanga >> dreapta;
		this->Lista_de_adiacenta[stanga].push_back(dreapta);
		this->Lista_de_adiacenta[dreapta].push_back(stanga);
		this->Costuri[stanga].push_back(i);
		this->Costuri[dreapta].push_back(i);
	}

}

void Graf_neorientat::afisare(ostream& out) {

	out << Nr_noduri << " ";
	out << Nr_muchii << endl;
	for (int i = 1; i < Lista_de_adiacenta.size(); i++) {
		out << i << ":" << endl;
		for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
			out << Lista_de_adiacenta[i][j] << " ";
		out << endl;
	}
}

istream& operator>>(istream& in, Graf_neorientat& g)
{
	g.citire(in);
	return in;
}
ostream& operator<<(ostream& out, Graf_neorientat& g)
{
	g.afisare(out);
	return out;
}

Graf_neorientat::~Graf_neorientat() {
	this->Lista_de_adiacenta.clear();
	this->Costuri.clear();
}

///--------------------------------BFS-------------------------------
vector<int> Graf_orientat::BFS(int Start_nod) const {
	vector<int>Distante(this->Nr_noduri + 1, -1);
	deque<int>coada;
	coada.push_back(Start_nod);
	Distante[Start_nod] = 0;

	while (!coada.empty()) {
		int primul_nod_din_coada = coada.front();
		for (int i = 0; i < Lista_de_adiacenta[primul_nod_din_coada].size(); i++) {
			int nod_curent = Lista_de_adiacenta[primul_nod_din_coada][i];
			if (Distante[nod_curent] == -1) {
				coada.push_back(nod_curent);
				Distante[nod_curent] = Distante[primul_nod_din_coada] + 1;
			}
		}
		coada.pop_front();
	}
	return Distante;
}

///--------------------------------DFS-------------------------------
void Graf_neorientat::DFS(int Nod_start, vector<int>& Vector_vizitati) const {
	Vector_vizitati[Nod_start] = 0; ///Marcam nodul de unde incepe dfs-ul ca vizitat
	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++)///parcurgem vecinii nodului
	{
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == -1)
			DFS(nod_curent, Vector_vizitati);
	}
}

///--------------------------------Nr componente conexe-------------------------------
int Graf_neorientat::Nr_componente_conexe() {

	vector<int>Vector_vizitati(Nr_noduri + 1, -1); ///Marcam toate norudrile nevizitate
											   ///-1 =>nod nevizitat
											   ///0 =>nod vizitat
	int Nr_comp_conexe = 0;
	for (int i = 1; i <= Nr_noduri; i++)
	{
		if (Vector_vizitati[i] == -1) {    ///Daca nodul este nevizitat se aplica DFS
			DFS(i, Vector_vizitati);
			Nr_comp_conexe++;
		}
	}
	return Nr_comp_conexe;
}

///--------------------------------Noduri critice-------------------------------
void Graf_neorientat::Afla_noduri_critice(int Nod_start, vector<int>& Vector_vizitati, vector<int>& Timp_curent, vector<int>& Timp_min, vector<int>& Vector_tati,
	vector<int>& Noduri_critice) const {
	static int timp_curent = 0;
	Vector_vizitati[Nod_start] = 0;
	int copii = 0;
	Timp_curent[Nod_start] = Timp_min[Nod_start] = ++timp_curent;
	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++) {///parcurgem vecinii nodului
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == -1) {
			copii++;
			Vector_tati[nod_curent] = Nod_start;
			Afla_noduri_critice(nod_curent, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Noduri_critice);
			Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_min[nod_curent]);
			if (Vector_tati[Nod_start] == 0 && copii > 1)///daca e radacina si are mai mult de un copil
				Noduri_critice.push_back(Nod_start);
			if (Vector_tati[Nod_start] != 0 && Timp_min[nod_curent] >= Timp_curent[Nod_start])///daca nu e radacina si fiul are gradul de intoarcere >= decat nivelul nodului
				Noduri_critice.push_back(Nod_start);
		}
		else
			if (nod_curent != Vector_tati[Nod_start])///actualizam distantele minime
				Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_curent[nod_curent]);
	}
}

vector<int> Graf_neorientat::ANC() const {
	vector<int>Noduri_critice;
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	vector<int>Timp_curent(Nr_noduri + 1);
	vector<int>Timp_min(Nr_noduri + 1);
	vector<int>Vector_tati(Nr_noduri + 1, 0);

	for (int i = 1; i < Lista_de_adiacenta.size(); i++)
		if (Vector_vizitati[i] == -1)
			Afla_noduri_critice(i, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Noduri_critice);

	return Noduri_critice;
}

///----------------------------------COMPONENTE BICONEXE-----------------------------------------------------------
void Graf_neorientat::Parcurgere_comp_biconexe(int Nod_start, vector<int>& Vector_vizitati, vector<int>& Timp_curent, vector<int>& Timp_min, vector<int>& Vector_tati,
	stack<pair<int, int>>& Stiva, vector<vector<pair<int, int>>>& Componente_biconexe, int& Nr_componente_biconexe) {

	Vector_vizitati[Nod_start] = 1;
	static int timp_curent = 0;
	int copii = 0;
	Timp_curent[Nod_start] = Timp_min[Nod_start] = ++timp_curent;
	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++) {///parcurgem vecinii nodului
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == -1) {
			copii++;
			Vector_tati[nod_curent] = Nod_start;
			pair<int, int>Muchii;
			Muchii.first = Nod_start;
			Muchii.second = nod_curent;
			Stiva.push(Muchii);
			Parcurgere_comp_biconexe(nod_curent, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Stiva, Componente_biconexe, Nr_componente_biconexe);
			Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_min[nod_curent]);
			if ((Timp_curent[Nod_start] == 1 && copii > 1) || (Timp_curent[Nod_start] > 1 && Timp_min[nod_curent] >= Timp_curent[Nod_start])) {  ///daca e nod critic scot din stiva
				while ((Stiva.top().first != Nod_start || Stiva.top().second != nod_curent) && (Stiva.top().first != nod_curent || Stiva.top().second != Nod_start)) {
					pair<int, int>Muchii;
					Muchii.first = Stiva.top().first;
					Muchii.second = Stiva.top().second;
					Componente_biconexe[Nr_componente_biconexe].push_back(Muchii);
					Stiva.pop();
				}

				pair<int, int>Muchii;
				Muchii.first = Stiva.top().first;
				Muchii.second = Stiva.top().second;
				Componente_biconexe[Nr_componente_biconexe].push_back(Muchii);
				Stiva.pop();
				Nr_componente_biconexe++;
			}
		}
		else
			if (nod_curent != Vector_tati[Nod_start]) { ///actualizam distantele minime
				Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_curent[nod_curent]);

				if (Timp_curent[nod_curent] < Timp_curent[Nod_start]) {
					pair<int, int>Muchii;
					Muchii.first = Nod_start;
					Muchii.second = nod_curent;
					Stiva.push(Muchii);

				}
			}
	}
}

vector<vector<pair<int, int>>> Graf_neorientat::Componente_biconexe(int& Nr_comp_bi) {
	int Nr_comp_biconexe = 0;
	vector<vector<pair<int, int>>>Componente_biconexe(Nr_noduri);
	stack <pair<int, int>>Stiva;
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	vector<int>Timp_curent(Nr_noduri + 1, -1);
	vector<int>Timp_min(Nr_noduri + 1, -1);
	vector<int>Vector_tati(Nr_noduri + 1, -1);
	for (int i = 1; i < Lista_de_adiacenta.size(); i++) {
		if (Vector_vizitati[i] == -1)
			Parcurgere_comp_biconexe(i, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Stiva, Componente_biconexe, Nr_comp_biconexe);
		int verifica = 0;
		while (Stiva.size() > 0) {
			verifica = 1;
			pair<int, int>Muchii;
			Muchii.first = Stiva.top().first;
			Muchii.second = Stiva.top().second;
			Componente_biconexe[Nr_comp_biconexe].push_back(Muchii);
			Stiva.pop();
		}

		if (verifica == 1) {
			Nr_comp_biconexe++;
		}
	}
	Nr_comp_bi = Nr_comp_biconexe;
	return Componente_biconexe;
}

///----------------------------------------SORTARE TOPOLOGICA--------------------------------------------
vector<int> Graf_orientat::Sortare_topologica() const {
	vector<int>Rezultat;
	vector<int>Vector_dependente(Nr_noduri + 1, 0);
	queue<int>Coada;
	for (int i = 1; i < Lista_de_adiacenta.size(); i++)
		for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
			Vector_dependente[Lista_de_adiacenta[i][j]]++;
	for (int i = 1; i < Vector_dependente.size(); i++)
		if (Vector_dependente[i] == 0)
			Coada.push(i);

	while (!Coada.empty())
	{
		int nod_curent = Coada.front();
		Rezultat.push_back(nod_curent);
		Coada.pop();
		for (int j = 0; j < Lista_de_adiacenta[nod_curent].size(); j++)
		{
			Vector_dependente[Lista_de_adiacenta[nod_curent][j]]--;
			if (Vector_dependente[Lista_de_adiacenta[nod_curent][j]] == 0)
				Coada.push(Lista_de_adiacenta[nod_curent][j]);
		}
	}
	return Rezultat;
}

///---------------------------------------------- MUCHII CRITICE----------------------------------------------------
void Graf_neorientat::Afla_muchii_critice(int Nod_start, vector<int>& Vector_vizitati, vector<int>& Timp_curent, vector<int>& Timp_min, vector<int>& Vector_tati,
	vector<pair<int, int>>& Muchii_critice) {
	static int timp_curent = 0;
	Vector_vizitati[Nod_start] = 0;
	Timp_curent[Nod_start] = Timp_min[Nod_start] = ++timp_curent;
	for (int nod_curent : Lista_de_adiacenta[Nod_start]) {///parcurgem vecinii nodului
		if (nod_curent == Vector_tati[Nod_start]) continue;
		if (Vector_vizitati[nod_curent] == -1) {
			Vector_tati[nod_curent] = Nod_start;
			Afla_muchii_critice(nod_curent, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Muchii_critice);
			Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_min[nod_curent]);
			if (Timp_min[nod_curent] > Timp_curent[Nod_start])
			{
				pair<int, int>Muchii;
				Muchii.first = Nod_start;
				Muchii.second = nod_curent;
				Muchii_critice.push_back(Muchii);
			}
		}
		else
			///actualizam distantele minime
			Timp_min[Nod_start] = min(Timp_min[Nod_start], Timp_curent[nod_curent]);
	}

}

vector<pair<int, int>> Graf_neorientat::AMC() {
	vector<pair<int, int>>Muchii_critice;
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	vector<int>Timp_curent(Nr_noduri + 1);
	vector<int>Timp_min(Nr_noduri + 1);
	vector<int>Vector_tati(Nr_noduri + 1, -1);

	for (int i = 0; i < Lista_de_adiacenta.size(); i++)
		if (Vector_vizitati[i] == -1)
			Afla_muchii_critice(i, Vector_vizitati, Timp_curent, Timp_min, Vector_tati, Muchii_critice);
	return Muchii_critice;
}
///--------------------------------------Havel Hakimi------------------------------------------------------------
bool Graf_neorientat::Havel_Hakimi_algoritm(vector<int>& Secventa_de_grade) {
	bool val = true;

	while (val == true) {
		sort(Secventa_de_grade.begin(), Secventa_de_grade.end(), greater <>());

		if (Secventa_de_grade[0] == 0)
			return true;

		int Grad_curent = Secventa_de_grade[0];
		Secventa_de_grade.erase(Secventa_de_grade.begin() + 0);

		if (Grad_curent > Secventa_de_grade.size())
			return false;

		for (int i = 0; i < Grad_curent; i++)
		{
			Secventa_de_grade[i]--;
			if (Secventa_de_grade[i] < 0)
				return false;
		}
	}
}

///-------------------------------------------------------- CTC--------------------------------------------------
void Graf_orientat::DFS_transpusa(int Nod_start, vector<int>& Vector_vizitati, int Nr_componente_conexe, vector<vector<int>>& Componente_tare_conexe) {
	Vector_vizitati[Nod_start] = 1; ///Marcam nodul de unde incepe dfs-ul ca vizitat
	Componente_tare_conexe[Nr_componente_conexe].push_back(Nod_start);
	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++)///parcurgem vecinii nodului
	{
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == 0)
			DFS_transpusa(nod_curent, Vector_vizitati, Nr_componente_conexe, Componente_tare_conexe);
	}
}

void Graf_orientat::DFS(int Nod_start, vector<int>& Vector_vizitati, stack<int>& Stiva) {
	Vector_vizitati[Nod_start] = 0; ///Marcam nodul de unde incepe dfs-ul ca vizitat

	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++)///parcurgem vecinii nodului
	{
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == -1)
			DFS(nod_curent, Vector_vizitati, Stiva);
	}
	Stiva.push(Nod_start);

}
void Graf_orientat::Transpusa() {
	vector<vector<int>>Lista_de_adiacenta_transpusa(Nr_noduri + 1);
	for (int i = 1; i < Lista_de_adiacenta.size(); i++)
		for (int j = 0; j < Lista_de_adiacenta[i].size(); j++)
		{
			Lista_de_adiacenta_transpusa[Lista_de_adiacenta[i][j]].push_back(i);
		}
	this->Lista_de_adiacenta.clear();
	Lista_de_adiacenta.resize(Nr_noduri + 1);
	for (int i = 1; i < Lista_de_adiacenta_transpusa.size(); i++)
		for (int j = 0; j < Lista_de_adiacenta_transpusa[i].size(); j++)
			this->Lista_de_adiacenta[i].push_back(Lista_de_adiacenta_transpusa[i][j]);
}

vector<vector<int>> Graf_orientat::CTC(int& Nr_componente_conexe) {
	vector<vector<int>>Componente_tare_conexe(Nr_noduri + 1);
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	stack<int>Stiva;
	for (int i = 1; i < Lista_de_adiacenta.size(); i++)
		if (Vector_vizitati[i] == -1)
			DFS(i, Vector_vizitati, Stiva);
	Transpusa();
	while (!Stiva.empty())
	{
		int nod_curent = Stiva.top();
		Stiva.pop();
		if (Vector_vizitati[nod_curent] == 0)
		{
			Nr_componente_conexe++;
			DFS_transpusa(nod_curent, Vector_vizitati, Nr_componente_conexe, Componente_tare_conexe);
		}
	}

	return Componente_tare_conexe;
}

///----------------------------------------------APM--------------------------------------------------------
vector<int> Graf_neorientat::Arbore_partial_de_cost_minim(int& costuri, int& nr_muchii) const {
	vector<int>Vector_tati(Nr_noduri + 1, -1);
	vector<bool>In_MST(Nr_noduri + 1, false);
	vector<int>Cost_min(Nr_noduri + 1, costuri);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	costuri = 0;
	Cost_min[1] = 0;
	pq.push({ 0,1 });
	while (!pq.empty()) {
		int nod = pq.top().second;
		int cost = pq.top().first;
		pq.pop();

		if (In_MST[nod] == true) {
			continue;
		}
		costuri = costuri + cost;
		nr_muchii++;
		In_MST[nod] = true;
		for (int i = 0; i < Lista_de_adiacenta[nod].size(); i++) {
			int nod_curent = Lista_de_adiacenta[nod][i];
			int cost_curent = Costuri[nod][i];
			if (In_MST[nod_curent] == false && cost_curent < Cost_min[nod_curent]) {
				Cost_min[nod_curent] = cost_curent;
				pq.push({ Cost_min[nod_curent],nod_curent });
				Vector_tati[nod_curent] = nod;
			}
		}
	}
	return Vector_tati;
}

///-------------------------------------------------Paduri de multimi disjuncte----------------------------------------------
int Graf_neorientat::Gaseste_tata_arbore(int element, vector<int>& Vector_tati) {
	int e = element, copie_element;
	///  caut tatal arborelui
	while (e != Vector_tati[e])
		e = Vector_tati[e];
	/// actualizez tatal arborelui in vectorul de tati
	while (element != Vector_tati[element]) {
		copie_element = Vector_tati[element];
		Vector_tati[element] = e;
		element = copie_element;
	}
	return e;
}
void Graf_neorientat::Reuniune_de_multimi(int M1, int M2, vector<int>& Vector_tati) {
	int m1 = Gaseste_tata_arbore(M1, Vector_tati);
	int m2 = Gaseste_tata_arbore(M2, Vector_tati);
	Vector_tati[m2] = m1;
}

vector<string> Graf_neorientat::Pad_de_mult_dis_infoarena(vector<tuple<int, int, int>> Vector_tupluri) {
	vector<int>Vector_tati(Nr_noduri + 1);
	vector<string> Raspunsuri;
	for (int i = 1; i <= Nr_noduri; i++)
		Vector_tati[i] = i;
	for (auto element : Vector_tupluri)
	{
		if (get<0>(element) == 1)
		{

			if (Gaseste_tata_arbore(get<1>(element), Vector_tati) != Gaseste_tata_arbore(get<2>(element), Vector_tati))
				Reuniune_de_multimi(get<1>(element), get<2>(element), Vector_tati);
		}
		else {

			if (Gaseste_tata_arbore(get<1>(element), Vector_tati) == Gaseste_tata_arbore(get<2>(element), Vector_tati))
				Raspunsuri.push_back("DA");
			else
				Raspunsuri.push_back("NU");
		}
	}
	return Raspunsuri;
}
///--------------------------------------------------------Dijkstra--------------------------------------------------------
void Graf_orientat::Dijkstra(int Nod_start, vector<int>& Distante) {
	vector<bool>Vector_vizitati(Nr_noduri + 1, false);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	Distante[Nod_start] = 0;
	pq.push({ 0,Nod_start });
	while (!pq.empty()) {
		int nod_curent = pq.top().second;
		pq.pop();
		if (Vector_vizitati[nod_curent] == false) {
			Vector_vizitati[nod_curent] = true;
			/////  Parcurgem vecinii
			for (int i = 0; i < Lista_de_adiacenta[nod_curent].size(); i++)
			{
				int vecin = Lista_de_adiacenta[nod_curent][i];
				int cost = Costuri[nod_curent][i];
				/////  Actualizez distantele de la nodul de start la nodul curent
				if (Distante[nod_curent] + cost < Distante[vecin]) {
					Distante[vecin] = Distante[nod_curent] + cost;
					pq.push({ Distante[vecin],vecin });
				}
			}
		}
	}
}

vector<int> Graf_orientat::Dijkstra_infoarena() {
	vector<int>Distante(Nr_noduri + 1, oo);
	Dijkstra(1, Distante);
	return Distante;
}

///-------------------------------------------------Bellman_ford-------------------------------------
void Graf_orientat::Bellman_ford(int Nod_start, vector<int>& Distante, bool& ciclu) {
	vector<int>Vector_vizitati(Nr_noduri + 1, 0);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
	Distante[Nod_start] = 0;
	pq.push({ 0,Nod_start });
	while (!pq.empty()) {
		int nod_curent = pq.top().second;
		pq.pop();
		Vector_vizitati[nod_curent]++;
		if (Vector_vizitati[nod_curent] < Nr_noduri - 1) {
			/////  Parcurgem vecinii
			for (int i = 0; i < Lista_de_adiacenta[nod_curent].size(); i++)
			{
				int vecin = Lista_de_adiacenta[nod_curent][i];
				int cost = Costuri[nod_curent][i];
				/////  Actualizez distantele de la nodul de start la nodul curent
				if (Distante[nod_curent] + cost < Distante[vecin]) {
					Distante[vecin] = Distante[nod_curent] + cost;
					pq.push({ Distante[vecin],vecin });
				}
			}
		}
		else {
			ciclu = true;
			while (!pq.empty())
				pq.pop();
		}
	}

}
vector<int> Graf_orientat::Bellman_ford_infoarena() {
	vector<int>Distante(Nr_noduri + 1, oo);
	bool ciclu = false;
	Bellman_ford(1, Distante, ciclu);
	if (ciclu == true)
		return {};
	else
		return Distante;
}
void Graf_neorientat::Afla_distante_dfs(int Nod_start, vector<int>& Vector_vizitati, vector<int>& Distante) {
	Vector_vizitati[Nod_start] = 0; ///Marcam nodul de unde incepe dfs-ul ca vizitat
	for (int i = 0; i < Lista_de_adiacenta[Nod_start].size(); i++)///parcurgem vecinii nodului
	{
		int nod_curent = Lista_de_adiacenta[Nod_start][i];
		if (Vector_vizitati[nod_curent] == -1)
		{
			Distante[nod_curent] = Distante[Nod_start] + 1;
			Afla_distante_dfs(nod_curent, Vector_vizitati, Distante);
		}
	}
}
int Graf_neorientat::Diametru_arbore() {
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	vector<int> Distante(Nr_noduri + 1, 1);
	Afla_distante_dfs(1, Vector_vizitati, Distante);
	int maxim = -1, nod;
	for (int i = 1; i <= Nr_noduri; i++)
		if (Distante[i] > maxim)
		{
			nod = i;
			maxim = Distante[i];
		}
	Vector_vizitati.clear();
	Distante.clear();
	Vector_vizitati.resize(Nr_noduri + 1, -1);
	Distante.resize(Nr_noduri + 1, 1);
	Afla_distante_dfs(nod, Vector_vizitati, Distante);
	return  *max_element(Distante.begin(), Distante.end());
}

void Graf_orientat::Roy_Floyd(vector<vector<long long>>& Matrice) {
	for (int k = 0; k < Nr_noduri; k++)
		for (int i = 0; i < Nr_noduri; i++)
			for (int j = 0; j < Nr_noduri; j++)
				if (Matrice[i][j] > Matrice[i][k] + Matrice[k][j])
					Matrice[i][j] = Matrice[i][k] + Matrice[k][j];
}
int Graf_orientat::Bfs_flux(const int& Start_nod, const int& destinatie, vector<int> Vector_vizitati, const vector<vector<int>>& Vec_fluxuri, vector<int>& Vector_tati) {
	deque<int>coada;
	coada.push_back(Start_nod);
	Vector_vizitati[Start_nod] = 0;
	while (!coada.empty())
	{
		int primul_nod_din_coada = coada.front();
		for (int i = 0; i < Lista_de_adiacenta[primul_nod_din_coada].size(); i++)
		{
			int nod_curent = Lista_de_adiacenta[primul_nod_din_coada][i];
			if (Costuri[primul_nod_din_coada][nod_curent - 1] == Vec_fluxuri[primul_nod_din_coada][nod_curent - 1] || Vector_vizitati[nod_curent] != -1)
				continue;
			coada.push_back(nod_curent);
			Vector_vizitati[nod_curent] = 0;
			Vector_tati[nod_curent] = primul_nod_din_coada;
		}
		coada.pop_front();
	}
	return Vector_vizitati[destinatie];
}
int Graf_orientat::Flux_maxim() {
	vector<int>vec(Nr_noduri + 1, 0);
	vector<int>Vector_vizitati(Nr_noduri + 1, -1);
	vector<vector<int>>Vector_fluxuri(Nr_noduri + 1, vector<int>(Nr_noduri, 0));
	vector<int> Vector_tati(Nr_noduri + 1, -1);
	int rezultat = 0;
	while (Bfs_flux(1, Nr_noduri, Vector_vizitati, Vector_fluxuri, Vector_tati) == 0)
	{
		int f_min = oo;
		for (int nod = Nr_noduri; nod > 1; nod = Vector_tati[nod])
			f_min = min(f_min, Costuri[Vector_tati[nod]][nod - 1] - Vector_fluxuri[Vector_tati[nod]][nod - 1]);
		rezultat += f_min;
		for (int nod = Nr_noduri; nod > 1; nod = Vector_tati[nod])
		{
			Vector_fluxuri[Vector_tati[nod]][nod - 1] += f_min;

		}
		Vector_tati.resize(Nr_noduri + 1, -1);
	}
	return  rezultat;
}

vector<int> Graf_neorientat::Ciclu_eulerian(const int& Start) {
	vector<int>Rezultat;
	for (int i = 1; i <= Nr_noduri; i++)
		if (Lista_de_adiacenta[i].size() % 2 != 0)
		{
			Rezultat.push_back(-1);
			return Rezultat;
		}
	vector<bool>Vector_vizitati_muchii(Nr_muchii + 1, false);

	stack<int>Stiva;
	Stiva.push(Start);
	while (!Stiva.empty())
	{
		int nod_curent = Stiva.top();
		if (Lista_de_adiacenta[nod_curent].size())
		{
			int nod = Lista_de_adiacenta[nod_curent].back();
			int muchie = Costuri[nod_curent].back();
			Lista_de_adiacenta[nod_curent].pop_back();
			Costuri[nod_curent].pop_back();
			if (Vector_vizitati_muchii[muchie] == false)
			{
				Vector_vizitati_muchii[muchie] = true;
				Stiva.push(nod);
			}
		}
		else
		{
			Stiva.pop();
			Rezultat.push_back(nod_curent);
		}
	}
	return Rezultat;
}
/// --------------------------Probleme infoarena-----------------------------------------

void BFS_infoarena_rezolvare() {///BFS - PROBLEMA INFOARENA
	ifstream f_in("bfs.in");
	ofstream f_out("bfs.out");

	int Nr_noduri, Nr_muchii, Start_nod, Nod_st, Nod_dr;
	f_in >> Nr_noduri;
	f_in >> Nr_muchii;
	f_in >> Start_nod;
	vector<vector<int>>Lista_de_adiacenta(Nr_noduri + 1);

	for (int i = 0; i < Nr_muchii; i++)
	{
		f_in >> Nod_st >> Nod_dr;
		Lista_de_adiacenta[Nod_st].push_back(Nod_dr);
	}

	Graf_orientat graf(Nr_noduri, Nr_muchii, Lista_de_adiacenta);

	const vector<int> Distante = graf.BFS(Start_nod);
	for (int i = 1; i < Distante.size(); i++)
		f_out << Distante[i] << " ";
	f_in.close();
	f_out.close();
	Lista_de_adiacenta.clear();
}


void DFS_infoarena_rezolvare() { ///DFS INFOARENA
	ifstream f_in("dfs.in");
	ofstream f_out("dfs.out");
	Graf_neorientat graf;
	f_in >> graf;
	f_out << graf.Nr_componente_conexe();
	f_in.close();
	f_out.close();
}

void Sortare_vector_perechi(vector<vector<pair<int, int>>>& Comp_biconexe, int Numar_comp_biconexe) {
	for (int i = 0; i < Numar_comp_biconexe; i++) ///Sortam perechile de muchii
	{
		sort(Comp_biconexe[i].begin(), Comp_biconexe[i].end());
	}
}

void Componente_biconexe_infoarena_rezolvare() {
	ifstream f_in("biconex.in");
	ofstream f_out("biconex.out");
	int Numar_comp_biconexe = 0;
	Graf_neorientat graf;
	f_in >> graf;
	vector<vector<pair<int, int>>>Componente_biconexe = graf.Componente_biconexe(Numar_comp_biconexe);
	f_out << Numar_comp_biconexe << '\n';
	Sortare_vector_perechi(Componente_biconexe, Numar_comp_biconexe);///sortam perechile de muchii
	for (int i = 0; i < Numar_comp_biconexe; i++)
	{
		vector<bool>Verifica(graf.get_nr_noduri() + 1, false);
		for (int j = 0; j < Componente_biconexe[i].size(); j++) {
			if (Verifica[Componente_biconexe[i][j].first] == false)
			{
				f_out << Componente_biconexe[i][j].first << " ";
				Verifica[Componente_biconexe[i][j].first] = true;
			}
			if (Verifica[Componente_biconexe[i][j].second] == false)
			{
				f_out << Componente_biconexe[i][j].second << " ";
				Verifica[Componente_biconexe[i][j].second] = true;
			}
		}
		f_out << '\n';
	}
	f_in.close();
	f_out.close();
}


void Sortare_topologica_infoarena_rezolvare() {
	ifstream f_in("sortaret.in");
	ofstream f_out("sortaret.out");
	Graf_orientat g;
	f_in >> g;
	const vector<int> Rezultat = g.Sortare_topologica();
	for (int i = 0; i < Rezultat.size(); i++)
		f_out << Rezultat[i] << " ";
	f_in.close();
	f_out.close();
}
vector<pair<int, int>> Graf_neorientat::Afla_muchii_critice_rezolvare() {
	return AMC();
}

void Afla_muchii_critice_rezolvare_problema() {
	int Nr_noduri, Nr_muchii, Start_nod, Nod_st, Nod_dr;
	cin >> Nr_noduri;
	cin >> Nr_muchii;
	vector<pair<int, int>>Vector_perechi_muchii(Nr_noduri + 1);

	for (int i = 0; i < Nr_muchii; i++)
	{
		cin >> Nod_st;
		cin >> Nod_dr;
		pair<int, int>Muchii;
		Muchii.first = Nod_st;
		Muchii.second = Nod_dr;
		Vector_perechi_muchii.push_back(Muchii);
	}
	vector<vector<int>>Lista_de_adiacenta(Nr_noduri + 1);
	for (int i = 0; i < Vector_perechi_muchii.size(); i++)
	{
		Lista_de_adiacenta[Vector_perechi_muchii[i].first].push_back(Vector_perechi_muchii[i].second);
		Lista_de_adiacenta[Vector_perechi_muchii[i].second].push_back(Vector_perechi_muchii[i].first);
	}
	Graf_neorientat graf(Nr_noduri, Nr_muchii, Lista_de_adiacenta);
	vector<pair<int, int>>Muchii_critice = graf.Afla_muchii_critice_rezolvare();
	vector<vector<int>>Rezultat;
	for (int i = 0; i < Muchii_critice.size(); i++) {
		vector<int>Vector;
		Vector.push_back(Muchii_critice[i].first);
		Vector.push_back(Muchii_critice[i].second);
		Rezultat.push_back(Vector);
	}
	if (Rezultat.size() != 0)
		cout << "[";
	for (int i = 0; i < Muchii_critice.size(); i++)
	{
		cout << "[" << Muchii_critice[i].first << "," << Muchii_critice[i].second << "]";
		if (i == Muchii_critice.size() - 1)
			cout << "]";
		else
			cout << ",";
	}
}

void Havel_Hakimi_rezolvare() {
	int Nr_noduri;
	cout << "Introduceti numarul de noduri: ";
	cin >> Nr_noduri;
	Graf_neorientat g(Nr_noduri);
	vector<int>Secventa_de_grade;
	int nr;
	for (int i = 0; i < Nr_noduri; i++)
	{
		cin >> nr;
		Secventa_de_grade.push_back(nr);
	}
	if (g.Havel_Hakimi_algoritm(Secventa_de_grade) == true)
		cout << "Da, este posibil.";
	else
		cout << "Nu, nu este posibil.";

}

void Sorteaza_muchii(vector<vector<int>>& Componente_tare_conexe) {
	for (int i = 1; i < Componente_tare_conexe.size(); i++)
		sort(Componente_tare_conexe[i].begin(), Componente_tare_conexe[i].end());
}

void Componente_tare_conexe_infoarena_rezolvare() {
	ifstream f_in("ctc.in");
	ofstream f_out("ctc.out");
	Graf_orientat g;
	f_in >> g;
	int Nr_componente_tare_conexe = 0;
	vector<vector<int>>Componente_tare_conexe = g.CTC(Nr_componente_tare_conexe);
	f_out << Nr_componente_tare_conexe;
	Sorteaza_muchii(Componente_tare_conexe);
	for (int i = 1; i < Componente_tare_conexe.size(); i++)
	{
		f_out << '\n';
		for (int j = 0; j < Componente_tare_conexe[i].size(); j++)
			f_out << Componente_tare_conexe[i][j] << " ";
	}
	f_in.close();
	f_out.close();
}

void APM_rezolvare() {
	ifstream f_in("apm.in");
	ofstream f_out("apm.out");
	int Nr_noduri, Nr_muchii, nod_st, nod_dr, cost;
	f_in >> Nr_noduri;
	f_in >> Nr_muchii;
	vector<vector<int>>Lista_de_adiacenta(Nr_noduri + 1);
	vector<vector<int>>Costuri(Nr_noduri + 1);
	for (int i = 1; i <= Nr_muchii; i++) {
		f_in >> nod_st >> nod_dr >> cost;
		Lista_de_adiacenta[nod_st].push_back(nod_dr);
		Lista_de_adiacenta[nod_dr].push_back(nod_st);
		Costuri[nod_st].push_back(cost);
		Costuri[nod_dr].push_back(cost);
	}
	int nr_muchii_APM = 0, costuri = oo;
	Graf_neorientat g(Nr_noduri, Nr_muchii, Lista_de_adiacenta, Costuri);
	const vector<int>rez = g.Arbore_partial_de_cost_minim(costuri, nr_muchii_APM);
	f_out << costuri << endl << nr_muchii_APM - 1 << endl;
	for (int i = 2; i <= Nr_noduri; i++)
	{
		f_out << rez[i] << " " << i << endl;
	}
}

void Padure_de_multimi_disjuncte() {
	ifstream f_in("disjoint.in");
	ofstream f_out("disjoint.out");
	int Nr_noduri, Nr_muchii, op, x, y;
	f_in >> Nr_noduri >> Nr_muchii;
	vector<tuple <int, int, int>> Vector_tupluri;
	for (int i = 1; i <= Nr_muchii; i++)
	{
		f_in >> op >> x >> y;
		Vector_tupluri.push_back(make_tuple(op, x, y));
	}
	Graf_neorientat graf(Nr_noduri, Nr_muchii);
	vector<string>Raspunsuri = graf.Pad_de_mult_dis_infoarena(Vector_tupluri);
	for (auto raspuns : Raspunsuri)
		f_out << raspuns << '\n';

	f_in.close();
	f_out.close();
}
void Dijkstra_Infoarena_rezolvare() {
	ifstream f_in("dijkstra.in");
	ofstream f_out("dijkstra.out");
	int Nr_noduri, Nr_muchii, nod_st, nod_dr, cost;
	f_in >> Nr_noduri >> Nr_muchii;
	Graf_orientat graf(Nr_noduri, Nr_muchii, {}, {});
	for (int i = 1; i <= Nr_muchii; i++) {
		f_in >> nod_st >> nod_dr >> cost;
		graf.Adauga_in_lista_de_adiacenta(nod_st, nod_dr, cost);
	}
	vector<int>Distante = graf.Dijkstra_infoarena();
	for (int i = 2; i < Distante.size(); i++)
		if (Distante[i] == oo)
			f_out << 0 << " ";
		else
			f_out << Distante[i] << " ";
	f_in.close();
	f_out.close();
}

void Bellman_ford_infoarena_rezolvare() {
	ifstream f_in("bellmanford.in");
	ofstream f_out("bellmanford.out");
	int Nr_noduri, Nr_muchii, nod_st, nod_dr, cost;
	f_in >> Nr_noduri;
	f_in >> Nr_muchii;
	vector<vector<int>>Lista_de_adiacenta(Nr_noduri + 1);
	vector<vector<int>>Costuri(Nr_noduri + 1);
	for (int i = 1; i <= Nr_muchii; i++) {
		f_in >> nod_st >> nod_dr >> cost;
		Lista_de_adiacenta[nod_st].push_back(nod_dr);
		Costuri[nod_st].push_back(cost);
	}
	Graf_orientat g(Nr_noduri, Nr_muchii, Lista_de_adiacenta, Costuri);
	vector<int>Distante = g.Bellman_ford_infoarena();
	if (Distante.size() == 0)
		f_out << "Ciclu negativ";
	for (int i = 2; i < Distante.size(); i++)
		f_out << Distante[i] << " ";
	f_in.close();
	f_out.close();
}
void Diametru_arbore_infoarena()
{
	ifstream f_in("darb.in");
	ofstream f_out("darb.out");
	int Nr_noduri;
	f_in >> Nr_noduri;
	vector<vector<int>> Lista_de_adiacenta(Nr_noduri + 1);
	for (int i = 1; i <= Nr_noduri - 1; i++)
	{
		int nod_st, nod_dr;
		f_in >> nod_st;
		f_in >> nod_dr;
		Lista_de_adiacenta[nod_st].push_back(nod_dr);
		Lista_de_adiacenta[nod_dr].push_back(nod_st);
	}
	int Nr_muchii = Nr_noduri - 1;
	Graf_neorientat g(Nr_noduri, Nr_muchii, Lista_de_adiacenta);
	f_out << g.Diametru_arbore();
	f_in.close();
	f_out.close();
}
void Roy_Floyd_infoarena() {
	int Nr_noduri, val;
	cin >> Nr_noduri;
	vector<vector<long long>>Matrice(Nr_noduri);
	for (int i = 0; i < Nr_noduri; i++)
		for (int j = 0; j < Nr_noduri; j++)
		{
			cin >> val;
			if (val == 0 && i != j)
				Matrice[i].push_back(oo);
			else
				Matrice[i].push_back(val);
		}
	Graf_orientat g(Nr_noduri);
	g.Roy_Floyd(Matrice);
	cout << '\n';
	for (int i = 0; i < Nr_noduri; i++)
	{
		for (int j = 0; j < Nr_noduri; j++)
			if (Matrice[i][j] == oo)
				cout << 0 << " ";
			else
				cout << Matrice[i][j] << " ";
		cout << '\n';
	}
}
void Flux_maxim_infoarena() {
	ifstream f_in("maxflow.in");
	ofstream f_out("maxflow.out");
	int Nr_noduri, Nr_muchii, nod_st, nod_dr, cost;
	f_in >> Nr_noduri;
	f_in >> Nr_muchii;
	vector<vector<int>>Lista_de_adiacenta(Nr_noduri + 1);
	vector<vector<int>>Costuri(Nr_noduri + 1, vector<int>(Nr_noduri, 0));
	for (int i = 1; i <= Nr_muchii; i++) {
		f_in >> nod_st >> nod_dr >> cost;
		Lista_de_adiacenta[nod_st].push_back(nod_dr);
		Costuri[nod_st][nod_dr - 1] = cost;
	}
	Graf_orientat g(Nr_noduri, Nr_muchii, Lista_de_adiacenta, Costuri, 1);
	f_out << g.Flux_maxim();
}

void Ciclu_Eulerian_infoarena()
{
	ifstream f_in("ciclueuler.in");
	ofstream f_out("ciclueuler.out");
	Graf_neorientat g;
	f_in >> g;
	vector<int>Rezultat = g.Ciclu_eulerian(1);
	for (int i = 0; i < Rezultat.size(); i++)
		f_out << Rezultat[i] << " ";

	f_in.close();
	f_out.close();
}
int main()
{
	///BFS_infoarena_rezolvare();
	///DFS_infoarena_rezolvare();
	///Componente_biconexe_infoarena_rezolvare();
	///Sortare_topologica_infoarena_rezolvare();
	///Afla_muchii_critice_rezolvare_problema();
	///Havel_Hakimi_rezolvare();
	///Componente_tare_conexe_infoarena_rezolvare();
	///APM_rezolvare();
	///Padure_de_multimi_disjuncte();
	///Dijkstra_Infoarena_rezolvare();
	///Bellman_ford_infoarena_rezolvare();
	///Diametru_arbore_infoarena();
	///Roy_Floyd_infoarena();
	///Flux_maxim_infoarena();
	///Ciclu_Eulerian_infoarena();

	return 0;
}
