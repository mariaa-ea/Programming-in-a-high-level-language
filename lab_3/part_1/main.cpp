//-----------------------------------------------------------------------
// Лабораторная работа #3 (часть 1) по дисциплине "Программирование на языке высокого уровня. C++"
// Преподаватель: Иванова Т.В.
// Студент      : Емельянова М.А.
// 
// Laboratory work #3 (part 1) in the discipline "Programming in a high-level language. C++"
// Teacher      : Ivanova T.V.
// Student      : Emelyanova M.A.
//-----------------------------------------------------------------------

#include <fstream>

#include "Sample.h"

int main() {
    // Создаем объект класса Sample, заполненный 0 (int) и размером 3x3 -> 5x5
    Sample<int> samp_1;
    samp_1.Resize(3);
    std::cout << "Size of the first sample: " << samp_1.GetSize() << " x " << samp_1.GetSize() << std::endl;
    samp_1.Resize(5);
    std::cout << "New size of the first sample: " << samp_1.GetSize() << " x " << samp_1.GetSize() << std::endl << std::endl;

    // Создаем второй неквадратный объект 9x12 -> 18x24
    Sample<int> samp_2;
    samp_2.Resize(9, 12);
    std::cout << "Size of the second sample: " << samp_2.GetSizeX() << " x " << samp_2.GetSizeY() << std::endl;
    samp_2.Resize(18, 24);
    std::cout << "New size of the second sample: " << samp_2.GetSizeX() << " x " << samp_2.GetSizeY() << std::endl << std::endl;

    // Попытка обратиться к элементу с недопустимым индексом
    try {
        int value = samp_2(-1, 2);
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl << std::endl;
    }

    // Рассмотрим оператор ()
    // Для неконстантных переменных, помимо обращения к элеменатам выборки, можно присваивать значения
    samp_1(0, 0) = 10; // Меняем значение элемента
    std::cout << "Value at (0, 0): " << samp_1(0, 0) << std::endl;

    // Константный оператор круглых скобок позволяет только обращаться к элементам выборки
    const Sample<int>& const_samp = samp_1;
    std::cout << "Value at (1, 1): " << const_samp(1, 1) << std::endl << std::endl;

    // Проверка метода IsSquare
    std::cout << "IsSquare (samp_1): " << (samp_1.IsSquare() ? "true" : "false") << std::endl;

    // Проверка метода IsZeroSize
    std::cout << "IsZeroSize (samp_1): " << (samp_1.IsZeroSize() ? "true" : "false") << std::endl;

    // Проверка метода IsEqualSize
    std::cout << "IsEqualSize (samp_1, samp2): " << (samp_1.IsEqualSize(samp_2) ? "true" : "false") << std::endl << std::endl;

    // Инициализируем все элементы выборки значением 7
    samp_2.Resize(2, 4);
    samp_2.SetValue(7);

    // Выводим элементы выборки
    std::cout << "Samp_2 (Resize(2, 4)) values after SetValue(7):" << std::endl;
    samp_2.PrintMatrix();
    
    // Получаем указатель на данные выборки
    int* ptr = samp_2.GetPointer();

    // Выводим значения через указатель
    std::cout << "Samp_2 values through pointer:" << std::endl;
    for (int i = 0; i < samp_2.GetSizeX() * samp_2.GetSizeY(); ++i) {
        std::cout << ptr[i] << " ";
    }
    std::cout << std::endl << std::endl;

    // Заполним матрицу значениями и распечатаем на экран
    samp_1.Resize(3);
    int val_1 = 1;
    for (int i = 0; i < samp_1.GetSizeX(); ++i) {
        for (int j = 0; j < samp_1.GetSizeY(); ++j) {
            samp_1(i, j) = val_1;
            val_1++;
        }
    }

    std::cout << "Samp_1 values before operations:" << std::endl;
    samp_1.PrintMatrix();

    // Вычисляем максимальное значение в выборке
    int max = samp_1.GetMax();
    std::cout << "Max value in the sample_1: " << max << std::endl << std::endl;

    // Применяем оператор += и выводим результат
    samp_1 += 10; 
    std::cout << "Samp_1 values after += 10:" << std::endl;
    samp_1.PrintMatrix();

    // Применяем оператор *= и выводим результат
    samp_1 *= 2;  
    std::cout << "Samp_1 values after *= 2:" << std::endl;
    samp_1.PrintMatrix();

    // Применяем оператор /= и выводим результат
    samp_1 /= 3;  
    std::cout << "Sample_1 values after /= 3:" << std::endl;
    samp_1.PrintMatrix();

    // Создаем два объекта класса Sample с размером 3x3
    samp_2.Resize(3, 3);

    // Заново инициализируем значения в обоих выборках
    int k = 1;
    for (int i = 0; i < samp_1.GetSizeX(); ++i) {
        for (int j = 0; j < samp_2.GetSizeY(); ++j) {
            samp_1(i, j) = k;
            samp_2(i, j) = k * 2;
            ++k;
        }
    }

    // Выводим элементы выборки samp_1
    std::cout << "New samp_1 values:" << std::endl;
    samp_1.PrintMatrix();

    // Выводим элементы выборки samp_2
    std::cout << "New samp_2 values:" << std::endl;
    samp_2.PrintMatrix();

    // Применяем оператор += и выводим результат
    samp_1 += samp_2;
    std::cout << "Samp_1 values after += samp_2:" << std::endl;
    samp_1.PrintMatrix();

    // Применяем оператор -=
    samp_1 -= samp_2;
    std::cout << "Samp_1 values after -= samp_2:" << std::endl;
    samp_1.PrintMatrix();

    // Применяем оператор *=
    samp_1 *= samp_2;
    std::cout << "Samp_1 values after *= samp_2:" << std::endl;
    samp_1.PrintMatrix();

    // Сохраняем выборку в файл
    std::ofstream outFile("sample.txt");
    if (outFile.is_open()) {
        samp_1.Save(outFile);
        outFile.close();
        std::cout << "Samp_1 saved to file 'sample.txt'" << std::endl;
    }
    else {
        std::cerr << "Unable to open file for saving" << std::endl;
    }
    std::cout << std::endl;

    // Создаем новый объект для загрузки выборки и выводим выборку из файла
    Sample<int> loadedSample;
    std::ifstream inFile("sample.txt");
    if (inFile.is_open()) {
        loadedSample.Load(inFile);
        inFile.close();
        std::cout << "Sample loaded from file 'sample.txt'" << std::endl;

        std::cout << "Loaded Sample values:" << std::endl;
        loadedSample.PrintMatrix();
    }
    else {
        std::cerr << "Unable to open file for loading" << std::endl;
    }

    // Попробуем словить ошибку, когда массив имеет разные значения строк и столбцов
    samp_2.Resize(3, 2);

    try {
        if (samp_2.IsSquare()) {
            std::cout << "Sample_2 is square" << std::endl;
        }
    }
    catch (const std::logic_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Size of Sample: " << samp_2.GetSizeX() << " x " << samp_2.GetSizeY() << std::endl;
    }

    return 0;
}
