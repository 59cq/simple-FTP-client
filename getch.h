#ifndef GETCH_H
#define GETCH_H
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// 修改终端的控制方式，1取消回显、确认 ２获取数据 3还原
static int getch(void)
{
    // 记录终端的配置信息
    struct termios _old;
    // 获取终端的配置信息
    tcgetattr(STDIN_FILENO,&_old);
    // 设置新的终端配置   
    struct termios _new = _old;
    // 取消确认、回显
    _new.c_lflag &= ~(ICANON|ECHO);
    // 设置终端配置信息
    tcsetattr(STDIN_FILENO,TCSANOW,&_new);

    // 在新模式下获取数据   
    int key_val = 0; 
    do{
    	key_val += getchar();
    }while(stdin->_IO_read_end - stdin->_IO_read_ptr);

    // 还原配置信息
    tcsetattr(STDIN_FILENO,TCSANOW,&_old); 
    return key_val; 
}

#endif//GETCH_H

