#pragma warning(disable:4996)
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#define N 500

//void define(int i, int t, char* ptr);
void HUSH(int flag, int i);
void define(int str, int stol);
void rem_lot(int str, char* arr_for, char* nam_fun);
void rem_few(int str, char* nam_fun, int pos_fun, char** per_fun);
int if_(int str, int stol);
int ifdef (int str, int stol, int one);

char*** a = NULL;
int* len_str = NULL;
int qu_str = 0;

int main(int argc, char* argv[]) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	FILE* f;
	FILE* fpout = fopen("D:\\Statistic\\Creator.txt", "wt");
	char filename[100] = { 0 }; //имя файла

	if (argc == 2)
		strcpy(filename, argv[1]);
	while ((f = fopen(filename, "r")) == NULL)
	//Открытие файла для чтения
	{
		printf("Cannot open input file.\n");
		printf("Please enter a file address: ");
		gets_s(filename);
	}

	int str = 0; //строки
	int stol = 0; //столбики
	int sim = 0; //символы массива
	int t = 0;//переход по символам считывающей строки

	len_str = (int*)calloc(N, sizeof(int)); //размерности строк 
	a = (char***)calloc(N,sizeof(char**)); // массив слов строк

	while (!feof(f)) 
	{
		bool flag = 0;
		char line[N];// чтение строки
		char* ptr = fgets(line, N, f);
		a[str] = (char**)calloc(N, sizeof(char*));

		if (ptr == NULL)
			break; 

		while (ptr[t] != '\0')
		{
			a[str][stol] = (char*)calloc(N, sizeof(char));

			while (ptr[t] != ' ' && ptr[t] != '(' && ptr[t] != ')' && ptr[t] != ',' && ptr[t] != ';'
				&& ptr[t] != '+' && ptr[t] != '-' && ptr[t] != '=' && ptr[t] != '*' && ptr[t] != '/'
				&& ptr[t] != '^' && ptr[t] != '%' && ptr[t] != '!' && ptr[t] != '@' && ptr[t] != '"'
				&& ptr[t] != '[' && ptr[t] != ']' && ptr[t] != '\0' && ptr[t] != -1 && ptr[t] != '\n'
				&& ptr[t] != '\t' && ptr[t] != '>' && ptr[t] != '<' && ptr[t] != '#')
			{
				a[str][stol][sim++] = ptr[t++];
				flag = 1;
			}

			if (flag == 1)
			{
				a[str][stol][sim++] = '\0';
				a[str][stol] = (char*)realloc(a[str][stol], sim + 1);
				//printf("%s", a[str][stol]);
			}
			else
			{
				a[str][stol][0] = ptr[t++];
				a[str][stol][1] = '\0';	
				//printf("%s", a[str][stol]);
			}

			stol++;
			sim = 0;
			flag = 0;
		}

		a[str] = (char**)realloc(a[str], stol * sizeof(char*));
		len_str[str] = stol;
		str++;
		stol = 0;
		t = 0;
	}
	a = (char***)realloc(a, str * sizeof(char**));
	len_str = (int*)realloc(len_str, str * sizeof(int));
	qu_str = str;
	fclose(f);
	HUSH(0, 0);

	for (int str = 0; str < qu_str; str++)
	{
		for (int stol = 0; stol < len_str[str]; stol++)
		{
			//if (a[str][0][0] != '.')
				printf("%s", a[str][stol]);
			//else break;
		}
	}
	free(a);
	return 0;
}

