#include "calc_psf.h"
#include <fftw3.h>
#include "dqt_psf.h"


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

void CalcPSF::CalcFFT(SampleComplex& Sample_psf) {
	// Смещение выборки
	ShiftSample(Sample_psf);

	// Создаем план для fftw
	fftw_plan oPlan = fftw_plan_dft_2d(Sample_psf.GetSize(), Sample_psf.GetSize(),
		(fftw_complex*)Sample_psf.GetPointer(),
		(fftw_complex*)Sample_psf.GetPointer(),
		FFTW_BACKWARD, FFTW_ESTIMATE);

	// Преобразование Фурье
	fftw_execute(oPlan);

	// Удаляем план
	fftw_destroy_plan(oPlan);

	// Создание комплексного числа с вещественной 1/sqrt(n*n) и мнимой 0
	std::complex<double> coeff(1. / sqrt(double(Sample_psf.GetSize() * Sample_psf.GetSize())), 0.);

	// Домножение полученного спектра на коэффициент
	Sample_psf *= coeff;

	// Смещение спектра
	ShiftSample(Sample_psf);
}

void CalcPSF::CalcPupilFunction(const OpticalParameters& opt_params, SampleComplex& Pupil_p) {
	Pupil_p.SetValue(0.);

	for (int j = 0; j < Pupil_p.GetSize(); ++j) {
		double ro_y = Pupil_p.CalcCoord(j, opt_params.m_double_step_pupil);

		for (int i = 0; i < Pupil_p.GetSize(); ++i) {

			double ro_x = Pupil_p.CalcCoord(i, opt_params.m_double_step_pupil);

			if (ro_x * ro_x + ro_y * ro_y <= 1) {
				double ro2 = ro_x * ro_x + ro_y * ro_y;
				double fi = 0;

				if (ro_y == 0) {
					if (ro_x >= 0) {
						fi = PI / 2;
					}
					else {
						fi = - PI / 2;
					}
				}
				else {
					if (ro_y >= 0) {
						fi = atan(ro_x / ro_y);
					}
					else {
						fi = PI + atan(ro_x / ro_y);
					}
				}

				double W = 2 * PI * (opt_params.m_double_defocus * (2 * ro2 - 1) + opt_params.m_double_astigmatism * ro2 * cos(2 * fi));
				Pupil_p(i, j) = std::complex<double>(cos(W), sin(W));
			}
		}
	}
}

void CalcPSF::ShiftSample(SampleComplex& Sample_psf) {
	double pi = 2 * acos(0.);

	for (int j = 0; j < Sample_psf.GetSize(); j++) {
		for (int i = 0; i < Sample_psf.GetSize(); i++) {
			// Вычисление сдвигового коэффициента по x
			std::complex<double> coeff = std::complex<double>(cos(-2. * pi / 2 * i), sin(-2. * pi / 2 * i));

			// Домножение на сдвиговый коэффициент по x
			Sample_psf(i, j) *= coeff;

			// Вычисление сдвигового коэффициента по y
			coeff = std::complex<double>(cos(-2. * pi / 2 * j), sin(-2. * pi / 2 * j));

			// Домножение на сдвиговой коэффициент по y
			Sample_psf(i, j) *= coeff;
		}
	}
}
