//Tool.cpp：工具函数实现文件，包含GUI界面显示、车辆信息打印、车辆入场/出场处理、设置更改等功能的实现
#include "Tool.hpp"
#define parkfile "park.json"
#define queuefile "queue.json"
#define settingfile "setting.json"
#include <limits>

bool GUI()//打印GUI界面，返回值为是否继续执行程序
{
	Setting setting;//停车场设置
	json j;
	loadSettingFromFile(settingfile, setting);//从json文件中初始化停车场设置
	MyZhan Park(setting.MaxCarNumber);//加载停车场内的车辆信息
	MyQueue queue;//加载便道上的车辆信息
	loadQueueFromFile(queuefile, queue);
	loadParkFromFile(parkfile, Park);
	string log;
	while(check(Park,queue,log))//检查停车场是否有空位，若有空位则将便道上的第一辆车移入停车场
	{
		cout << log <<endl;
	}
	printColored("        ┌───────────────────────────────┐\n", "绿色");
	printColored("        │        停车场管理系统         │\n", "绿色");
	printColored("        └───────────────────────────────┘\n\n","绿色");
	cout << "当前停车场设定：" << endl;
	cout << "停车场容量：" << Park.getCarNumber()<<"/" << setting.MaxCarNumber << "辆";
	cout << " 每小时收费：" << setting.PayPerHour << "元" << endl;
	printCars(Park, queue);//打印停车场内和便道上的车辆信息
	cout << "请选择操作>>>" << endl;
	cout << "	[1] 车辆入场" << endl;
	cout << "	[2] 车辆出场" << endl;
	cout << "	[3] 更改设置" << endl;
	cout << "	[0] 退出程序" << endl;
	cout << ">>>";
	cin.clear();  // 清除任何错误标志
	string choice;
	if (!getline(cin, choice)) {
		cin.clear();
		// 如果读取失败，给个默认值或重新尝试
		choice = "";
	}
	if (choice == "1")
	{
		EnterCar(Park, queue, setting);
		return true;
	}
	else if (choice == "2")
	{
		int index;
		cout << "请输入要驶离的车位号：";
		while (!(cin >> index) || index < 1 || index > Park.getCarNumber()) {
			cin.clear(); // 清除错误标志
			cin.ignore(10000, '\n'); // 忽略错误输入
			printColored("无效的车位号，请重新输入！\n", "红色");
			cout << "请输入要驶离的车位号：";
		}
		ExitCar(Park, queue, setting,index);
		return true;
	}
	else if (choice=="3")
	{
		ChangeSetting(setting,Park);
		return true;
	}
	else if (choice == "0") 
	{
		return false;
	}
	else
	{
		printColored("无效的选择，请重新输入。\n", "红色");
		system("pause");
		system("cls");
		return true;
	}
			
}
void printCars(const MyZhan& zhan, const MyQueue& queue)//打印停车场内和便道上的车辆信息
{
	if (zhan.getCarNumber() == 0 && queue.getCarNumber() == 0)
	{
		printColored("\n停车场和便道内没有车辆\n\n", "红色");
	}
	else
	{
		int len = max(zhan.getCarNumber(), queue.getCarNumber());
		cout << "\n停车场车辆信息：" << endl;
		string a="车牌号",b="驶入时间",c="便道车辆";
		string header = padTo20(a) + " " + padTo20(b) + " " ;
		cout << "   ";
		printColored(header, "绿色");
		cout << "	便道车辆\n";
		for (int i = 0; i < len; i++)
		{
			if(i <9)
			{
				cout << " "<<i+1<<" ";
			}
			else
			{
				cout << i + 1 << " ";
			}
			if(i < zhan.getCarNumber())
			{
				printColored(padTo20(zhan[i].id), "黄色");
				printColored(" " + padTo20(zhan[i].EnterDate), "黄色");
			}
			else
			{
				printColored(padTo20("---"), "黄色");
				cout << " ";
				printColored(padTo20("---"), "黄色");
			}
			if(i < queue.getCarNumber())
			{
				printColored("	" + padTo20(queue[i].id), "蓝色");
			}
			else
			{
				printColored("	" + padTo20("---"), "蓝色");
			}
			cout << endl;
		}
		return;
	}
}
void EnterCar(MyZhan& zhan, MyQueue& queue, const Setting& setting)//车辆入场
{
	Car newCar;
	cout << "请输入车牌号：";
	getline(cin, newCar.id);
	newCar.EnterDate = getCurrentLocalTime();//记录进入时间为当前时间
	if(zhan.getCarNumber() < setting.MaxCarNumber)
	{
		zhan.push(newCar);
		saveParkToFile(parkfile, zhan);//保存停车场内的车辆信息到json文件中
		string txt = "车辆 " + newCar.id + " 已驶入" ;
		txt += "，停在" + to_string(zhan.getCarNumber()) + "号位\n";
		printColored(txt, "绿色");
	}
	else
	{
		queue.push(newCar);
		saveQueueToFile(queuefile, queue);//保存便道上的车辆信息到json文件中
		string txt = "车辆 " + newCar.id + " 已进入便道等待\n";
		printColored(txt, "黄色");
	}
	system("pause");
	system("cls");
}
void ExitCar(MyZhan& zhan, MyQueue& queue, const Setting& setting, int index) {
	if (index < 1 || index > zhan.getCarNumber()) {
		printColored("无效的车位号！\n", "红色");
		system("pause");
		system("cls");
		return;
	}
	// 将车位号转换为 0-based 索引
	int pos = index - 1;
	// 临时栈，用于存放被挡住的车
	MyZhan temp(setting.MaxCarNumber); // 容量足够
	// 1. 将 pos 后面的车辆依次弹出并存入临时栈
	int originalCount = zhan.getCarNumber();
	for (int i = originalCount - 1; i > pos; --i) 
	{
		Car car = zhan.pop(); // 弹出的是最后面的车
		cout << "车辆" << car.id << " 暂时驶离停车场\n";
		temp.push(car);
	}
	// 2. 现在栈顶就是第 pos 辆车，弹出它（出场）
	Car exitingCar = zhan.pop();
	string exitTime = getCurrentLocalTime();
	int hours = hoursDifference(exitTime, exitingCar.EnterDate);
	double fee = hours * setting.PayPerHour;
	cout << "车辆 " << exitingCar.id << " 于 " << exitTime << " 驶离，停车费用：" << fee << "元\n";
	// 3. 将临时栈中的车按原顺序放回（注意临时栈是后进先出，需反向放回）
	while (temp.getCarNumber() > 0) {
		Car car = temp.pop();
		cout << "车辆" << car.id << " 重新进入停车场,停在"<<index++<<"号位\n";
		zhan.push(car);
	}
	// 4. 检查便道是否有车可以进入停车场
	string log;
	while (check(zhan, queue, log)) {
		cout << log << endl;
	}

	// 5. 保存数据
	saveParkToFile(parkfile, zhan);
	saveQueueToFile(queuefile, queue);

	system("pause");
	system("cls");
}
void ChangeSetting(Setting& setting,MyZhan& Park)//更改设置
{
	double newPay;
	int newMax;
	cout << "请输入新的停车场容量：";
	cin >> newMax;
	if (newMax < Park.getCarNumber())
	{
		cout << "新的容量不能小于当前停车场内的车辆数量！" << endl;
		return;
	}else if(newMax<=0)
	{
		cout << "新的容量必须大于0！" << endl;
		return;
	}
	else{
				setting.MaxCarNumber = newMax;
	}
	cout << "请输入新的每小时收费：";
	cin >> newPay;
	if (newPay < 0)
	{
		cout << "新的每小时收费必须大于或等于0！" << endl;
		return;
	}
	else
	{
		setting.PayPerHour = newPay;
	}
	saveSettingToFile(settingfile, setting);//保存设置对象到json对象
	cout << "设置已更新！" << endl;
	system("pause");
	system("cls");
}
void printColored(const std::string& text, const std::string& colorName) {
	// 获取标准输出句柄
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		// 句柄无效时直接输出（例如程序输出被重定向）
		std::cout << text;
		return;
	}

	// 保存当前控制台属性，以便输出后恢复
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	WORD originalAttributes = 0;
	if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
		originalAttributes = csbi.wAttributes;
	}

	// 定义颜色名称到 Windows 颜色属性的映射表
	static const std::map<std::string, WORD> colorMap = {
		{"红色", FOREGROUND_RED},
		{"绿色", FOREGROUND_GREEN},
		{"蓝色", FOREGROUND_BLUE},
		{"黄色", FOREGROUND_RED | FOREGROUND_GREEN},
		{"紫色", FOREGROUND_RED | FOREGROUND_BLUE},
		{"青色", FOREGROUND_GREEN | FOREGROUND_BLUE},
		{"白色", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},
		{"黑色", 0},
		// 可添加更多颜色（如加亮版本）
		{"亮红色", FOREGROUND_RED | FOREGROUND_INTENSITY},
		{"亮绿色", FOREGROUND_GREEN | FOREGROUND_INTENSITY},
		{"亮蓝色", FOREGROUND_BLUE | FOREGROUND_INTENSITY},
	};

	// 查找颜色名称，若找到则设置对应颜色
	auto it = colorMap.find(colorName);
	if (it != colorMap.end()) {
		SetConsoleTextAttribute(hConsole, it->second);
	}
	// 若颜色名称未定义，则保持当前颜色不变

	// 输出文本
	std::cout << text;

	// 恢复原始控制台属性
	SetConsoleTextAttribute(hConsole, originalAttributes);
}
int hoursDifference(const std::string& datetime1, const std::string& datetime2) {
	std::tm tm1 = {}, tm2 = {};
	std::istringstream ss1(datetime1), ss2(datetime2);
	// 解析字符串到 tm 结构
	ss1 >> std::get_time(&tm1, "%Y-%m-%d %H:%M:%S");
	ss2 >> std::get_time(&tm2, "%Y-%m-%d %H:%M:%S");
	if (ss1.fail() || ss2.fail()) {
		// 解析失败，可根据需要处理（这里返回0）
		return 0;
	}
	// 设置 tm_isdst 为 -1 让系统自动判断夏令时
	tm1.tm_isdst = -1;
	tm2.tm_isdst = -1;
	// 转换为 time_t（本地时间）
	std::time_t t1 = std::mktime(&tm1);
	std::time_t t2 = std::mktime(&tm2);
	// 计算秒差，转换为小时
	double diffSeconds = std::difftime(t1, t2); // t1 - t2
	double diffHours = diffSeconds / 3600.0;
	// 向上取整并返回 int
	return static_cast<int>(std::ceil(diffHours));
}
/**
 * 获取当前本地时间，格式：YYYY-MM-DD HH:MM:SS
 * @return 时间字符串，如果失败返回空字符串
 */
