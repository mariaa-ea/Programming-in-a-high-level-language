//-----------------------------------------------------------------------
// Лабораторная работа #3 (часть 2, 3) по дисциплине "Программирование на языке высокого уровня. C++"
// Преподаватель: Иванова Т.В.
// Студент      : Емельянова М.А.
// 
// Laboratory work #3 (part 2, 3) in the discipline "Programming in a high-level language. C++"
// Teacher      : Ivanova T.V.
// Student      : Emelyanova M.A.
//-----------------------------------------------------------------------
//
// ЧТОБЫ ОСУЩЕСТВИТЬ ПРОВЕРКУ ПОДКЛЮЧЕНИЯ БИБЛИОТЕКИ, 
// НУЖНО В ПРОЕКТ ВСТАВИТЬ ПУТЬ, ДЛЯ ЭТОГО:
// 1. КЛИКНУТЬ В ПОЛЕ ПРОЕКТА
// 2. ПЕРЕЙТИ В "СВОЙСТВА"
// 3. ВЫБРАТЬ КАТАЛОГИ VS
// 4. ВСТАВИТЬ ПУТЬ БИБЛИОТЕКИ C:\Program Files\boost_1_84_0;C:\Program Files\fftw-3.3.5-dll64; В ПОЛЯ 
// - "Каталоги включения" и "Каталоги библиотек"
// 5. Перейти в cd PS C:\Program Files\fftw-3.3.5-dll64>
// 6. Ввести в PowerShell: lib /machine:x64 /def:libfftw3-3.def
// p.s. "x64" - номер системы
// 
// ----------------------------------
// ПРОВЕРКА НОМЕРА СИСТЕМЫ
// ----------------------------------
//#include <iostream>
//
//int main() {
//    if (sizeof(void*) == 8) {
//        std::cout << "64-bit operating system" << std::endl;
//    }
//    else {
//        std::cout << "32-bit operating system" << std::endl;
//    }
//    return 0;
//}
// ----------------------------------
// 
// ДЛЯ ПРОВЕРКИ ПОДКЛЮЧЕНИЯ БИБЛИОТЕКИ <boost/...> запустить код ниже
// 
//#include <iostream>
//#include <boost/version.hpp>
// 
//int main() {
//    std::cout << "Boost version: " << BOOST_LIB_VERSION << std::endl;
//    return 0;
//}
// В случае, если возникли сложности, следовать инструкции: https://russianblogs.com/article/3339529859/
// 
// ----------------------------------

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <fftw3.h>

#include "SampleComplex.h"
#include "SampleMatrix.h"

