//Tool.hpp - 工具函数和界面相关函数的声明
#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <limits>
#include "StructDate.hpp"
#include "FileCtrl.hpp"

/**
 * 以指定颜色在控制台输出文本（仅 Windows）
 * @param text      要输出的字符串
 * @param colorName 颜色名称，支持：红色、绿色、蓝色、黄色、紫色、青色、白色、黑色等
 */
 //颜色输出函数，参数为要输出的字符串和颜色名称（仅 Windows）
void printColored(const std::string& text, const std::string& colorName);
bool GUI();//打印GUI界面，返回值为是否继续执行程序
void printCars(const MyZhan& zhan, const MyQueue& queue);//打印停车场内和便道上的车辆信息
void EnterCar(MyZhan& zhan, MyQueue& queue, const Setting& setting);//车辆入场
void ExitCar(MyZhan& zhan, MyQueue& queue, const Setting& setting,const int index);//车辆出场
void ChangeSetting(Setting& setting,MyZhan& park);//更改设置
//计算两个日期时间字符串之间的小时差，格式为 "YYYY-MM-DD HH:MM:SS"
int hoursDifference(const std::string& datetime1, const std::string& datetime2);
std::string getCurrentLocalTime();//获取当前本地时间，格式为 "YYYY-MM-DD HH:MM:SS"
std::string padTo20(const std::string& s);//将字符串填充到20个字符宽，右对齐，不足部分用空格填充
bool check(MyZhan& Park,MyQueue& queue,string& log);//检查停车场是否有空位，若有空位则将便道上的第一辆车移入停车场
std::string intToString(int num);//整数转字符串