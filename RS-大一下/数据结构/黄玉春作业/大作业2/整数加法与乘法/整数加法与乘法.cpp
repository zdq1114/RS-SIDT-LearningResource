#include<stdio.h>
typedef struct {
	int coef;
	int expn;
}ElemType;

typedef struct LNode {
	ElemType data;
	struct LNode* next;
}*Link, *Position;

typedef struct {
	Link head, tail;//ָ��ͷ����β���
	int len;//���Ա�Ԫ�ظ���
}Linklist;

typedef Linklist polyn;

void InitList(Linklist &L) {//��ʼ������
	L.head = new LNode; L.head->next = NULL;
	L.tail = L.head; L.len = 0;
}

Position GetHead(Linklist L) {//���ͷ���
	return L.head;
}

Position GetLast(Linklist L) {//������һ�����
	return L.tail;
}

int cmp(ElemType a, ElemType b) {//�Ƚ�ָ��
	if (a.expn < b.expn)return -1;
	else if (a.expn == b.expn)return 0;
	else return 1;
}

void InsFirst(Link h, Link s) {//hָ��ͷ��㣬��s��ָ�������һ�����֮ǰ
	s->next = h->next;
	h->next = s;
}

void DelFirst(Link h, Link &q) {//hָ��ͷ��㣬ɾ�������һ����㣬����q����
	q = h->next;
	h->next = q->next;
}

void Append(Linklist &L, Link s) {//��ָ��s��ָ���˴���ָ����������һ�����������Ա�L�����һ�����֮�󣬲��ı�����L��βָ��ָ���µ�β���
	L.tail->next = s;
	while (s) {
		s = s->next;
	}
	L.tail = s;
}

Position NextPos(Linklist L, Link p) {//��֪pָ�����Ա�L�е�һ����㣬����p��ָλ�õ�ֱ�Ӻ��λ�ã����ޣ��򷵻�NULL
	return p->next;
}

void FreeNode(Link &p) {//�ͷ�p��ָ�Ľ��
	delete p;
}

void MakeNode(Link &p, ElemType e) {//����һ����pָ���ֵΪe�Ľ��
	p = new LNode;
	p->data = e;
}

ElemType GetCurElem(Link p) {//����p��ָ����ֵ
	return p->data;
}

void SetCurElem(Link &p, ElemType e) {//��e����p��ָ����ֵ
	p->data = e;
}

int nLen_of_File(const char* path) {//��ȡ�ı������ֵĳ���
	FILE *fp = nullptr;
	fopen_s(&fp, path, "rt");
	char* p = nullptr;
	int nCount = 0; char ch;
	while (ch = fgetc(fp) != EOF) {
		nCount++;
	}
	fclose(fp);
	return nCount;
}

void CreatPolyn(polyn &P, int n, const char *path) {//���ı��л��һ������ʽ����
	InitList(P); Position head = GetHead(P);
	Position current;
	FILE *fp = nullptr;
	fopen_s(&fp, path, "rt");
	for (int i = 0; i < n; i++) {
		ElemType e;
		e.coef = fgetc(fp) - '0'; e.expn = i;
		current = new LNode; MakeNode(current, e);
		current->next = NULL;
		P.tail->next = current;
		P.tail = current;
	}
	fclose(fp);
	P.len = n;
}

void DestroyPolyn(polyn &P) {//��������
	Position ha = GetHead(P), qa = NextPos(P, ha);
	while (qa) {
		FreeNode(ha); ha = qa;
		qa = NextPos(P, ha);
	}
	FreeNode(ha);
}

bool ListEmpty(Linklist L) {//�ж��Ƿ�Ϊ������
	if (L.head->next == NULL)return true;
	return false;
}

