#pragma once
#include <vector>
#include <cstdarg>

typedef std::vector<double> vector;

class matrix {
public:
	matrix();
	matrix(int, int);
	matrix(const matrix&);
	matrix(matrix&&);
	~matrix();
	void create(int, int);
	void set(double db ...);

	friend vector operator*(const matrix&, const vector& a);
	matrix& operator=(const matrix&);
	matrix& operator=(matrix&&);
private:
	int rows;
	int cols;
	vector* data;
};
