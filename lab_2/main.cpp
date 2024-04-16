//-----------------------------------------------------------------------
// Лабораторная работа #2 по дисциплине "Программирование на языке высокого уровня. C++"
// Преподаватель: Иванова Т.В.
// Студент      : Емельянова М.А.
// 
// Laboratory work #2 in the discipline "Programming in a high-level language. C++"
// Teacher      : Ivanova T.V.
// Student      : Emelyanova M.A.
//-----------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <set>
#include <algorithm>

int main() {
	setlocale(LC_ALL, "Russian");

	// Массив
	const int size = 10;

	int* arr = new int[size];
	for (int i = 0; i < size; i++) {
		arr[i] = i * size;
	}
	std::cout << "Элементы массива:" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;

	int sum_m = 0;
	for (int i = 0; i < size; i++) {
		sum_m += arr[i];
	}

	double average = static_cast<double>(sum_m) / size;
	std::cout << "Среднее арифметическое массива: " << average << std::endl << std::endl;

	// Вариант #4
	const int m_1 = 0;
	const int m_2 = 15;

	int* arr_2 = new int[size];
	std::copy(arr, arr + size, arr_2);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(arr_2, arr_2 + size, gen);

	std::cout << "Второй массив после перемешивания:" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << arr_2[i] << " ";
	}
	std::cout << std::endl;

	int sum_arr_2 = 0;
	for (int i = 0; i < size; i++) {
		sum_arr_2 += arr_2[i];
	}
	double average_arr_2 = static_cast<double>(sum_arr_2) / size;
	std::cout << "Среднее значение второго массива: " << average_arr_2 << std::endl << std::endl;

	int* res_mult = new int[size];
	for (int i = 0; i < size; i++) {
		res_mult[i] = arr[i] * arr_2[i];
	}

	std::cout << "Результат перемножения массивов:" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << res_mult[i] << " ";
	}
	std::cout << std::endl << std::endl;

	int* res_sq = new int[size];
	for (int i = 0; i < size; i++) {
		res_sq[i] = res_mult[i] * res_mult[i];
	}

	std::cout << "Результат возведения в квадрат:" << std::endl;
	for (int i = 0; i < size; i++) {
		std::cout << res_sq[i] << " ";
	}
	std::cout << std::endl << std::endl;

	delete[] arr;
	delete[] arr_2;
	delete[] res_mult;
	delete[] res_sq;


	// Вектор
	std::vector<int> vect(10);
	std::uniform_int_distribution<int> distr(-20, 20);

	for (int i = 0; i < 10; i++) {
		vect[i] = distr(gen);
	}

	std::cout << "Элементы вектора:" << std::endl;
	for (int i = 0; i < 10; i++) {
		std::cout << vect[i] << " ";
	}
	std::cout << std::endl;

	double sum_v = 0;
	for (const auto& i : vect) {
		sum_v += i * i;
	}

	double rms = sqrt(sum_v / 10);
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "Среднее квадратичное вектора: " << rms << std::endl << std::endl;
	std::cout << std::defaultfloat;


	// Список 
	std::list<double> my_list;
	char choice;

	do {
		double num;
		std::cout << "Введите число для его добавления в список: ";
		while (!(std::cin >> num)) {
			std::cout << "Введите число: ";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		my_list.push_back(num);

		do {
			std::cout << "Продолжить ввод? (+/-): ";
			std::cin >> choice;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << std::endl;
		} while (choice != '+' && choice != '-');
	} while (choice == '+');

	std::cout << "Введенные значения:" << std::endl;
	for (auto& num : my_list) {
		int precision = std::numeric_limits<double>::digits10 + 1;
		std::cout << std::setprecision(precision) << num << " ";
	}
	std::cout << std::endl << std::endl << std::defaultfloat;

	double max_abs = 0;
	for (const auto& num : my_list) {
		if (std::fabs(num) > std::fabs(max_abs)) {
			max_abs = num;
		}
	}
	std::cout << "Наибольшее по модулю значение: " << max_abs << std::endl << std::endl;


	// Ассоциативный массив
	std::ifstream file("glasses.txt");
	if (!file.is_open()) {
		std::cerr << "Ошибка открытия файла" << std::endl;
		return 1;
	}

	std::map<std::string, double> glasses;
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string glass_name;
		double refractive_index;
		iss >> glass_name >> refractive_index;
		glasses[glass_name] = refractive_index;
	}

	file.close();

	std::cout << "Список стекол:" << std::endl;
	for (auto it = glasses.begin(); it != glasses.end(); ++it) {
		std::cout << it->first << ": " << std::fixed << std::setprecision(3) << it->second << std::endl;
	}
	std::cout << std::endl;

	auto min_max = std::minmax_element(glasses.begin(), glasses.end(),
		[](const auto& a, const auto& b) { return a.second < b.second; });

	std::cout << "Стекло с минимальным показателем преломления: " << min_max.first->first << std::endl;
	std::cout << "Стекло с максимальным показателем преломления: " << min_max.second->first << std::endl << std::endl;


	// Очередь и множество
	std::deque<int> my_deq;
	std::set<int> my_set;

	std::uniform_int_distribution<int> distr_new(-10, 10);

	std::generate_n(std::back_inserter(my_deq), 15, [&]() { return distr_new(gen); });

	std::cout << "Очередь из 15 элементов:" << std::endl;
	for (auto it = my_deq.begin(); it != my_deq.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	for (auto it = my_deq.begin(); it != my_deq.end(); ++it) {
		my_set.insert(*it);
	}
	std::cout << std::endl;

	std::cout << "Уникальные значения (множество):" << std::endl;
	for (auto it = my_set.begin(); it != my_set.end(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << std::endl;

	return 0;
}
