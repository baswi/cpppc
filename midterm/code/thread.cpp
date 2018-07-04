#include <iostream>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <queue>
using namespace std;

int main() {
	int c = 0;
	bool done = false;
	queue<int> goods;
	std::mutex m;
	std::condition_variable cv;

	thread producer([&]() {
		for (int i = 0; i < 500; ++i) {
			std::unique_lock<std::mutex> lk(m);
			goods.push(i);
			c++;
			lk.unlock();
		}
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk);
		done = true;
	});

	thread consumer([&]() {
		while (!done) {
			while (!goods.empty()) {
				std::lock_guard<std::mutex> lk(m);
				goods.pop();
				c--;
			}
			cv.notify_all();
		}
	});

	producer.join();
	consumer.join();
	cout << "Net: " << c << endl;




	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	return 0;
}

