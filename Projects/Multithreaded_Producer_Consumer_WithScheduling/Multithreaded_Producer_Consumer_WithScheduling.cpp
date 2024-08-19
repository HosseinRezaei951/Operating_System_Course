#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable> 
#include <conio.h>
#include <ctime>
using namespace std;

mutex mx;
condition_variable cv;
deque<int> buffer[5];
queue <pair<int, int>> Requests_Que; /* first is ThreadNumber & second is BurstTime */


bool _consumer[5] = { false };
bool _producer[5] = { false };
bool _report = false;

int producing_list[5] = { 0 };
int consuming_list[5] = { 0 };
bool producing_finished[5] = { false };
bool consuming_finished[5] = { false };
bool requests[5] = { false };

bool finished = false;

void SJF(void)
{
	while (true)
	{
		if (finished == true)
			return;
		if (!Requests_Que.empty())
		{
			{
				lock_guard<mutex> lk(mx);
				int Threadnumber;

				pair<int, int> _tmppair;
				pair<int, int> _Goalpair;

				int BurstTime;
				int _minimum = 999;
				queue <pair<int, int>> _tmpque = Requests_Que;
				while (!_tmpque.empty())
				{
					_tmppair = _tmpque.front();
					BurstTime = _tmppair.second;
					if (BurstTime < _minimum)
					{
						_Goalpair = _tmppair;
					}
					_tmpque.pop();
				}
				Threadnumber = _Goalpair.first;
				queue <pair<int, int>> _tmpque2;

				while (!Requests_Que.empty())
				{
					_tmppair = Requests_Que.front();

					if (_tmppair == _Goalpair)
					{
						Requests_Que.pop();
					}
					else
					{
						_tmpque2.push(_tmppair);
						Requests_Que.pop();
					}
				}
				Requests_Que = _tmpque2;

				//Threadnumber request was accepted.
				requests[Threadnumber] = true;
			}
			cv.notify_all();
		}
	}
}

void RR(void)
{

	while (true)
	{
		if (finished == true)
			return;
		if (!Requests_Que.empty())
		{
			{
				lock_guard<mutex> lk(mx);
				int Threadnumber;
				Threadnumber = Requests_Que.front().first;
				Requests_Que.pop();
				//Threadnumber request was accepted.
				requests[Threadnumber] = true;
			}
			cv.notify_all();
			//let 4 sec be the QT
			{
				std::this_thread::sleep_for(std::chrono::seconds(4s));
			}
		}

	}
}

void Thread_Master()
{
	int _count = 0;
	int Count_P = 0;
	int Count_C = 0;
	int p[5] = { 0 };
	int c[5] = { 0 };
	auto start = std::chrono::system_clock::now();
	while (true)
	{

		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [] {return _report; });
		}

		{
			lock_guard<mutex> lk(mx);
			for (int i = 1; i < 5; i++)
			{
				if (producing_list[i] != 0)
				{
					printf(" theared %d is producing %d \n", i, producing_list[i]);
					Count_P++;
					p[i]++;
				}
				else if (consuming_list[i] != 0)
				{
					printf(" theared %d is consuming %d \n", i, consuming_list[i]);
					Count_C++;
					c[i]++;
				}
			}
			{
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> diff = end - start;
				printf("\n");
				printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
				printf("* Time lefted is : %f s\n", diff);
				printf("*	thread 1: %d data produced   & %d data consumed \n", p[1], c[1]);
				printf("*	thread 2: %d data produced   & %d data consumed \n", p[2], c[2]);
				printf("*	thread 3: %d data produced   & %d data consumed \n", p[3], c[3]);
				printf("*	thread 4: %d data produced   & %d data consumed \n", p[4], c[4]);
				printf("*		 %d Total data produced \n", Count_P);
				printf("*		 %d Total data consumed \n", Count_C);
				printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
				printf("\n");
			}

			for (int i = 1; i < 5; i++)
			{

				producing_list[i] = 0;
				consuming_list[i] = 0;
			}


			for (int i = 1; i < 5; i++)
			{
				if (consuming_finished[i] == true)
				{
					_count++;
					printf(" ************************************************************* theared %d ended. *\n", i);
					consuming_finished[i] = false;
					if (_count == 4)
					{
						finished = true;
						return;
					}
				}

			}



			_report = false;
		}
		cv.notify_all();
	}
}



void producer(int n, int number, int ArrivalTime, int BurstTime) {
	for (int i = 1; i <= n; ++i)
		/*int i = 1;
		while(true)*/
	{


		using namespace std::chrono_literals;
		auto start = chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(std::chrono::seconds(ArrivalTime));
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> elapsed = end - start;
		printf("==============> theared  %d : woke up after %f ms \n", number, elapsed.count());

		Requests_Que.push(make_pair(number, BurstTime));
		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [&] { return (requests[number]); });
		}
		{

			lock_guard<mutex> lk(mx);
			printf("==============> theared  %d : accepted \n", number);

			using namespace std::chrono_literals;
			auto start2 = chrono::high_resolution_clock::now();
			std::this_thread::sleep_for(std::chrono::seconds(BurstTime));
			auto end2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsed2 = end2 - start2;
			printf("==============> theared  %d : is working for %f ms \n", number, elapsed2.count());
			buffer[number].push_back(i);
			producing_list[number] = i;

			_report = true;
			_consumer[number] = true;
			_producer[number] = false;
			requests[number] = false;

		}
		cv.notify_all();
		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [&] { return (_producer[number] && !_report); });
		}

	}
	{
		lock_guard<mutex> lk(mx);
		_report = true;
		_consumer[number] = true;
		_producer[number] = false;
		producing_finished[number] = true;
	}
	cv.notify_all();
}

