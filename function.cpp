#include "function.h"


#define	ANGLE 0.05
#define	STEP 6
#define	PLUS 1.25
#define	MINUS (1 / PLUS)
#define SIN_ANGLE sin(ANGLE)
#define COS_ANGLE cos(ANGLE)
#define OFFSET 425


vector VerPyr[4];
vector VerCube[8];

vector VerPyrPr[4];
vector VerCubePr[8];

double center_pyr[4];
double center_cube[4];

matrix ROTATE(4, 4);
matrix ZOOM(4, 4);

matrix PROJECTION(4, 4);



void NoResize() {
	HWND hWnd = GetConsoleWindow();
	HMENU hm;

	hm = GetSystemMenu(hWnd, FALSE);
	RemoveMenu(hm, SC_SIZE, MF_BYCOMMAND | MF_REMOVE);
	RemoveMenu(hm, SC_MAXIMIZE, MF_BYCOMMAND | MF_REMOVE);
	DrawMenuBar(hWnd);
}
void DeleteCur() {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}
void SetSize(int x, int y, int w, int h) {
	system("mode con cols=20 lines=20");
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, x, y, w, h, TRUE);
}



void shwap(vector** v1, vector** v2, int d) {
	if ((**v1)[d] > (**v2)[d]) {
		vector* V = *v1;
		*v1 = *v2;
		*v2 = V;
	}
}

void create_pyr_PR() {
	for (size_t i = 0; i < 4; ++i)
		VerPyrPr[i] = PROJECTION * VerPyr[i];
}
void create_cube_PR() {
	for (size_t i = 0; i < 8; ++i)
		VerCubePr[i] = PROJECTION * VerCube[i];
}

double X(vector& v1, vector& v2, double y) {
	return ((v2[0] - v1[0]) * (y - v1[1]) / (v2[1] - v1[1]) + v1[0]);
}
void paint_trinagle(bitmap& bmp, vector& v1, vector& v2, vector& v3, COLORREF col) {
	double j;
	int k, K;
	double Xa, Xb;

	vector* v1_ptr = &v1, * v2_ptr = &v2, * v3_ptr = &v3;
	shwap(&v1_ptr, &v2_ptr, 1);
	shwap(&v1_ptr, &v3_ptr, 1);
	shwap(&v2_ptr, &v3_ptr, 1);

	for (j = (*v1_ptr)[1]; j < (*v2_ptr)[1]; j += 0.5) {

		if (j < 0) continue;
		if (j >= bmp.height) break;

		Xa = X((*v1_ptr), (*v2_ptr), j);
		Xb = X((*v1_ptr), (*v3_ptr), j);

		K = int(max(Xa, Xb));
		for (k = int(min(Xa, Xb)); k < K; ++k) {

			if (k < 0) continue;
			if (k >= bmp.width) break;

			bmp(int(j), k) = col;
		}
	}
	for (j = (*v2_ptr)[1]; j < (*v3_ptr)[1]; j += 0.5) {
		if (j < 0) continue;
		if (j >= bmp.height) break;

		Xa = X((*v2_ptr), (*v3_ptr), j);
		Xb = X((*v1_ptr), (*v3_ptr), j);

		K = int(max(Xa, Xb));
		for (k = int(min(Xa, Xb)); k < K; ++k) {

			if (k < 0) continue;
			if (k >= bmp.width) break;

			bmp(int(j), k) = col;
		}
	}
}

double find_center(vector& v1, vector& v2, vector& v3) {
	return  (v1[2] + v2[2] + v3[2]);
}
double find_center(vector& v1, vector& v2, vector& v3, vector& v4) {
	return  (v1[2] + v2[2] + v3[2] + v4[2]);
}

double min_pyr_y() {
	size_t i;
	double min = VerPyrPr[0][1];
	for (i = 1; i < 4; ++i)
		if (VerPyrPr[i][1] < min)
			min = VerPyrPr[i][1];
	return min;
}
double min_cube_y() {
	size_t i;
	double min = VerCubePr[0][1];
	for (i = 1; i < 8; ++i)
		if (VerCubePr[i][1] < min)
			min = VerCubePr[i][1];
	return min;
}

