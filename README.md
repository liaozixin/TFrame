# TFrame绘图框架
基于win32封装，使得程序不必通过WinMain作为入口函数，框架十分轻量级，可以满足基本的绘图需要。ps.设计之初是作为软光栅渲染器的底层绘图接口使用。

## 目录结构
```
KeyMap.hpp
main.cpp
README.md
TFrame.cpp
TFrame.hpp
```
KeyMap.hpp为框架定义的键盘映射   
TFrame.cpp框架中各类方法的实现   
TFrame.hpp框架中各类的定义

## 框架介绍
* 简化Win32绘图过程中的复杂过程，使用时只需向Frame_Buffer指定位置绘制即可。
* 可直接获取运行时鼠标在窗口中的位置。
* 监听键盘按键。

## 使用方法
引入TFrame.hpp即可。  
详见main.cpp其中包含了一些典型用法。  

## 类中各方法详细介绍
* Frame_Buffer
   * 变量 
      * size---Frame_Buffer在内存中的实际长度，其值等于width * height * channels。
      * width---Frame的宽度。
      * height---Frame的高度。
      * channels---Frame的通道数。
      * _color_buffer---指向Frame_Buffer中实际储存像素值的内存空间。
      * init_gray---初始化Frame背景灰度值,用以区分前景点与背景点。
   * 方法
      * Frame_Buffer(int width, int height, int channels, int gray)   
      width：Frame_Buffer的宽度，应与窗口宽度相同。 
      height：Frame_Buffer的高度，应与窗口高度相同。   
      channels：Frame_Buffer的通道数， 应与窗口通道数相同，因为窗口绘制时只支持RGB模式，应此此项应设为3。  
      gray：Frame_Buffer?????????

      * const unsigned char* getColBuf()   
      获取指向Frame_Buffer中保存实际像素颜色值的内存空间。

      * void setBuffer(std::initializer_list<int> list)   
      设置Frame中指定位置的像素值  
      例如：  
         setBuffer({ Point.x, Point.y, R, G, B })

      * void setBuffer(int index, int val)   
      以索引的方式直接设置_color_buffer中指定位置的值。

      * void clearn()   
      清空Frame_Buffer中的内容。

      * void initFB(int init_gray)   
      用init_gray灰度初始化Frame_Buffer。

      * int getIndex(int x, int y)   
      获取Frame中坐标为(x,y)的像素点在_color_buffer中的实际位置。
* TWin   
   * 变量
      * _width---窗口宽。
      * _heigth---窗口高。
      * _title---窗口标题。
      * _hwnd---窗口句柄。
      * _hinstance---窗口句柄实例。
      * _wndclass---窗口类。
      * _msg---窗口产生消息。
      * _bg_color---窗口背景颜色。
      * mouse_pos---窗口中鼠标位置。
   * 方法
      * TWin(int width, int height, const char* title)   
      设置窗口宽高及标题。

      * bool pollEvent()    
      发送窗口产生的各类消息，配合while使用，详见main.cpp。

      * void initBG(int R, int G, int B)  
      初始化窗口背景。

      * void drawFB(Frame_Buffer* fb)   
      绘制传入的Frame_Buffer。

      * void destory()   
      窗口销毁函数。

      * static bool ifKeyDown(int Key)   
      监听按键Key是否按下，使用见main.cpp。

      * bool initWin()   
      窗口初始化函数（自动调用）。

      * bool geneWin()   
      产生窗口函数（自动调用）。 

      * static LRESULT CALLBACK winProc(HWND, UINT, WPARAM, LPARAM)   
      窗口消息处理函数。
## 其他
如有问题欢迎提出。代码在VS2019下编译。