void consumer(int number, float Goal)
{
	srand((unsigned int)time(0));
	while (true)
	{
		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [&] { return (producing_finished[number] && !_report && !_producer[number]) || (_consumer[number] && !_report); });
		}

		//finished consuming
		if (producing_finished[number] && buffer[number].empty())
		{
			{
				lock_guard<mutex> lk(mx);
				_report = true;
				consuming_finished[number] = true;
			}
			cv.notify_all();
			break;
		}

		else if (!producing_finished[number] && !buffer[number].empty())
		{
			float random_number = (float)0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((float)0.2 - (float)0)));
			printf("==============> theared  %d : random_number is = %f \n", number, random_number);
			if (random_number < Goal)
			{
				{
					lock_guard<mutex> lk(mx);
					int item = buffer[number].front();
					consuming_list[number] = item;
					buffer[number].pop_front();

					printf("==============> theared  %d : OK! picked up! \n", number);
					_report = true;
					_producer[number] = true;
					_consumer[number] = false;
				}
				cv.notify_all();
			}
			else
			{
				printf("==============> theared  %d : can't pick up OoPs!!! \n", number);
				{
					lock_guard<mutex> lk(mx);
					_report = true;
					_producer[number] = true;
					_consumer[number] = false;
				}
				cv.notify_all();
			}

		}

		else if (producing_finished[number] && !buffer[number].empty())
		{
			float random_number = (float)0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((float)0.2 - (float)0)));
			printf("==============> theared  %d : random_number is = %f \n", number, random_number);
			if (random_number < Goal)
			{
				{
					lock_guard<mutex> lk(mx);
					int item = buffer[number].front();
					buffer[number].pop_front();
					consuming_list[number] = item;
					printf("==============> theared  %d : OK! picked up! \n", number);
					_report = true;
					_producer[number] = false;
					_consumer[number] = false;
				}
				cv.notify_all();
			}
			{
				printf("==============> theared  %d : can't pick up OoPs!!! \n", number);
				{
					lock_guard<mutex> lk(mx);
					_report = true;
					_producer[number] = false;
					_consumer[number] = false;
				}
				cv.notify_all();
			}

		}
		else if (!producing_finished[number] && buffer[number].empty())
		{
			{
				printf("==============> theared  %d : Buffer is emty!!! \n", number);

				{
					lock_guard<mutex> lk(mx);
					_report = true;
					_producer[number] = true;
					_consumer[number] = false;
				}
				cv.notify_all();
			}
		}
		else
		{
			printf("==============> theared  %d : OoPs!!! ............  BUUUUUUUUUG !!!\n", number);
			_getch();
		}

	}
}


void Thread_Luncher(int n, int Thread_Number, int ArrivalTime, int BurstTime, float Tread_Goal)
{
	thread producer_1(producer, n, Thread_Number, ArrivalTime, BurstTime);
	thread consumer_1(consumer, Thread_Number, Tread_Goal);

	producer_1.join();
	consumer_1.join();


}


int main()
{
	char i[10];
	bool exit = false;

	cout << "which scheduling do you want? SJF(1) OR RR(2) : ";
	cin >> i;
	while (exit != true)
	{
		if (strcmp("1", i) == 0)
		{
			cout << "started!" << endl;
			float Goal1 = (float)1 / (float)8;
			float Goal2 = (float)1 / (float)12;
			float Goal3 = (float)1 / (float)10;
			float Goal4 = (float)1 / (float)6;
			int n = 10;
			thread Thread_Luncher_1(Thread_Luncher, n, 1, 5, 2, Goal1);
			thread Thread_Luncher_2(Thread_Luncher, n, 2, 3, 4, Goal2);
			thread Thread_Luncher_3(Thread_Luncher, n, 3, 7, 1, Goal3);
			thread Thread_Luncher_4(Thread_Luncher, n, 4, 2, 3, Goal4);
			thread Thread_Master_1(Thread_Master);
			thread SJF_1(SJF);

			Thread_Luncher_1.join();
			Thread_Luncher_2.join();
			Thread_Luncher_3.join();
			Thread_Luncher_4.join();
			Thread_Master_1.join();
			SJF_1.join();

			cout << "finished!" << endl;
			_getch();
			break;
		}
		else if (strcmp("2", i) == 0)
		{
			cout << "started!" << endl;
			float Goal1 = (float)1 / (float)8;
			float Goal2 = (float)1 / (float)12;
			float Goal3 = (float)1 / (float)10;
			float Goal4 = (float)1 / (float)6;
			int n = 10;
			thread Thread_Luncher_1(Thread_Luncher, n, 1, 5, 2, Goal1);
			thread Thread_Luncher_2(Thread_Luncher, n, 2, 3, 4, Goal2);
			thread Thread_Luncher_3(Thread_Luncher, n, 3, 7, 1, Goal3);
			thread Thread_Luncher_4(Thread_Luncher, n, 4, 2, 3, Goal4);
			thread Thread_Master_1(Thread_Master);
			thread RR_1(RR);

			Thread_Luncher_1.join();
			Thread_Luncher_2.join();
			Thread_Luncher_3.join();
			Thread_Luncher_4.join();
			Thread_Master_1.join();
			RR_1.join();

			cout << "finished!" << endl;
			_getch();
			break;
		}
		else
		{
			cout << "\n Wrong !!! ... try again." << endl;
			cout << "which scheduling do you want? SJF(1) OR RR(2) : ";
			cin >> i;
		}
	}

	return 0;
}