#pragma once
#include "class.h"
#include <string>
bool isVisited[100];
using namespace std;
int mat[100][3];
void printMatrix(Graph g) {
	//��DFA��״̬ת�Ʊ����һ����ά������

	for (int i = 0; i < g.nodes.size(); i++) {
		mat[i][0] = -1;
		mat[i][1] = -1;
		mat[i][2] = -1;
	}
	for (int k = 0; k < g.nodes.size(); k++) {
		Edge* p = new Edge();
		p = g.nodes[k].edge;
		while (p) {
			mat[k][0] = k;
			if (p->wt == '0')
				mat[k][1] = p->adjvex;
			else if (p->wt == '1')
				mat[k][2] = p->adjvex;

			p = p->next;
		}
		//cout << endl;
	}

}



void DFS(Graph G, int i, vector<bool> final, bool* isDeath) {
	//����������������鿴һ�������Ƿ���ڵ���ֹ״̬��·��
	if (final[i])
		*isDeath = true;
	isVisited[i] = true;
	Edge* p = new Edge();
	p = G.nodes[i].edge;
	while (p) {
		if (isVisited[p->adjvex] == false)
			DFS(G, p->adjvex, final, isDeath);
		p = p->next;
	}

}

void deleteAdjacencyListElement(Graph* G, int n, int vertex) {
	//����ɾ����״̬
	Edge* p = new Edge();
	Edge* q = new Edge();
	Edge* r = new Edge();
	//�����������飬��ɾ�����������еı�ɾ����
	for (int i = 0; i < n; i++) {
		p = G->nodes[i].edge;
		for (int j = vertex + 1; j <= n; j++) {
			G->nodes[j - 1].final = G->nodes[j].final;
			G->nodes[j - 1].start = G->nodes[j].start;
			G->nodes[j - 1].edge = G->nodes[j].edge;
		}
		n--;
		break;

	}



	//ɾ����ɾ�������ӵı���Ϣ
	while (p) {
		q = p->next;
		delete p;
		p = q;
	}
	//ɾ��������������position��صı߽��
	for (int i = 0; i <= n; i++) {
		p = G->nodes[i].edge;
		while (p) {
			if (p->adjvex == vertex) {
				if (p == G->nodes[i].edge) {
					G->nodes[i].edge = p->next;
				}
				else {
					r->next = p->next;
				}
				q = p;
				p = p->next;
				delete q;
			}
			else {
				if (p->adjvex > vertex) {
					p->adjvex--;
				}
				r = p;
				p = p->next;
			}
		}
	}
	G->nodes.pop_back();

}
//����Ƿ�����״̬����
void check(Graph* G) {
	vector<bool> finalvex;
	for (int i = 0; i < G->nodes.size(); i++) {
		finalvex.push_back(false);
	}
	bool isDeath = false;
	int Deathvex = -1;
	for (int i = 0; i < G->nodes.size(); i++)
	{
		if (G->nodes[i].final)
			finalvex[i] = true;
	}
	for (int i = 0; i < G->nodes.size(); i++) {
		if (!finalvex[i]) {
			for (int k = 0; k < 100; k++)
				isVisited[k] = false;
			DFS(*G, i, finalvex, &isDeath);//���ĳ���Ƿ���·���ﵽ��̬��
			if (!isDeath) {
				Deathvex = i;
				deleteAdjacencyListElement(G, G->nodes.size() - 1, Deathvex);//û����ɾ����״̬�����

				break;
			}
		}
	}
	if (!isDeath)//���������״̬����ɾ�����ټ��һ�Ρ�
		check(G);



}

