#include "calculator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
//运行状态
bool running = true;

int main() {
	fomulas = new map<string, deque<double>>;
	while (running) {
		print_interface();
		string input;
		cin >>input;
		if (input.compare("quit") == 0) {
			running = false;
			break;
		}
		command_index(input);
		input.clear();
		cin.clear();
		//cin.ignore(2, '\n');
	}
	delete fomulas;
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
		cout << "非法长度！合法长度范围为1-20" << endl;
		cout << "多项式长度：";
		cin.clear();
		cin.ignore(1024, '\n');
		cin >> len;
	}
	//输入系数
	cout << "输入系数：（多项式项的次数从大到小，用空格隔开）" << endl;
	deque<double> coe;
	cin.clear();
	cin.ignore(1024, '\n');
	for (int i = 1; i <= len; i++) {
		double single_coe=0;
		cin >> single_coe;
		coe.push_front(single_coe);
	}
	//输入名称
	string name;
	cout << "多项式名为：";
	cin.clear();
	cin.ignore(1024, '\n');
	cin >>name;
	bool illegal = false;
	while (name.size() <= 0) {
		cout << "名称不得为空！" << endl;
		cout << "多项式名为：";
		cin.clear();
		cin.ignore(1024, '\n');
		cin >> name;
	}
	//名称合法性检测
	for (char c : name) {
		if (!isLetter(c)) {
			illegal = true;
		}
	}
	while (illegal) {
		cout << "名称只能包含英文字母！" << endl;
		cout << "多项式名为：";
		cin.clear();
		cin.ignore(1024, '\n');
		illegal = false;
		cin >> name;
		for (char c : name) {
			if (!isLetter(c)) {
				illegal = true;
			}
		}
	}
	//储存多项式
	auto result=fomulas->insert(pair<string, deque<double>>(name, coe));
	//检查多项式是否已经存在
	if (result.second == false) {
		cout << "输入失败，该多项式已存在，是否要继续输入(y/n)：";
	}
	else {
		//判断是否要继续输入
		cout << "输入成功，是否要继续输入(y/n)：";
	}
	string answer;
	cin >>answer;
	while (answer.size()<=0||(answer.at(0) != 'y' && answer.at(0) != 'n'))
	{
		cout << "非法输入！" << endl;
		cout << "是否要继续输入(y/n)：";
		cin.clear();
		cin.ignore(1024, '\n');
		cin >> answer;
	}
	if (answer.at(0) == 'y') {
		save_fomula();
	}
	else if (answer.at(0) == 'n') {
		return ;
	}
}

void look_fomula() {
	cout << "多项式数目：" << fomulas->size() << endl;
	cout << "所有多项式名称：" << endl;
	for (map<string, deque<double>>::iterator it = fomulas->begin(); it != fomulas->end(); it++) {
		cout << it->first << "\t";
	}
	cout << endl;
	cout << "要查看的多项式名为：";
	cin.clear();
	cin.ignore(1024, '\n');
	string name;
	cin >> name;
	print_fomula(name);
}

void compute_fomula() {
	cout << "请输入表达式：";
	string cmd;
	cin.clear();
	cin.ignore(1024, '\n');
	getline(cin, cmd);
	//去除空格
	cmd = strim_space(cmd);
	//检查表达式正确性
	bool isRight = examCmd(cmd);
	if (isRight) {
		//调用混合计算函数计算结果
		deque<double> result = compute(cmd);
		//输出结果
		cout << cmd << " = ";
		print_fomula(result);
		cout << "运算成功，是否继续运算(y/n)：";
		string input;
		cin.clear();
		//cin.ignore(1024, '\n');
		cin >> input;
		while (input.size() <= 0 || (input.at(0) != 'y' && input.at(0) != 'n')) {
			cout << "非法输入！" << endl;
			input.clear();
			cin.clear();
			cin.ignore(1024, '\n');
			cin >> input;
		}
		if (input.at(0) == 'y') {
			compute_fomula();
		}
		else if (input.at(0) == 'n') {
			return;
		}
	}
	else {
		return;
	}
}

