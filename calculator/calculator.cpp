#include "calculator.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <sstream>
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
	case 3:reverse_fomula();
		break;
	case 4:division_fomula();
		break;
	case 5:root_fomula();
		break;
	case 6:look_fomula();
		break;
	case 7:draw_fomula();
		break;
	case 8:fitting_fomula();
		break;
	default:
		cout << "不存在此功能！" << endl;
	}
}

void draw_fomula() {
	cout << "所有多项式名称：" << endl;
	for (map<string, deque<double>>::iterator it = fomulas->begin(); it != fomulas->end(); it++) {
		cout << it->first << "\t";
	}
	cout << endl;
	cout << "要作图的多项式名为：";
	cin.clear();
	cin.ignore(1024, '\n');
	string name;
	cin >> name;
	auto it = fomulas->find(name);
	if (it == fomulas->end()) {
		cout << "不存在此多项式！" << endl;
		return;
	}
	cout << "作图原点x值为：";
	double zero_point = 0;
	cin.clear();
	cin.ignore(1024, '\n');
	cin >> zero_point;
	draw_graph(it->second,zero_point);
}

void draw_graph(deque<double> fomula, double zero_point) {
	//暂定y轴上下高度为20
	const double height = 20;
	const double wide = 55;
	//单位点之间的宽度
	const double node_blank = 4;
	//精度
	/*double accuracy = 0.3;*/
	double unit = 1;
	double maxValue = -99999999;
	vector<double> values;
	for (int i = zero_point-wide; i <= zero_point+wide; i++) {
		double v = getValueAt(fomula, i);
		if (v > maxValue) {
			maxValue = v;
		}
		values.push_back(v);
	}
	//当大于height时就增加单位大小
	while (maxValue/unit >=  height) {
		unit = maxValue /  height+1;
	}
	/*accuracy = maxValue / (unit * 50);*/
// 	if (maxValue >= 1000) {
// 		accuracy = accuracy + 0.1;
// 	}
// 	if (accuracy < 0.1) {
// 		accuracy = 0.1;
// 	}
	cout << "x轴单位：" << node_blank << endl;
	cout << "y轴单位：" << unit*node_blank << endl;
	/*cout << "精度：" << accuracy << endl;*/
	//cout << "坐标原点x值：" << zero_point << endl;
	//更新和调整函数值
	for (int i = 0; i < values.size(); i++) {
		values[i] = height- values[i] / unit;
	}
	// i：y | j：x
	bool* draw = new bool[(int)(2 * wide)];
	for (int i = 1; i < 2 * wide; i++) {
		draw[i] = true;
	}
	for (int i = 1; i <= 2 * height; i++) {
		for (int j = -wide; j < wide; j++) {
			if (i == height) {
				//画x轴
				if (j % (int)node_blank == 0) {
					cout << "+";
				}
				else {
					cout << "-";
				}
				if (draw[(int)(j + wide)] && i < 2 * height && abs(i - values.at(j + wide)) < abs((i * 1.0 + 1) - values.at(j + wide))) {
					draw[(int)(j + wide)] = false;
				}
			}
			else if (j == 0) {
				//画y轴
				if (i % (int)node_blank == 0) {
					cout << "+";
				}
				else {
					cout << "|";
				}
			}
			/*else if (abs(i - values.at(j + wide)) <= accuracy) {
				cout << "*";
			}*/
			else if (draw[(int)(j + wide)]&&i < 2 * height && abs(i - values.at(j + wide)) < abs((i*1.0 + 1) - values.at(j + wide))) {
				cout << "*";
				draw[(int)(j + wide)] = false;
			}
			else {
				cout << " ";
				if (i < 2 * height && abs(i - values.at(j + wide)) > abs((i*1.0 + 1) - values.at(j + wide))) {
					draw[(int)(j + wide)] = true;
				}
			}
		}
		cout << endl;
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
			if (cmd.at(i) >= '0' && cmd.at(i) <= '9'||cmd.at(i)=='-') {
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
			if (cmd.at(i) >= '0' && cmd.at(i) <= '9' || cmd.at(i) == '-') {
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
			left_bra++;
			bracket = true;
		}
		else if (c == ')') {
			if (bracket) {
				cout << "括号内不能为空" << endl;
				return false;
			}
			bracket = false;
			right_bra++;
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
			break;
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
	cout << endl;
	cout << "请选择：";
	string name;
	cin.clear();
	cin.ignore(1024, '\n');
	getline(cin, name);
	if (name.size() <= 0 || fomulas->find(name) == fomulas->end()) {
		cout << "不存在此多项式！" << endl;
		return;
	}
	deque<double> fo = fomulas->find(name)->second;
	//检查是否可逆
	if (abs(fo.at(0)) <= ACCURACY) {
		cout << "此多项式不可逆！" << endl;
		return ;
	}
	deque<double> re_fo;
	//计算第一项
	re_fo.push_back(1.0 / fo.at(0));
	for (int i = 1; i < fo.size(); i++) {
		double coe=0;
		for (int j = 0; j <= i - 1; j++) {
			coe = coe - re_fo.at(j) * (fo.at(i - j) / fo.at(0));
		}
		re_fo.push_back(coe);
	}
	//打印逆元
	cout << "逆元为：" << name << "^-1 = ";
	print_fomula(re_fo);
}

void division_fomula() {
	cout << "请输入表达式(F/G)：";
	string cmd;
	cin.clear();
	cin.ignore(1024, '\n');
	getline(cin, cmd);
	string F_str, G_str;
	bool second = false;
	//提取两个多项式
	for (char c : cmd) {
		if (c == '/') {
			second = true;
			continue;
		}
		if (second) {
			G_str += c;
		}
		else {
			F_str += c;
		}
	}
	//检查两个多项式是否存在
	deque<double> F, G;
	auto f_it = fomulas->find(F_str);
	if (f_it == fomulas->end()) {
		cout <<"多项式 "<<F_str<<" 不存在！"<<endl;
		return;
	}
	auto g_it = fomulas->find(G_str);
	if (g_it == fomulas->end()) {
		cout << "多项式 " << G_str << " 不存在！" << endl;
		return;
	}
	F = f_it->second;
	G = g_it->second;
	//Fr(x)∗Gr(x)^−1 = Qr(x)(modx^(n−m + 1))
	//求Fr(x)和Qr(x)
	reverse(F.begin(), F.end());
	reverse(G.begin(), G.end());
	//被除多项式次数要不低于除多项式
	if (F.size() < G.size()) {
		cout << "被除多项式次数要不低于除多项式!" << endl;
		return;
	}
	//求Gr(x)^-1，先弹出Gr(x)前的所有0
	while (abs(G.at(0)) <= ACCURACY) {
		G.pop_front();
	}
	deque<double> re_G;
	re_G.push_back(1.0 / G.at(0));
	for (int i = 1; i < G.size(); i++) {
		double coe = 0;
		for (int j = 0; j <= i - 1; j++) {
			coe = coe - re_G.at(j) * (G.at(i - j) / G.at(0));
		}
		re_G.push_back(coe);
	}
	//求出Qr(x)
	deque<double> Q;
	Q = operation_mutiply(F, re_G);
	//mod(x^(n-m+1))
	int times = F.size() - G.size() + 1;
	while (Q.size() > times) {
		Q.pop_back();
	}
	reverse(Q.begin(), Q.end());
	//求R(x)
	//先把G和F反转回来
	reverse(G.begin(), G.end());
	reverse(F.begin(), F.end());
	deque<double> Q_G = operation_mutiply(Q, G);
	deque<double> R;
	for (int i = 0; i < Q_G.size(); i++) {
		R.push_back(F.at(i) - Q_G.at(i));
	}
	//去除R末尾的0项
	while (R.size() > 0 && abs(R.at(R.size() - 1)) <= ACCURACY) {
		R.pop_back();
	}
	//打印结果
	cout << "商为：";
	print_fomula(Q);
	cout << "余数为：";
	print_fomula(R);
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

void root_fomula() {
	cout << "当前已储存的多项式有：" << endl;
	for (map<string, deque<double>>::iterator it = fomulas->begin(); it != fomulas->end(); it++) {
		cout << it->first << "\t";
	}
	cout << endl;
	cout << "多项式名为：";
	string name;
	cin.clear();
	cin.ignore(1024, '\n');
	getline(cin, name);
	auto it = fomulas->find(name);
	if (it == fomulas->end()) {
		cout << "不存在此多项式！" << endl;
		return;
	}
	deque<double> fo = it->second;
	deque<double> fo_d = operation_derivation(fo);
	//精度
	const double limit = 0.00001;
	double x = 1;
	double x0 = 0;
	long int num = 0;
	while (fabs(x - x0) > limit) {
		num++;
		if (num > 200) {
			//如果进行200次循环后，x-x0仍然很大，则可以认为没有根
			if (fabs(x - x0) > 0.1) {
				cout << "此函数不存在函数值为0的根" << endl;
				return;
			}
		}
		x0 = x;
		x = x0 - getValueAt(fo,x0) / getValueAt(fo_d,x0);
	}
	cout << name << " 的存在实数根为：" << x << endl;
}

vector<string> dividedByAdd(string cmd) {
	int countable = 0;
	vector<string> parts;
	string piece;
	for (char c : cmd) {
		if (c == '(') {
			countable++;
		}
		else if (c == ')') {
			countable--;
		}
		if (countable==0&&c == '+') {
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
	int countable = 0;
	vector<string> parts;
	string piece;
	for (char c : cmd) {
		if (c == '(') {
			countable ++;
		}
		else if (c == ')') {
			countable --;
		}
		if (countable==0 && c == '*') {
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
	cout << "\t  ======================================多项式计算器=====================================" << endl;
	cout << "\t    1.输入  2.混合运算  3.求逆元  4.除法/取模运算  5.求根  6.查看  7.作图  8.多项式拟合" << endl;
	cout << "\t  =======================================================================================" << endl;
	cout << "\t  -----------------------------------输入 quit 退出程序----------------------------------" << endl;
	cout << "请选择：";
}

void print_fomula(deque<double> fomula) {
	if (fomula.size() <= 0) {
		cout << "0" << endl;
		return;
	}
	deque<double> coe = fomula;
	//从去掉为0的最高次项
	while (fabs(coe.at(coe.size() - 1)) <= ACCURACY) {
		coe.pop_back();
	}
	bool firstNot0 = false;
	//从大到小排列
	reverse(coe.begin(), coe.end());
	for (int i = 0; i < coe.size(); i++) {
		//不显示系数为0的项
		if (abs(coe.at(i))<=ACCURACY && coe.size()>1) {
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

double sum_fomula(deque<double> fomula, int n) {
	double all = 0;
	for (int i = 0; i < n; i++) {
		all = all + fomula.at(i);
	}
	return all;
}

double sum_multi(deque<double> fo1, deque<double> fo2, int n) {
	double all = 0;
	for (int i = 0; i < n; i++) {
		all = all + fo1.at(i) * fo2.at(i);
	}
	return all;
}

double sum_pow(deque<double> fomula, int n, int m) {
	double all = 0;
	for (int i = 0; i < n; i++) {
		all = all + pow(fomula.at(i), m);
	}
	return all;
}

double sum_pow_multi(deque<double> fo1, deque<double> fo2, int n, int m) {
	double all = 0;
	for (int i = 0; i < n; i++) {
		all = all + pow(fo1.at(i), m) * fo2.at(i);
	}
	return all;
}

deque<double> operation_polynomial_fitting(deque<double> fo1, deque<double> fo2, int n, int m) {
	static double M[101][101] = { 0 };
	//求增广矩阵
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= m; j++) {
			M[i][j] = sum_pow(fo1, n, i + j - 2);
		}
		M[i][m + 1] = sum_pow_multi(fo1, fo2, n, i - 1);
	}
	M[1][1] = n;
	//求解方程组
	for (int i = 1; i < m; i++) {
		for (int j = i + 1; j < m + 1; j++) {
			double p = 0;
			if (M[i][i] != 0) {
				p = M[j][i] / M[i][i];
			}
			for (int k = i; k < m + 2; k++) {
				M[j][k] = M[j][k] - M[i][k] * p;
			}
		}
	}
	deque<double> coe;
	coe.push_back(M[m][m + 1] / M[m][m]);
	//求解系数
	for (int i = m - 1; i >= 1; i--) {
		coe.push_front((M[i][m + 1] - auxiliary_operation_polynomial_fitting(M, coe, i + 1, m)) / M[i][i]);
	}
	memset(M, 0, sizeof(M));
	return coe;
}

double auxiliary_operation_polynomial_fitting(double M[][101], deque<double> coe, int l, int m) {
	double all = 0;
	for (int i= l; i <= m; i++) {
		all = all + M[l - 1][i] * coe.at(i - l);
	}
	return all;
}

void fitting_fomula() {
	cout << "输入要拟合的点的个数：";
	cin.clear();
	cin.ignore(1024, '\n');
	int num=0;
	cin >> num;
	if (num <= 0) {
		cout << "点的个数不合理！" << endl;
		return;
	}
	cout << "输入要拟合的多项式的点的坐标（格式：x1 y1 x2 y2 ……)：" << endl;
	cin.clear();
	deque<double> dx;
	deque<double> dy;
	for (int i = 1; i <= 2 * num; i++) {
		double input = 0;
		cin >> input;
		if (i % 2 == 0) {
			dy.push_back(input);
		}
		else {
			dx.push_back(input);
		}
	}
	cout << "输入要拟合的多项式次数：";
	cin.clear();
	int m = 0;
	cin >> m;
	while (m+1 > dx.size()) {
		cout << "多项式系数个数大于点的个数，无法进行拟合！" << endl;
		cout << "输入要拟合的多项式次数：";
		m = 0;
		cin >> m;
	}
	//进行拟合
	deque<double> fomula = operation_polynomial_fitting(dx, dy, dx.size(), m + 1);
	string _name = "FIT";
	int i = 0;
	string name = "FIT";
	while (fomulas->find(name) != fomulas->end()) {
		i++;
		stringstream ss;
		ss << i;
		string number = ss.str();
		name.clear();
		name = _name + number;
	}
	fomulas->insert(pair<string,deque<double>>(name,fomula));
	cout << "多项式拟合结果（已自动储存该多项式）：" << endl;
	cout << name << " = ";
	print_fomula(fomula);
}