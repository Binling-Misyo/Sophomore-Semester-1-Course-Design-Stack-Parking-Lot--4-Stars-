//FileCtrl.hpp
#pragma once
#include <iostream>
#include<fstream>
#include "StructDate.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

void initSettingFormJson(const json& j,Setting& setting);//json对象转设置对象
void SaveSettingToJson(json& j, Setting& setting);//设置对象转json对象
bool loadSettingFromFile(const std::string& filename, Setting& setting);//从json文件中加载停车场设置
bool saveSettingToFile(const std::string& filename, Setting& setting);//将停车场设置保存到json文件中

void loadFormJson(const json& j, MyZhan& park);//json对象转停车场对象
void SaveToJson(json& j, MyZhan& park);//停车场对象转json对象
bool loadParkFromFile(const std::string& filename, MyZhan& park);//从json文件中加载停车场内的车辆信息
bool saveParkToFile(const std::string& filename, MyZhan& park);//将停车场内的车辆信息保存到json文件中

void loadQueueFromJson(const json& j, MyQueue& queue);//json对象转便道对象
void SaveQueueToJson(json& j, MyQueue& queue);//便道对象转json对象
bool loadQueueFromFile(const std::string& filename, MyQueue& queue);//从json文件中加载便道上的车辆信息
bool saveQueueToFile(const std::string& filename, MyQueue& queue);//将便道上的车辆信息保存到json文件中