deque<double> compute(string cmd) {
	deque<double> result;
	//按照加号拆分
	vector<string> addParts = dividedByAdd(cmd);
	bool aFirst = true;
	for (auto addP : addParts) {
		deque<double> addR;
		//对每一块按乘号拆分
		vector<string> multiParts = dividedByMutiply(addP);
		bool mFirst = true;
		for (auto multiP : multiParts) {
			deque<double> multiR;
			//现在的式子只有可能是4种情况：导数、积分、导数和积分、只有一个多项式或括号
			bool hasD = false;//有导数
			bool hasI = false;//有积分
			if (multiP.at(0) == '$') {
				hasI = true;
			}
			if (multiP.at(multiP.size() - 1) == '!') {
				hasD = true;
			}
			
			if (hasD&&!hasI) {
				//如果只有导数
				multiP = multiP.substr(0, multiP.size() - 1);//去掉求导符号
				if (isSurByBrackets(multiP)) {
					multiP = strim_bracket(multiP);
					multiR = operation_derivation(compute(multiP));
				}
				else {
					auto it = fomulas->find(multiP);
					if (it != fomulas->end()) {
						multiR = operation_derivation(it->second);
					}
					else {
						cout << "出现未知多项式" << multiP << "，多项式合法检测有问题！" << endl;
						return result;
					}
				}
			}
			else if (!hasD && hasI) {
				//如果只有积分
				pair<double, double> coe = split_integral(&multiP);
				if (isSurByBrackets(multiP)) {
					//如果是括号，就递归调用计算值
					multiP = strim_bracket(multiP);
					deque<double> fo = compute(multiP);
					//再计算定积分
					multiR.push_back(operation_integral(fo, coe.first, coe.second));
				}
				else {
					//如果不是括号，直接积分
					auto it = fomulas->find(multiP);
					if (it != fomulas->end()) {
						multiR.push_back(operation_integral(it->second, coe.first, coe.second));
					}
					else {
						cout << "出现未知多项式" << multiP << "，多项式合法检测有问题！" << endl;
						return result;
					}
				}
			}
			else if (hasD && hasI) {
				//如果既有导数又有积分,先求导，再积分
				//先去掉求导符
				multiP = multiP.substr(0, multiP.size() - 1);
				//去掉积分符
				pair<double, double> coe = split_integral(&multiP);
				if (isSurByBrackets(multiP)) {
					multiP = strim_bracket(multiP);
					deque<double> fo = compute(multiP);
					//求导
					fo = operation_derivation(fo);
					//积分
					multiR.push_back(operation_integral(fo, coe.first, coe.second));
				}
				else {
					auto it = fomulas->find(multiP);
					if (it != fomulas->end()) {
						//求导
						deque<double> fo = operation_derivation(it->second);
						//积分
						multiR.push_back(operation_integral(fo, coe.first, coe.second));
					}
					else {
						cout << "出现未知多项式" << multiP << "，多项式合法检测有问题！" << endl;
						return result;
					}
				}
			}
			else if (!hasD && !hasI) {
				//如果只有一个多项式或者是括号
				bool hasBra = isSurByBrackets(multiP);
				//如果是括号就去除括号后递归调用
				if (hasBra) {
					multiP = strim_bracket(multiP);
					multiR = compute(multiP);
				}
				else {
					//如果不是，就直接赋值
					auto it = fomulas->find(multiP);
					if (it != fomulas->end()) {
						multiR = it->second;
					}
					else {
						cout << "出现未知多项式"<<multiP<<"，多项式合法检测有问题！" << endl;
						return result;
					}
				}
			}
			//如果是第一块，就是addR
			if (mFirst) {
				mFirst = false;
				addR = multiR;
			}
			else {
				addR = operation_mutiply(addR, multiR);
			}
		}
		if (aFirst) {
			aFirst = false;
			result = addR;
		}
		else {
			result = operation_add(result, addR);
		}
	}
	return result;
}

