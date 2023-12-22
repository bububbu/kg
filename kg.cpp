#include <conio.h>
#include "function.h"

int main()
{

	system("color 28");
	SetSize(10, 20, 1255, 680);
	DeleteCur();
	NoResize();

	int c;
	char flag = '+';
	void(*move)(bitmap&, char, char) = move_pyramid;
	void(*rotate)(bitmap&, char, char) = rotate_pyramid;
	void(*zoom)(bitmap&, char, char) = zoom_pyramid;

	bitmap bmp1(
		606, 620, 10, 10, RGB_BMP(255, 255, 255)
	);
	bitmap bmp2(
		606, 620, 626, 10, RGB_BMP(255, 255, 255)
	);
	bitmap* bmp = &bmp1;

	init();

	move_pyramid(bmp1, 'x', '+');
	move_cube(bmp2, 'x', '+');
	bmp1.draw();
	bmp2.draw();

	while (1) {
		c = _getch();
		switch (c) {
		case 72:
			move(*bmp, 'y', '-');
			bmp->draw();
			break;
		case 77:
			move(*bmp, 'x', '+');
			bmp->draw();
			break;
		case 80:
			move(*bmp, 'y', '+');
			bmp->draw();
			break;
		case 75:
			move(*bmp, 'x', '-');
			bmp->draw();
			break;
		case 49:
			move = move_pyramid;
			rotate = rotate_pyramid;
			zoom = zoom_pyramid;
			bmp = &bmp1;
			break;
		case 50:
			move = move_cube;
			rotate = rotate_cube;
			zoom = zoom_cube;
			bmp = &bmp2;
			break;
		case 120:
			rotate(*bmp, 'x', flag);
			bmp->draw();
			break;
		case 121:
			rotate(*bmp, 'y', flag);
			bmp->draw();
			break;
		case 122:
			rotate(*bmp, 'z', flag);
			bmp->draw();
			break;
		case 32:
			(flag == '+') ? flag = '-' : flag = '+';
			break;
		case 43:
			zoom(*bmp, '+', '\0');
			bmp->draw();
			break;
		case 45:
			zoom(*bmp, '-', '\0');
			bmp->draw();
			break;
		}
	}

	return 0;
}


