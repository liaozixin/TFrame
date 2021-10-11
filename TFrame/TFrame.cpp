#include "TFrame.hpp"
#include <assert.h>

#include <string>
#include <map>
#include <optional>
#include <vector>
#include <windowsx.h>

#include <iostream>

class WinRegister
{
public:
    bool Register(HWND hwnd, TWin* win)
    {
        auto pt = _list.emplace(hwnd, win);
        return pt.second;
    };

    bool cancellation(HWND hwnd)
    {
        auto iter = _list.find(hwnd);
        if (iter != _list.end())
        {
            _list.erase(iter);
            return true;
        }
        else
            return false;

    };

    std::optional<TWin*> find(HWND hwnd)
    {
        auto iter = _list.find(hwnd);
        if (iter != _list.end())
            return { iter->second };
        else
            return std::nullopt;
    };
private:
    std::map<HWND, TWin*> _list;
};

static WinRegister wr;



TWin::TWin(int width, int height, const char* title)
{
	assert(width >= 100 && height >= 100 && title != nullptr);
	_width = width;
	_heigth = height;
    _bg_color = new unsigned char[width * height * 3];

    //窄字符转换宽字符
    int buffer_size = MultiByteToWideChar(CP_ACP, 0, title, -1, NULL, 0);
    _title = new wchar_t[buffer_size];
    MultiByteToWideChar(CP_ACP, 0, title, -1, _title, buffer_size);

    mouse_pos.x = 0;
    mouse_pos.y = 0;
    assert(initWin() && geneWin());
}

TWin::~TWin()
{
    wr.cancellation(_hwnd);
    delete(_bg_color);
}

bool TWin::pollEvent()
{
    BOOL bRet;
    bRet = GetMessage(&_msg, _hwnd, 0, 0);
    
    if (bRet == 0 || bRet == -1)
        return false;
    else
    {
        TranslateMessage(&_msg);
        DispatchMessage(&_msg);
        return true;
    }
}

void TWin::initBG(int R, int G, int B)
{
    int size = _width * _heigth * 3;
    for (size_t i = 0; i < size; i += 3)
    {
        _bg_color[i + 0] = B;
        _bg_color[i + 1] = G;
        _bg_color[i + 2] = R;
    }
}


void TWin::drawFB(Frame_Buffer* fb)
{
    HDC hDc = GetDC(_hwnd);
    
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = _width;
    bmi.bmiHeader.biHeight = _heigth;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = _width * _heigth * 3;

   
    for (size_t i = 0; i < fb->size; i++)
    {
        if (fb->getColBuf()[i] == fb->init_gray)
            fb->setBuffer(i, _bg_color[i]);
    }
    StretchDIBits(hDc, 0, 0, _width, _heigth,
        0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight,
        fb->getColBuf(), &bmi, DIB_RGB_COLORS, SRCCOPY);
    
   
    ReleaseDC(_hwnd, hDc);
}

void TWin::destory()
{
    DestroyWindow(_hwnd);
}

bool TWin::ifKeyDown(int key)
{
    return (GetAsyncKeyState(key) & 0x8000f);
}

bool TWin::initWin()
{
     _wndclass.style = CS_HREDRAW | CS_VREDRAW; //窗口高度、宽度、位置改变时重绘窗口。
    _wndclass.lpfnWndProc = winProc; //指定处理窗口消息函数
    _wndclass.cbWndExtra = 0; //是对窗口的，每实例化一个窗口都有这么一个附加空间。类似于C++类中的成员变量。
    _wndclass.cbClsExtra = 0; //是对类的（这里所说的类不是C++中的类，而是WNDCLASS结构的变量用RegisterClass注册后，在系统中记录的一组信息，是窗口的一个模板），用该类生成的所有窗口共享该附加空间。类似于C++类中的static变量。
    _wndclass.hInstance = _hinstance; //执行实体句柄。
    _wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //设置窗口图标
    _wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); //设置鼠标图标
    _wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0)); //窗口背景色
    _wndclass.lpszMenuName = NULL; //菜单
    _wndclass.lpszClassName = TEXT("MainWindow"); //类名

    if (!RegisterClass(&_wndclass))
        return false;
    return true;
}

bool TWin::geneWin()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeigth = GetSystemMetrics(SM_CYSCREEN);

    _hwnd = CreateWindow(
        TEXT("MainWindow"),
        _title,
        WS_OVERLAPPEDWINDOW ^ WS_SIZEBOX ^ WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX,
        screenWidth / 2 - _width / 2,
        screenHeigth / 2 - _heigth / 2,
        _width,
        _heigth,
        NULL,
        NULL,
        _hinstance,
        NULL);

    if (_hwnd == nullptr)
    {
        return false;
    }
    wr.Register(_hwnd, this);
    ShowWindow(_hwnd, SW_SHOW);
    return true;
}

LRESULT TWin::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    TWin* win{ nullptr };
    auto iter = wr.find(hwnd);
    if (iter.has_value())
        win = *iter;
   
    switch (msg)
    {
    case WM_MOUSEMOVE:
        if (win != nullptr)
        {
            win->mouse_pos.x = GET_X_LPARAM(lParam) > 0 ? GET_X_LPARAM(lParam) : 0;
            win->mouse_pos.y = GET_Y_LPARAM(lParam) > 0 ? GET_Y_LPARAM(lParam) : 0;
            SendMessageA(hwnd, WM_PAINT, wParam, lParam);
        }
        break;
    case WM_PAINT:

        break;
    case WM_ERASEBKGND:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}




Frame_Buffer::Frame_Buffer(int width, int height, int channels, int gray) :width(width), height(height),
                                                                        channels(channels), size(width* height* channels),
                                                                        _color_buffer(new unsigned char[size]),
                                                                        init_gray(gray)
{
    initFB(gray);
}



const unsigned char* Frame_Buffer::getColBuf() const
{
    return _color_buffer;
}

void Frame_Buffer::setBuffer(std::initializer_list<int> list)
{
    int num = list.size() - 2;
    assert(num == channels);
    std::vector<int> vals(list.size());

    {
        int i = 0;
        for (auto it = list.begin(); it != list.end(); ++it, ++i)
        {
            vals[i] = *it;
        }
    }
    
    if ((vals[0] < width && vals[0] >= 0) && (vals[1] < height && vals[1] >= 0))
    {
        int index = getIndex(vals[0], vals[1]);
        for (size_t i = 2; i < vals.size(); i++)
        {
            _color_buffer[index + i - 2] = vals[i];
        }
    }
}

void Frame_Buffer::setBuffer(int index, int val)
{
    _color_buffer[index] = val;
}

void Frame_Buffer::clearn()
{
    initFB(init_gray);
}


inline void Frame_Buffer::initFB(int init_gray)
{
    memset(_color_buffer, init_gray, size);
}

inline int Frame_Buffer::getIndex(int x, int y)
{
    return x * channels + (height - y - 1) * width * channels;
}


