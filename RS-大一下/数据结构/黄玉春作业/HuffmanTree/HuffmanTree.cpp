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
	//该函数用于制作频率表，从filepath文件中读取字符，以STL中的map模板做匹配，并保存到output文件中去
	//freq是map模板，由char字符对应到int频率
	string str;
	ifstream in(filepath, ios::in);//以读取方式打开文件
	while (getline(in, str)) {
		for (int i = 0; i != str.size(); i++) {
			if (str[i] == ' ')str[i] = '-';//'-' for space，如果该字符为空格，则以'-'代替
			if (freq.end() == freq.find(str[i])) {//如果没有在频率表中找到对应的字符，则插入该字符频率并置为1
				freq.insert(make_pair(str[i], 1));
			}
			else freq[str[i]]++;//找到该字符，使频率加1
		}
		if (freq.end() == freq.find('\n'))freq.insert(make_pair('!', 1));//'!' for end of line，若读到换行符，则以'!'代替换行符
		else freq['!']++;
	}
	freq.insert(make_pair('+', 1));//'+' for end of message，文件读完后，用'+'代替文件结尾，并置为1
	ofstream out(output, ios::out);
	out << "字符\t频率" << endl;
	map<char, int>::iterator iter;
	//在频率表中输出频率
	for (iter = freq.begin(); iter != freq.end(); iter++) {
		out << (*iter).first << '\t' << (*iter).second << endl;	
	}
	in.close();
	out.close();
}

void CodeFile(const char* output,map<char,char*> &code,map<char,int> freq,HuffmanCode HC) {
	//该函数用于输出字符编码的文件，结果保存到CodeFile中
	//code是map模板，由char字符对应到char*该字符的编码，freq是频率表，HC是保存字符编码的指针
	//其中code和frep中的第一个char字符是一一对应的
	int i = 1;
	map<char, int>::iterator fiter = freq.begin();
	while (fiter != freq.end()) {
		code.insert(make_pair((*fiter).first, HC[i]));
		i++;
		fiter++;
	}
	ofstream out(output, ios::out);
	out << "字符\t代码" << endl;
	map<char, char*>::iterator citer = code.begin();
	for (; citer != code.end(); citer++) {
		out << (*citer).first << '\t' << (*citer).second << endl;
	}
	out.close();
}

void Select(HuffmanTree HT, int n, int &s1, int &s2){
	//从HT中选出双亲结点为0且权最小的两个结点，保存到s1,s2中去，其中s1<s2
	int i, min;
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0){
			min = i;//初始化min
			break;
		}
	}
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0){
			if (HT[i].weight<HT[min].weight)
				min = i;//更新min，找到最小的
		}
	}
	s1 = min;//找到s1
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0 && i != (s1)){
			min = i;//重新初始化min
			break;
		}
	}
	for (i = 1; i <= n; i++){
		if (HT[i].parent == 0 && i != (s1)){
			if (HT[i].weight<HT[min].weight)
				min = i;//从剩下的结点中，找到权值最小的
		}
	}
	s2 = min;//找到s2
}

void HuffmanCoding(HuffmanTree &HT, HuffmanCode &HC, int *w, int n) {
	//w存放n个字符的权值，构造树HT，求出n个字符的赫夫曼编码HC
	if (n <= 1)return;
	int m = 2 * n - 1;
	HT = new HTNode[m+1];//0号单元未用
	HuffmanTree p = NULL;
	int i;
	for (p = HT + 1, i = 1; i <= n; i++, p++, w++) {
		*p = { *w,0,0,0 };
	}//初始化表的前n行
	for (; i <= m; i++, p++) {
		*p = { 0,0,0,0 };
	}//初始化表的n+1到m行
	for (i = n + 1; i <= m; i++) {
		//在HT[1...i-1]选择parent为0且weight最小的两个结点，其序号分别为s1和s2
		int s1, s2;
		Select(HT, i - 1, s1, s2);
		HT[s1].parent = i; HT[s2].parent = i;
		HT[i].lchild = s1; HT[i].rchild = s2;//左孩子结点的权小于右孩子结点的权
		HT[i].weight = HT[s1].weight + HT[s2].weight;
	}
	//——从叶子到根逆序求每个字符的赫夫曼编码
	HC = new char*[n+1];//分配n个字符编码的头指针向量
	char* cd = new char[n];//分配求编码的工作空间
	cd[n - 1] = '\0';//编码结束符
	for (i = 1; i <= n; i++) {//逐个字符求赫夫曼编码
		int start = n - 1;//编码结束符位置
		for (int c = i, f = HT[i].parent; f != 0; c = f, f = HT[f].parent) {//从叶子到根逆向求编码
			//左0右1
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