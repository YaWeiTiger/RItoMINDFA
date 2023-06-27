#pragma once
class Edge
{
public:
	char wt;//��#��ʾ��ת��
	int adjvex;
	Edge* next;
	Edge()
	{
		wt = '\0';
		adjvex = -1;
		next = nullptr;
	}
};
class Node
{
public:
	bool start;//�Ƿ�����ʼ״̬ �� true �� false
	bool final;//�Ƿ�����ֹ״̬ 
	Edge* edge;
	Node()
	{
		start = false;
		final = false;
		edge = nullptr;
	}
};
class Graph
{
public:
	std::vector<Node> nodes{};
	Graph()
	{

	}
};
//���ڽ�������ʽ����
void dra(Graph* graph)
{
	for (int i = 0; i < graph->nodes.size(); i++)
	{
		Edge* temp = graph->nodes[i].edge;
		for (; temp;)
		{
			std::cout << i << '-' << temp->wt << '-' << temp->adjvex << std::endl;
			temp = temp->next;
		}
	}
}
void createMap(Graph* G, int i, char c, int j) {
	Edge* p1 = new Edge();
	p1->adjvex = j;
	p1->wt = c;//�洢��ͷ 
	p1->next = G->nodes[i].edge;            //ͷ�巨����߽�� 
	G->nodes[i].edge = p1;
}
using namespace std;

bool ExistCircle(Graph* G, int index) {
	bool flag = false;
	Edge* p = G->nodes[index].edge;
	while (p) {
		if (p->adjvex == index) {
			flag = true;
			break;
		}
		p = p->next;
	}
	return flag;
}
bool IsToEmpty(Graph* G, Node n) {
	bool flag = false;//falseΪ��ָ���״̬
	Edge* p = n.edge;
	while (p) {
		if (G->nodes[p->adjvex].final == true) {
			flag = true;
			break;
		}
		else {
			p = p->next;
		}
	}
	return flag;
}
//����С��DFAת��Ϊ�����ķ������
void PrintRE(Graph* G) {
	//�����������ڵ�״̬
	for (int i = 0; i < G->nodes.size(); i++) {
		Edge* p = G->nodes[i].edge;
		//�ж��Ƿ�����ֹ״̬
		if (G->nodes[i].final != true) {
			if (IsToEmpty(G, G->nodes[i])) {
				cout << "P" << i << "-->";
				while (p) {
					if (G->nodes[p->adjvex].final != true) {
						cout << p->wt << "P" << p->adjvex << "|";
					}
					else {
						/*if (ExistCircle(G, p->adjvex)) {
							cout << p->wt << "P" << p->adjvex << "|" << p->wt << "|";
						}*/
						if(G->nodes[p->adjvex].edge){
							cout << p->wt << "P" << p->adjvex << "|" << p->wt << "|";
						}
						else {
							cout << p->wt << "|";
						}
					}
					p = p->next;
				}
			}
			else {
				cout << "P" << i << "-->";
				while (p) {
					cout << p->wt << "P" << p->adjvex << "|";
					p = p->next;
				}
			}
		}
		else {
			cout << "P" << i << "-->";
			if (G->nodes[i].edge) {
				while (p) {
					cout << p->wt << "P" << p->adjvex << "|";
					p = p->next;
				}
			}
		}
		cout << endl;
	}
}
