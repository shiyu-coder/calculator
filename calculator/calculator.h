#pragma once
#include <deque>
#include <string.h>
#include <string>
#include <map>
using namespace std;
/**
 * calculator库：
 * 提供基本的多项式运算函数和运算流程控制函数
 */

//储存所有的多项式
//多项式的系数从小到大排列
map<string, deque<double>> *fomulas;

//功能函数////////////////////////////////////////////////////
//输入
extern void save_fomula();
//查看
extern void look_fomula();
//混合运算
extern void compute_fomula();
///////////////////////////////////////////////////////////////

//打印界面
extern void print_interface();
//打印一个多项式
extern void print_fomula(string name);
//功能筛选器
extern void command_index(string index);

//求导运算
extern deque<double> operation_derivation(deque<double> fomula);
//积分运算
extern double operation_integral(deque<double> fomula, double begin, double end);
