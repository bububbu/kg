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

	// —оздаем контекст пам€ти, совместимый
	// с контекстом отображени€
	hMemDC = CreateCompatibleDC(hDC);

	// ¬ыбираем изображение bitmap в контекст пам€ти
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// ≈сли не было ошибок, продолжаем работу
	if (hOldbm) {
		// ƒл€ контекста пам€ти устанавливаем тот же
		// режим отображени€, что используетс€ в
		// контексте отображени€
		SetMapMode(hMemDC, GetMapMode(hDC));

		// ќпредел€ем размеры изображени€
		GetObject(hBitmap, sizeof(BITMAP), (COLORREF*)&bm);

		ptSize.x = bm.bmWidth;   // ширина
		ptSize.y = bm.bmHeight;  // высота

		// ѕреобразуем координаты устройства в логические
		// дл€ устройства вывода
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		// ѕреобразуем координаты устройства в логические
		// дл€ контекста пам€ти
		DPtoLP(hMemDC, &ptOrg, 1);

		// –исуем изображение bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		// ¬осстанавливаем контекст пам€ти
		SelectObject(hMemDC, hOldbm);
	}
	else {
		system("pause");
	}
	// ”дал€ем контекст пам€ти
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}
