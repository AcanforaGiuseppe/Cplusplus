#include <vector>
#include <memory>
#include <iostream>
#include <thread>
#include <mutex>

thread_local int counter = 0;
std::mutex mutex0;
int totalCounter = 0;

int main(int argc, char** argv)
{
	std::thread thread0([]()
		{
			//int counter = 0;
			for (int i = 0; i < 1000000000; i++)
			{
				//mutex0.lock();
				counter++;
				//mutex0.unlock();
			}
			mutex0.lock();
			totalCounter += counter;
			mutex0.unlock();

			std::cout << std::this_thread::get_id() << " " << counter << std::endl;
		});

	std::thread thread1([]()
		{
			//int counter = 0;
			for (int i = 0; i < 1000000000; i++)
			{
				//mutex0.lock();
				counter++;
				//mutex0.unlock();
			}
			mutex0.lock();
			totalCounter += counter;
			mutex0.unlock();

			std::cout << std::this_thread::get_id() << " " << counter << std::endl;
		});

	std::cout << "Waiting for threads to finish..." << std::endl;

	thread1.detach();
	thread0.join();
	//thread1.join();

	return 0;
}