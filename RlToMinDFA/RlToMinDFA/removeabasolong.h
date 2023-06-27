#include <iostream>//������Ҫ�õĿ�
#include<vector>//����vector��
#include "class.h"

bool v1[100][100];
bool v0[100][100];
bool vk[100][100];

bool isEmpty(Edge*);//�����ж��Ƿ�Ϊ��ת��
//bool ExistCircle(Graph*, int);
int CircleWt(Graph*, int);//���ػ��ϵ�Ȩֵ
void RemoveCircle(Graph*);//ȥ��������ȥ���ڵ��ϵĻ�
bool isEmpty(Edge* edg) {
	return(edg->wt == '#');
}//�ж�һ���ߵ�Ȩֵ�Ƿ�Ϊ�գ������򷵻�true����֮����false
int CircleWt(Graph* G, int index) {//index������ʾ�ǵڼ����ڵ�
	int ret = -1;//��ʼ������ֵ
	Edge* p = G->nodes[index].edge;//��ʼ������ָ��
	for (; p;) {//����index�����ı߲�Ϊ��ʱ
		if (p->adjvex == index) {//��������������ǻ��Ļ�
			ret = p->wt;//����ֵΪ���ϵ�Ȩֵ
		}
		p = p->next;//����ƶ�����ָ��
	}
	return ret;//����ret
}
void RemoveCircle(Graph* G) {//��һ��������ȥ���ĺ���
	for (int i = 0; i < G->nodes.size(); i++) {//��ÿһ���ڵ���б���
		if (ExistCircle(G, i)) {//����ýڵ���ڵĻ���ִ��if�����
			int wt_ = CircleWt(G, i);//����wt_��ֵΪ���ϵ�Ȩֵ
			Edge* p = new Edge();//��ʼ��һ��������ָ��p
			p = G->nodes[i].edge;//pָ��ýڵ�ĵ�һ����
			for (; p;) {//���ߴ���ʱ
				if (isEmpty(p)) {//��pָ��ı��ϵ�Ȩֵ�����жϣ���Ϊ�գ���ִ��if�����
					Edge* q = new Edge();//����һ���µıߣ��������ָ��Ľڵ��⣬������Ϣ�Ը���p
					q->adjvex = p->adjvex;
					q->wt = wt_;
					q->next = p->next;
					p->next = q;//�ý�����ͨ��������ֱ��ָ��ȨֵΪ�յı�ָ��Ľڵ�
				}
				p = p->next;//�������ڵ����
			}
		}
	}
}

