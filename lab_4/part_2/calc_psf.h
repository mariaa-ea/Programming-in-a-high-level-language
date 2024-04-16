#pragma once
#include "sample_complex.h"

struct OpticalParameters {
	// Размер выборки
	int m_int_sample_sizes = 0;

	// Параметры оптической системы
	double m_double_wavelength = 0;
	double m_double_back_aperture = 0;
	double m_double_magnification = 0;
	double m_double_defocus = 0;
	double m_double_astigmatism = 0;

	// Численные параметры
	double m_double_step_obj = 0;
	double m_double_step_obj_can = 0;
	double m_double_step_im = 0;
	double m_double_step_im_can = 0;
	double m_double_diam_pupil = 0;
	double m_double_step_pupil = 0;

	double m_double_aperture = 0;
};

// Позволяет осуществлять расчет ФРТ 
class CalcPSF
{
public:
	CalcPSF() = default;
	~CalcPSF() = default;

	// Вычисляет ФРТ по имеющимся параметрам opt_params
	void Calc(const OpticalParameters& opt_params, Sample<double>& PSF);

private:
	// Вычисление преобразования Фурье от oSample_p (+ сдвиг до и после Фурье) 
	void CalcFFT(SampleComplex& Sample_psf);
	// Сдвиг для преобразования Фурье
	void ShiftSample(SampleComplex& Sample_psf);
	// Создает зрачковую функцию
	void CalcPupilFunction(const OpticalParameters& opt_params, SampleComplex& Pupil_p);
};
