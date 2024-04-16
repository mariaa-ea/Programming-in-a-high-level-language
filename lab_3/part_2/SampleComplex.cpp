#include "SampleComplex.h"

Sample<double> SampleComplex::GetAbs() {
	Sample<double> abs_data(this->GetSizeX(), this->GetSizeY());

	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			std::complex<double> compl_num = (*this)(i, j);
			double abs_val = std::abs(compl_num);
			abs_data(i, j) = abs_val;
		}
	}
	return abs_data;
}

Sample<double> SampleComplex::GetPhase() {
	Sample<double> phase_data(this->GetSizeX(), this->GetSizeY());

	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			std::complex<double> compl_num = (*this)(i, j);
			double phase_val = std::arg(compl_num);
			// если хочется в градусах, то нужно раскомментить строку ниже и закомментить строку выше
			// double phase_val = std::arg(compl_num) * (180.0 / (atan(1) * 4)); // конвертация в градусы
			phase_data(i, j) = phase_val;
		}
	}
	return phase_data;
}

SampleComplex SampleComplex::GetIntensity() {
	SampleComplex intens_data(this->GetSizeX());

	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			std::complex<double> compl_num = (*this)(i, j);
			double intensity = std::norm(compl_num); // квадрат модуля
			intens_data(i, j) = std::real(intensity); // оставляем только вещественную часть
		}
	}
	return intens_data;
}

Sample<double> SampleComplex::ComplexToDouble() {
	Sample<double> return_data(this->GetSizeX(), this->GetSizeY());
	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			return_data(i, j) = std::real((*this)(i, j)); // сохраняем вещественную часть комплексного числа
		}
	}
	return return_data;
}

SampleComplex SampleComplex::DoubleToComplex(Sample<double>& temp) {
	SampleComplex return_data(temp.GetSize());
	for (int i = 0; i < temp.GetSize(); i++) {
		for (int j = 0; j < temp.GetSize(); j++) {
			return_data(i, j) = std::complex<double>(temp(i, j), 0.0);
		}
	}
	return return_data;
}

// Этот метод и все ниже работают в режиме дозаписи, то есть сохраняют все данные в 
// указанном файле и записывают новые

void SampleComplex::WriteRealPartToFile(const std::string& filename) {
	std::string output_path = GetOutputFilePath(filename);
	std::ofstream file(output_path, std::ios::app); // для того, чтобы убрать режим дозаписи, удалите: std::ios::app
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing real part of the sample.");
	}

	Sample<double> real_part(this->GetSizeX(), this->GetSizeY());
	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			real_part(i, j) = std::real((*this)(i, j));
		}
	}

	real_part.Save(file);
	file.close();
	std::cout << "Real part of the sample has been successfully written to the file." << std::endl;
}

void SampleComplex::WriteImaginaryPartToFile(const std::string& filename) {
	std::string output_path = GetOutputFilePath(filename);
	std::ofstream file(output_path, std::ios::app);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing imaginary part of the sample.");
	}

	Sample<double> imag_part(this->GetSizeX(), this->GetSizeY());
	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			imag_part(i, j) = std::imag((*this)(i, j));
		}
	}

	imag_part.Save(file);
	file.close();
	std::cout << "Imaginary part of the sample has been successfully written to the file." << std::endl;
}

void SampleComplex::WriteAbsToFile(const std::string& filename) {
	std::string output_path = GetOutputFilePath(filename);
	std::ofstream file(output_path, std::ios::app);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing absolute values of the sample.");
	}

	Sample<double> abs_data = GetAbs();
	abs_data.Save(file);

	file.close();
	std::cout << "Absolute values of the sample have been successfully written to the file." << std::endl;
}

void SampleComplex::WritePhaseToFile(const std::string& filename) {
	std::string output_path = GetOutputFilePath(filename);
	std::ofstream file(output_path, std::ios::app);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing phase values of the sample.");
	}

	Sample<double> phase_data = GetPhase();
	phase_data.Save(file);

	file.close();
	std::cout << "Phase values of the sample have been successfully written to the file." << std::endl;
}

void SampleComplex::WriteIntensityToFile(const std::string& filename) {
	std::string output_path = GetOutputFilePath(filename);
	std::ofstream file(output_path, std::ios::app);
	if (!file.is_open()) {
		throw std::runtime_error("Unable to open file for writing intensity values of the sample.");
	}

	SampleComplex intens_data = GetIntensity();
	intens_data.Save(file);

	file.close();
	std::cout << "Intensity values of the sample have been successfully written to the file." << std::endl;
}

std::string SampleComplex::GetOutputFilePath(const std::string& filename) {
	std::string dir_name = "output";
	std::wstring wideDirectoryName(dir_name.begin(), dir_name.end());

	if (!CreateDirectoryW(wideDirectoryName.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
		std::cerr << "Failed to create directory 'output'" << std::endl;
	}

	std::string output_path = "./output/" + filename;
	return output_path;
}
