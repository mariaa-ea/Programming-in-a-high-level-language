#include "SampleMatrix.h"

void SampleMatrix::CreateIdentityMatrix() {
	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < this->GetSizeY(); j++) {
			if (i == j) {
				(*this)(i, j) = 1.0; // Устанавливаем единицы на главной диагонали
			}
			else {
				(*this)(i, j) = 0.0; // Устанавливаем нули в остальных ячейках
			}
		}
	}
}

SampleMatrix SampleMatrix::operator*(const SampleMatrix& otherMatrix) {
	// Проверка на возможность умножения матриц
	// По определению произведения матриц можно перемножать только такие матрицы,
	//    у которых число столбцов в первой матрице равно
	//        числу строк во второй матрице. (x, y) -> (строки, столбцы)
	if (this->GetSizeY() != otherMatrix.GetSizeX()) {
		throw std::invalid_argument("Matrix dimensions are not compatible for multiplication.");
	}

	SampleMatrix result(this->GetSizeX(), otherMatrix.GetSizeY());
	for (int i = 0; i < this->GetSizeX(); i++) {
		for (int j = 0; j < otherMatrix.GetSizeY(); j++) {
			double sum = 0.0;
			for (int k = 0; k < this->GetSizeY(); k++) {
				sum += (*this)(i, k) * otherMatrix(k, j);
			}
			result(i, j) = sum;
		}
	}
	return result;
}

SampleMatrix& SampleMatrix::operator*=(double value) {
	for (int i = 0; i < GetSizeX(); ++i) {
		for (int j = 0; j < GetSizeY(); ++j) {
			(*this)(i, j) *= value;
		}
	}
	return *this;
}

SampleMatrix& SampleMatrix::operator*=(const SampleMatrix& otherMatrix) {
	*this = *this * otherMatrix;
	return *this;
}

SampleMatrix SampleMatrix::operator/(const SampleMatrix& otherMatrix) {
	throw std::logic_error("Matrix division is not defined. Use other methods for matrix operations.");
}

SampleMatrix& SampleMatrix::operator/=(const SampleMatrix& otherMatrix) {
	throw std::logic_error("Matrix division is not defined. Use other methods for matrix operations.");
}

SampleMatrix SampleMatrix::operator~() {
	SampleMatrix transposed(GetSizeY(), GetSizeX());
	for (int i = 0; i < GetSizeX(); ++i) {
		for (int j = 0; j < GetSizeY(); ++j) {
			transposed(j, i) = (*this)(i, j);
		}
	}
	return transposed;
}

SampleMatrix SampleMatrix::GetInverseMatrix() const {
	if (GetSizeX() != GetSizeY()) {
		throw std::invalid_argument("Matrix is not square, inverse matrix cannot be computed.");
	}

	int n = GetSizeX();
	SampleMatrix identity(n);
	SampleMatrix aug_matr(n, 2 * n);

	// Создаем расширенную матрицу [A | I]
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			aug_matr(i, j) = (*this)(i, j);
			aug_matr(i, j + n) = (i == j) ? 1.0 : 0.0;
		}
	}

	// Прямой ход метода Гаусса
	for (int i = 0; i < n; ++i) {
		double pivot = aug_matr(i, i);
		if (pivot == 0.0) {
			throw std::runtime_error("Matrix is singular, inverse matrix cannot be computed.");
		}

		for (int j = 0; j < 2 * n; ++j) {
			aug_matr(i, j) /= pivot;
		}

		for (int k = 0; k < n; ++k) {
			if (k != i) {
				double factor = aug_matr(k, i);
				for (int j = 0; j < 2 * n; ++j) {
					aug_matr(k, j) -= factor * aug_matr(i, j);
				}
			}
		}
	}

	// Извлекаем обратную матрицу из расширенной матрицы
	SampleMatrix inv_matr(n);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			inv_matr(i, j) = aug_matr(i, j + n);
		}
	}
	return inv_matr;
}

bool SampleMatrix::operator==(const SampleMatrix& otherMatrix) const {
	if (GetSizeX() != otherMatrix.GetSizeX() || GetSizeY() != otherMatrix.GetSizeY()) {
		return false;
	}

	for (int i = 0; i < GetSizeX(); ++i) {
		for (int j = 0; j < GetSizeY(); ++j) {
			if ((*this)(i, j) != otherMatrix(i, j)) {
				return false;
			}
		}
	}
	return true;
}
