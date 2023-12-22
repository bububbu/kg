#include "bitmap.h"

bitmap::bitmap(int w, int h, int x0, int y0, COLORREF col) : width(w), height(h), x(x0), y(y0) {
	hDC = GetDC(GetConsoleWindow());
	color = col;
	mapIMG = new COLORREF[width * height * k];
	for (int i = 0; i < width * height * k; ++i)
		mapIMG[i] = col;
	bmp = { 0, w, h, 32 * w, 1, 32, NULL };
	bmp.bmBits = (COLORREF*)mapIMG;
}
bitmap::~bitmap() {
	delete[] mapIMG;
}
COLORREF& bitmap::operator()(int i, int j) {
	return mapIMG[i * width * k + j];
}
void bitmap::draw() {
	hBitmap = CreateBitmapIndirect(&bmp);

	// ������� �������� ������, �����������
	// � ���������� �����������
	hMemDC = CreateCompatibleDC(hDC);

	// �������� ����������� bitmap � �������� ������
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// ���� �� ���� ������, ���������� ������
	if (hOldbm) {
		// ��� ��������� ������ ������������� ��� ��
		// ����� �����������, ��� ������������ �
		// ��������� �����������
		SetMapMode(hMemDC, GetMapMode(hDC));

		// ���������� ������� �����������
		GetObject(hBitmap, sizeof(BITMAP), (COLORREF*)&bm);

		ptSize.x = bm.bmWidth;   // ������
		ptSize.y = bm.bmHeight;  // ������

		// ����������� ���������� ���������� � ����������
		// ��� ���������� ������
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		// ����������� ���������� ���������� � ����������
		// ��� ��������� ������
		DPtoLP(hMemDC, &ptOrg, 1);

		// ������ ����������� bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		// ��������������� �������� ������
		SelectObject(hMemDC, hOldbm);
	}
	else {
		system("pause");
	}
	// ������� �������� ������
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}
