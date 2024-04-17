# Лабораторная работа №6

## Описание:

Данная лабораторная работа предполагает создание итогового приложения с определенными требованиями.

## Задачи:

- Добавить иконку к итоговому приложению.
- Создать zip-архив, включающий исполняемый файл (exe) и необходимые библиотеки (dll).
- Отправить результаты через файлообменник.

## Примечание:
1) Все **необходимые** файлы для _запуска_ **итогового приложения** представлены в папке **`./Release`**.
2) Измененный `CMakeLists.txt` и иконка (`graph.ico`, `dqt_psf.rc`) находятся в текущей директории. 
3) В файле `calc_psf.cpp` после профилирования в _релизе_ метод `Calc` возвращен в исходную реализацию (вычисление ФРТ осуществляется в количестве 1 шт) и имеет следующий вид:
```cpp
void CalcPSF::Calc(const OpticalParameters& opt_params, Sample<double>& PSF) {

	PSF.Resize(opt_params.m_int_sample_sizes);
	PSF.SetValue(0.);

	// Зрачковая функция
	SampleComplex Pupil(opt_params.m_int_sample_sizes);
	CalcPupilFunction(opt_params, Pupil);

	// Обратное Фурье
	CalcFFT(Pupil);

	// Нормировка
	Pupil *= std::complex<double>(opt_params.m_double_step_pupil / opt_params.m_double_step_obj_can, 0.);

	// Преобразование комплексных чисел в вещественные
	Sample<double> pupil_double = Pupil.GetIntensity().ComplexToDouble();

	PSF = pupil_double;
	PSF *= (1 / (PI * PI));
}
```

Все оставшиеся изменения, внесенные в ходе профилирования, были сохранены в финальной версии приложения. 
