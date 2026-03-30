//FileCtrl.cpp
#pragma once
#include <iostream>
#include "StructDate.hpp"
#include "json.hpp"
#include"fileCtrl.hpp"

using namespace std;

void initSettingFormJson(const json& j, Setting& setting)//初始化停车场设置
{
	j.at("MaxCarNumber").get_to(setting.MaxCarNumber);
	j.at("PayPerHour").get_to(setting.PayPerHour);
}
void SaveSettingToJson(json& j, Setting& setting)//保存停车场设置
{
	j = json{
		{"MaxCarNumber", setting.MaxCarNumber},
		{"PayPerHour", setting.PayPerHour}
	};
}
bool loadSettingFromFile(const std::string& filename, Setting& setting)
{
	json j;
	std::ifstream inFile(filename);
	if (!inFile.is_open())
	{
		// 文件不存在：创建默认设置并写入文件
		Setting defaultSetting{ 10, 2.0 };//默认设置
		json j_default;
		SaveSettingToJson(j_default, defaultSetting);
		std::ofstream outFile(filename);
		if (!outFile.is_open())
		{
			std::cerr << "无法创建文件: " << filename << std::endl;
			return false;
		}
		outFile << j_default.dump(4);
		outFile.close();
		j = j_default;  // 使用刚生成的默认 JSON
	}
	else
	{
		// 文件存在：读取并解析
		try
		{
			inFile >> j;
		}
		catch (const json::parse_error& e)
		{
			std::cerr << "JSON 解析错误: " << e.what() << std::endl;
			return false;
		}
	}
	// 无论文件是否存在，此时 j 都持有正确的 JSON 数据
	initSettingFormJson(j, setting);
	return true;
}
bool saveSettingToFile(const std::string& filename, Setting& setting)//将停车场设置保存到json文件中
{
	json j;
	SaveSettingToJson(j, setting);
	ofstream outFile(filename);
	if (!outFile.is_open()) {
		std::cerr << "无法打开文件！" << std::endl;
		return false;
	}
	outFile << j.dump(4);   // 缩进4个空格
	outFile.close();
	return true;
}

void loadFormJson(const json& j, MyZhan& park)//加载停车场内的车辆信息
{
	//废弃函数，暂不使用
}
void SaveToJson(const json& j, MyZhan& park)//保存停车场内的车辆信息
{
	//废弃函数，暂不使用
}
bool loadParkFromFile(const std::string& filename, MyZhan& park)//从json文件中加载停车场内的车辆信息
{
	json ij;
	ifstream inFile(filename);
	if (!inFile.is_open()) 
	{
		// 如果文件不存在，创建一个空数组的 JSON 文件
		json j = json::array();
		std::ofstream file(filename);
		if (file.is_open()) 
		{
			file << j.dump(4);  // 可选缩进
			file.close();
		}
		ij = j;
	}
	else 
	{
		try {
			inFile >> ij;
		}
		catch (const json::parse_error& e) {
			cerr << "JSON 解析错误: " << e.what() << endl;
			return false;
		}
		if (!ij.is_array()) 
		{
			cerr << "JSON 根节点不是数组" << endl;
			return false;
		}
		for(int i=0; i < ij.size(); i++)
		{
			Car car;
			car.id = ij[i].at("id").get<string>();
			car.EnterDate = ij[i].at("EnterDate").get<string>();
			park.push(car);
		}
	}
	return true;
}
bool saveParkToFile(const std::string& filename, MyZhan& park)//将停车场内的车辆信息保存到json文件中
{
	json j = json::array();
	for(int i = 0; i < park.getCarNumber(); i++)
	{
		json carJson;
		carJson["id"] = park[i].id;
		carJson["EnterDate"] = park[i].EnterDate;
		j.push_back(carJson);
	}
	ofstream outFile(filename);
	if (!outFile.is_open()) {
		std::cerr << "无法打开文件！" << std::endl;
		return false;
	}
	outFile << j.dump(4);   // 缩进4个空格
	outFile.close();
	return true;
}

void loadQueueFromJson(const json& j, MyQueue& queue)//加载便道上的车辆信息
{
	//废弃函数，暂不使用
}
void SaveQueueToJson(const json& j, MyQueue& queue)//保存便道上的车辆信息
{
	//废弃函数，暂不使用
}
bool loadQueueFromFile(const std::string& filename, MyQueue& queue)//从json文件中加载便道上的车辆信息
{
	json ij;
	ifstream inFile(filename);
	if(!inFile.is_open())
	{
		while (!queue.isEmpty()) {
			queue.pop();
		}
		json j = json::array();
		std::ofstream file(filename);
		if (file.is_open()) 
		{
			file << j.dump(4);  // 可选缩进
			file.close();
			ij = j;
		}
		else
		{
			cerr << "自动创建文件失败！" << endl;
		}
	}
	else
	{
		try {
			inFile >> ij;  // 可能抛出 parse_error
		}
		catch (const json::parse_error& e) {
			std::cerr << "JSON 解析错误: " << e.what() << std::endl;
			return false;  // 加载失败
		}
		if (!ij.is_array()) 
		{
			cerr << "JSON 根节点不是数组" << endl;
			return false;
		}
		for(int i=0; i < ij.size(); i++)
		{
			Car car;
			car.id = ij[i].at("id").get<string>();
			car.EnterDate = ij[i].at("EnterDate").get<string>();
			queue.push(car);
		}
	}
	return true;
}
bool saveQueueToFile(const std::string& filename, MyQueue& queue)//将便道上的车辆信息保存到json文件中
{
	json j = json::array();
	for(int i = 0; i < queue.getCarNumber(); i++)
	{
		json carJson;
		carJson["id"] = queue[i].id;
		carJson["EnterDate"] = queue[i].EnterDate;
		j.push_back(carJson);
	}
	ofstream outFile(filename);
	if (!outFile.is_open()) {
		std::cerr << "无法打开文件！" << std::endl;
		return false;
	}
	outFile << j.dump(4);   // 缩进4个空格
	outFile.close();
	return true;
}
