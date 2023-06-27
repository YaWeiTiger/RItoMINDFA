#pragma once
class Edge
{
public:
	char wt;//用#表示空转移
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
	bool start;//是否是起始状态 是 true 否 false
	bool final;//是否是终止状态 
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
//以邻接链表形式储存
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
	p1->wt = c;//存储弧头 
	p1->next = G->nodes[i].edge;            //头插法插入边结点 
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
	bool flag = false;//false为不指向空状态
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
//将最小化DFA转化为正则文法并输出
void PrintRE(Graph* G) {
	//遍历所有相邻的状态
	for (int i = 0; i < G->nodes.size(); i++) {
		Edge* p = G->nodes[i].edge;
		//判断是否是终止状态
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
