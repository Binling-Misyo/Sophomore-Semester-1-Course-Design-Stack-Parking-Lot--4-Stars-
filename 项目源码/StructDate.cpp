//  StructDate.cpp
#include "StructDate.hpp"
// ---------- MyZhan 成员函数实现 ----------
MyZhan::MyZhan(int MaxCarNumber)
{
    this->MaxCarNumber = MaxCarNumber;
    Cars = new Car[MaxCarNumber];
    CarNumber = 0;
}
MyZhan::~MyZhan()
{
    delete[] Cars;
}
bool MyZhan::push(Car NewCar)
{
    if (CarNumber < MaxCarNumber) {
        Cars[CarNumber++] = NewCar;
        return true;
    }
    return false;
}
Car MyZhan::pop()
{
    if (CarNumber > 0) {
        return Cars[--CarNumber];
    }
    return Car();   // 返回默认构造的空Car
}
Car MyZhan::top()
{
    if (CarNumber > 0) {
        return Cars[CarNumber - 1];
    }
    return Car();
}
// ---------- MyQueue 成员函数实现 ----------
MyQueue::MyQueue()
{
    MaxCarNumber = 20;
    Cars = new Car[MaxCarNumber];
    Front = 0;
    Rear = 0;
}

MyQueue::~MyQueue()
{
    delete[] Cars;
}

void MyQueue::push(Car NewCar)
{
    // 如果队列已满，自动扩容
    if (isFull()) {
        sizeUp();
    }
    Cars[Rear] = NewCar;
    Rear = (Rear + 1) % MaxCarNumber;
}

Car MyQueue::pop()
{
    if (isEmpty()) {
        return Car();   // 返回空车
    }
    Car temp = Cars[Front];
    Front = (Front + 1) % MaxCarNumber;
    return temp;
}

Car MyQueue::front()
{
    if (isEmpty()) {
        return Car();
    }
    return Cars[Front];
}

Car MyQueue::rear()
{
    if (isEmpty()) {
        return Car();
    }
    // 队尾元素在 Rear 的前一个位置（循环处理）
    int last = (Rear - 1 + MaxCarNumber) % MaxCarNumber;
    return Cars[last];
}
void MyQueue::sizeUp()
{
    int newSize = MaxCarNumber * 2;
    Car* newCars = new Car[newSize];
    int count = 0;
    // 将原队列中所有元素复制到新数组的头部
    while (!isEmpty()) {
        newCars[count++] = pop();
    }
    delete[] Cars;
    Cars = newCars;
    Front = 0;
    Rear = count;
    MaxCarNumber = newSize;
}