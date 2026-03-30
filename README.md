# Sophomore-Semester-1-Course-Design-Stack-Parking-Lot--4-Stars-
大二上学期数据结构课程设计，由于本人手慢高难题都被抢走了，只好写了这个栈和队列的，额外学了json来增加项目亮点，由于未知原因无法接收存储中文数据（不打算改了）
-依然vs2022编译开发，64位Windows系统。
# 停车场管理系统项目文档

## 1. 引言

随着城市化进程的加速，汽车保有量不断增加，停车场管理成为日常生活中常见的问题。为了模拟实际停车场的运作流程，并运用数据结构知识解决具体问题，本小组开发了停车场管理系统。系统采用栈模拟停车场、队列模拟便道，实现了车辆入场、出场、费用计算、状态查看及数据持久化等功能，全面覆盖课程设计要求，并通过控制台菜单提供友好的交互界面。

## 2. 系统需求分析

### 2.1 功能需求

- **基础设置管理**：支持设置停车场容量和每小时停车费用，并可随时修改。
- **车辆入场管理**：记录车牌号和进入时间，有空位则停入停车场（按到达顺序从北向南排列），无空位则进入便道等待。
- **车辆出场管理**：根据用户指定的车位号，让后续车辆暂时退出，待目标车辆驶离后按原序恢复，同时计算停车费用（不足1小时按1小时计费），并自动将便道上的第一辆车移入停车场。
- **状态查看**：实时显示停车场内每个车位的车辆信息（车牌号、进入时间）以及便道上所有等待车辆的信息。
- **数据持久化**：系统配置和车辆信息保存于文件中，程序启动时自动加载，退出时自动保存。

### 2.2 性能需求

- 操作响应及时，无明显延迟。
- 能够处理连续多次的车辆出入，便道队列支持动态扩容，理论上无容量上限。
- 文件读写稳定，格式清晰易读。

## 3. 系统总体设计

### 3.1 系统架构

系统采用模块化设计，主要分为以下层次：

- **用户交互层**：控制台菜单界面，接收用户输入，展示系统状态。
- **业务逻辑层**：包含车辆入场、出场、设置修改等核心功能，调用数据层接口。
- **数据层**：负责数据的内部存储（栈、队列）和外部存储（文件读写）。

### 3.2 功能模块划分

- **初始化与配置模块**：读取配置文件，创建停车场和便道对象。
- **车辆入场模块**：接收车牌号，自动记录当前时间，调用数据层存储。
- **车辆出场模块**：根据车位号执行让路逻辑、费用计算，并更新数据。
- **状态显示模块**：格式化输出停车场和便道信息。
- **文件存储模块**：封装 JSON 读写操作，提供保存和加载函数。
- **工具函数模块**：包含时间处理、字符串填充等辅助功能。

## 4. 系统详细设计

### 4.1 数据结构设计

#### 4.1.1 车辆信息结构 `Car`

```cpp
struct Car {
    std::string id;        // 车牌号
    std::string EnterDate; // 进入时间（格式：YYYY-MM-DD HH:MM:SS）
};
```

#### 4.1.2 停车场 —— 顺序栈 `MyZhan`

停车场内的车辆按到达时间顺序从北向南排列，符合栈的“后进先出”特性（离开时最后进入的车先退出让路）。采用动态数组实现，容量可配置。

- **成员变量**：
  - `Car* Cars`：指向动态数组的指针。
  - `int CarNumber`：当前栈中元素个数（栈顶指针）。
  - `int MaxCarNumber`：栈的最大容量。
- **成员函数**：
  - `MyZhan(int MaxCarNumber)`：构造函数，分配内存。
  - `~MyZhan()`：析构函数，释放内存。
  - `bool push(Car NewCar)`：入栈，成功返回 true，满则返回 false。
  - `Car pop()`：出栈，返回栈顶元素，栈空则返回默认 Car。
  - `Car top()`：返回栈顶元素但不弹出。
  - `int getCarNumber() const`：获取当前车辆数。
  - `int getMaxCarNumber() const`：获取容量。
  - `Car operator[](int index) const`：下标访问，用于遍历。

