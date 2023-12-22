
#include "matrix.h"

matrix::matrix() : data(nullptr), rows(0), cols(0) {}
matrix::matrix(int N, int M) {
	data = nullptr;
	create(N, M);
}
matrix::matrix(const matrix& B) {
	int i;
	data = nullptr;
	create(B.rows, B.cols);
	for (i = 0; i < cols; ++i)
		data[i] = B.data[i];
}
matrix::matrix(matrix&& B) {
	data = B.data;
	rows = B.rows;
	cols = B.cols;
	B.data = nullptr;
}
matrix::~matrix() {
	create(0, 0);
}

void matrix::create(int N, int M) {
	int i;
	if (data) delete[] data;
	if (N && M) {
		data = new vector[M];
		for (i = 0; i < M; ++i)
			data[i].resize(N);
		rows = N;
		cols = M;
	}
	else {
		data = nullptr;
		rows = cols = 0;
	}
}
void matrix::set(double db ...) {
	int i, j;
	va_list ap;
	va_start(ap, db);
	data[0][0] = db;
	for (j = 1; j < cols; j++) {
		data[j][0] = (double)va_arg(ap, double);
	}
	for (i = 1; i < rows; ++i)
		for (j = 0; j < cols; j++) {
			data[j][i] = (double)va_arg(ap, double);
		}
	va_end(ap);
}

matrix& matrix::operator=(const matrix& B) {
	int i;
	if (this != &B) {
		if (rows != B.rows || cols != B.cols) {
			delete[] data;
			create(B.rows, B.cols);
		}
		for (i = 0; i < cols; ++i)
			data[i] = B.data[i];
	}
	return *this;
}
matrix& matrix::operator=(matrix&& B) {
	if (this != &B) {
		delete[] data;
		data = B.data;
		rows = B.rows;
		cols = B.cols;
		B.data = nullptr;
		B.rows = 0;
		B.cols = 0;
	}
	return *this;
}

vector operator*(const matrix& A, const vector& a) {
	int i, j;
	vector res;
	res.resize(a.size());
	for (i = 0; i < A.rows; ++i) {
		res[i] = 0;
		for (j = 0; j < A.cols; ++j) {
			res[i] += A.data[j][i] * a[j];
		}
	}
	return res;
}







