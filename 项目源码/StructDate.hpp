//StructDate.hpp
#pragma once
#include <string>

using namespace std;
//停车场设置
struct Setting {
	int MaxCarNumber;//停车场容量
	double PayPerHour;//每小时收费
};//车辆信息
struct Car {
	string id;//车牌号
	string EnterDate;//进入时间
};
//停车场类,顺序栈
class MyZhan
{
private:
	Car* Cars;//停车场内的车辆
	int CarNumber;//栈顶指针，指向停车场内最后一个空位
	int MaxCarNumber;//停车场容量
public:
	MyZhan(int MaxCarNumber);//构造停车场，参数为停车场容量
	~MyZhan();//析构函数
	bool push(Car NewCar);//入场
	Car pop();//出场
	Car top();//查看栈顶车辆信息
	int getCarNumber() const { return CarNumber; }//获取停车场内车辆数量
	int getMaxCarNumber() const { return MaxCarNumber; }//获取停车场容量
	Car operator[](int index) const { return Cars[index]; }//重载[]运算符，获取停车场内指定位置的车辆信息
};
//便道类，循环变长队列
class MyQueue
{
private:
	Car* Cars;//便道上的车辆
	int Front;//队头指针，指向便道上第一个车
	int Rear;//队尾指针，指向便道上最后一个车的下一个位置
	int MaxCarNumber;//便道容量
public:
	MyQueue();//构造便道
	~MyQueue();//析构函数
	void push(Car NewCar);//车辆进入便道
	Car pop();//车辆离开便道
	int getCarNumber() const { return ( Rear-Front + MaxCarNumber) % MaxCarNumber; }//获取便道上车辆数量
	Car front();//查看队头车辆信息
	Car rear();//查看队尾车辆信息
	bool isEmpty() { return Rear == Front; }//判断便道是否为空
	bool isFull() { return (Rear + 1) % MaxCarNumber == Front; }//判断便道是否已满
	void sizeUp();//便道容量加倍
	Car operator[](int index) const { return Cars[(Front + index) % MaxCarNumber]; }//重载[]运算符，获取便道上指定位置的车辆信息
};