int main() {
	// Лабораторная работа #3. Часть 2
	//------------- НАЧАЛО проверок для класса SampleComplex ------------

	// Создание объекта SampleComplex
	SampleComplex sampleComplex(2); // Создаем объект с размером 2x2

	// Заполнение объекта sampleComplex комплексными числами 
	sampleComplex(0, 0) = std::complex<double>(3.0, 4.0);
	sampleComplex(0, 1) = std::complex<double>(-2.0, 1.0);
	sampleComplex(1, 0) = std::complex<double>(-1.0, -3.0);
	sampleComplex(1, 1) = std::complex<double>(2.0, -2.0);

	// Вывод исходной выборки
	std::cout << "Original SampleComplex:" << std::endl;
	sampleComplex.PrintMatrix();

	// Вызов метода GetAbs() для вычисления модуля всех элементов выборки
	Sample<double> absData = sampleComplex.GetAbs();
	std::cout << "Absolute Values (Modulus):" << std::endl;
	absData.PrintMatrix();

	// Вызов метода GetPhase() для вычисления фазы всех элементов выборки
	Sample<double> phaseData = sampleComplex.GetPhase();
	std::cout << "Phase Values:" << std::endl;
	phaseData.PrintMatrix();

	// Вывод GetIntensity() интенсивности элементов выборки
	SampleComplex intensityData = sampleComplex.GetIntensity();
	std::cout << "Intensity Values:" << std::endl;
	intensityData.PrintMatrix();

	// Преобразование объекта sampleComplex из SampleComplex в Sample<double>
	Sample<double> doubleSample = sampleComplex.ComplexToDouble();
	std::cout << "SampleComplex after conversion to Sample<double> (for Original SampleComplex):" << std::endl;
	doubleSample.PrintMatrix();

	// Преобразование объекта doubleSample из Sample<double> в SampleComplex
	SampleComplex newSampleComplex = sampleComplex.DoubleToComplex(doubleSample);
	std::cout << "Sample<double> after conversion back to SampleComplex:" << std::endl;
	newSampleComplex.PrintMatrix();

	// Вызываем методы для записи вещественной и мнимой частей в файлы
	sampleComplex.WriteRealPartToFile("real_part.txt");
	sampleComplex.WriteImaginaryPartToFile("imaginary_part.txt");

	// Вызываем методы для записи модуля, фазы и интенсивнности в файлы
	sampleComplex.WriteAbsToFile("abs_values.txt");
	sampleComplex.WritePhaseToFile("phase_values.txt");
	sampleComplex.WriteIntensityToFile("intensity_values.txt");
	std::cout << "-----------------------------" << std::endl << std::endl;
	// ------------- КОНЕЦ проверок для класса SampleComplex -------------
	// 
	// ------------- НАЧАЛО проверок для класса SampleMatrix -------------

	// Создаем объект SampleMatrix и преобразуем его в единичную матрицу
	SampleMatrix matr_1(2);
	matr_1.CreateIdentityMatrix();

	std::cout << "Identity Matrix:" << std::endl;
	matr_1.PrintMatrix();

	// Создаем вторую матрицу размером 2х2 и заполняем ее данными
	SampleMatrix matr_2(2);
	matr_2(0, 0) = 1.0;
	matr_2(0, 1) = 2.0;
	matr_2(1, 0) = 3.0;
	matr_2(1, 1) = 4.0;
	//matr_2(2, 0) = 5.0;
	//matr_2(2, 1) = 6.0;

	std::cout << "Second matrix:" << std::endl;
	matr_2.PrintMatrix();

	// Умножаем матрицы
	SampleMatrix result = matr_1 * matr_2;
	std::cout << "Result of matrix multiplication:" << std::endl;
	result.PrintMatrix();

	// Проверяем умножение матрицы на число
	std::cout << "Original matrix:" << std::endl;
	matr_1.PrintMatrix();

	double scalar = 2;
	matr_1 *= scalar;
	std::cout << "Result of matrix multiplication by scalar:" << std::endl;
	matr_1.PrintMatrix();

	std::cout << "Multiply the matrix above by the matrix below:" << std::endl;
	matr_2.PrintMatrix();

	matr_1 *= matr_2;
	std::cout << "Result of matrix-matrix multiplication:" << std::endl;
	matr_1.PrintMatrix();

	// Проверка работоспособности методов по получению обратной и транспонированной матрицы
	std::cout << "Original Matrix:" << std::endl;
	matr_2.PrintMatrix();

	// Проверим, что методы работают корректно:
	// при умножении исходной матрицы на обратную должна получиться единичная
	try {
		// Получаем обратную матрицу
		SampleMatrix inverseMatrix = matr_2.GetInverseMatrix();

		// Выводим обратную матрицу
		std::cout << "Inverse Matrix:" << std::endl;
		inverseMatrix.PrintMatrix();

		// Проверяем свойство: исходная матрица * обратная матрица = единичная матрица
		SampleMatrix identityMatrix = matr_2 * inverseMatrix;
		std::cout << "Checking the property: original matrix * inverse matrix = identity matrix:" << std::endl;
		identityMatrix.PrintMatrix();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	// Проверяем свойство: дважды транспонированная матрица равна исходной
	if (matr_2 == ~~matr_2) {
		std::cout << "The property is satisfied: Twice Transposed Matrix is equal to Original Matrix" << std::endl;
	}
	else {
		std::cout << "The property is not satisfied" << std::endl;
	}

	std::cout << "-----------------------------" << std::endl << std::endl;
	// ------------- КОНЕЦ проверок для класса SampleMatrix --------------
	// 
	//
	// Лабораторная работа #3. Часть 3
	// Сравнение перемножения матриц при помощи boost::numeric::ublas 
	// и методов своего проекта

	// Создаем и заполняем первую матрицу с помощью boost
	std::cout << "Output using the boost library:" << std::endl << std::endl;
	boost::numeric::ublas::matrix<double> matrix1(2, 3);
	for (std::size_t i = 0; i < matrix1.size1(); ++i) {
		for (std::size_t j = 0; j < matrix1.size2(); ++j) {
			matrix1(i, j) = static_cast<double>(i * matrix1.size2() + j + 1);
		}
	}

	std::cout << "Matrix 1:" << std::endl;
	std::cout << matrix1 << std::endl;

	// Создаем и заполняем вторую матрицу
	boost::numeric::ublas::matrix<double> matrix2(3, 2);
	for (std::size_t i = 0; i < matrix2.size1(); ++i) {
		for (std::size_t j = 0; j < matrix2.size2(); ++j) {
			matrix2(i, j) = static_cast<double>(i * matrix2.size2() + j + 1);
		}
	}

	std::cout << "Matrix 2:" << std::endl;
	std::cout << matrix2 << std::endl;

	// Перемножаем матрицы
	boost::numeric::ublas::matrix<double> res = boost::numeric::ublas::prod(matrix1, matrix2);

	// Выводим результат перемножения
	std::cout << "Result of matrix multiplication matrix1 by matrix2 using the boost library:" << std::endl;
	std::cout << res << std::endl << std::endl;

	// OUTPUT библиотеки boost отличается от написанного мною и имеет следующий вид:
	// 
	// Matrix 1 :
	// [2, 3] ((1, 2, 3), (4, 5, 6))
	// Matrix 2 :
	// [3, 2] ((1, 2), (3, 4), (5, 6))
	// Result of matrix multiplication matrix1 by matrix2 using the boost library:
	// [2, 2] ((22, 28), (49, 64))

	// Создаем матрицы с помощью написанных мною методов
	matr_1.Resize(2, 3);
	for (int i = 0; i < matr_1.GetSizeX(); ++i) {
		for (int j = 0; j < matr_1.GetSizeY(); ++j) {
			matr_1(i, j) = static_cast<double>(i * matr_1.GetSizeY() + j + 1);
		}
	}

	matr_2.Resize(3, 2);
	for (int i = 0; i < matr_2.GetSizeX(); ++i) {
		for (int j = 0; j < matr_2.GetSizeY(); ++j) {
			matr_2(i, j) = static_cast<double>(i * matr_2.GetSizeY() + j + 1);
		}
	}

	std::cout << "Output using my class:" << std::endl << "matr_1:" << std::endl;
	matr_1.PrintMatrix();
	std::cout << "matr_2:" << std::endl;
	matr_2.PrintMatrix();
	SampleMatrix my_res = matr_1 * matr_2;
	std::cout << "Result of matrix multiplication matr_1 by matr_2 using my class:" << std::endl;
	my_res.PrintMatrix();

	// OUTPUT моего проекта: 
	// matr_1:
	// 1 2 3
	// 4 5 6

	// matr_2 :
	// 1 2
	// 3 4
	// 5 6

	// Result of matrix multiplication matr_1 by matr_2 using my class :
	// 22 28
	// 49 64

	// Преобразование Фурье
	// Создаем объект SampleComplex и заполняем его единичными значениями
	SampleComplex sample(64);
	//int value = 0; // для заполнения числами - расскомментировать
	for (int i = 0; i < sample.GetSize(); ++i) {
		for (int j = 0; j < sample.GetSize(); ++j) {
			sample(i, j) = std::complex<double>(1.0, 0.0); // а тут - 1.0 поменять на value++
		}
	}
	//sample.PrintMatrix();

	std::cout << "The input array is created and filled with unit values." << std::endl;

	// Создаем планы преобразования Фурье
	fftw_plan p_forward = fftw_plan_dft_2d(sample.GetSize(), sample.GetSize(), (fftw_complex*)sample.GetPointer(),
		(fftw_complex*)sample.GetPointer(), FFTW_FORWARD, FFTW_ESTIMATE);
	fftw_plan p_backward = fftw_plan_dft_2d(sample.GetSize(), sample.GetSize(), (fftw_complex*)sample.GetPointer(),
		(fftw_complex*)sample.GetPointer(), FFTW_BACKWARD, FFTW_ESTIMATE);

	// Выполняем прямое преобразование Фурье
	fftw_execute(p_forward);
	std::cout << "The Fourier transform is executed successfully." << std::endl;

	// Записываем результат преобразования в файл
	std::string filename_ford = "./output/fourier_result_2d.txt";
	std::ofstream file(filename_ford);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing Fourier result.");
	}

	for (int i = 0; i < sample.GetSize(); ++i) {
		for (int j = 0; j < sample.GetSize(); ++j) {
			file << std::real(sample(i, j)) << " " << std::imag(sample(i, j)) << std::endl;
		}
	}
	file.close();
	std::cout << "The Fourier result has been successfully written to the file." << std::endl;

	// Выполняем обратное преобразование Фурье
	fftw_execute(p_backward);
	std::cout << "The inverse Fourier transform is executed successfully." << std::endl;

	// Записываем результат обратного преобразования в файл
	std::string filename_back = "./output/inverse_fourier_2d.txt";
	std::ofstream file_back(filename_back);
	if (!file_back.is_open()) {
		throw std::runtime_error("Unable to open file for writing Inverse Fourier result.");
	}

	for (int i = 0; i < sample.GetSizeX(); ++i) {
		for (int j = 0; j < sample.GetSizeY(); ++j) {
			file_back << std::real(sample(i, j)) / static_cast<double>(sample.GetSize() * sample.GetSize()) << " "
				<< std::imag(sample(i, j)) / static_cast<double>(sample.GetSize() * sample.GetSize()) << std::endl;
		}
	}

	file_back.close();
	std::cout << "The Inverse Fourier result has been successfully written to the file." << std::endl;

	// Освобождаем память и уничтожаем планы
	fftw_destroy_plan(p_forward);
	fftw_destroy_plan(p_backward);

	return 0;
}