//���ֺ�����������С��DFA
Graph *Div(Graph* G) {
	printMatrix(*G);
	bool newD = false;
	int div[100]; //�����Ӧ���ֳ����ĵڼ������ֿ�
	int numOfdiv = 1;//���ּ�������
	//������̬����Ϊ"0"���ϣ���̬Ϊ"1"����
	for (int i = 0; i < G->nodes.size(); i++) {
		if (G->nodes[i].final) {
			div[i] = 1;
		}
		else div[i] = 0;
	}

	//��ʼ���֣��˲������Լ������Լ������д�ģ������е��ң����ܻ�ɬ����⣬��ע�Ϳ����޷����͡�����������������ʣ������Իش�
again:
	for (int i = 0; i <= numOfdiv; i++) {

		int now[100]; int k = 0;
		bool isSame = false;
		bool newDiv = false;
		bool Istp = false;
		//��ʱ�������ڴ�ŵ�ǰ���ּ���
		for (int j = 0; j < G->nodes.size(); j++) {
			if (div[j] == i) {
				now[k++] = j;
			}
		}
		//�����ǰ��������ͬһ�ַ��õ���״̬һ�£���ֱ�������˴��ַ�ѭ�����
		for (int t = 1; t < 3; t++) {
			for (int i1 = 0; i1 < k; i1++) {
				if (div[mat[now[0]][t]] == div[mat[now[i1]][t]])
					isSame = true;
				else {
					isSame = false;
					break;
				}
			}
			if (isSame && t == 1)//�����˴��ַ�ѭ�����
				continue;
			if (isSame) 
				goto out;//��������ַ���һ�£���ü��ϲ��û���

			//����Ϊ��һ�����
			int tp = 0;
			int temp[100];
			int nowDiv = numOfdiv;
			int newnow[100];
			int newLength = 0;
			int newIndex = 0;
			//��������Ԫ�أ��������ת��״̬�Ƿ�����ͬһ����
			for (int key = 0; key <= nowDiv; key++) {

				for (int m = 0; m < k; m++) {

					if (div[mat[now[m]][t]] == key) {
						temp[now[m]] = tp;//��ʱ����Ŷ�Ӧת�Ƽ���
						Istp = true;
						if (tp == 0) {
							newnow[newIndex++] = now[m];
							newLength++;//��һ�����ּ��Ϸ���ԭ����
						}
						if (tp > 0) {
							div[now[m]] = numOfdiv + 1;//ʣ�໮�ּ��ϲ����¼��ϣ���������+1
							newDiv = true;//��־�¼��ϲ���
						}
					}
				}
				if (Istp)
					tp++;
				if (newDiv) {
					numOfdiv++;
					newD = true;//newD��ʾһ���ֻ�����Ϻ�����»��֣���Ҫ�ٻ���һ��
				}
				newDiv = false;
				Istp = false;

			}
			for (int i = 0; i < newLength; i++) {
				now[i] = newnow[i];
			}
			k = newLength;

		}

	out:
		isSame = false;
	}
	if (newD) {
		newD = false;
		goto again;//�ٻ���
	}

	//������ԭDFA���ֺ�������DFA����
	int newGraph[100];
	for (int i = 0; i <= numOfdiv; i++) {

		for (int n = 0; n < G->nodes.size(); n++) {
			if (div[n] == i) newGraph[i] = n;
		}
	}

	Graph* g1 = new Graph();
	//ԭDFA״̬��������СDFA
	for (int i = 0; i <= numOfdiv; i++) {
		g1->nodes.push_back(*new Node());
		for (int k = 0; k < G->nodes.size(); k++) {
			if (div[k] == i && G->nodes[k].start == true) {
				g1->nodes[i].start = true;
			}
			if (div[k] == i && G->nodes[k].final == true) {
				g1->nodes[i].final = true;
			}
		}
	}
	//������DFA�ı߼�
	for (int i = 0; i <= numOfdiv; i++) {

		for (int t = 1; t < 3; t++) {
			if (t == 1)
				createMap(g1, i, '0', div[mat[newGraph[i]][t]]);
			else if (t == 2)
				createMap(g1, i, '1', div[mat[newGraph[i]][t]]);
		}

	}
	cout << endl;
	//����Ƿ�����״̬������ɾ��
	check(g1);
	
	//��ӡ��С��DFA״̬ת�Ʊ�
	cout << "������£�" << endl;
	cout << " " << "\t" << "0" << "\t" << "1" << endl;
	for (int k = 0; k < g1->nodes.size(); k++) {
		Edge* p = new Edge();
		p = g1->nodes[k].edge;
		if (g1->nodes[k].start)
			cout << "#";
		if (g1->nodes[k].final)
			cout << "*";
		int c1 = -1, c2 = -1;
		while (p) {
			if (p->wt == '0')
				c1 = p->adjvex;
			else if (p->wt == '1')
				c2 = p->adjvex;
			p = p->next;
		}
		string s1, s2;
		if (c1 == -1) {
			s1 = "N";
		}
		else {
			s1 += "q";
			string s3 = to_string(c1 + 1);
			s1 += s3;
		}
		if (c2 == -1) {
			s2 = "N";
		}
		else {
			s2 += "q";
			string s4 = to_string(c2 + 1);
			s2 += s4;
		}
		cout << "q" << (k + 1) << "\t" << s1 << "\t" << s2 << endl;
	}

	return g1;
}
