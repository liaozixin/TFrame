#include "TFrame.hpp"

#include <iostream>
#include "TFrame.hpp"

int main()
{
	TWin win(400, 400, "hello");//定义窗口
	win.initBG(255, 0, 0);//初始化窗口背景颜色
	Frame_Buffer fb(400, 400, 3, 0);//设置Frame_Buffer大小、通道数、背景灰度值。
	int x = 0;
	int y = 200;
	while (win.pollEvent())
	{
		fb.setBuffer({ x, y, 255, 255, 255 });
		win.drawFB(&fb);
		++x;
		if (TWin::ifKeyDown(TF_ESC))//判断ESC是否按下
			win.destory();
		std::cout << win.mouse_pos.x << " " << win.mouse_pos.y << std::endl;//打印鼠标坐标
	}
	return 0;
}