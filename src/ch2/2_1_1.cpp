#include <iostream>
#include <thread>

class background_task
{
public:
	void operator()() const
	{
		do_something();
		do_something_else();
	}
	static void do_something()
	{
		std::cout << "Do something....\n";
	}

	static void do_something_else()
	{
		std::cout << "Do something else....\n";
	}
};

struct func
{
	int& i;
	func(int& i_) : i(i_){}
	void operator()()
	{
		for (unsigned j = 0; j < 100000; ++j)
		{
			do_something(i);
		}
	}
	
	void do_something(int i)
	{
	}
	
};

/*поему уупс? Я передаю ссылку на int, а затем отсоединияю поток. My_thread вероятно
еще будет выполняться при выходе из уупс.
 Поток будет обращаться к уже уничтоженной переменной 
и произойдет взлом жопы*/
void oops()
{
	int some_local_state = 0;
	func my_func(some_local_state);
	std::thread my_thread(my_func);
	my_thread.detach();
}

int main()
{
	//background_task f;
	std::thread my_thread(background_task()); //compiler thinks that is function definiot

	std::thread my_thread2((background_task())); //extra brackets - it works
	std::thread my_thread3{ background_task() }; //list initializer - it also works
	my_thread2.join();
	my_thread3.join();
	std::thread my_thread4([] {
		background_task::do_something();
		background_task::do_something_else();
		});
	my_thread4.join();



}