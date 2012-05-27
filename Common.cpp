#include "Common.h"

int random(int n) {
	return rand() % n;
}

string str(int x) {
    stringstream s;
    s << x;
    return s.str();
}

RECT CRect(int Left,int Top,int Right,int Bottom)
{
	RECT Ret;
	Ret.left=Left;
	Ret.top=Top;
	Ret.right=Right;
	Ret.bottom=Bottom;
	return Ret;
}