//���úϲ�˼�룬Ѱ��һ��������·�����ڿ�����.
void findPath(Graph* G, int begin, int end) {//�ú����ǽ��������м�·������ĳ·���н�����һ��1���������v1���飬������һ��0����v0��ȫ�մ���v��
	vector<char> path;//����һ������������߹���Ȩֵ·��
	vector<int> k;//����һ������������߹��Ķ���
	bool isvisited[100];
	int visitTime[100];
	for (int i = 0; i < 100; i++) {
		isvisited[i] = false;
		visitTime[i] = 0;
	}
	k.push_back(begin);//Ѱ�Ҵ�ĳ�㿪ʼ����ĳһ��end������·��
	isvisited[begin] = true;
	int now = begin;
	Edge* p = new Edge();
	p = G->nodes[now].edge;
	while (k.size() > 0) {
		p = G->nodes[now].edge;
		for (int i = 0; i < visitTime[now]; i++)
			p = p->next;
		if (!p) {
			isvisited[k.back()] = false;
			k.pop_back();
			if (k.size() == 0)
				break;
			path.pop_back();
			now = k.back();
			p = G->nodes[now].edge;
		}

		else if (!isvisited[p->adjvex]) {
			path.push_back(p->wt);
			visitTime[now]++;
			now = p->adjvex;
			isvisited[now] = true;
			k.push_back(now);
			if (now == end) {//���Ѱ�ҵ����е㣬�ж�·����Ϣ
				int p0 = 0;
				int p1 = 0;
				for (int i = 0; i < path.size(); i++) {
					if (path[i] == '0')
						p0++;
				}
				for (int i = 0; i < path.size(); i++) {
					if (path[i] == '1')
						p1++;
				}
				if (p0 == 1) {
					if (p1 < 1){
						v0[begin][end] = true;//����һ��0����v0
					}
				}
				else if (p1 == 1) {
					if (p0 < 1) {
						v1[begin][end] = true;//����һ��1����v1
					}
				}
				else if (p1 == 0 && p0 == 0)
					vk[begin][end] = true;//ȫ�մ���v��

				isvisited[now] = false;
				k.pop_back();
				path.pop_back();
				now = k.back();

			}
		}

		else visitTime[now]++;
	}
}
void connect(bool begin[][100], bool end[][100], Graph* G, bool* isNew, int x) {

	for (int i = 0; i < G->nodes.size(); i++) {
		for (int j = 0; j < G->nodes.size(); j++) {
			if (begin[i][j]) {
				for (int k = 0; k < G->nodes.size(); k++)
					if (end[j][k]) {
						if (x == 0) {
							if (!begin[i][k]) {
								begin[i][k] = true;
								*isNew = true;
							}
						}
						else if (x == 1) {
							if (!end[i][k]) {
								end[i][k] = true;
								*isNew = true;
							}
						}
					}
			}

		}
	}

}//�ϳ���������ֱ�ӱߵĺ�������v1[0][1]=true;vk[1][3]=true;�������һ���ߴ�0��3��ȨֵΪ1.��v1[0][3]=true
void initSet(Graph* G) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 2; j++) {
			v1[i][j] = false;
			v0[i][j] = false;
			vk[i][j] = false;
		}
	}//������������·����
	for (int i = 0; i < G->nodes.size(); i++) {
		if (ExistCircle(G, i)) {
			if (CircleWt(G, i) == 0) {
				v0[i][i] = true;
			}
			if (CircleWt(G, i) == 1) {
				v1[i][i] = true;
			}

		}
		//���Ǽ�·����������Ҳ����·����
		for (int j = 0; j < G->nodes.size(); j++) {
			if (i != j)
				findPath(G, i, j);
		}
	}//����������������ļ�·����Ѱ������·������·����

	bool isNew = true;

	while (isNew) {
		isNew = false;
		connect(v1, vk, G, &isNew, 0);//0�������ϲ�v1��vk���õ��Ľ������ǰ�ߣ�1���ǵõ�����������
		connect(v0, vk, G, &isNew, 0);
		connect(vk, v1, G, &isNew, 1);
		connect(vk, v0, G, &isNew, 1);
	}//������·���ϲ���ֱ��û����·������
}


void connectEdge(Graph* G) {

	for (int i = 0; i < G->nodes.size(); i++) {
		for (int j = 0; j < G->nodes.size(); j++) {
			if (v1[i][j]) createMap(G, i, '1', j);
			if (v0[i][j]) createMap(G, i, '0', j);
		}

	}
}//���ϲ�����v1��v0������Ϊtrue�ı�����

void del(Graph* p)
{
	for (int i = 0; i < p->nodes.size(); i++)
	{
		Edge* head = new Edge();
		head->next = p->nodes[i].edge;
		Edge* temp0 = head;
		Edge* temp1 = p->nodes[i].edge;
		for (; temp1;)
		{
			if (temp1->wt == '#')
			{
				temp0->next = temp1->next;
				delete temp1;
				temp1 = temp0->next;
			}
			else
			{
				temp0 = temp0->next;
				temp1 = temp1->next;
			}
		}
		p->nodes[i].edge = head->next;
	}
}//ɾ��ȨֵΪ�յı�
void isFinal(Graph* G) {//�����ʼ״̬���ڱߵ�
	for (int i = 0; i < G->nodes.size(); i++) {

		if (vk[0][i] == true && G->nodes[i].final == true)
			G->nodes[0].final = true;
	}
}
void deleteEmpty(Graph* G) {
	RemoveCircle(G);
	initSet(G);
	isFinal(G);
	connectEdge(G);
	del(G);
}//���в����ļ��Ϻ����������������ɹ�ȥ��ת��