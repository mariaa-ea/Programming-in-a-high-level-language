//-----------------------------------------------------------------------
// Лабораторная работа #1 по дисциплине "Программирование на языке высокого уровня. C++"
// Преподаватель: Иванова Т.В.
// Студент      : Емельянова М.А.
// 
// Laboratory work #1 in the discipline "Programming in a high-level language. C++"
// Teacher      : Ivanova T.V.
// Student      : Emelyanova M.A.
//-----------------------------------------------------------------------

#include "functions.h"

int main() {
	setlocale(LC_ALL, "Russian");

	double x, y;
	int choice;
	bool flag = true;

	while (flag) {
		std::wcout << L"Меню:" << std::endl;
		std::wcout << L"1. Ввести координаты с клавиатуры" << std::endl;
		std::wcout << L"2. Считать координаты из файла" << std::endl;
		std::wcout << L"Для выхода из программы нажмите любую иную клавишу." << std::endl;
		std::wcin >> choice;

		switch (choice) {
		case 1:
			input_coord(x, y);
			print_result(x, y);
			print_to_file(x, y);
			break;
		case 2:
			read_coord(&x, &y);
			print_result(x, y);
			print_to_file(x, y);
			break;
		default:
			flag = false;
			closed_program();
			break;
		}
	}
	return 0;
}

void input_coord(double& x, double& y) {
	std::wcout << L"Введите координаты точки (x, y): ";

	while (!(std::wcin >> x >> y)) {
		std::wcout << L"\nНекорректный ввод. Введите числа для координат x и y: ";
		std::wcin.clear();
		std::wcin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

bool square_area(double x, double y) {
	return (std::fabs(x) <= 1 && std::fabs(y) <= 1);
}

bool circular_area(double x, double y) {
	return (x * x + y * y <= 1);
}

void print_result(double x, double y) {
	if (square_area(x, y)) {
		std::wcout << L"Точка находится внутри квадратной области единичного радиуса." << std::endl;
	}
	else {
		std::wcout << L"Точка находится вне квадратной области единичного радиуса." << std::endl;
	}

	if (circular_area(x, y)) {
		std::wcout << L"Точка находится внутри круглой области единичного радиуса.\n" << std::endl;
	}
	else {
		std::wcout << L"Точка находится вне круглой области единичного радиуса.\n" << std::endl;
	}
}

void closed_program() {
	std::wcout << L"Программа завершена." << std::endl;
}

void read_coord(double* x, double* y) {
	std::string filename;
	bool file_opened = false;

	while (!file_opened) {
		std::wcout << L"\nДля выхода из программы нажмите Q." << std::endl;
		std::wcout << L"Введите имя файла для чтения координат: ";
		std::cin >> filename;

		if (filename == "q" || filename == "Q") {
			closed_program();
			exit(0);
		}

		std::ifstream file(filename);
		if (file.is_open()) {
			file >> *x >> *y;
			file.close();
			file_opened = true;
			std::wcout << L"\nКоординаты из файла: x = " << *x << ", y = " << *y << std::endl;
		}
		else {
			std::wcout << L"Такого файла не существует. Попробуйте еще раз.\n" << std::endl;
		}
	}
}

void print_to_file(double& x, double& y) {
	std::wcout << L"Записать результаты в файл? (+/-): ";
	char answer;
	std::cin >> answer;
	std::wcout << "\n";

	if (answer == '+') {
		std::ofstream out("output_coord.txt", std::ios::app);

		if (out.is_open()) {
			out << "Координаты точки: x = " << x << ", y = " << y << std::endl;
			out << "Результаты проверки координат:" << std::endl;

			if (square_area(x, y)) {
				out << "Точка находится внутри квадратной области единичного радиуса." << std::endl;
			}
			else {
				out << "Точка находится вне квадратной области единичного радиуса." << std::endl;
			}

			if (circular_area(x, y)) {
				out << "Точка находится внутри круглой области единичного радиуса.\n" << std::endl;
			}
			else {
				out << "Точка находится вне круглой области единичного радиуса.\n" << std::endl;
			}

			out.close();
			std::wcout << L"Результаты успешно добавлены в файл 'output_coord.txt'.\n" << std::endl;
		}
	}
	else {
		std::wcout << L"Результаты не сохранены.\n" << std::endl;
	}
}
