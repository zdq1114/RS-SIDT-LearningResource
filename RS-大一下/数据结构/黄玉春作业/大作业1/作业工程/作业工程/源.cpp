#include<stdio.h>
#include<stdlib.h>
#include<time.h>


//��ýϴ���
int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

int main()
{
	//����output.txt�ļ�
	FILE* fp = nullptr;
	fopen_s(&fp, "output.txt", "wt");

	srand((unsigned)time(nullptr));
	int arr[8];
	for (int i = 0; i < 8; i++) {
		arr[i] = rand() * 20 / RAND_MAX - 10;
		//�ж��Ƿ��ظ�
		for (int j = 0; j < i; j++) {
			if (arr[j] == arr[i]) arr[i] = rand() * 20 / RAND_MAX - 10;
		}
		fprintf_s(fp, "%d ", arr[i]);
	}
	fclose(fp);

	int opt[8];
	opt[0] = arr[0];//�߽�
	int begin[8] = { 0 }, end = 0;//��ÿ����״̬opt�����п�ͷ������begin����
	for (int i = 1; i < 8; i++) {
		int temp = opt[i - 1];
		opt[i] = max(arr[i], temp + arr[i]);//״̬ת�Ʒ���
											//�ж�opt[i]�Ŀ�ͷ�Ƕ���
		if (arr[i] > temp + arr[i]) begin[i] = i;
		else begin[i] = begin[i - 1];
	}
	int max = opt[0];
	for (int i = 1; i < 8; i++) {
		if (opt[i] > max) {
			max = opt[i]; end = i;//�ж���״̬�����Ž⣬���ó����н�β
		}
	}
	int start = begin[end];
	fopen_s(&fp, "output.txt", "at");
	fprintf_s(fp, "\n����������������");
	for (; start <= end; start++) {
		fprintf_s(fp, "%d ", arr[start]);
	}
	fprintf(fp, "\n������%d", max);
	fclose(fp);
	return 0;
}
