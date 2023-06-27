#include <iostream>//导入需要用的库
#include<vector>//导入vector库
#include "class.h"

bool v1[100][100];
bool v0[100][100];
bool vk[100][100];

bool isEmpty(Edge*);//用来判断是否为空转移
//bool ExistCircle(Graph*, int);
int CircleWt(Graph*, int);//返回环上的权值
void RemoveCircle(Graph*);//去环函数，去掉节点上的环
bool isEmpty(Edge* edg) {
	return(edg->wt == '#');
}//判断一条边的权值是否为空，若是则返回true，反之返回false
int CircleWt(Graph* G, int index) {//index用来表示是第几个节点
	int ret = -1;//初始化返回值
	Edge* p = G->nodes[index].edge;//初始化搜索指针
	for (; p;) {//当第index个结点的边不为空时
		if (p->adjvex == index) {//如果发现这条边是环的话
			ret = p->wt;//返回值为环上的权值
		}
		p = p->next;//向后移动搜索指针
	}
	return ret;//返回ret
}
void RemoveCircle(Graph* G) {//对一个结点进行去环的函数
	for (int i = 0; i < G->nodes.size(); i++) {//对每一个节点进行遍历
		if (ExistCircle(G, i)) {//如果该节点存在的话，执行if代码段
			int wt_ = CircleWt(G, i);//变量wt_赋值为环上的权值
			Edge* p = new Edge();//初始化一个边搜索指针p
			p = G->nodes[i].edge;//p指向该节点的第一条边
			for (; p;) {//当边存在时
				if (isEmpty(p)) {//对p指向的边上的权值进行判断，若为空，则执行if代码段
					Edge* q = new Edge();//申请一条新的边，让其除了指向的节点外，其余信息皆复制p
					q->adjvex = p->adjvex;
					q->wt = wt_;
					q->next = p->next;
					p->next = q;//该结点可以通过这条边直接指向权值为空的边指向的节点
				}
				p = p->next;//边搜索节点后移
			}
		}
	}
}

//采用合并思想，寻找一条边所有路径存在可能性.
void findPath(Graph* G, int begin, int end) {//该函数是将遍历所有简单路径，将某路径中仅接受一次1的两点存入v1数组，仅接受一个0存入v0，全空存入v空
	vector<char> path;//利用一个容器存放已走过的权值路径
	vector<int> k;//利用一个容器存放已走过的顶点
	bool isvisited[100];
	int visitTime[100];
	for (int i = 0; i < 100; i++) {
		isvisited[i] = false;
		visitTime[i] = 0;
	}
	k.push_back(begin);//寻找从某点开始，到某一点end的所有路径
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
			if (now == end) {//如果寻找到了中点，判断路径信息
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
						v0[begin][end] = true;//仅有一个0存入v0
					}
				}
				else if (p1 == 1) {
					if (p0 < 1) {
						v1[begin][end] = true;//仅有一个1存入v1
					}
				}
				else if (p1 == 0 && p0 == 0)
					vk[begin][end] = true;//全空存入v空

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

}//合成两个集合直接边的函数，如v1[0][1]=true;vk[1][3]=true;则代表有一条边从0到3，权值为1.故v1[0][3]=true
void initSet(Graph* G) {
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 2; j++) {
			v1[i][j] = false;
			v0[i][j] = false;
			vk[i][j] = false;
		}
	}//遍历三个顶点路径集
	for (int i = 0; i < G->nodes.size(); i++) {
		if (ExistCircle(G, i)) {
			if (CircleWt(G, i) == 0) {
				v0[i][i] = true;
			}
			if (CircleWt(G, i) == 1) {
				v1[i][i] = true;
			}

		}
		//将非简单路径，即自身环也纳入路径集
		for (int j = 0; j < G->nodes.size(); j++) {
			if (i != j)
				findPath(G, i, j);
		}
	}//遍历任意两个顶点的简单路径，寻找所需路径存入路径集

	bool isNew = true;

	while (isNew) {
		isNew = false;
		connect(v1, vk, G, &isNew, 0);//0代表从如合并v1与vk，得到的结果存入前者，1则是得到结果存入后者
		connect(v0, vk, G, &isNew, 0);
		connect(vk, v1, G, &isNew, 1);
		connect(vk, v0, G, &isNew, 1);
	}//将所有路径合并，直至没有新路径产生
}


void connectEdge(Graph* G) {

	for (int i = 0; i < G->nodes.size(); i++) {
		for (int j = 0; j < G->nodes.size(); j++) {
			if (v1[i][j]) createMap(G, i, '1', j);
			if (v0[i][j]) createMap(G, i, '0', j);
		}

	}
}//将合并完后的v1，v0集合里为true的边生成

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
}//删除权值为空的边
void isFinal(Graph* G) {//如果起始状态存在边到
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
}//所有操作的集合函数，操作结束即成功去空转移