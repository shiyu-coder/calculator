#pragma once
#include <deque>
#include <string.h>
#include <string>
#include <map>
using namespace std;
/**
 * calculator�⣺
 * �ṩ�����Ķ���ʽ���㺯�����������̿��ƺ���
 */

//�������еĶ���ʽ
//����ʽ��ϵ����С��������
map<string, deque<double>> *fomulas;

//���ܺ���////////////////////////////////////////////////////
//����
extern void save_fomula();
//�鿴
extern void look_fomula();
//�������
extern void compute_fomula();
///////////////////////////////////////////////////////////////

//��ӡ����
extern void print_interface();
//��ӡһ������ʽ
extern void print_fomula(string name);
//����ɸѡ��
extern void command_index(string index);

//������
extern deque<double> operation_derivation(deque<double> fomula);
//��������
extern double operation_integral(deque<double> fomula, double begin, double end);
