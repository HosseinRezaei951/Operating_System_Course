#include <iostream>
#include <conio.h>
#include <vector>
#include <time.h>
#include <thread>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <future>
using namespace std;

#define Number_Of_Floors 9
#define Elevator_Start_Location 0
#define Max_Capatcity 500
#define Max_Work 100

#define Min_People_Weight 40
#define Max_People_Weight 110

#define Max_Request_In_EVEN_Floors 20
#define Max_Request_In_ODD_Floors 13

mutex mx;
condition_variable cv;

struct People
{
	unsigned int Weight = 0;
	unsigned int Source = 0;
	unsigned int Destination = 0;
};


bool _Finish_Generat_People = false;

vector<vector<People>> Floors_Requests;

class Elevator
{
private:
	int Location; 
	int Direction; // 0: stop , 1: going up , 2: going down
	int Capacity;
	int Maximum_Work_Can_Do;
	vector<People> People_Inside;
	vector<unsigned int> OutSide_Requests;
	vector<unsigned int> InSide_Requests;


public:
	Elevator() {};
	~Elevator() {};
	Elevator(int _Location,int _Maximum_work);

	
	void Insert_New_Request(int _Request_Number, vector<unsigned int> &_Requests);
	void Erase_Old_Request(int _Request_Number, vector<unsigned int> &_Requests);
	void Start();
	void Move();

	int Which_Direction(vector<unsigned int> _Requests);
	int Which_Location(vector<unsigned int> _Requests);

	void Print_Floors_Requests();
	void Print_Moves();
	void Check_Floors_Requests();
	void People_GetIn();
	void People_GetOut();

	
};



Elevator::Elevator(int _Location, int _Maximum_work)
{
	Location = _Location;
	Capacity = 0;
	Maximum_Work_Can_Do = _Maximum_work;
	Direction = 0;
	thread _Start(&Elevator::Start, this);
	_Start.join();
	
	
}

void Elevator::Insert_New_Request(int _Request_Number, vector<unsigned int> &_Requests)
{
	for (int i = 0; i < _Requests.size(); i++)
	{
		if (_Requests[i] == _Request_Number)  
		{
			return;
		}
	}
	_Requests.push_back(_Request_Number);
	return;
}

void Elevator::Erase_Old_Request(int _Request_Number, vector<unsigned int> &_Requests)
{
	for (int i = 0; i < _Requests.size(); i++)
	{
		if (_Requests[i] == _Request_Number)  
		{
			_Requests.erase(_Requests.begin() + i);
			return;
		}
	}
}

void Elevator::Check_Floors_Requests()
{
	for (int i = 0; i < Floors_Requests.size(); i++)
	{
		if (Floors_Requests[i].empty() == false)
		{
			Insert_New_Request(i, OutSide_Requests);
		}
	}
	return;
}

void Elevator::Print_Floors_Requests()
{
	bool flag = false;
	printf(" \n\n ======================================================================================================== \n");
	printf(" *** Floors_Requests *** \n");
	for (int i = 0; i < Floors_Requests.size(); i++)
	{
		flag = false;
		if (Floors_Requests[i].empty() != true)
		{
			flag = true;
			printf(" \nFloor %d : \n", i);
			for (int j = 0; j < Floors_Requests[i].size(); j++)
			{
				printf(" W = %d , D = %d |", Floors_Requests[i][j].Weight, Floors_Requests[i][j].Destination);

			}
		}
		if (flag == true)
		{
			printf("\n");
		}
	}
	printf(" \n ======================================================================================================== \n");

}

void Elevator::Print_Moves()
{
	
	printf(" \n\n ======================================================================================================== \n");
	printf("  *** Elevator *** \n");
	printf(" \n Location : %d\n",Location);
	printf(" \n Direction : %d\n", Direction);
	printf(" \n Capacity : %d\n", Capacity);
	printf(" \n Maximum_Work_Can_Do : %d\n", Maximum_Work_Can_Do);
	printf(" \n                                  >>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<                                    \n");
	printf(" \n People_Inside : \n");
	if (People_Inside.empty() != true)
	{
		for (int i = 0; i < People_Inside.size(); i++)
		{
			printf(" W = %d , S = %d , D = %d |", People_Inside[i].Weight, People_Inside[i].Source, People_Inside[i].Destination);
		}
			
	}
	printf("\n");
	printf(" \n InSide_Requests : \n");
	if (InSide_Requests.empty() != true)
	{
		for (int i = 0; i < InSide_Requests.size(); i++)
		{
			printf(" %d |", InSide_Requests[i]);
		}
	}
	printf("\n");
	printf(" \n OutSide_Requests : \n");
	if (OutSide_Requests.empty() != true)
	{
		for (int i = 0; i < OutSide_Requests.size(); i++)
		{
			printf(" %d |", OutSide_Requests[i]);
		}
	}
	printf(" \n ======================================================================================================== \n");
	
	//_getch();
}

