#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<fstream>
#include<string>
#include<map>
#include<iostream>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
typedef struct {
	int weight;
	int parent;
	int lchild;
	int rchild;
}HTNode,*HuffmanTree;

typedef char** HuffmanCode;

void FrequencyTable(const char *filepath, const char *output, map<char,int> &freq) {
	//�ú�����������Ƶ�ʱ���filepath�ļ��ж�ȡ�ַ�����STL�е�mapģ����ƥ�䣬�����浽output�ļ���ȥ
	//freq��mapģ�壬��char�ַ���Ӧ��intƵ��
	string str;
	ifstream in(filepath, ios::in);//�Զ�ȡ��ʽ���ļ�
	while (getline(in, str)) {
		for (int i = 0; i != str.size(); i++) {
			if (str[i] == ' ')str[i] = '-';//'-' for space��������ַ�Ϊ�ո�����'-'����
			if (freq.end() == freq.find(str[i])) {//���û����Ƶ�ʱ����ҵ���Ӧ���ַ����������ַ�Ƶ�ʲ���Ϊ1
				freq.insert(make_pair(str[i], 1));
			}
			else freq[str[i]]++;//�ҵ����ַ���ʹƵ�ʼ�1
		}
		if (freq.end() == freq.find('\n'))freq.insert(make_pair('!', 1));//'!' for end of line�����������з�������'!'���滻�з�
		else freq['!']++;
	}
	freq.insert(make_pair('+', 1));//'+' for end of message���ļ��������'+'�����ļ���β������Ϊ1
	ofstream out(output, ios::out);
	out << "�ַ�\tƵ��" << endl;
	map<char, int>::iterator iter;
	//��Ƶ�ʱ������Ƶ��
	for (iter = freq.begin(); iter != freq.end(); iter++) {
		out << (*iter).first << '\t' << (*iter).second << endl;	
	}
	in.close();
	out.close();
}

void CodeFile(const char* output,map<char,char*> &code,map<char,int> freq,HuffmanCode HC) {
	//�ú�����������ַ�������ļ���������浽CodeFile��
	//code��mapģ�壬��char�ַ���Ӧ��char*���ַ��ı��룬freq��Ƶ�ʱ�HC�Ǳ����ַ������ָ��
	//����code��frep�еĵ�һ��char�ַ���һһ��Ӧ��
	int i = 1;
	map<char, int>::iterator fiter = freq.begin();
	while (fiter != freq.end()) {
		code.insert(make_pair((*fiter).first, HC[i]));
		i++;
		fiter++;
	}
	ofstream out(output, ios::out);
	out << "�ַ�\t����" << endl;
	map<char, char*>::iterator citer = code.begin();
	for (; citer != code.end(); citer++) {
		out << (*citer).first << '\t' << (*citer).second << endl;
	}
	out.close();
}

void Select(HuffmanTree HT, int n, int &s1, int &s2){
	//��HT��ѡ��˫�׽��Ϊ0��Ȩ��С��������㣬���浽s1,s2��ȥ������s1<s2
	int i, min;
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0){
			min = i;//��ʼ��min
			break;
		}
	}
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0){
			if (HT[i].weight<HT[min].weight)
				min = i;//����min���ҵ���С��
		}
	}
	s1 = min;//�ҵ�s1
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0 && i != (s1)){
			min = i;//���³�ʼ��min
			break;
		}
	}
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0 && i != (s1)){
			if (HT[i].weight<HT[min].weight)
				min = i;//��ʣ�µĽ���У��ҵ�Ȩֵ��С��
		}
	}
	s2 = min;//�ҵ�s2
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n) {
	//w���n���ַ���Ȩֵ��������HT�����n���ַ��ĺշ�������HC
	if (n <= 1)return;
	int m = 2 * n - 1;
	HT = new HTNode[m+1];//0�ŵ�Ԫδ��
	HuffmanTree p = NULL;
	int i;
	for (p = HT + 1, i = 1; i <= n; i++, p++, w++) {
		*p = { *w,0,0,0 };
	}//��ʼ�����ǰn��
	for (; i <= m; i++, p++) {
		*p = { 0,0,0,0 };
	}//��ʼ�����n+1��m��
	for (i = n + 1; i <= m; i++) {
		//��HT[1...i-1]ѡ��parentΪ0��weight��С��������㣬����ŷֱ�Ϊs1��s2
		int s1, s2;
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i; HT[s2].parent = i;
		HT[i].lchild = s1; HT[i].rchild = s2;//���ӽ���ȨС���Һ��ӽ���Ȩ
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//������Ҷ�ӵ���������ÿ���ַ��ĺշ�������
	HC = new char*[n+1];//����n���ַ������ͷָ������
	char* cd = new char[n];//���������Ĺ����ռ�
	cd[n - 1] = '\0';//���������
	for (i = 1; i <= n; i++) {//����ַ���շ�������
		int start = n - 1;//���������λ��
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) {//��Ҷ�ӵ������������
			//��0��1
			if (HT[f].lchild == c) cd[--start] = '0';
			else cd[--start] = '1';
		}
		HC[i] = new char[n - start];
		strcpy(HC[i], &cd[start]);
	}
	delete[] cd;
}

void Encodedmessage(const char* infile, const char *outfile, map<char, char*>code) {
	ifstream in(infile, ios::in);
	ofstream out(outfile, ios::out | ios::binary);
	string str;
	int i = 0;
	while (getline(in,str)) {
		for (i = 0; i < str.size(); i++) {
			if (str[i] == ' ') out << code['-'];
			else out << code[str[i]];
			out << ' ';
		}
		out << code['!'] << ' ';
	}
	out << code['+'];
	in.close();
	out.close();
}

void Decodemessage(const char* infile, const char *outfile, map<char, char*>code) {
	ifstream in(infile, ios::in | ios::binary);
	ofstream out(outfile, ios::out);
	char cd[100];
	map<char, char*>::iterator iter;
	while (in.getline(cd, 20, ' ')) {
		for (iter = code.begin(); iter != code.end(); iter++) {
			if (!strcmp(cd,(*iter).second)) {
				if ((*iter).first == '-')out << ' ';
				else if ((*iter).first == '!')out << '\n';
				else if ((*iter).first == '+')break;
				else out << (*iter).first;
			}
		}
	}
	in.close();
	out.close();
}

void main() {
	map<char, int> freq;
	FrequencyTable("A4-message.txt", "frequency.txt", freq);
	int n = freq.size();
	int *w = new int[n];
	map<char, int>::iterator fiter = freq.begin();
	for (int i = 0; i < n; i++) {
		w[i] = (*fiter).second;
		fiter++;
	}
	HuffmanTree HT = NULL;
	HuffmanCode HC;
	HuffmanCoding(HT, HC, w, n);
	map<char, char*> code;
	map<char, char*>::iterator citer = code.begin();
	CodeFile("code.txt", code, freq, HC);
	Encodedmessage("A4-message.txt", "encodedmessage.bin", code);
	Decodemessage("encodedmessage.bin", "decodedmessage.txt", code);
	delete[] w;
	code.clear(); freq.clear();
	cout << "Success!" << endl;
}