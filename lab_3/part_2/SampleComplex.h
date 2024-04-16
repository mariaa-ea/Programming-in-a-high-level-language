#pragma once

#include <windows.h>
#include <complex>
#include "Sample.h"

// Дочерний класс Sample. Класс комплексных чисел
class SampleComplex : public Sample<std::complex<double>> {
public:
	// Конструктор по умолчанию 
	SampleComplex() = default;
	// Конструктор квадратной выборки
	SampleComplex(int size) : Sample<std::complex<double>>(size, size) {}
	// Деструктор 
	~SampleComplex() = default;

public:
	// Методы класса 

	// Возвращает модуль комплексных чисел выборки
	Sample<double> GetAbs();
	// Возвращает фазу комплексных чисел выборки (по ум. в радианах)	
	Sample<double> GetPhase();
	// Возвращает интенсивность комплексных чисел выборки
	SampleComplex GetIntensity();

	// Осуществляет преобразование из SampleComplex в Sample<double>
	Sample<double> ComplexToDouble();
	// Осуществляет преобразование Sample<double> в SampleComplex
	SampleComplex DoubleToComplex(Sample<double>& temp);

	// Записывает реальную часть значений выборки в файл
	void WriteRealPartToFile(const std::string& filename);
	// Записывает мнимую часть значений выборки в файл
	void WriteImaginaryPartToFile(const std::string& filename);
	// Записывает модуль значений выборки в файл
	void WriteAbsToFile(const std::string& filename);
	// Записывает фазу значений выборки в файл
	void WritePhaseToFile(const std::string& filename);
	// Записывает интесивность значений выборки в файл
	void WriteIntensityToFile(const std::string& filename);

private:
	// Формирует путь к файлу в папке "./output/".
	// (создана для хранения всех выводов класса в папке ./output/)
	std::string GetOutputFilePath(const std::string& filename);
};
