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
	short t_n;
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

short reverse(short& input, short& n, Rotor& r, short turns)
{
	input = (input + turns) % n;

	for (int i = 0; i < n; i++)
	{
		if (r.perm[i]%n == input)
		{
			return i + 1;
		}
	}
	return 0;
}

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


//w tescie 2 nie dziala dlatego, ze sa dwa takie same rotory i one sie napisuja
void generate_cipher(Task t)
{
	//tablica tylko do przechowywania obrotow danych rotorow - kolejne indeksy to kolejne rotory w maszynie
	short* turns = (short*)malloc(t.ro_n * sizeof(short));

	//wyglada podstepnie, ale po prostu dodaje pozycje startowa do liczby obrotow rotora - przygotowuje go do dzialania
	for (int i = 0; i < t.ro_n; i++)
	{
		turns[i] = t.s_pos[i] - 1;
	}
	for (int i = 0; i < t.msg_size; i++)
	{
		turns[0]++;

		for (int j = 0; j < t.ro_n; j++)
		{
			//jesli dany rotor ma notche
			if (t.machine.rotors[t.ro_id[j]].notch_n && j < t.ro_n - 1)
			{
				for (int k = 0; k < t.machine.rotors[t.ro_id[j]].notch_n; k++)
				{
					if ((t.machine.rotors[t.ro_id[j]].notch[k] - 1 == turns[j])%t.machine.n )		//notche jeszcze nie dzialaja
					{
						turns[j + 1]++;
					}
				}
			}

			t.msg[i] = t.machine.rotors[t.ro_id[j]].perm[(t.msg[i] - 1 + turns[j] % t.machine.n) % t.machine.n];
			t.msg[i] = (t.msg[i] > turns[j] % t.machine.n) ? t.msg[i] - turns[j] % t.machine.n :
				t.machine.n - turns[j] % t.machine.n + t.msg[i];
		}

		t.msg[i] = t.machine.reflectors[t.re_n].perm[t.msg[i] - 1];
		
		for (int j = t.ro_n - 1; j >= 0; j--)
		{
			t.msg[i] = reverse(t.msg[i], t.machine.n, t.machine.rotors[t.ro_id[j]], turns[j]);
			t.msg[i] = (t.msg[i] > turns[j] % t.machine.n) ? t.msg[i] - turns[j] % t.machine.n :
				t.machine.n - turns[j] % t.machine.n + t.msg[i];
		}

		printf("%hu ", t.msg[i]);
	}
	printf("\n");
	free(turns);
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