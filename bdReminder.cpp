// Не вижу смысла бить код на функции.
// 

#include "bdReminder.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>

int main()
{	// Поехали! Ох уж эти даты. Для арифметических операций один тип, для вывода в консоль другой.
	// Нам надо хранить имя друга и его дату рождения, но потом сравнивать дни рождения с текущей
	// датой поэлементно - долго. Будем хранить не дни рождения, а разницу между днём рождения и 
	// текущей датой. А map эту информацию ещё и автоматически отсортирует. Но у нас дни рождений
	// могут совпадать, значит multimap.
	std::tm birthday = {};
	std::tm* temp_time;
	std::time_t curTime, t_time;
	std::string name, temp = "";
	double delta;
	std::multimap<double, std::string> mBirthdays;

	curTime = std::time(nullptr); // Сохраняем текущее время
	while(temp != "END")
	{
		std::cout << "Enter NAME your friend: ";
		std::cin >> name;
		temp = name;
		// Традиционно введённую строку в верхний регистр
		std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
		if (temp != "END")
		{
			std::cout << "Enter his(her) birthday (dd.mm.yyyy): ";
			std::cin >> std::get_time(&birthday, "%d.%m.%Y");
			// Переводим в локальное время
			temp_time = std::localtime(&curTime);
			// Чтобы избежать проблем при переходе к новым суткам
			// устанавливаем у ДР тоже время, как в переменной curTime
			// Ну и конечно год, год меняем на текущий.
			// Его можно было бы запомнить, а потом вернуть, но в условии задачи
			// этого не требуется.
			birthday.tm_year = temp_time->tm_year;
			birthday.tm_hour = temp_time->tm_hour;
			birthday.tm_min = temp_time->tm_min;
			birthday.tm_sec = temp_time->tm_sec;
			// Определяем разницу между текущей датой и ДР
			delta = std::difftime(mktime(&birthday), curTime);
			// Разница может быть отрицательная, если ДР до текущей даты и если её сохранить
			// в map, то она будет в начале, нам это невыгодно, поэтому мы задвигаем такие ДР
			// в конец, т.е. считаем, что в году 365 дней, в дне 24 часа, в часе 60 минут, а
			// в минуте 60 секунд прибавим к этому произведению delta и она окажется в конце miltimap
			if (delta < 0)
				delta = 365 * 24 * 60 * 60 + delta;
			// Вставляем пару в multimap
			mBirthdays.insert({delta, name});
		}
	}
	// Если в MAP есть записи, то выводим ближайшие дни рождения в формате день / месяц.
	if(mBirthdays.size() > 0)
	{
		// Рассчитываем дату ДР
		t_time = curTime + mBirthdays.begin()->first;
		temp_time = std::localtime(&t_time);
		// Создаём иттератор для multimap
		std::pair <std::multimap <double, std::string>::iterator, std::multimap <double, std::string>::iterator> ii;
		// Определяем записи в которых одинковый KEY из начала multimap
		ii = mBirthdays.equal_range(mBirthdays.begin()->first);
		std::multimap<double, std::string>::iterator it;
		for (it = ii.first; it != ii.second; ++it)
		{	// Если delta = 0, т.е. это сегодня, выводим на экран соответствующее сообщение
			if (mBirthdays.begin()->first == 0)
				std::cout << it->second << "'s birthday is now!!!\n";
			else
				// Если не сегодня, то выводится другая строка.
				std::cout << std::string(30, '=') << '\n' << it->second << "'s birthday is " << std::put_time(temp_time, "%d/%m") << '\n';
		}
	}
	std::cout << "Thank you for use my program.\n";
	return 0;
}
