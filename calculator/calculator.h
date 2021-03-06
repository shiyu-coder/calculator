﻿#pragma once
#include <deque>
#include <string.h>
#include <string>
#include <map>
#include <vector>
using namespace std;
/**
 * calculator库：
 * 提供基本的多项式运算函数和运算流程控制函数
 */

//判断double类型变量相等的精度
#define ACCURACY 0.0000001

//储存所有的多项式
//多项式的系数从小到大排列
map<string, deque<double>> *fomulas;
//运算符重载
bool operator<(const deque<double> d1, const deque<double> d2);

//功能函数////////////////////////////////////////////////////
//输入
extern void save_fomula();
//查看
extern void look_fomula();
//混合运算引导
extern void compute_fomula();
//求逆元
extern void reverse_fomula();
//除法/取模运算
extern void division_fomula();
//求根
extern void root_fomula();
//作图
extern void draw_fomula();
//多项式拟合
extern void fitting_fomula();
///////////////////////////////////////////////////////////////

//打印界面
extern void print_interface();
//打印一个多项式
extern void print_fomula(string name);
extern void print_fomula(deque<double> fomula);
//功能筛选器
extern void command_index(string index);
//去除字符串两端的括号
extern string strim_bracket(string str);
//将标准积分式拆分成起点，终点和积分多项式（或括号）
extern pair<double, double> split_integral(string *cmd);
//去除字符串中的空格
extern string strim_space(string str);
//检测一个字符是否是字母
extern bool isLetter(char c);

//混合计算函数
extern deque<double> compute(string cmd);
//加法计算
extern deque<double> operation_add(deque<double> f1, deque<double> f2);
//乘法运算
extern deque<double> operation_mutiply(deque<double> f1, deque<double> f2);
//求导运算
extern deque<double> operation_derivation(deque<double> fomula);
//积分运算
extern double operation_integral(deque<double> fomula, double begin, double end);
//绘制函数图像
extern void draw_graph(deque<double> fomula, double zero_point);
//计算多项式在某一点的值
extern double getValueAt(deque<double> fomula, double key);
//检查混合算式正确性
extern bool examCmd(string cmd);
//将混合算式，按照加法分开,忽略括号中的加号
extern vector<string> dividedByAdd(string cmd);
//将已经加法分开的混合算式按照乘法分开，忽略括号中的乘号
extern vector<string> dividedByMutiply(string cmd);
//检测是否是被括号包住的混合算式
extern bool isSurByBrackets(string cmd);
//计算deque<double>中值的累加
extern double sum_fomula(deque<double> fomula,int n);
//计算乘积和
extern double sum_multi(deque<double> fo1, deque<double> fo2, int n);
//计算次方和(m:次方数）
extern double sum_pow(deque<double> fomula, int n, int m);
//计算次方后再乘积和
extern double sum_pow_multi(deque<double> fo1, deque<double> fo2, int n, int m);
//多项式拟合运算，按次数从低到高排列返回系数
extern deque<double> operation_polynomial_fitting(deque<double> fo1, deque<double> fo2, int n, int m);
//被operation_polynomial_fitting函数调用的一个计算函数
extern double auxiliary_operation_polynomial_fitting(double M[][101],deque<double> coe, int l, int m);