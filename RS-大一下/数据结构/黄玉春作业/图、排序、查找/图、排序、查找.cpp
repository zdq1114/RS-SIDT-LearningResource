#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<iostream>
using namespace std;
#define INFINITY INT_MAX//最大值∞
#define MAX_VERTEX_NUM 20//最大顶点个数
#define MAXQSIZE 100
#define OK 1
#define ERROR 0
typedef int Status;
typedef int VRType;
typedef struct {
	char name[10] = { 0 };
	int value = 0;
}VertexType;
typedef int QElemType;
typedef struct ArcCell {
	VRType adj;
}ArcCell,AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];
typedef struct {
	VertexType vex[MAX_VERTEX_NUM];
	AdjMatrix arcs;
	int vexnum, arcnum;
}MGraph;
typedef struct {
	QElemType *base;
	int front;
	int rear;
}SqQueue;

Status InitQueue(SqQueue &Q) {
	//构造一个空队列Q
	Q.base = new QElemType[MAXQSIZE];
	if (!Q.base) exit(OVERFLOW);
	Q.front = Q.rear = 0;
	return OK;
}

Status EnQueue(SqQueue &Q, QElemType e) {
	if ((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR;//队列满
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MAXQSIZE;
	return OK;
}

Status DeQueue(SqQueue &Q, QElemType &e) {
	//若队列不空，则删除Q的对头元素，用e返回其值，并返回OK
	//否则返回ERROR
	if (Q.front == Q.rear)return ERROR;
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % MAXQSIZE;
	return OK;
}

bool QueueEmpty(SqQueue Q) {
	if (Q.front == Q.rear)return true;
	else return false;
}

int LocateVex(MGraph G, VertexType vex) {
	int i;
	for (i = 0; i < G.vexnum; i++) {
		if (!strcmp(G.vex[i].name , vex.name))break;
	}
	return i;
}

Status CreateUDN(MGraph &G){
	cout << "请输入顶点数和边数:";
	cin >> G.vexnum >> G.arcnum;
	for (int i = 0; i < G.vexnum; i++) {
		getchar();
		cout << "请输入第" << i+1 << "个顶点名称:";
		cin>> G.vex[i].name;
	}
	for (int i = 0; i < G.vexnum; i++) {
		for (int j = 0; j < G.vexnum; j++) {
			G.arcs[i][j] = { INFINITY };
		}
	}
	for (int k = 0; k < G.arcnum; k++) {
		VertexType v1, v2;
		int w;
		getchar();
		cout << "请输入一条边依附的顶点及权值:";
		//scanf("%s %s %d", &v1.name, &v2.name, &w);
		cin>>v1.name; getchar();
		cin>> v2.name; getchar();
		cin>>w; 

		int i = LocateVex(G, v1);
		int j = LocateVex(G, v2);
		G.arcs[i][j].adj = w;
		G.arcs[j][i].adj = w;
	}
	for (int i = 0; i < G.vexnum; i++) {
		for (int j = 0; j < G.vexnum; j++) {
			if (G.arcs[i][j].adj != INFINITY) {
				G.vex[i].value += G.arcs[i][j].adj;
			}
		}
	}
	return OK;
}


void BFSTraverse(MGraph G/*, Status(*Visit) (int v)*/,VertexType *arr) {
	bool visited[MAX_VERTEX_NUM];
	for (int v = 0; v < G.vexnum; v++) {
		visited[v] = false;
	}
	SqQueue Q;
	InitQueue(Q);
	for (int v = 0; v < G.vexnum; v++) {
		if (!visited[v]) {
			//cout << G.vex[v].name << " ";
			printf("%s ", G.vex[v].name);
			strcpy(arr[v].name , G.vex[v].name);
			arr[v].value = G.vex[v].value;
			visited[v] = true; /*Visit(v);*/
			EnQueue(Q, v);
			while (!QueueEmpty(Q)) {
				QElemType u;
				DeQueue(Q, u);
				for (int w = 0; w <= G.vexnum; w++) {
					if (G.arcs[u][w].adj != INFINITY && !visited[w]) {
						visited[w] = true; /*Visit(w);*/
						//cout<< G.vex[w].name << " ";
						printf("%s ", G.vex[w].name);
						strcpy(arr[w].name , G.vex[w].name);
						arr[w].value = G.vex[w].value;
						EnQueue(Q, w);
					}
				}
			}
		}
	}
}

void Popsort(VertexType *arr,int n) {
	for (int i = 0; i < n; i++) {
		for (int j = i; j < n; j++) {
			VertexType temp;
			if (arr[i].value > arr[j].value) {
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
		}
	}
}

int Search_Bin(int key, VertexType *arr, int n) {
	int low, high, mid;
	low = 0;
	high = n - 1;
	while (low<=high){    //査找范围不为0时执行循环体语句
		mid = (low + high) / 2;    //求中间位置
		if (key<arr[mid].value)    //key小于中间值时
			high = mid - 1;    //确定左子表范围
		else if (key>arr[mid].value)    //key 大于中间值时
			low = mid + 1;    //确定右子表范围
		else if (key == arr[mid].value){    //当key等于中间值时，证明查找成功
			return mid;
		}
	}
	return -1;
}

void main() {
	MGraph G;
	CreateUDN(G);
	cout << endl;
	cout << endl;
	cout << "该图的邻接矩阵为:" << endl;
	for (int i = 0; i < G.vexnum; i++) {
		for (int j = 0; j < G.vexnum; j++) {
			if (G.arcs[i][j].adj == INFINITY) cout << "∞\t";
			else cout << G.arcs[i][j].adj << "\t";
		}
		cout << endl;
	}
	VertexType Vex[MAX_VERTEX_NUM];
	cout << endl;
	cout << endl;
	cout << "按广度优先查找后结果为：";
	BFSTraverse(G,Vex);
	cout << "\n";
	cout << endl;
	cout << "根据顶点属性值由小到大排列：";
	Popsort(Vex,G.vexnum);
	for (int i = 0; i < G.vexnum; i++) {
		cout << Vex[i].name << " ";
	}
	cout << endl;
	cout << endl;
	int key;
	cout << "请输入需要查找的属性值：";
	cin >> key;
	int index = Search_Bin(key,Vex , G.vexnum);
	cout << endl;
	cout << "该顶点名称为：" << Vex[index].name << endl;
}