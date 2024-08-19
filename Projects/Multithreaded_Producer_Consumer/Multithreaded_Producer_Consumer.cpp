#include <thread>
#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable> 
#include <conio.h>
using namespace std;

mutex mx;
condition_variable cv;
deque<int> buffer[5];

bool _consumer = false;
bool _producer = false;
bool _report = false;

int producing_list[5] = { 0 };
int consuming_list[5] = { 0 };
bool producing_finished[5] = { false };
bool consuming_finished[5] = { false };

void Thread_Master()
{
	int _count = 0;
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
					cout << " theared " << i << " is producing " << producing_list[i] << endl;


				}
				else if (consuming_list[i] != 0)
				{
					cout << " theared " << i << " is consuming " << consuming_list[i] << endl;


				}

			}

			for (int i = 1; i < 5; i++)
			{

				producing_list[i] = 0;
				consuming_list[i] = 0;
			}



			{
				for (int i = 1; i < 5; i++)
				{
					if (consuming_finished[i] == true)
					{
						_count++;
						cout << " ====> theared " << i << " ended." << endl;
						consuming_finished[i] = false;
						if (_count == 4)
						{
							return;
						}
					}

				}
			}


			_report = false;
		}
		cv.notify_all();
	}
}



void producer(int n, int number) {
	for (int i = 1; i <= n; ++i)
	{
		{
			lock_guard<mutex> lk(mx);
			buffer[number].push_back(i);
			producing_list[number] = i;

			_consumer = true;
			_report = true;
			_producer = false;

		}
		cv.notify_all();

		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [] { return (_producer && !_report); });
		}

	}
	{
		lock_guard<mutex> lk(mx);
		producing_finished[number] = true;
	}
	cv.notify_all();
}

void consumer(int number) {
	while (true)
	{
		{
			unique_lock<mutex> lk(mx);
			cv.wait(lk, [&] { return producing_finished[number] || (_consumer && !_report) || (producing_finished[number] && !_report); });
		}

		if (!buffer[number].empty())
		{
			{
				lock_guard<mutex> lk(mx);
				int item = buffer[number].front();
				buffer[number].pop_front();
				consuming_list[number] = item;

				_producer = true;
				_report = true;
				_consumer = false;
			}
			cv.notify_all();

		}
		else if (producing_finished[number] && buffer[number].empty())
		{
			{
				lock_guard<mutex> lk(mx);
				_report = true;
				consuming_finished[number] = true;
			}
			cv.notify_all();
			break;
		}
	}
}


void Thread_Luncher(int Thread_Number, int Number_Of_Data)
{
	thread producer_1(producer, Number_Of_Data, Thread_Number);
	thread consumer_1(consumer, Thread_Number);

	producer_1.join();
	consumer_1.join();

}

int main() {
	thread Thread_Luncher_1(Thread_Luncher, 1, 10);
	thread Thread_Luncher_2(Thread_Luncher, 2, 10);
	thread Thread_Luncher_3(Thread_Luncher, 3, 10);
	thread Thread_Luncher_4(Thread_Luncher, 4, 10);
	thread Thread_Master_1(Thread_Master);

	Thread_Luncher_1.join();
	Thread_Luncher_2.join();
	Thread_Luncher_3.join();
	Thread_Luncher_4.join();
	Thread_Master_1.join();

	cout << "finished!" << endl;
	_getch();
	return 0;
}