void Elevator::Start()
{
	int Des_Location = Location;
	while (Maximum_Work_Can_Do != 0)
	{
		
		if (InSide_Requests.empty() == true)
		{

			Check_Floors_Requests();
			
			while (InSide_Requests.empty() == true)
			{
				Print_Floors_Requests();
				Direction = Which_Direction(OutSide_Requests);
				Des_Location = Which_Location(OutSide_Requests);
				if (Direction == 0)
				{
					People_GetIn();
					break;
				}
				else
				{
					while (Des_Location != Location)
					{
						Move();
						People_GetIn();
						Print_Moves();
					}
					break;
				}
				/*else
				{
					Move();
					People_GetIn();
					Print_Moves();
				}*/


			}


		}
		else if (InSide_Requests.empty() != true)
		{
			Print_Floors_Requests();
			Direction = Which_Direction(InSide_Requests);
			Des_Location = Which_Location(InSide_Requests);
			if (Direction == 0)
			{
				People_GetOut();
				People_GetIn();
				break;
			}
			else
			{
				while (Des_Location != Location)
				{
					Move();
					People_GetOut();
					People_GetIn();
					Print_Moves();
				}
				
			}
		}

	}
	{
		lock_guard<mutex> lk(mx);
		_Finish_Generat_People = true;
	}
	cv.notify_all();
	return;
}

void Elevator::Move()
{
	if (Direction == 0) //Stoping Here
	{
		return ;
	}
	else if (Direction == 1 &&  ((Location + 1) <= (Number_Of_Floors - 1))) //Moving UP
	{
		Location = Location + 1;
		return ;
	}
	else if (Direction == 2 && ((Location - 1) >= 0)) //Moving Down
	{
		Location = Location - 1;
		return ;
	}
}

int Elevator::Which_Direction(vector<unsigned int> _Requests)
{
	if (_Requests.empty() != true)
	{
		if (_Requests.front() == Location) //We have some requests at this Floor and we should stop
		{
			return 0;
		}
		else if (_Requests.front() > Location) //We have some requests at the Up Floor and we should go Up
		{
			return 1;
		}
		else if (_Requests.front() < Location) //We have some requests at the Down Floor and we should go Down
		{
			return 2;
		}
	}
	else
	{
		return 0; //We don't have any request and we should stop
	}
}

int Elevator::Which_Location(vector<unsigned int> _Requests)
{
	if (_Requests.empty() != true)
	{
		return _Requests.front();
	}
	else
	{
		return Location; //We don't have any request and we should stop
	}
}
void Elevator::People_GetIn()
{
	if (Floors_Requests[Location].empty() != true)
	{
		for (int i = 0; i <Floors_Requests[Location].size(); i++)
		{
			if (Floors_Requests[Location][i].Weight + Capacity < Max_Capatcity)
			{
				Capacity = Floors_Requests[Location][i].Weight + Capacity;
				People_Inside.push_back(Floors_Requests[Location][i]);
				Insert_New_Request(Floors_Requests[Location][i].Destination, InSide_Requests);

				Floors_Requests[Location].erase(Floors_Requests[Location].begin() + i);
				Erase_Old_Request(Location, OutSide_Requests);
			}
		}
	}
}

void Elevator::People_GetOut()
{
	if (People_Inside.empty() != true)
	{
		for (int i = 0; i < People_Inside.size(); i++)
		{
			if (People_Inside[i].Destination == Location)
			{
				Capacity = Capacity - People_Inside[i].Weight;
				Maximum_Work_Can_Do--;
				Erase_Old_Request(People_Inside[i].Destination, InSide_Requests);
				People_Inside.erase(People_Inside.begin() + i);
			}
		}
	}
}



// ************************************************************************************************************

class Building
{
private:
	Elevator _Elevator;

public:
	Building();
	~Building();

	void People_Generator();

};


Building::Building()
{
	vector<vector<People>> _Floors_Requests(Number_Of_Floors);
	Floors_Requests = _Floors_Requests;

	
	thread _People(&Building::People_Generator, this);
	_Elevator = Elevator::Elevator(Elevator_Start_Location, Max_Work);


	_People.join();
	printf("\n ====> the END!");
}

Building::~Building()
{

}

void Building::People_Generator()
{
	unsigned int People_Weight;
	unsigned int People_Source_Floor;
	unsigned int People_Destination_Floor;
	People tmp_People;

	srand((unsigned int)time(0));
	while (_Finish_Generat_People != true)
	{
		
		this_thread::sleep_for(chrono::seconds(1));
		{
			
			
			unsigned int How_Many_People = (rand() % (3 - 1 + 1)) + 1;

			while (How_Many_People > 0)
			{
				People_Weight = (rand() % ((Max_People_Weight - Min_People_Weight) + 1)) + Min_People_Weight;
				People_Source_Floor = (rand() % ((Number_Of_Floors-1) - 0 + 1)) + 0;
				People_Destination_Floor = (rand() % ((Number_Of_Floors-1) - 0 + 1)) + 0;
				while (People_Destination_Floor == People_Source_Floor)
				{
					People_Destination_Floor = (rand() % ((Number_Of_Floors-1) - 0 + 1)) + 0;
				}

				if (People_Source_Floor % 2 == 0 && Floors_Requests[People_Source_Floor].size() < Max_Request_In_EVEN_Floors)
				{
					tmp_People.Weight = People_Weight;
					tmp_People.Source = People_Source_Floor;
					tmp_People.Destination = People_Destination_Floor;
					Floors_Requests[People_Source_Floor].push_back(tmp_People);
					How_Many_People--;
				}
				else if (People_Source_Floor % 2 != 0 && Floors_Requests[People_Source_Floor].size() < Max_Request_In_ODD_Floors)
				{
					tmp_People.Weight = People_Weight;
					tmp_People.Source = People_Source_Floor;
					tmp_People.Destination = People_Destination_Floor;
					Floors_Requests[People_Source_Floor].push_back(tmp_People);
					How_Many_People--;
				}
			}	

		}
		
	}
}

int main()
{	
	_getch();
	Building IT;
	
	_getch();
	return 0;
}