bool examCmd(string cmd) {
	if (cmd.size() <= 0) {
		cout << "算式不得为空！" << endl;
		return false;
	}
	//1.以多项式名、左括号、定积分符开头，以右括号、多项式名、求导符结尾
	if (!(isLetter(cmd.at(0)) || (cmd.at(0) == '(') || (cmd.at(0) == '$')) || !(isLetter(cmd.at(cmd.size() - 1)) || (cmd.at(cmd.size() - 1) == ')') || (cmd.at(cmd.size() - 1) == '!'))) {
		cout << "必须以多项式名、左括号、定积分符开头，以右括号、多项式名、求导符结尾" << endl;
		return false;
	}
	//2.加法和乘法后面只能是多项式名、左括号、定积分符号
	bool add_multi = false;
	for (char c : cmd) {
		if ((c == '+' || c == '*') && !add_multi) {
			add_multi = true;
			continue;
		}
		else if(add_multi) {
			add_multi = false;
			if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '(' || c == '$')) {
				cout << "加法和乘法后面只能是多项式名、左括号、定积分符号" << endl;
				return false;
			}
		}
	}
	//3.定积分符号后只能接 区间 + 左括号/多项式名，区间采用[a,b]固定格式
	bool f_integral = false;//发现定积分符号
	bool f_left = false;//发现左中括号
	bool f_comma = false;//发现逗号
	bool f_right = false;//发现右中括号
	bool left_point = false;//左侧小数点
	bool right_point = false;//右侧小数点
	for (int i = 0; i < cmd.size(); i++) {
		if (cmd.at(i) == '$'&&!f_integral) {
			//发现定积分标志，开始进行检测
			f_integral = true;
			continue;
		}
		if (f_integral) {
			//检测左中括号
			f_integral = false;
			if (cmd.at(i) == '[') {
				f_left = true;
				continue;
			}
			else {
				cout << "定积分符号后只能接 区间 + 左括号/多项式名，区间采用[a,b]固定格式" << endl;
				return false;
			}
		}
		if (f_left) {
			//在区间左侧只能是数字和一个'.'
			if (cmd.at(i) >= '0' && cmd.at(i) <= '9') {
				continue;
			}
			if (cmd.at(i) == '.' && !left_point) {
				left_point = true;
				continue;
			}
			if (cmd.at(i) == ',') {
				f_left = false;
				f_right = true;
				continue;
			}
			cout << "定积分符号后只能接 区间 + 左括号/多项式名，区间采用[a,b]固定格式" << endl;
			return false;
		}
		if (f_right) {
			//在区间右侧只能是数字和一个'.'
			if (cmd.at(i) >= '0' && cmd.at(i) <= '9') {
				continue;
			}
			if (cmd.at(i) == '.' && !right_point) {
				right_point = true;
				continue;
			}
			if (cmd.at(i) == ']') {
				f_right = false;
				if (i < cmd.size() - 1) {
					if (cmd.at(i + 1) == '(' || isLetter(cmd.at(i + 1))) {
						//初始化
						f_integral = false;
						f_left = false;
						f_comma = false;
						f_right = false;
						left_point = false;
						right_point = false;
						continue;
					}
				}
			}
			cout << "定积分符号后只能接 区间 + 左括号/多项式名，区间采用[a,b]固定格式" << endl;
			return false;
		}
	}
	//4.求导符号后面只能为双目运算符、右括号
	for (int i = 0; i < cmd.size()-1; i++) {
		if (cmd.at(i) == '!') {
			if (!(cmd.at(i + 1) == ')' || cmd.at(i + 1) == '+' || cmd.at(i + 1) == '*')) {
				cout << "求导符号后面只能为双目运算符、右括号" << endl;
				return false;
			}
		}
	}
	//5.括号要匹配，左括号后面只能是左括号、定积分符号或多项式名，右括号后面只能是右括号，双目运算符或求导符号
	//检查括号个数是否相同,不能有空括号
	bool bracket = false;
	int left_bra = 0, right_bra = 0;
	for (char c : cmd) {
		if (c == '(') {
			if (bracket) {
				cout << "括号内不能为空" << endl;
				return false;
			}
			left_bra++;
			bracket = true;
		}
		else if (c == ')') {
			if (bracket) {
				cout << "括号内不能为空" << endl;
				return false;
			}
			right_bra++;
			bracket = true;
		}
		else {
			bracket = false;
		}
	}
	if (left_bra != right_bra) {
		cout << "括号不匹配" << endl;
		return false;
	}
	//检查左括号后面只能是左括号、定积分符号或多项式名，右括号后面只能是右括号，双目运算符或求导符号
	for (int i = 0; i < cmd.size() - 1; i++) {
		if (cmd.at(i) == '(') {
			if (!(cmd.at(i + 1) == '(' || cmd.at(i + 1) == '$' || isLetter(cmd.at(i + 1)))) {
				cout << "左括号后面只能是左括号、定积分符号或多项式名" << endl;
				return false;
			}
		}
		else if (cmd.at(i) == ')') {
			if (!(cmd.at(i + 1) == ')' || cmd.at(i + 1) == '+' || cmd.at(i + 1) == '*' || cmd.at(i + 1) == '!' )) {
				cout << "右括号后面只能是右括号，双目运算符或求导符号" << endl;
				return false;
			}
		}
	}
	//6.多项式名后面只能是双目运算符、求导符或右括号
	for (int i = 0; i < cmd.size() - 1; i++) {
		if (isLetter(cmd.at(i))) {
			if (!(isLetter(cmd.at(i + 1)) || cmd.at(i + 1) == '+' || cmd.at(i + 1) == '*' || cmd.at(i + 1) == '!' || cmd.at(i + 1) == ')')) {
				cout << "多项式名后面只能是双目运算符、求导符或右括号" << endl;
				return false;
			}
		}
	}
	//7.不能有未知多项式
	//先获取所有的多项式
	bool isF = false;
	vector<string> fomula_str;
	string fo;
	for (char c : cmd) {
		if (isLetter(c)) {
			isF = true;
		}
		else {
			isF = false;
		}
		if (isF) {
			fo += c;
		}
		else {
			if (fo.size() > 0) {
				fomula_str.push_back(fo);
				fo.clear();
			}
		}
	}
	if (fo.size() > 0) {
		fomula_str.push_back(fo);
		fo.clear();
	}
	for (string str : fomula_str) {
		auto it = fomulas->find(str);
		if (it == fomulas->end()) {
			cout << "不能含有未知多项式" << endl;
			return false;
		}
	}
	return true;
}