void clear_pyramid(bitmap& bmp) {
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[2], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[1], VerPyrPr[2], VerPyrPr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[2], VerPyrPr[3], RGB_BMP(255, 255, 255));

	size_t i;
	double d = OFFSET - min_pyr_y();
	for (i = 0; i < 4; ++i) {
		VerPyrPr[i][1] += d;
	}

	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[2], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[1], VerPyrPr[2], VerPyrPr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[2], VerPyrPr[3], RGB_BMP(255, 255, 255));

	for (i = 0; i < 4; ++i) {
		VerPyrPr[i][1] -= d;
	}
}
void shadow_pyramid(bitmap& bmp) {
	size_t i;
	double d = OFFSET - min_pyr_y();
	for (i = 0; i < 4; ++i) {
		VerPyrPr[i][1] += d;
	}

	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[2], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerPyrPr[1], VerPyrPr[2], VerPyrPr[3], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[3], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[2], VerPyrPr[3], RGB_BMP(70, 70, 70));

	for (i = 0; i < 4; ++i) {
		VerPyrPr[i][1] -= d;
	}
}
void paint_pyramid(bitmap& bmp) {
	int i, j, k;
	int tmp[4] = { 0, 1, 2, 3 };

	double swp;
	double cntr[4];
	cntr[0] = find_center(VerPyrPr[0], VerPyrPr[1], VerPyrPr[2]);
	cntr[1] = find_center(VerPyrPr[1], VerPyrPr[2], VerPyrPr[3]);
	cntr[2] = find_center(VerPyrPr[0], VerPyrPr[1], VerPyrPr[3]);
	cntr[3] = find_center(VerPyrPr[0], VerPyrPr[2], VerPyrPr[3]);

	for (i = 0; i < 4; ++i) {
		k = i;
		for (j = i; j < 4; ++j)
			if (cntr[k] > cntr[j])
				k = j;
		if (i != k) {
			swp = cntr[i];
			cntr[i] = cntr[k];
			cntr[k] = swp;

			j = tmp[i];
			tmp[i] = tmp[k];
			tmp[k] = j;
		}
	}
	for (i = 0; i < 4; ++i) {
		switch (tmp[i]) {
		case 0:
			paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[2], RGB_BMP(220, 33, 245));
			break;
		case 1:
			paint_trinagle(bmp, VerPyrPr[1], VerPyrPr[2], VerPyrPr[3], RGB_BMP(15, 222, 17));
			break;
		case 2:
			paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[1], VerPyrPr[3], RGB_BMP(72, 123, 159));
			break;
		case 3:
			paint_trinagle(bmp, VerPyrPr[0], VerPyrPr[2], VerPyrPr[3], RGB_BMP(84, 148, 31));
			break;
		}
	}
}

