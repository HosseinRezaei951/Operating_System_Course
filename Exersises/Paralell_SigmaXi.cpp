#include <iostream>
#include <conio.h>
#include <ctime>
#include <omp.h>
using namespace std;


void Parallel(int i,int X)
{
	unsigned long long int Sum = 0;
	clock_t begin = clock();
#pragma omp parallel for reduction(+:Sum)
	for (int j = 0; j <= i; j++)
	{
		//printf("%d \n", i);

		Sum += ((unsigned long long int)(pow(X, j)));

	}
	clock_t end = clock();
	double duration = (end - begin) / double(CLOCKS_PER_SEC) * 1000;
	cout << "\n \n \n";
	cout << "Sum is: " << Sum << endl;
	cout << "duration: " << duration <<" ms"<< endl;
	
}

void NOT_Parallel(int i, int X)
{
	unsigned long long int Sum = 0;
	clock_t begin = clock(); 
	for (int j = 0; j <= i; j++)
	{
		Sum = Sum + (unsigned long long int)(pow(X, j));

	}
	clock_t end = clock();
	double duration = (end - begin) / double(CLOCKS_PER_SEC) * 1000;
	cout << "\n \n \n";
	cout << "Sum is: " << Sum << endl;
	cout << "duration: " << duration << " ms" << endl;
	
}

int main()
{
	int fun = 0;
	int X = 0, i = 0;
	while (true)
	{
		
		cout << "1. Parallel and 2.Not Parallel ..... 0. Exit" << endl;
		cout << " plz enter your dunction: ";
		cin >> fun;
		
		if (fun == 1)
		{
			cout << "Plz enter X: ";
			cin >> X;
			cout << "Plz enter i: ";
			cin >> i;
			Parallel(i, X);

			_getch();
			system("cls");
		}
		else if (fun == 2)
		{
			cout << "Plz enter X: ";
			cin >> X;
			cout << "Plz enter i: ";
			cin >> i;
			NOT_Parallel(i, X);

			_getch();
			system("cls");
		}
		else if (fun == 0)
		{
			system("cls");
			break;
		}
		else
		{
			cout << "wrong !!! press enter ... " << endl;
			_getch();
			system("cls");
		}
	}
	return 0;
}