bool isLetter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

pair<double, double> split_integral(string *cmd) {
	//标记是否到了读取起点和终点的位置
	bool timeForDouble = false;
	bool first = true;//记录double的位置
	//记录多项式开始的位置
	int fomula_start = 0;
	string begin, end;
	int count = 0;
	for (char c : *cmd) {
		if (c == ']') {
			//开始读取多项式
			fomula_start = count + 1;
			timeForDouble = false;
		}
		count++;
		if (c == '[') {
			timeForDouble = true;
			continue;
		}
		if (c == ',') {
			first = false;
			continue;
		}
		if (timeForDouble) {
			if (first) {
				begin += c;
			}
			else {
				end += c;
			}
		}
	}
	pair<double, double> coe(atof(begin.c_str()), atof(end.c_str()));
	*cmd = cmd->substr(fomula_start, cmd->size() - fomula_start);
	return coe;
}

string strim_space(string str) {
	string s_str;
	for (char c : str) {
		if (c != ' ') {
			s_str += c;
		}
	}
	return s_str;
}

string strim_bracket(string str) {
	if (str.size() <= 0) {
		return str;
	}
	if (str.at(0) == '(') {
		str = str.substr(1, str.size() - 1);
	}
	if (str.at(str.size() - 1) == ')') {
		str = str.substr(0, str.size() - 1);
	}
	return str;
}

deque<double> operation_add(deque<double> f1, deque<double> f2) {
	int len = -1;
	deque<double> d;
	if (f1.size() > f2.size()) {
		len = f2.size();
		d = f1;
		for (int i = 0; i < len; i++) {
			d[i] = d[i] + f2.at(i);
		}
	}
	else {
		len = f1.size();
		d = f2;
		for (int i = 0; i < len; i++) {
			d[i] = d[i] + f1.at(i);
		}
	}
	return d;
}

void reverse_fomula() {
	cout << "当前已储存的多项式有：" << endl;
	for (map<string, deque<double>>::iterator it = fomulas->begin(); it != fomulas->end(); it++) {
		cout << it->first << "\t";
	}
	cout << "请选择：";
	string name;
	cin.clear();
	cin.ignore(1024, '\n');
	cin >> name;

}

deque<double> operation_mutiply(deque<double> f1, deque<double> f2) {
	//计算最高次项
	int maxf = f1.size() + f2.size() - 2 ;
	deque<double> d;
	for (int i = 0; i <= maxf; i++) {
		d.push_back(0);
		for (int j = 0; j <= i; j++) {
			if (j > f1.size() - 1 || (i - j) > f2.size() - 1) {
				//若次数超限，就不做任何操作
			}
			else {
				d[i] = d[i] + f1.at(j) * f2.at(i - j);
			}
		}
	}
	return d;
}

vector<string> dividedByAdd(string cmd) {
	bool countable = true;
	vector<string> parts;
	string piece;
	for (char c : cmd) {
		if (c == '(') {
			countable = false;
		}
		else if (c == ')') {
			countable = true;
		}
		if (countable&&c == '+') {
			if (piece.size() > 0) {
				parts.push_back(piece);
				piece.clear();
			}
		}
		else {
			piece += c;
		}
	}
	//处理最后的一段
	if (piece.size() > 0) {
		parts.push_back(piece);
	}
	return parts;
}