void HUSH(int flag, int i) {
	const char* func[] = { "define", "if", "ifdef", "ifndef", "elif", "else", "endif" }; // константы #
	
	bool into = 0;
	bool def = 0;
	/*
	flag = -1 - не выполнился #elif()
	flag = 0 - выполнилось условие
	flag != 0 - не выполнилось равенство условия
	*/
	for(int str = 0; str < qu_str; str++)
	{
		for (int stol = 0; stol < len_str[str];  stol++)
		{
			//printf("%s", a[str][stol]);
			if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ') continue;

			if (a[str][stol][0] == '#')
			{
				//printf("(%s)", a[str][stol]);
				stol++;
				if (into == 0)
				for (int func_elem = 0; func_elem < 7 ; func_elem++)
				{
					if (!(strcmp(a[str][stol], func[func_elem])))
					{
						switch (func_elem) {
							case 1: if (flag == 0) flag = if_(str, stol);
									else  into = 1;
									break;
							case 0: define(str, ++stol); break;
							case 2: if (def == 0)
							{
								if (flag == 0) flag = ifdef(str, stol, 0);
								else  into = 1;
								def = 1;
							}
								  else 
							{
								if (flag != 0 && flag != -1) flag = ifdef(str, stol ,0);
								else flag = -1;
							}
									break; //ifdef
							case 3: if (def == 0)
							{
								if (flag == 0) flag = ifdef(str, stol, 1);
								else  into = 1;
								def = 1;
							}
								  else
							{
								if (flag != 0 && flag != -1) flag = ifdef(str, stol, 1);
								else flag = -1;
							}
								  break; //ifndef
							case 4: if (flag != 0 && flag != -1) flag = if_(str, stol);
									else flag = -1;
									break; //elif(a)
							case 5: if (flag != 0 && flag != -1) flag = 0;
									else if (flag == 0) flag = -1;
									break; //else
							case 6: flag = 0;  break; //endif(a)
							default:  break;
						}
						a[str][0][0] = '.';
						break;
					}
				}
				else if (!(strcmp(a[str][stol], func[6])))
				{
					into = 0;
					def = 0;
					a[str][0][0] = '.';
				}
			}

			if (a[str][stol][0] != '#' && flag != 0 || into != 0)
			{
				a[str][0][0] = '.';
				continue;
			}
		}
	}
}


void define(int str, int stol)
{
	char* arr_for = (char*)calloc(N, sizeof(char)); // массив с формулой макроопределения
	char** per_fun = (char**)calloc(N, sizeof(char*)); //имена переменных функции
	char* nam_fun = (char*)calloc(32, sizeof(char)); // имя функции
	char* buffer = (char*)malloc(32); // слово читается до 32 символов

	int pos_for = 0; //позиции переменных в функцим
	int pos_fun = 0; //для занесения в массив переменных
	int funFlag = 0;
	int flag = -1;

	/*
	flag = -1 начало
	flag = 0 поиск имени функции
	flag = 1 переменные функции
	flag = 2 формула для замены макроопределения
	flag = 3 не функция
	*/

	for (stol; stol < len_str[str]; stol++)
	{
		funFlag = 0;
		if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ' ) continue;
		if (flag != 3 && a[str][stol][0] == ',') continue;
		if (flag == -1) { nam_fun = a[str][stol++]; flag = 0; funFlag = 1;}

		if ((a[str][stol][0] == '(' || a[str][stol][0] == ')' || a[str][stol][0] == '&' ||
			a[str][stol][0] == '|' || a[str][stol][0] == '*' || a[str][stol][0] == '\\' ||
			a[str][stol][0] == '+' || a[str][stol][0] == '-' || a[str][stol][0] == '^') && flag == 2)
			arr_for[pos_for++] = a[str][stol][0];

		else if (flag == 2)
			for (int y = 0; y < pos_fun; y++)
				if (!strcmp(a[str][stol], per_fun[y]))
				{
					arr_for[pos_for++] = *itoa(y, buffer, 10);
					break;
				}

		if (a[str][stol][0] == '(' && flag == 0 && funFlag == 1) flag = 1;
		else if (flag == 0) flag = 3;
		if (a[str][stol][0] == ')' && flag == 1) flag = 2;
		if (flag == 1 || flag == 3) per_fun[pos_fun++] = a[str][stol];
	}
	per_fun = (char**)realloc(per_fun, (pos_fun + 1) * sizeof(char*));
	arr_for = (char*)realloc(arr_for, (pos_for + 1) * sizeof(char));

	if (flag == 2) rem_lot(str + 1, arr_for, nam_fun);
	if (flag == 3) rem_few(str + 1, nam_fun,  pos_fun , per_fun);

	for (int stol = 0; stol < len_str[str]; stol++)
	{
		printf("%s", a[str][stol]);
	}

	free(buffer);
	free(nam_fun);
	free(per_fun);
	free(arr_for);
}

