#pragma once

#include <vector>
#include <algorithm>
#include <iostream>

// Базовый класс для выборки (квадратной или прямоугольной)
template <class PAR>
class Sample
{
protected:
	// число элементов выборки по x и y
	int m_size_x = 0, m_size_y = 0;
	// массив данных
	std::vector<PAR> m_data;

public:
	// конструктор
	Sample() = default;
	// деструктор
	~Sample() = default;

	// изменение размера для квадратной выборки
	void Resize(int size);
	// изменение размера для прямоугольной выборки
	void Resize(int size_x, int size_y);
	// печать элементов выборки
	void PrintMatrix() const;

	// размер выборки (для квадратной)
	int GetSize() const;
	// размер выборки по X и Y
	int GetSizeX() const;
	int GetSizeY() const;

	// оператор получения значения элемента с номером i,j
	PAR& operator()(int i, int j);
	// оператор получения const значения элемента с номером i,j
	const PAR& operator()(int i, int j) const;

	// проверка квадратная ли выборка
	bool IsSquare() const;
	// проверка что размер != 0
	bool IsZeroSize() const;
	// проверка равенства размеров с другой выборкой
	bool IsEqualSize(const Sample<PAR>& temp) const;

	// инициализация всех элементов выборки одинаковым значением
	void SetValue(PAR value);
	// получение указателя на всю выборку
	PAR* GetPointer();
	// вычисление максимального значения
	PAR GetMax() const;

	// операции с выборкой и числом
	// сложение всех элементов выборки и числа
	Sample<PAR>& operator+=(PAR value);
	// умножение всех элементов выборки на число
	Sample<PAR>& operator*=(PAR value);
	// деление всех элементов выборки на число
	Sample<PAR>& operator/=(PAR value);

	// операции с двумя выборками
	// присваивание
	Sample<PAR>& operator= (const Sample<PAR>& temp);
	// поэлементное сложение двух выборок
	Sample<PAR>& operator+=(const Sample<PAR>& temp);
	// поэлементное вычитание двух выборок
	Sample<PAR>& operator-=(const Sample<PAR>& temp);
	// поэлементное перемножение двух выборок
	Sample<PAR>& operator*=(const Sample<PAR>& temp);

	// вывод выборки в поток (файл или экран)
	std::ostream& Save(std::ostream& out);
	// перегрузка оператора чтения выборки из файла 
	std::istream& Load(std::istream& out);

protected:
	// проверяет квадратная ли выборка, если нет - создает исключение
	void CheckSquare() const;
	// проверяет: равны ли значения двух выборк или нет
	void CheckEqualSize(const Sample<PAR>& temp) const;
};


template <class PAR>
void Sample<PAR>::Resize(int size) {
	m_size_x = size;
	m_size_y = size;
	m_data.resize(size * size);
}

template <class PAR>
void Sample<PAR>::Resize(int size_x, int size_y) {
	m_size_x = size_x;
	m_size_y = size_y;
	m_data.resize(size_x * size_y);
}

template <class PAR>
int Sample<PAR>::GetSize() const {
	CheckSquare();
	return m_size_x;
}

template <class PAR>
int Sample<PAR>::GetSizeX() const {
	return m_size_x;
}

template <class PAR>
int Sample<PAR>::GetSizeY() const {
	return m_size_y;
}

template <class PAR>
PAR& Sample<PAR>::operator()(int i, int j) {
	if (i < 0 || i >= m_size_x || j < 0 || j >= m_size_y) {
		throw std::out_of_range("Index out of range");
	}
	return m_data[i * m_size_y + j];
}

template <class PAR>
const PAR& Sample<PAR>::operator()(int i, int j) const {
	if (i < 0 || i >= m_size_x || j < 0 || j >= m_size_y) {
		throw std::out_of_range("Index out of range");
	}
	return m_data[i * m_size_y + j];
}

template <class PAR>
bool Sample<PAR>::IsSquare() const {
	CheckSquare();
	return true;
}

template <class PAR>
bool Sample<PAR>::IsZeroSize() const {
	return m_size_x == 0 || m_size_y == 0;
}

template <class PAR>
bool Sample<PAR>::IsEqualSize(const Sample<PAR>& temp) const {
	return m_size_x == temp.m_size_x && m_size_y == temp.m_size_y;
}

template <class PAR>
void Sample<PAR>::SetValue(PAR value) {
	m_data.assign(m_size_x * m_size_y, value);
}

template <class PAR>
PAR* Sample<PAR>::GetPointer() {
	return m_data.data();
}

template <class PAR>
PAR Sample<PAR>::GetMax() const {
	if (m_data.empty()) {
		throw std::logic_error("Sample is empty");
	}
	return *std::max_element(m_data.begin(), m_data.end());
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator+=(PAR value) {
	for (PAR& i : m_data) {
		i += value;
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator*=(PAR value) {
	for (PAR& i : m_data) {
		i *= value;
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator/=(PAR value) {
	if (value == 0) {
		throw std::invalid_argument("Division by zero");
	}
	for (PAR& i : m_data) {
		i /= value;
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator=(const Sample<PAR>& temp) {
	if (this != &temp) {
		m_size_x = temp.m_size_x;
		m_size_y = temp.m_size_y;
		m_data = temp.m_data;
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator+=(const Sample<PAR>& temp) {
	CheckEqualSize(temp);
	for (int i = 0; i < m_data.size(); ++i) {
		m_data[i] += temp.m_data[i];
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator-=(const Sample<PAR>& temp) {
	CheckEqualSize(temp);
	for (int i = 0; i < m_data.size(); ++i) {
		m_data[i] -= temp.m_data[i];
	}
	return *this;
}

template <class PAR>
Sample<PAR>& Sample<PAR>::operator*=(const Sample<PAR>& temp) {
	CheckEqualSize(temp);
	for (int i = 0; i < m_data.size(); ++i) {
		m_data[i] *= temp.m_data[i];
	}
	return *this;
}

template <class PAR>
std::ostream& Sample<PAR>::Save(std::ostream& out) {
	out << m_size_x << " " << m_size_y << std::endl;
	for (int i = 0; i < m_size_x; ++i) {
		for (int j = 0; j < m_size_y; ++j) {
			out << m_data[i * m_size_y + j] << " ";
		}
		out << std::endl;
	}
	return out;
}

template <class PAR>
std::istream& Sample<PAR>::Load(std::istream& in) {
	in >> m_size_x >> m_size_y;
	m_data.resize(m_size_x * m_size_y);
	for (int i = 0; i < m_size_x * m_size_y; ++i) {
		in >> m_data[i];
	}
	return in;
}

template <class PAR>
void Sample<PAR>::PrintMatrix() const {
	for (int i = 0; i < m_size_x; ++i) {
		for (int j = 0; j < m_size_y; ++j) {
			std::cout << m_data[i * m_size_y + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

template <class PAR>
void Sample<PAR>::CheckSquare() const {
	if (m_size_x != m_size_y) {
		throw std::logic_error("Sample is not square");
	}
}

template <class PAR>
void Sample<PAR>::CheckEqualSize(const Sample<PAR>& temp) const {
	if (!IsEqualSize(temp)) {
		throw std::invalid_argument("Sizes of samples are not equal");
	}
}