vector<string> dividedByMutiply(string cmd) {
	bool countable = true;
	vector<string> parts;
	string piece;
	for (char c : cmd) {
		if (c == '(') {
			countable = false;
		}
		else if (c == ')') {
			countable = true;
		}
		if (countable && c == '*') {
			if (piece.size() > 0) {
				parts.push_back(piece);
				piece.clear();
			}
		}
		else {
			piece += c;
		}
	}
	//处理最后的一段
	if (piece.size() > 0) {
		parts.push_back(piece);
	}
	return parts;
}

bool isSurByBrackets(string cmd) {
	return cmd.size() && (cmd.at(0) == '(') && (cmd.at(cmd.size() - 1) == ')');
}

void print_interface() {
	cout << "\t\t============================多项式计算器============================" << endl;
	cout << "\t\t   1.输入  2.混合运算  3.求逆元  4.除法/取模运算  5.求根  6.查看" << endl;
	cout << "\t\t====================================================================" << endl;
	cout << "\t\t-------------------------输入 quit 退出程序-------------------------" << endl;
	cout << "请选择：";
}

void print_fomula(deque<double> fomula) {
	deque<double> coe = fomula;
	bool firstNot0 = false;
	//从大到小排列
	reverse(coe.begin(), coe.end());
	for (int i = 0; i < coe.size(); i++) {
		//不显示系数为0的项
		if (coe.at(i) == 0 && i + 1 != coe.size()) {
			continue;
		}
		if (fabs(coe.at(i)) > ACCURACY) {
			//出现不为0的系数了
			firstNot0 = true;
		}
		if (i != 0 && firstNot0) {
			if (coe.at(i) < 0) {
				cout << " - ";
			}
			else if (coe.at(i) > 0) {
				cout << " + ";
			}
			if (fabs(fabs(coe.at(i)) - 1) >= ACCURACY||i+1==coe.size()) {
				cout << fabs(coe.at(i));
			}
		}
		else if (abs(abs(coe.at(i)) - 1) >= ACCURACY || i + 1 == coe.size()) {
			//绝对值为不为1
			cout << coe.at(i);
		}
		else if (abs(coe.at(i) - 1) >= ACCURACY || i + 1 == coe.size()) {
			//系数为-1
			cout << '-';
		}

		if (coe.size() - 1 - i > 0) {
			if ((coe.size() - 1 - i) == 1) {
				cout << "x";
			}
			else {
				cout << "x^" << (coe.size() - 1 - i);
			}
		}
	}
	cout << endl;
}

void print_fomula(string name) {
	map<string, deque<double>>::iterator it = fomulas->find(name);
	if (it == fomulas->end()) {
		cout << "不存在这个多项式!" << endl;
		return;
	}
	cout << it->first << " = ";
	print_fomula(it->second);
}

deque<double> operation_derivation(deque<double> fomula) {
	deque<double> d = fomula;
	//处理只有常数的特殊情况，若只有常数，求导后为0
	if (d.size() == 1) {
		d.at(0) = 0;
		return d;
	}
	//先变系数，在弹出首项变次数
	for (int i = 0; i < d.size(); i++) {
		d[i] = d.at(i) * i;
	}
	d.pop_front();
	return d;
}

double operation_integral(deque<double> fomula, double begin, double end) {
	deque<double> d = fomula;
	//先积分，再求值
	//先提高次数
	d.push_front(0);
	//再除以次数
	for (int i = 1; i < d.size(); i++) {
		d[i] = d.at(i) / i;
	}
	//求值
	double v1 = getValueAt(d, begin);
	double v2 = getValueAt(d, end);
	double v = v2 - v1;
	return v;
}

bool operator<(const deque<double> d1, const deque<double> d2) {
	int len = (d1.size() > d2.size()) ? d2.size() : d1.size();
	for (int i = 1; i <= len; i++) {
		if (d1.at(i) < d2.at(i)) {
			return true;
		}
		else if (d1.at(i) > d2.at(i)) {
			return false;
		}
	}
	if (d1.size() > d2.size()) {
		return false;
	}
	else if (d1.size() < d2.size()) {
		return true;
	}
	return false;
}

double getValueAt(deque<double> fomula, double key) {
	double value=0;
	//处理原点的情况
	if (abs(key - 0) <= ACCURACY) {
		value = fomula.at(0);
		return value;
	}
	for (int i = 0; i < fomula.size(); i++) {
		value = value + fomula.at(i) * pow(key, i);
	}
	return value;
}