#define _CRT_SECURE_NO_WARNINGS 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

struct Rotor {
	short* perm;						//tablica do przechowywania jego permutacji
	short notch_n;						//ilosc liter powodujacych obrot innych wirnikow
	short* notch;						//tablica do przechowywania tych liter
	short t_n;							//liczba obrotow, ktore wykonal rotor od swojego pierwotnego polozenia
};

struct Reflector {
	short* perm;						//tablica do przechowywania jego permutacji
};

struct Machine {
	short n;							//wielkosc alfabetu
	short m;							//liczba wirnikow
	short l;							//liczba reflektorow
	Rotor* rotors;						//tablica przechowujaca rotory
	Reflector* reflectors;				//tablica przechowujaca reflektory
};

struct Task {
	short p;
	short ro_n;							//liczba obcenie uzywanych rotorow
	short* ro_id;						//tablica indeksow obecnie uzywanych rotorow
	short* s_pos;						//tablica pozycji startowych rotorow
	short re_n;							//indeks obecnie uzywanego reflektora
	short msg_size;						//rozmiar tablicy, ktora zawiera zdanie do zaszyfrowania
	short* msg;							//tablica do przechowywania wiadomosci do zakodowania
	Machine machine;
};


void machine_config(Task& t)
{
	scanf("%hu", &t.machine.n);
	scanf("%hu", &t.machine.m);
	t.machine.rotors = (Rotor*)malloc(t.machine.m * sizeof(Rotor));

	short j = 0;
	short x = 0;
	for (int i = 0; i < t.machine.m; i++)
	{
		j = 0;
		t.machine.rotors[i].t_n = 0;
		t.machine.rotors[i].perm = (short*)malloc(t.machine.n * sizeof(short));

		while (j < t.machine.n)
		{
			scanf("%hu ", &t.machine.rotors[i].perm[j]);
			j++;
		}
		scanf("%hu", &t.machine.rotors[i].notch_n);
		if (t.machine.rotors[i].notch_n)
		{
			t.machine.rotors[i].notch = (short*)malloc(t.machine.rotors[i].notch_n * sizeof(short));
			x = 0;
			while (x < t.machine.rotors[i].notch_n)
			{
				scanf("%hu", &t.machine.rotors[i].notch[x]);
				x++;
			}
		}
	}

	scanf("%hu", &t.machine.l);
	t.machine.reflectors = (Reflector*)malloc(t.machine.l * sizeof(Reflector));
	for (int i = 0; i < t.machine.l; i++)
	{
		t.machine.reflectors[i].perm = (short*)malloc(t.machine.n * sizeof(short)); // czy to "int" jest potrzebne?
		j = 0;
		while (j < t.machine.n)
		{
			scanf("%hu ", &t.machine.reflectors[i].perm[j]);
			j++;
		}
	}
}

void generate_cipher(Task t)
{
	//wyglada podstepnie, ale po prostu dodaje pozycje startowa do liczby obrotow rotora - przygotowuje go do dzialania
	for (int i = 0; i < t.ro_n; i++)
	{
		t.machine.rotors[i].t_n = t.s_pos[i] - 1;
	}

	for (int i = 0; i < t.msg_size; i++)
	{
		for (int j = 0; j < t.ro_n; j++)
		{


			//tutaj ogarne jeszcze raz sprawe z kodowaniem, tylko ze teraz bede bazowal na ilosci obrotow danego rotora


		}

	}
}

int main()
{ 
	Task task;
	machine_config(task);
	scanf("%hu", &task.p);
	task.msg_size = 2;
	short x = 0;
	for (int i = 0; i < task.p; i++)
	{
		scanf("%hu ", &task.ro_n);
		task.ro_id = (short*)malloc(task.ro_n * sizeof(short));
		task.s_pos = (short*)malloc(task.ro_n * sizeof(short));

		for (int j = 0; j < task.ro_n; j++)
		{
			scanf("%hu ", &task.ro_id[j]);
			scanf("%hu ", &task.s_pos[j]);
		}
		scanf("%hu ", &task.re_n);

		task.msg = (short*)malloc(task.msg_size * sizeof(short));

		x = 0;

		while (std::cin >> task.msg[x])
		{
			if (task.msg[x] == 0)
			{
				break;
			}
			else
			{
				x++;
			}
			if (x == task.msg_size)
			{
				task.msg_size++;
				task.msg = (short*)realloc(task.msg, task.msg_size * sizeof(short));
			}
		}
		task.msg_size = x;


		generate_cipher(task);



		free(task.msg);
		free(task.ro_id);
		free(task.s_pos);
		task.msg = NULL;
		task.ro_id = NULL;
		task.s_pos = NULL;
	}
	return 0;
}