#### 4.1.3 便道 —— 循环队列 `MyQueue`

便道上的车辆按到达顺序排队，符合队列的“先进先出”特性。采用循环数组实现，并支持动态扩容。

- **成员变量**：
  - `Car* Cars`：指向动态数组的指针。
  - `int Front, Rear`：队头、队尾指针（Rear 指向下一个空闲位置）。
  - `int MaxCarNumber`：当前队列容量。
- **成员函数**：
  - `MyQueue()`：构造函数，初始容量为 20。
  - `~MyQueue()`：析构函数，释放内存。
  - `void push(Car NewCar)`：入队，若满则自动扩容。
  - `Car pop()`：出队，返回队首元素，队空则返回默认 Car。
  - `Car front()`：返回队首元素但不删除。
  - `bool isEmpty()`：判断队列是否为空。
  - `bool isFull()`：判断队列是否已满。
  - `void sizeUp()`：将队列容量加倍，并保持元素顺序。
  - `int getCarNumber() const`：计算当前队列中元素个数。
  - `Car operator[](int index) const`：按逻辑顺序（从队头开始）访问元素。

#### 4.1.4 设置结构 `Setting`

```cpp
struct Setting {
    int MaxCarNumber;   // 停车场容量
    double PayPerHour;  // 每小时收费
};
```

### 4.2 核心算法设计

#### 4.2.1 车辆入场

1. 获取当前系统时间作为进入时间。
2. 若停车场未满，将新车入栈（`push`），并保存数据。
3. 若停车场已满，将新车入队列（`push`），并保存数据。

#### 4.2.2 车辆出场（让路算法）

以目标车位号 `index`（1-based）为例：

1. 将 `index` 之后的车辆从停车场栈中依次弹出，并压入一个临时栈 `temp`，同时输出“车辆 XXX 暂时离开”。
2. 弹出目标车辆（即此时栈顶），计算停车费用并输出。
3. 将临时栈 `temp` 中的车辆依次弹出并重新压回停车场栈，恢复原顺序，输出“车辆 XXX 重新进入”。
4. 调用 `check` 函数，将便道上的第一辆车（如有）移入停车场，并重复直到停车场满或便道空。
5. 保存数据。

**费用计算**：  
利用 `hoursDifference` 函数计算出场时间与入场时间的小时差，向上取整（`ceil`），乘以每小时收费得到费用。

#### 4.2.3 便道车辆自动移入

```cpp
bool check(MyZhan& Park, MyQueue& queue, string& log) {
    if (Park.getCarNumber() < Park.getMaxCarNumber() && queue.getCarNumber() > 0) {
        Car car = queue.pop();
        car.EnterDate = getCurrentLocalTime(); // 更新进入时间为当前时间
        Park.push(car);
        saveParkToFile(parkfile, Park);
        saveQueueToFile(queuefile, queue);
        log = "车辆 " + car.id + " 已从便道进入停车场，停在" + to_string(Park.getCarNumber()) + "号位";
        return true;
    }
    return false;
}
```

该函数在车辆出场后以及每次菜单刷新前被调用，确保便道车辆及时入场。

#### 4.2.4 队列动态扩容

当队列满时，`sizeUp` 函数被调用：

1. 分配新数组，大小为原容量的两倍。
2. 将原队列中的所有元素按出队顺序（从 `Front` 开始）依次复制到新数组的头部。
3. 释放原数组，更新指针，并将 `Front` 置为 0，`Rear` 置为原元素个数。

### 4.3 文件存储设计

采用 `nlohmann/json` 单头文件库进行 JSON 序列化与反序列化。系统使用三个文件：

- `setting.json`：存储停车场容量和每小时收费。
- `park.json`：存储停车场内车辆数组。
- `queue.json`：存储便道上车辆数组。