void rem_lot(int str, char* arr_for, char* nam_fun) {
	char** per_fun = (char**)calloc(N, sizeof(char*)); //имена переменных функции
	char** sav_ost = (char**)calloc(N, sizeof(char*)); // сохранение символов после функции
	int str_stol = 0;

	int pos_for = 0; //позиции переменных в функцим
	int pos_fun = 0; //для занесения в массив переменных
	int flag = -1;
	int undef = 0;

	/*
	flag = -1 начало
	flag = 0 найдено имя функции
	flag = 1 переменные функции
	flag = 2 замена на макроопределение
	*/
	for (str; str < qu_str; str++)
	{
		undef = 0;
		for (int stol = 0; stol < len_str[str]; stol++)
		{
			if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ' || a[str][stol][0] == ',') continue;

			if (!(strcmp(a[str][stol], "ifdef")) || !(strcmp(a[str][stol], "ifndef")) || !(strcmp(a[str][stol], "undef")))
			{
				if (!(strcmp(a[str][stol++], "undef"))) undef = 1;
				for (stol; stol < len_str[str]; stol++)
				{
					if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ' || a[str][stol][0] == ',') continue;
					if (undef == 0 && !(strcmp(a[str][stol], nam_fun))) a[str][stol][0] = '#';
					else if (undef == 1 && !(strcmp(a[str][stol], nam_fun))) undef = 2;
					break;
				}
			}
			if (undef == 2) break;

			if (!(strcmp(a[str][stol], nam_fun)) && flag == -1) {
				flag = 0; str_stol = stol;
			}

			if (a[str][stol][0] == ')' && flag == 1) flag = 2;

			if (flag == 1) {per_fun[pos_fun] = (char*)calloc(_msize(a[str][stol]) + 1, 1); strcpy(per_fun[pos_fun++], a[str][stol]); }

			if (a[str][stol][0] == '(' && flag == 0) flag = 1;

			if (flag == 2)
			{	
				int i = 0;
				per_fun = (char**)realloc(per_fun, (pos_fun + 1) * sizeof(char*));
				stol++;
				for (int asd = stol; asd < len_str[str]; asd++, i++) // запись символов после замены
				{
					sav_ost[i] = (char*)calloc(N, sizeof(char));
					strcpy(sav_ost[i],a[str][asd]);
					sav_ost[i] = (char*)realloc(sav_ost[i], len_str[str] - stol);
				}
				sav_ost = (char**)realloc(sav_ost, i * sizeof(char*));
				stol = str_stol;

				a[str] = (char**)realloc(a[str], N * sizeof(char*));
				for (int array = 0; array < _msize(arr_for); array++, stol++) // замена на макроопределение
				{
					a[str][stol] = (char*)calloc(N, sizeof(char));
					if (arr_for[array] >= '0' && arr_for[array] <= '9')// замена слов
					{
						strcpy(a[str][stol], per_fun[arr_for[array] - '0' - 1]);
						a[str][stol] = (char*)realloc(a[str][stol], strlen(per_fun[arr_for[array] - '0' - 1 ]) + 1); //имена переменных функции
					}
					else // замена символов
					{
						a[str][stol][0] = arr_for[array]; a[str][stol][1] = '\0'; a[str][stol] = (char*)realloc(a[str][stol], 2); 
					}
				}
				
				for(int k = 0;  k < i; k++, stol++) { // добавление символов остатка
					a[str][stol] = (char*)calloc(N, sizeof(char));
					strcpy(a[str][stol], sav_ost[k]);
					a[str][stol] = (char*)realloc(a[str][stol], strlen(sav_ost[k]) + 1 );
				}
				
				a[str] = (char**)realloc(a[str], (stol) * sizeof(char*));

				len_str[str] = stol;

				stol = str_stol;
				per_fun = (char**)calloc(N, sizeof(char*)); //имена переменных функции
				sav_ost = (char**)calloc(N, sizeof(char*)); // сохранение символов после функции
				str_stol = 0;

				pos_for = 0; //позиции переменных в функцим
				pos_fun = 0; //для занесения в массив переменных
				flag = -1;
			}
		}
		if (undef == 2) break;
	}
}
void rem_few(int str, char* nam_fun, int pos_fun, char** per_fun) {
	
	int undef = 0;
	int str_stol = 0;
	char** sav_ost = (char**)calloc(N, sizeof(char*)); // сохранение символов после функции
	//printf("-----%s---- ", nam_fun);

	for (str; str < qu_str; str++)
	{
		for (int stol = 0; stol < len_str[str]; stol++)
		{
			str_stol = stol;
			undef = 0;
			if (!(strcmp(a[str][stol], "ifdef"))|| !(strcmp(a[str][stol], "ifndef"))|| !(strcmp(a[str][stol], "undef")))
			{
				if (!(strcmp(a[str][stol++], "undef"))) undef = 1;
				for (stol; stol < len_str[str]; stol++)
				{
					if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ' || a[str][stol][0] == ',') continue;
					if (undef == 0 && !(strcmp(a[str][stol], nam_fun))) a[str][stol][0] = '#';
					else if (undef == 1 && !(strcmp(a[str][stol], nam_fun))) undef = 2;
					break;
				}
			}
			if (!(strcmp(a[str][stol], nam_fun)) && undef == 0)
			{
				int i = 0;
				stol++;
				for (int asd = stol; asd < len_str[str]; asd++, i++) // запись символов после замены
				{
					sav_ost[i] = (char*)calloc(N, sizeof(char));
					strcpy(sav_ost[i], a[str][asd]);
					sav_ost[i] = (char*)realloc(sav_ost[i], strlen(a[str][asd]) + 1);
				}

				stol = str_stol ;

				a[str] = (char**)realloc(a[str], (N) * sizeof(char*));

				for (int st = 0; st < pos_fun -1 ; st++, stol++) { // добавление символов остатка
					a[str][stol] = (char*)calloc(N, sizeof(char));
					strcpy(a[str][stol], per_fun[st]);
					a[str][stol] = (char*)realloc(a[str][stol], strlen(per_fun[st]) + 1);
				}
				stol++;

				for (int k = 0; k < i; k++, stol++) { // добавление символов остатка
					a[str][stol] = (char*)calloc(N, sizeof(char));
					strcpy(a[str][stol], sav_ost[k]);
					a[str][stol] = (char*)realloc(a[str][stol], strlen(sav_ost[k]) + 1);
				}

				a[str] = (char**)realloc(a[str], (stol + 3) * sizeof(char*));

				len_str[str] = stol;

				stol = str_stol;
				sav_ost = (char**)calloc(N, sizeof(char*)); // obnulение символов после функции
			}

		}
		if (undef == 2) break;
	}
	free(sav_ost);
	free(nam_fun);
}