void CarryBit(polyn &p) {//��λ
	Position h = GetHead(p);
	int temp1, temp2;
	Position q = NextPos(p, h);
	while (q != NULL) {
		temp1 = q->data.coef;
		q->data.coef %= 10;
		temp2 = temp1 / 10;
		q = q->next;
		if (q != NULL) q->data.coef += temp2;
	}
	if (temp2 != 0) {
		int expn = p.tail->data.expn;
		p.tail->next = new LNode;
		p.tail = p.tail->next;
		p.tail->data.coef = temp2; p.tail->data.expn = expn + 1;
		p.tail->next = NULL;
	}
}

void AddPolyn(polyn &Pa, polyn &Pb) {
	//����ʽ�ӷ���Pa=Pa+Pb��������������ʽ�Ľ�㹹�ɡ��Ͷ���ʽ��
	Position ha = GetHead(Pa), hb = GetHead(Pb);
	Position qa = NextPos(Pa, ha), qb = NextPos(Pb, hb);
	while (qa&&qb) {
		ElemType a = GetCurElem(qa), b = GetCurElem(qb);
		switch (cmp(a, b)) {
		case -1:
			ha = qa; qa = NextPos(Pa, qa); break;
		case 0:
			ElemType sum;
			sum.coef = a.coef + b.coef; sum.expn = a.expn;
			SetCurElem(qa, sum); ha = qa;
			DelFirst(hb, qb); FreeNode(qb); qb = NextPos(Pb, hb); qa = NextPos(Pa, ha);
			break;
		case 1:
			DelFirst(hb, qb); InsFirst(ha, qb);
			qb = NextPos(Pb, hb); ha = NextPos(Pa, ha); break;
		}
	}
	if (!ListEmpty(Pb)) { Append(Pa, qb); Pa.tail = Pb.tail; }
	CarryBit(Pa);
	FreeNode(hb);
}

void MultiplyPolyn(polyn &Pa, polyn &Pb) {
	Position ha = GetHead(Pa), hb = GetHead(Pb);
	polyn result; InitList(result);
	Position qb = NextPos(Pb, hb);
	for (int i = 0; i < Pb.len; i++) {
		Position qa = NextPos(Pa, ha);
		polyn temp; InitList(temp);
		Position ht = GetHead(temp);
		for (int j = 0; j < Pa.len; j++) {
			Link qt = new LNode;
			ElemType e;
			ElemType ea = GetCurElem(qa), eb = GetCurElem(qb);
			e.coef = ea.coef * eb.coef; e.expn = ea.expn + eb.expn;
			MakeNode(qt, e); InsFirst(ht, qt); ht = qt;
			temp.tail = qt;
			qa = NextPos(Pa, qa);
		}
		AddPolyn(result, temp);
		qb = NextPos(Pb, qb);
	}
	CarryBit(result);
	DestroyPolyn(Pa); DestroyPolyn(Pb);
	Pa = result;
}

void main() {
	char* path1 = "infile1.txt"; char* path2 = "infile2.txt"; char* path3 = "outfile.txt";
	int alen = nLen_of_File(path1);
	int blen = nLen_of_File(path2);
	polyn Pa, Pb;
	CreatPolyn(Pa, alen, path1); CreatPolyn(Pb, blen, path2);
	AddPolyn(Pa, Pb);

	FILE* fp = nullptr;
	fopen_s(&fp, path3, "wt");

	Position ha = GetHead(Pa);
	Position qa = NextPos(Pa, ha);
	fprintf_s(fp, "pa + pb = ");
	while (qa) {
		fprintf_s(fp, "%d", qa->data.coef);
		qa = qa->next;
	}

	fprintf_s(fp, "\n");

	CreatPolyn(Pa, alen, path1); CreatPolyn(Pb, blen, path2);
	MultiplyPolyn(Pa, Pb);

	ha = GetHead(Pa);
	qa = NextPos(Pa, ha);
	fprintf_s(fp, "pa * pb = ");
	while (qa) {
		fprintf_s(fp, "%d", qa->data.coef);
		qa = qa->next;
	}

	printf_s("Succeeded!\n");

	fclose(fp);
	DestroyPolyn(Pa);
}