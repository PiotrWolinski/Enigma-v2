#define _CRT_SECURE_NO_WARNINGS 

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

struct Rotor {
	short* perm;						//tablica do przechowywania jego permutacji
	short notch_n;						//ilosc liter powodujacych obrot innych wirnikow
	short* notch;						//tablica do przechowywania tych liter
	short* shift;						//tablica zawierajaca przesuniecia permutacji wzgledem indeksu 
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
	scanf("%hd", &t.machine.n);
	scanf("%hd", &t.machine.m);
	t.machine.rotors = (Rotor*)malloc(t.machine.m * sizeof(Rotor));

	short j = 0;
	short x = 0;
	for (int i = 0; i < t.machine.m; i++)
	{
		j = 0;
		t.machine.rotors[i].perm = (short*)malloc(t.machine.n * sizeof(short));
		t.machine.rotors[i].shift = (short*)malloc(t.machine.n * sizeof(short));

		while (j < t.machine.n)
		{
			scanf("%hd ", &t.machine.rotors[i].perm[j]);
			t.machine.rotors[i].shift[t.machine.rotors[i].perm[j] - 1] = j - t.machine.rotors[i].perm[j] + 1;
			j++;
		}
		scanf("%hd", &t.machine.rotors[i].notch_n);
		if (t.machine.rotors[i].notch_n)
		{
			t.machine.rotors[i].notch = (short*)malloc(t.machine.rotors[i].notch_n * sizeof(short));
			x = 0;
			while (x < t.machine.rotors[i].notch_n)
			{
				scanf("%hd", &t.machine.rotors[i].notch[x]);
				x++;
			}
		}
	}

	scanf("%hd", &t.machine.l);
	t.machine.reflectors = (Reflector*)malloc(t.machine.l * sizeof(Reflector));
	for (int i = 0; i < t.machine.l; i++)
	{
		t.machine.reflectors[i].perm = (short*)malloc(t.machine.n * sizeof(short)); // czy to "int" jest potrzebne?
		j = 0;
		while (j < t.machine.n)
		{
			scanf("%hd ", &t.machine.reflectors[i].perm[j]);
			j++;
		}
	}
}


void generate_cipher(Task t)
{
	//tablica tylko do przechowywania obrotow danych rotorow - kolejne indeksy to kolejne rotory w maszynie
	//musi byc takiego rozmiaru, bo inaczej stos wyrzuca blad przy zwalnianiu
	short* turns = (short*)malloc(t.ro_n * sizeof(short));

	short first_moved = 0;
	short second_moved = 0;
	short second_notched = 0;
	short third_notched = 0;

	for (int i = 0; i < t.ro_n; i++)
	{
		turns[i] = t.s_pos[i] - 1;
	}

	for (int i = 0; i < t.msg_size; i++)
	{
		second_notched = 0;
		third_notched = 0;

		//sprawdzam czy pierwszy jest na notchu
		if (t.ro_n > 1 && t.machine.rotors[t.ro_id[0]].notch_n)
		{
			for (int k = 0; k < t.machine.rotors[t.ro_id[0]].notch_n; k++)
			{
				if ((turns[0] + 1) % t.machine.n == t.machine.rotors[t.ro_id[0]].notch[k] - 1)
				{
					second_notched = 1;
					break;
				}
			}
		}

		//sprawdzam czy drugi jest na notchu
		if (t.ro_n > 2 && t.machine.rotors[t.ro_id[1]].notch_n)
		{
			for (int k = 0; k < t.machine.rotors[t.ro_id[1]].notch_n; k++)
			{
				if ((turns[1] + 1) % t.machine.n == t.machine.rotors[t.ro_id[1]].notch[k] - 1)
				{
					third_notched = 1;
					break;
				}
			}
		}

		if (t.ro_n > 2 && third_notched == 1 && second_moved == 1)
		{
			turns[0]++;
			turns[1]++;
			turns[2]++;
		}
		else if (t.ro_n > 1 && second_notched == 1 && first_moved == 1)
		{
			turns[0]++;
			turns[1]++;
			second_moved = 1;
		}
		else
		{
			turns[0]++;
			first_moved = 1;
		}

		for (int j = 0; j < t.ro_n; j++)
		{
			t.msg[i] = t.machine.rotors[t.ro_id[j]].perm[(t.msg[i] - 1 + turns[j] % t.machine.n) % t.machine.n];
			t.msg[i] = (t.msg[i] > turns[j] % t.machine.n) ? t.msg[i] - turns[j] % t.machine.n :
				t.machine.n - turns[j] % t.machine.n + t.msg[i];
		}

		t.msg[i] = t.machine.reflectors[t.re_n].perm[t.msg[i] - 1];
		
		for (int j = t.ro_n - 1; j >= 0; j--)
		{
			t.msg[i] = ((t.msg[i] + turns[j]) % t.machine.n) ? (t.msg[i] + turns[j]) % t.machine.n : t.machine.n;
			t.msg[i] = t.msg[i] + t.machine.rotors[t.ro_id[j]].shift[t.msg[i] - 1];
			t.msg[i] = (t.msg[i] > turns[j] % t.machine.n) ? t.msg[i] - turns[j] % t.machine.n :
				t.machine.n - turns[j] % t.machine.n + t.msg[i];
		}
		printf("%hd ", t.msg[i]);
	}
	printf("\n");
	free(turns);
}

int main()
{ 
	Task task;
	machine_config(task);
	scanf("%hd", &task.p);
	task.msg_size = 2;
	short x = 0;
	for (int i = 0; i < task.p; i++)
	{
		scanf("%hd ", &task.ro_n);
		task.ro_id = (short*)malloc(task.ro_n * sizeof(short));
		task.s_pos = (short*)malloc(task.ro_n * sizeof(short));

		for (int j = 0; j < task.ro_n; j++)
		{
			scanf("%hd ", &task.ro_id[j]);
			scanf("%hd ", &task.s_pos[j]);
		}
		scanf("%hd ", &task.re_n);

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