int if_(int str, int stol)
{
	int* arr_for = (int*)calloc(N/10, sizeof(int)); // массив с формулой макроопределения 
	char* znk = (char*)calloc(N/10, sizeof(char)); // массив с формулой макроопределения 

	int f = 0;
	int z = 0;
	int flag = 0;


	for (int stol = 0; stol < len_str[str]; stol++)
	{
		if (a[str][stol][0] >= '0' && a[str][stol][0] <= '9')
		{
			if (f > 2) {flag = -1; break;}
			arr_for[f++] = atoi(a[str][stol]);
		}
		else if (a[str][stol][0] == '-' || a[str][stol][0] == '+' || a[str][stol][0] == '*' || a[str][stol][0] == '/') znk[z++] = a[str][stol][0];

		else if (a[str][stol][0] == '=' || a[str][stol][0] == '>' || a[str][stol][0] == '<' || a[str][stol][0] == '!')
		{
			if (flag == 0)
			switch (znk[--z]) {
			case '*': arr_for[0] *= arr_for[--f]; break;
			case '/': arr_for[0] /= arr_for[--f]; break;
			case '%': arr_for[0] %= arr_for[--f]; break;
			case '+': arr_for[0] += arr_for[--f]; break;
			case '-': arr_for[0] -= arr_for[--f]; break;
			}
			f = 1;
			flag += a[str][stol][0];
		}

		else if (stol + 1 == len_str[str])
		{
			while (f > 2)
			{
				switch (znk[--z]) {
				case '*': arr_for[1] *= arr_for[--f]; break;
				case '/': arr_for[1] /= arr_for[--f]; break;
				case '%': arr_for[1] %= arr_for[--f]; break;
				case '+': arr_for[1] += arr_for[--f]; break;
				case '-': arr_for[1] -= arr_for[--f]; break;
				}
			}

			switch (flag) {
			case 122: if (arr_for[0] == arr_for[1]) flag = 0; break;// ==
			case 62: if (arr_for[0] > arr_for[1])flag = 0; break;// >
			case 60: if (arr_for[0] < arr_for[1]) flag = 0; break; // <
			case 94: if (arr_for[0] != arr_for[1]) flag = 0; break; // !=
			case 123: if (arr_for[0] >= arr_for[1])flag = 0; break;// >=
			case 121: if (arr_for[0] <= arr_for[1]) flag = 0; break; // <=
			}
		}
	}
	free(arr_for);
	free(znk);

	//printf("(%d)",flag);
	return flag;
}

int ifdef(int str, int stol, int one) {
	int flag = -1;

	for (stol = 0; stol < len_str[str]; stol++)
	{
		if (a[str][stol][0] == '\t' || a[str][stol][0] == ' ' || a[str][stol][0] == ',') continue;
		if (a[str][stol][0] == '#')
		{
			flag = 0;
		}
	}

	printf("(%d)", flag);
	if (one == 1)
		if (flag == 0)
			return (-1);
		else
			return (0);
	else
		return (flag);
}