std::string getCurrentLocalTime() {
	std::time_t now = std::time(nullptr);          // 获取当前日历时间（UTC）
	if (now == -1) {
		return "";                                 // 获取时间失败
	}

	std::tm tmBuf = { 0 };
	// 使用安全的 localtime_s（VS 专用）
	errno_t err = localtime_s(&tmBuf, &now);
	if (err != 0) {
		return "";                                 // 转换失败
	}

	std::ostringstream oss;
	oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}
std::string padTo20(const std::string& s) {
	if (s.length() >= 20) {
		return s;               // 长度已满或超过（但题目保证不超过20）
	}
	return s + std::string(20 - s.length(), ' ');  // 追加空格
}
bool check(MyZhan& Park,MyQueue& queue,string& log)//检查停车场是否有空位，若有空位则将便道上的第一辆车移入停车场
{
	if (Park.getCarNumber() < Park.getMaxCarNumber() && queue.getCarNumber() > 0)
	{
		Car car = queue.pop();
		car.EnterDate = getCurrentLocalTime();//更新进入时间为当前时间
		Park.push(car);
		saveParkToFile(parkfile, Park);//保存停车场内的车辆信息到json文件中
		saveQueueToFile(queuefile, queue);//保存便道上的车辆信息到json文件中
		log = "车辆 " + car.id + " 已从便道进入停车场";
		string txt = intToString(Park.getCarNumber())  + "号位";
		log += "，停在" + txt;
		return true;
	}
	return false;
}
std::string intToString(int num) {
	return std::to_string(num);
}