#include "calculator.h"
#include <iostream>
#include <vector>
using namespace std;
//����״̬
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
	cout << "------------����ʽ������ ���˳�------------" << endl;
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
		cout << "�����ڴ˹��ܣ�" << endl;
	}
}

void save_fomula() {
	int len = -1;
	//���볤��
	cout << "����ʽ���ȣ�";
	cin >> len;
	//���ȺϷ��Լ��
	while (len <= 0 || len > 20) {
		cout << "�Ƿ����ȣ��Ϸ����ȷ�ΧΪ1-10" << endl;
		cout << "����ʽ���ȣ�";
		cin >> len;
	}
	//����ϵ��
	cout << "����ϵ����������ʽ��Ĵ����Ӵ�С���ÿո������" << endl;
	vector<double> coe;
	for (int i = 1; i <= len; i++) {
		double single_coe=0;
		cin >> single_coe;
		coe.push_back(single_coe);
	}
	//��ת����С��������
	reverse(coe.begin(), coe.end());
	//��������
	string name;
	cout << "����ʽ��Ϊ��";
	cin >>name;

}

void look_fomula() {

}

void compute_fomula() {

}

void print_interface() {
	cout << "\t\t============================����ʽ������============================" << endl;
	cout << "\t\t   1.����  2.�������  3.����Ԫ  4.����/ȡģ����  5.���  6.�鿴" << endl;
	cout << "\t\t====================================================================" << endl;
	cout << "\t\t-------------------------���� quit �˳�����-------------------------" << endl;
	cout << "��ѡ��";
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