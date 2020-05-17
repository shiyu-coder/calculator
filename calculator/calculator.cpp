#include "calculator.h"
#include <iostream>
#include <vector>
using namespace std;
//运行状态
bool running = true;

int main() {
	while (running) {
		print_interface();
		string input;
		getline(cin, input);
		if (input.compare("quit") == 0) {
			running = false;
			break;
		}
		command_index(input);
		input.clear();
	}
	cout << "------------多项式计算器 已退出------------" << endl;
	return 0;
}

void command_index(string index) {
	if (index.size() <= 0) {
		return;
	}
	int int_index = atoi(index.c_str());
	switch (int_index) {
	case 1:save_fomula();
		break;
	case 2:compute_fomula();
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:look_fomula();
		break;
	default:
		cout << "不存在此功能！" << endl;
	}
}

void save_fomula() {
	int len = -1;
	//输入长度
	cout << "多项式长度：";
	cin >> len;
	//长度合法性检查
	while (len <= 0 || len > 20) {
		cout << "非法长度！合法长度范围为1-10" << endl;
		cout << "多项式长度：";
		cin >> len;
	}
	//输入系数
	cout << "输入系数：（多项式项的次数从大到小，用空格隔开）" << endl;
	vector<double> coe;
	for (int i = 1; i <= len; i++) {
		double single_coe=0;
		cin >> single_coe;
		coe.push_back(single_coe);
	}
	//反转，从小到大排列
	reverse(coe.begin(), coe.end());
	//输入名称
	string name;
	cout << "多项式名为：";
	cin >>name;

}

void look_fomula() {

}

void compute_fomula() {

}

void print_interface() {
	cout << "\t\t============================多项式计算器============================" << endl;
	cout << "\t\t   1.输入  2.混合运算  3.求逆元  4.除法/取模运算  5.求根  6.查看" << endl;
	cout << "\t\t====================================================================" << endl;
	cout << "\t\t-------------------------输入 quit 退出程序-------------------------" << endl;
	cout << "请选择：";
}

void print_fomula(string name) {

}

deque<double> operation_derivation(deque<double> fomula) {
	deque<double> d;
	return d;
}

double operation_integral(deque<double> fomula, double begin, double end) {
	return 0;
}