**存储示例（park.json）**：
```json
[
    {
        "id": "A12345",
        "EnterDate": "2025-03-14 10:30:00"
    },
    {
        "id": "B67890",
        "EnterDate": "2025-03-14 11:15:00"
    }
]
```

**关键函数**：
- `loadSettingFromFile`、`saveSettingToFile`
- `loadParkFromFile`、`saveParkToFile`
- `loadQueueFromFile`、`saveQueueToFile`

所有读取操作均包含异常处理（捕获 `parse_error` 和 `out_of_range`），在文件损坏或格式错误时返回默认值，保证程序不崩溃。

### 4.4 界面设计

系统采用控制台菜单，通过循环调用 `GUI()` 函数实现。主界面包括：

- 标题和当前设置显示。
- 停车场车辆表格（含车位号、车牌、驶入时间）。
- 便道车辆列表。
- 操作选项菜单。

使用 Windows API 的 `SetConsoleTextAttribute` 实现彩色输出，增强可读性（如正常信息为绿色，便道信息为黄色，错误信息为红色）。

## 5. 系统实现与测试

### 5.1 开发环境

- 操作系统：Windows 10/11
- 编译器：Visual Studio 2022
- 依赖库：nlohmann/json（单头文件）

### 5.2 核心代码片段

#### 5.2.1 停车场类实现

```cpp
bool MyZhan::push(Car NewCar) {
    if (CarNumber < MaxCarNumber) {
        Cars[CarNumber++] = NewCar;
        return true;
    }
    return false;
}

Car MyZhan::pop() {
    if (CarNumber > 0) {
        return Cars[--CarNumber];
    }
    return Car();
}
```

#### 5.2.2 队列扩容实现

```cpp
void MyQueue::sizeUp() {
    int newSize = MaxCarNumber * 2;
    Car* newCars = new Car[newSize];
    int count = 0;
    while (!isEmpty()) {
        newCars[count++] = pop();
    }
    delete[] Cars;
    Cars = newCars;
    Front = 0;
    Rear = count;
    MaxCarNumber = newSize;
}
```

### 5.3 测试情况

| 测试项           | 测试用例                                      | 预期结果                                               | 实际结果 |
|------------------|-----------------------------------------------|--------------------------------------------------------|----------|
| 程序首次启动     | 无配置文件                                    | 自动创建默认设置和空车辆文件                           | 通过     |
| 车辆入场         | 停车场容量5，当前3辆车，输入车牌“A11111”   | 车辆进入4号位，记录当前时间                            | 通过     |
| 停车场满时入场   | 停车场已满，便道有2辆车，再入场               | 新车进入便道等待                                       | 通过     |
| 车辆出场（中间） | 让3号位车辆离开                               | 4、5号暂退 → 3号出场计费 → 4、5复位 → 便道第一辆进入5号 | 通过     |
| 无效车位号       | 输入0或大于当前车辆数                         | 提示“无效的车位号”，重新输入                           | 通过     |
| 修改设置         | 新容量小于当前车辆数                          | 提示错误，不修改                                       | 通过     |
| 文件损坏         | 手动将 park.json 改为非法 JSON                 | 程序捕获异常，使用空数据继续运行                       | 通过     |

## 6. 总结与展望

### 6.1 完成情况

本系统完全实现了题目要求的全部功能，并进行了扩展：

- 队列动态扩容，使便道容量不再受限。
- 彩色控制台输出，提升用户体验。
- 全面的输入合法性检查，增强程序健壮性。

### 6.2 项目分工

- **略**

三人共同参与需求分析和最终测试，体现了良好的团队协作。

---

**附录：项目文件结构**

```
停车场管理系统/
├── FileCtrl.cpp
├── FileCtrl.hpp
├── StructDate.cpp
├── StructDate.hpp
├── Tool.cpp
├── Tool.hpp
├── main.cpp
├── json.hpp
├── parking.json
├── queue.json
├── setting.json
└── 实验报告.docx
```