void clear_cube(bitmap& bmp) {
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[2], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[2], VerCubePr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[4], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[4], VerCubePr[5], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[2], VerCubePr[5], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[5], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[3], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[6], VerCubePr[7], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[3], VerCubePr[4], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[4], VerCubePr[7], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[5], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[6], VerCubePr[7], RGB_BMP(255, 255, 255));

	size_t i;
	double d = OFFSET - min_cube_y();
	for (i = 0; i < 8; ++i) {
		VerCubePr[i][1] += d;
	}
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[2], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[2], VerCubePr[3], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[4], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[4], VerCubePr[5], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[2], VerCubePr[5], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[5], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[3], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[6], VerCubePr[7], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[3], VerCubePr[4], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[4], VerCubePr[7], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[5], VerCubePr[6], RGB_BMP(255, 255, 255));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[6], VerCubePr[7], RGB_BMP(255, 255, 255));

	for (i = 0; i < 8; ++i) {
		VerCubePr[i][1] -= d;
	}
}
void shadow_cube(bitmap& bmp) {
	size_t i;
	double d = OFFSET - min_cube_y();
	for (i = 0; i < 8; ++i) {
		VerCubePr[i][1] += d;
	}
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[2], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[2], VerCubePr[3], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[4], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[4], VerCubePr[5], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[1], VerCubePr[2], VerCubePr[5], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[5], VerCubePr[6], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[2], VerCubePr[3], VerCubePr[6], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[6], VerCubePr[7], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[0], VerCubePr[3], VerCubePr[4], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[3], VerCubePr[4], VerCubePr[7], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[5], VerCubePr[6], RGB_BMP(70, 70, 70));
	paint_trinagle(bmp, VerCubePr[4], VerCubePr[6], VerCubePr[7], RGB_BMP(70, 70, 70));

	for (i = 0; i < 8; ++i) {
		VerCubePr[i][1] -= d;
	}
}
void paint_cube(bitmap& bmp) {
	size_t i, j, k;
	int tmp[6] = { 0, 1, 2, 3, 4, 5 };
	double swp;
	double cntr[6];

	cntr[0] = find_center(VerCubePr[0], VerCubePr[1], VerCubePr[2], VerCubePr[3]);
	cntr[1] = find_center(VerCubePr[0], VerCubePr[1], VerCubePr[4], VerCubePr[5]);
	cntr[2] = find_center(VerCubePr[1], VerCubePr[2], VerCubePr[5], VerCubePr[6]);
	cntr[3] = find_center(VerCubePr[2], VerCubePr[3], VerCubePr[6], VerCubePr[7]);
	cntr[4] = find_center(VerCubePr[0], VerCubePr[3], VerCubePr[4], VerCubePr[7]);
	cntr[5] = find_center(VerCubePr[4], VerCubePr[5], VerCubePr[6], VerCubePr[7]);

	for (i = 0; i < 6; ++i) {
		k = i;
		for (j = i; j < 6; ++j)
			if (cntr[k] > cntr[j])
				k = j;
		if (i != k) {
			swp = cntr[i];
			cntr[i] = cntr[k];
			cntr[k] = swp;

			j = tmp[i];
			tmp[i] = tmp[k];
			tmp[k] = j;
		}
	}
	for (i = 0; i < 6; ++i) {
		switch (tmp[i]) {
		case 0:
			paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[2], RGB_BMP(220, 33, 245));
			paint_trinagle(bmp, VerCubePr[0], VerCubePr[2], VerCubePr[3], RGB_BMP(220, 33, 245));
			break;
		case 1:
			paint_trinagle(bmp, VerCubePr[0], VerCubePr[1], VerCubePr[4], RGB_BMP(72, 123, 159));
			paint_trinagle(bmp, VerCubePr[1], VerCubePr[4], VerCubePr[5], RGB_BMP(72, 123, 159));
			break;
		case 2:
			paint_trinagle(bmp, VerCubePr[1], VerCubePr[2], VerCubePr[5], RGB_BMP(123, 104, 238));
			paint_trinagle(bmp, VerCubePr[2], VerCubePr[5], VerCubePr[6], RGB_BMP(123, 104, 238));
			break;
		case 3:
			paint_trinagle(bmp, VerCubePr[2], VerCubePr[3], VerCubePr[6], RGB_BMP(218, 190, 30));
			paint_trinagle(bmp, VerCubePr[3], VerCubePr[6], VerCubePr[7], RGB_BMP(218, 190, 30));
			break;
		case 4:
			paint_trinagle(bmp, VerCubePr[0], VerCubePr[3], VerCubePr[4], RGB_BMP(144, 88, 125));
			paint_trinagle(bmp, VerCubePr[3], VerCubePr[4], VerCubePr[7], RGB_BMP(144, 88, 125));
			break;
		case 5:
			paint_trinagle(bmp, VerCubePr[4], VerCubePr[5], VerCubePr[6], RGB_BMP(15, 222, 17));
			paint_trinagle(bmp, VerCubePr[4], VerCubePr[6], VerCubePr[7], RGB_BMP(15, 222, 17));
			break;
		}
	}
}

