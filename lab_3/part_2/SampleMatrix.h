#pragma once
#include "Sample.h"

// Дочерний класс Sample. Адаптированный под матрицы класс
class SampleMatrix : public Sample<double> {
public:
	// Конструктор по умолчанию 
	SampleMatrix() = default;
	// Конструктор квадратной выборки
	SampleMatrix(int size) : Sample<double>(size, size) {}
	// Конструктор неквадратной выборки
	SampleMatrix(int size_x, int size_y) : Sample<double>(size_x, size_y) {}
	// Деструктор 
	~SampleMatrix() = default;

public:
	// Методы класса 

	// Создает единичную матрицу
	void CreateIdentityMatrix();

	// Оператор умножения для матричного произведения
	SampleMatrix operator*(const SampleMatrix& otherMatrix);
	// Переопределенный базовый (родительский) оператор *=
	SampleMatrix& operator*=(double value);
	// Переопределенный базовый оператор *= 
	SampleMatrix& operator*=(const SampleMatrix& otherMatrix);

	// Оператор деления для матричного произведения
	SampleMatrix operator/(const SampleMatrix& otherMatrix);
	// Переопределенный базовый оператор /=
	SampleMatrix& operator/=(const SampleMatrix& otherMatrix);

	// Оператор транспонирования матриц
	SampleMatrix operator~();
	// Перегрузка оператора ~= - невозможна

	// Вычисляет обратную матрицу
	SampleMatrix GetInverseMatrix() const;
	// Производит сравнение матриц поэлементно 
	bool operator==(const SampleMatrix& otherMatrix) const;
};
