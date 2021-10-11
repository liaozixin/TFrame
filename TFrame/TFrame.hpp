#pragma once
#include <Windows.h>
#include <initializer_list>
#include "KeyMap.hpp"

class Frame_Buffer
{
public:
	Frame_Buffer(int, int, int, int);
	const unsigned char* getColBuf() const;
	void setBuffer(std::initializer_list<int>);
	void setBuffer(int, int);
	void clearn();
public:
	const int size, width, height, channels;
	int init_gray;
private:
	unsigned char* _color_buffer;
private:
	inline void initFB(int);
	inline int getIndex(int, int);
};

struct MousePos
{
	int x;
	int y;
};


class TWin
{
public:
	TWin(int, int, const char*);
	~TWin();
	bool pollEvent();
	void initBG(int R, int G, int B);
	void drawFB(Frame_Buffer*);
	void destory();

public:
	static bool ifKeyDown(int);
public:
	MousePos mouse_pos;
private:
	int _width, _heigth;
	wchar_t* _title;

	HWND _hwnd;
	HINSTANCE _hinstance;
	WNDCLASS _wndclass;
	MSG _msg;

	unsigned char* _bg_color;
private:
	bool initWin();
	bool geneWin();
	static LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM);
};