void set_center_pyr() {
	center_pyr[0] = 0.0;
	center_pyr[1] = 0.0;
	center_pyr[2] = 0.0;
	center_pyr[3] = 1.0;
	for (size_t i = 0; i < 4; ++i) {
		center_pyr[0] += VerPyr[i][0];
		center_pyr[1] += VerPyr[i][1];
		center_pyr[2] += VerPyr[i][2];
	}
	center_pyr[0] /= 4.0;
	center_pyr[1] /= 4.0;
	center_pyr[2] /= 4.0;
}
void set_center_cube() {
	center_cube[0] = 0.0;
	center_cube[1] = 0.0;
	center_cube[2] = 0.0;
	center_cube[3] = 1.0;
	for (size_t i = 0; i < 8; ++i) {
		center_cube[0] += VerCube[i][0];
		center_cube[1] += VerCube[i][1];
		center_cube[2] += VerCube[i][2];
	}
	center_cube[0] /= 8.0;
	center_cube[1] /= 8.0;
	center_cube[2] /= 8.0;
}

void rotate_pyramid(bitmap& bmp, char ch, char pn) {
	size_t i;
	clear_pyramid(bmp);
	switch (ch) {
	case 'x':
		switch (pn) {
		case '+':
			ROTATE.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, COS_ANGLE, SIN_ANGLE, -COS_ANGLE * center_pyr[1] + center_pyr[1] - SIN_ANGLE * center_pyr[2],
				0.0, -SIN_ANGLE, COS_ANGLE, SIN_ANGLE * center_pyr[1] + center_pyr[2] - COS_ANGLE * center_pyr[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, COS_ANGLE, -SIN_ANGLE, -COS_ANGLE * center_pyr[1] + center_pyr[1] + SIN_ANGLE * center_pyr[2],
				0.0, SIN_ANGLE, COS_ANGLE, -SIN_ANGLE * center_pyr[1] + center_pyr[2] - COS_ANGLE * center_pyr[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	case 'y':
		switch (pn) {
		case '+':
			ROTATE.set(
				COS_ANGLE, 0.0, -SIN_ANGLE, -COS_ANGLE * center_pyr[0] + center_pyr[0] + SIN_ANGLE * center_pyr[2],
				0.0, 1.0, 0.0, 0.0,
				SIN_ANGLE, 0.0, COS_ANGLE, -SIN_ANGLE * center_pyr[0] + center_pyr[2] - COS_ANGLE * center_pyr[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				COS_ANGLE, 0.0, SIN_ANGLE, -COS_ANGLE * center_pyr[0] + center_pyr[0] - SIN_ANGLE * center_pyr[2],
				0.0, 1.0, 0.0, 0.0,
				-SIN_ANGLE, 0.0, COS_ANGLE, SIN_ANGLE * center_pyr[0] + center_pyr[2] - COS_ANGLE * center_pyr[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	case 'z':
		switch (pn) {
		case '+':
			ROTATE.set(
				COS_ANGLE, SIN_ANGLE, 0.0, -COS_ANGLE * center_pyr[0] + center_pyr[0] - SIN_ANGLE * center_pyr[1],
				-SIN_ANGLE, COS_ANGLE, 0.0, SIN_ANGLE * center_pyr[0] + center_pyr[1] - COS_ANGLE * center_pyr[1],
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				COS_ANGLE, -SIN_ANGLE, 0.0, -COS_ANGLE * center_pyr[0] + center_pyr[0] + SIN_ANGLE * center_pyr[1],
				SIN_ANGLE, COS_ANGLE, 0.0, -SIN_ANGLE * center_pyr[0] + center_pyr[1] - COS_ANGLE * center_pyr[1],
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	default:
		return;
	}
	for (i = 0; i < 4; ++i) {
		VerPyr[i] = ROTATE * VerPyr[i];
	}
	create_pyr_PR();
	shadow_pyramid(bmp);
	paint_pyramid(bmp);
}
void move_pyramid(bitmap& bmp, char ch, char pn) {
	size_t i, f = (ch == 'x') ? f = 0 : f = 1;
	int step;
	clear_pyramid(bmp);
	if (pn == '+')
		step = STEP;
	else
		step = -STEP;
	for (i = 0; i < 4; ++i) {
		VerPyr[i][f] += step;
	}
	center_pyr[f] += step;
	create_pyr_PR();
	shadow_pyramid(bmp);
	paint_pyramid(bmp);
}
void zoom_pyramid(bitmap& bmp, char ch, char) {
	size_t i;
	clear_pyramid(bmp);
	if (ch == '+') {
		ZOOM.set(
			PLUS, 0.0, 0.0, -PLUS * center_pyr[0] + center_pyr[0],
			0.0, PLUS, 0.0, -PLUS * center_pyr[1] + center_pyr[1],
			0.0, 0.0, PLUS, -PLUS * center_pyr[2] + center_pyr[2],
			0.0, 0.0, 0.0, 1.0
		);
	}
	else {
		ZOOM.set(
			MINUS, 0.0, 0.0, -MINUS * center_pyr[0] + center_pyr[0],
			0.0, MINUS, 0.0, -MINUS * center_pyr[1] + center_pyr[1],
			0.0, 0.0, MINUS, -MINUS * center_pyr[2] + center_pyr[2],
			0.0, 0.0, 0.0, 1.0
		);
	}
	for (i = 0; i < 4; ++i) {
		VerPyr[i] = ZOOM * VerPyr[i];
	}
	create_pyr_PR();
	shadow_pyramid(bmp);
	paint_pyramid(bmp);
}

void rotate_cube(bitmap& bmp, char ch, char pn) {
	size_t i;
	clear_cube(bmp);
	switch (ch) {
	case 'x':
		switch (pn) {
		case '+':
			ROTATE.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, COS_ANGLE, SIN_ANGLE, -COS_ANGLE * center_cube[1] + center_cube[1] - SIN_ANGLE * center_cube[2],
				0.0, -SIN_ANGLE, COS_ANGLE, SIN_ANGLE * center_cube[1] + center_cube[2] - COS_ANGLE * center_cube[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				1.0, 0.0, 0.0, 0.0,
				0.0, COS_ANGLE, -SIN_ANGLE, -COS_ANGLE * center_cube[1] + center_cube[1] + SIN_ANGLE * center_cube[2],
				0.0, SIN_ANGLE, COS_ANGLE, -SIN_ANGLE * center_cube[1] + center_cube[2] - COS_ANGLE * center_cube[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	case 'y':
		switch (pn) {
		case '+':
			ROTATE.set(
				COS_ANGLE, 0.0, -SIN_ANGLE, -COS_ANGLE * center_cube[0] + center_cube[0] + SIN_ANGLE * center_cube[2],
				0.0, 1.0, 0.0, 0.0,
				SIN_ANGLE, 0.0, COS_ANGLE, -SIN_ANGLE * center_cube[0] + center_cube[2] - COS_ANGLE * center_cube[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				COS_ANGLE, 0.0, SIN_ANGLE, -COS_ANGLE * center_cube[0] + center_cube[0] - SIN_ANGLE * center_cube[2],
				0.0, 1.0, 0.0, 0.0,
				-SIN_ANGLE, 0.0, COS_ANGLE, SIN_ANGLE * center_cube[0] + center_cube[2] - COS_ANGLE * center_cube[2],
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	case 'z':
		switch (pn) {
		case '+':
			ROTATE.set(
				COS_ANGLE, SIN_ANGLE, 0.0, -COS_ANGLE * center_cube[0] + center_cube[0] - SIN_ANGLE * center_cube[1],
				-SIN_ANGLE, COS_ANGLE, 0.0, SIN_ANGLE * center_cube[0] + center_cube[1] - COS_ANGLE * center_cube[1],
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);
			break;
		case '-':
			ROTATE.set(
				COS_ANGLE, -SIN_ANGLE, 0.0, -COS_ANGLE * center_cube[0] + center_cube[0] + SIN_ANGLE * center_cube[1],
				SIN_ANGLE, COS_ANGLE, 0.0, -SIN_ANGLE * center_cube[0] + center_cube[1] - COS_ANGLE * center_cube[1],
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);
			break;
		}
		break;
	default:
		return;
	}
	for (i = 0; i < 8; ++i) {
		VerCube[i] = ROTATE * VerCube[i];
	}
	create_cube_PR();
	shadow_cube(bmp);
	paint_cube(bmp);
}
void move_cube(bitmap& bmp, char ch, char pn) {
	size_t i, f = (ch == 'x') ? f = 0 : f = 1;
	int step;
	clear_cube(bmp);
	if (pn == '+')
		step = STEP;
	else
		step = -STEP;
	for (i = 0; i < 8; ++i) {
		VerCube[i][f] += step;
	}
	center_cube[f] += step;
	create_cube_PR();
	shadow_cube(bmp);
	paint_cube(bmp);
}
void zoom_cube(bitmap& bmp, char ch, char) {
	size_t i;
	clear_cube(bmp);
	if (ch == '+') {
		ZOOM.set(
			PLUS, 0.0, 0.0, -PLUS * center_cube[0] + center_cube[0],
			0.0, PLUS, 0.0, -PLUS * center_cube[1] + center_cube[1],
			0.0, 0.0, PLUS, -PLUS * center_cube[2] + center_cube[2],
			0.0, 0.0, 0.0, 1.0
		);
	}
	else {
		ZOOM.set(
			MINUS, 0.0, 0.0, -MINUS * center_cube[0] + center_cube[0],
			0.0, MINUS, 0.0, -MINUS * center_cube[1] + center_cube[1],
			0.0, 0.0, MINUS, -MINUS * center_cube[2] + center_cube[2],
			0.0, 0.0, 0.0, 1.0
		);
	}
	for (i = 0; i < 8; ++i) {
		VerCube[i] = ZOOM * VerCube[i];
	}
	create_cube_PR();
	shadow_cube(bmp);
	paint_cube(bmp);
}

void init() {
	for (size_t i = 0; i < 4; ++i) {
		VerPyr[i].resize(4);
		VerPyrPr[i].resize(4);
	}
	for (size_t i = 0; i < 8; ++i) {
		VerCube[i].resize(4);
		VerCubePr[i].resize(4);
	}
	PROJECTION.set(
		0.707, -0.408, 0.577, 0.0,
		0.0, 0.816, 0.577, 0.0,
		-0.707, -0.408, 0.577, 0.0,
		0.0, 0.0, 0.0, 1.0
	);

	VerPyr[0] = { 130.0, 130.0, 130.0, 1.0 };
	VerPyr[1] = { 230.0, 130.0, 130.0, 1.0 };
	VerPyr[2] = { 180.0, 230.0, 130.0, 1.0 };
	VerPyr[3] = { 180.0, 163.3, 230.0, 1.0 };

	VerCube[0] = { 90.0, 90.0, 90.0, 1.0 };
	VerCube[1] = { 140.0, 90.0, 90.0, 1.0 };
	VerCube[2] = { 140.0, 190.0, 90.0, 1.0 };
	VerCube[3] = { 90.0, 190.0, 90.0, 1.0 };
	VerCube[4] = { 90.0, 90.0, 140.0, 1.0 };
	VerCube[5] = { 140.0, 90.0, 140.0, 1.0 };
	VerCube[6] = { 140.0, 190.0, 140.0, 1.0 };
	VerCube[7] = { 90.0, 190.0, 140.0, 1.0 };

	set_center_pyr();
	set_center_cube();

	create_pyr_PR();
	create_cube_PR();
}