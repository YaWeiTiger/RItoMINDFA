#pragma once
#include "class.h"
#include <string>
bool isVisited[100];
using namespace std;
int mat[100][3];
void printMatrix(Graph g) {
	//将DFA的状态转移表存入一个二维数组中

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
	//利用深度优先搜索查看一个顶点是否存在到终止状态的路径
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
	//用于删除死状态
	Edge* p = new Edge();
	Edge* q = new Edge();
	Edge* r = new Edge();
	//遍历顶点数组，并删除顶点数组中的被删顶点
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



	//删除被删顶点链接的边信息
	while (p) {
		q = p->next;
		delete p;
		p = q;
	}
	//删除其它顶点中与position相关的边结点
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
//检查是否有死状态函数
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
			DFS(*G, i, finalvex, &isDeath);//检查某点是否有路径达到终态点
			if (!isDeath) {
				Deathvex = i;
				deleteAdjacencyListElement(G, G->nodes.size() - 1, Deathvex);//没有则删除该状态及其边

				break;
			}
		}
	}
	if (!isDeath)//如果检查出死状态并且删除，再检查一次。
		check(G);



}

//划分函数，生产最小化DFA
Graph *Div(Graph* G) {
	printMatrix(*G);
	bool newD = false;
	int div[100]; //顶点对应划分出来的第几个划分块
	int numOfdiv = 1;//划分集合总数
	//将非终态划分为"0"集合，终态为"1"集合
	for (int i = 0; i < G->nodes.size(); i++) {
		if (G->nodes[i].final) {
			div[i] = 1;
		}
		else div[i] = 0;
	}

	//开始划分，此部分是自己按照自己的理解写的，代码有点乱，可能晦涩难理解，简单注释可能无法解释。助教如有问题可以问，都可以回答。
again:
	for (int i = 0; i <= numOfdiv; i++) {

		int now[100]; int k = 0;
		bool isSame = false;
		bool newDiv = false;
		bool Istp = false;
		//临时数组用于存放当前划分集合
		for (int j = 0; j < G->nodes.size(); j++) {
			if (div[j] == i) {
				now[k++] = j;
			}
		}
		//如果当前集合输入同一字符得到的状态一致，则直接跳过此次字符循环检测
		for (int t = 1; t < 3; t++) {
			for (int i1 = 0; i1 < k; i1++) {
				if (div[mat[now[0]][t]] == div[mat[now[i1]][t]])
					isSame = true;
				else {
					isSame = false;
					break;
				}
			}
			if (isSame && t == 1)//跳过此次字符循环检测
				continue;
			if (isSame) 
				goto out;//如果所有字符都一致，则该集合不用划分

			//以下为不一致情况
			int tp = 0;
			int temp[100];
			int nowDiv = numOfdiv;
			int newnow[100];
			int newLength = 0;
			int newIndex = 0;
			//遍历所有元素，检查他们转移状态是否属于同一集合
			for (int key = 0; key <= nowDiv; key++) {

				for (int m = 0; m < k; m++) {

					if (div[mat[now[m]][t]] == key) {
						temp[now[m]] = tp;//临时数存放对应转移集合
						Istp = true;
						if (tp == 0) {
							newnow[newIndex++] = now[m];
							newLength++;//第一个划分集合放入原集合
						}
						if (tp > 0) {
							div[now[m]] = numOfdiv + 1;//剩余划分集合产生新集合，集合总数+1
							newDiv = true;//标志新集合产生
						}
					}
				}
				if (Istp)
					tp++;
				if (newDiv) {
					numOfdiv++;
					newD = true;//newD表示一整轮划分完毕后出现新划分，需要再划分一次
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
		goto again;//再划分
	}

	//以下是原DFA划分后生成新DFA过程
	int newGraph[100];
	for (int i = 0; i <= numOfdiv; i++) {

		for (int n = 0; n < G->nodes.size(); n++) {
			if (div[n] == i) newGraph[i] = n;
		}
	}

	Graph* g1 = new Graph();
	//原DFA状态赋予新最小DFA
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
	//生成新DFA的边集
	for (int i = 0; i <= numOfdiv; i++) {

		for (int t = 1; t < 3; t++) {
			if (t == 1)
				createMap(g1, i, '0', div[mat[newGraph[i]][t]]);
			else if (t == 2)
				createMap(g1, i, '1', div[mat[newGraph[i]][t]]);
		}

	}
	cout << endl;
	//检查是否有死状态，有则删除
	check(g1);
	
	//打印最小化DFA状态转移表
	cout << "结果如下：" << endl;
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
