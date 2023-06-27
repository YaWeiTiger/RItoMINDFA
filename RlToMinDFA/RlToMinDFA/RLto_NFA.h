#include<iostream>
#include<vector>
#include "class.h"
//深度优先搜索，用来遍历到终止状态
int deepFirst(std::vector<Node> first, int x,std::vector<bool> judge,int k=-1)
{
	judge[x] = true;
	if (first[x].final)
	{
		return x;
	}
	Edge* temp = first[x].edge;
	for (; temp;)
	{
		if (judge[temp->adjvex])
		{
			temp = temp->next;
			continue;
		}
		k=deepFirst(first, temp->adjvex,judge);
		return k;
	}
	return k;  
}

//正则表达式转带空转移的NFA
Graph* rto_n()
{
	Graph* graph = new Graph();
	graph->nodes.push_back(*new Node());
	//创建状态栈
	std::vector<Node> NFAstack{};
	//创建操作符栈
	std::vector<char> OPstack{};
	std::cout << "输入正则表达式：";
	//字符栈用来存正则表达式串
	std::vector<char> string{};
	char c;
	int fnum=-1;
	for (; ; )
	{
		c = std::cin.get();
		string.push_back(c);
		//如果当前接收的字符是1或者0，需要判断前一个字符，从而判断是否要添加乘运算符（用x表示）
		if (c =='0'||c=='1')
		{
			graph->nodes.push_back(*new Node());
			graph->nodes.push_back(*new Node());
			Edge* eTemp = new Edge;
			eTemp->adjvex = graph->nodes.size() - 1;
			eTemp->wt = c;
			graph->nodes[graph->nodes.size() - 2].edge = eTemp;
			graph->nodes[graph->nodes.size() - 2].start = true;
			graph->nodes[graph->nodes.size() - 1].final= true;
			//如果是（和+或者栈为空则不需要添加
			if (string.size() == 1 || string[string.size() - 2] == '(' || string[string.size() - 2] == '+')
			{
				NFAstack.push_back(graph->nodes[graph->nodes.size() - 2]);
			}
			//否则添加，且要判断运算符栈栈顶元素优先级
			else
			{
				for(; OPstack.size()!=0&&OPstack[OPstack.size() - 1] == '*';)
				{
					std::vector<bool> judge{};
					judge.push_back(false);
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						judge.push_back(false);
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
						{
							int f = deepFirst(graph->nodes, i, judge);
							Edge* te1 = new Edge();
							te1->wt = '#';
							te1->adjvex = f;
							Edge* temp1 = graph->nodes[i].edge;
							graph->nodes[i].edge = te1;
							te1->next = temp1;

							Edge* te2 = new Edge();
							te2->wt = '#';
							te2->adjvex = i;
							Edge* temp2 = graph->nodes[f].edge;
							graph->nodes[f].edge = te2;
							te2->next = temp2;

							NFAstack.pop_back();
							NFAstack.push_back(graph->nodes[i]);
							OPstack.pop_back();
							break;
						}
					}
				}
				OPstack.push_back('x');
				NFAstack.push_back(graph->nodes[graph->nodes.size() - 2]);
			}
		}
		//如果是+，判断栈顶元素优先级进行相应运算操作
		else if (c == '+')
		{
			if (OPstack.size() == 0 || OPstack[OPstack.size() - 1] == '(' || OPstack[OPstack.size() - 1] == '+')
			{
				OPstack.push_back('+');
			}
			else
			{
				for (; OPstack.size() != 0 && (OPstack[OPstack.size() - 1] == '*'|| OPstack[OPstack.size() - 1] == 'x');)
				{
					if (OPstack[OPstack.size() - 1] == '*')
					{
						std::vector<bool> judge{};
						judge.push_back(false);
						for (int i = 1; i < graph->nodes.size(); i++)
						{
							judge.push_back(false);
						}
						for (int i = 1; i < graph->nodes.size(); i++)
						{
							if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
							{
								int f = deepFirst(graph->nodes, i, judge);
								Edge* te1 = new Edge();
								te1->wt = '#';
								te1->adjvex = f;
								Edge* temp1 = graph->nodes[i].edge;
								graph->nodes[i].edge = te1;
								te1->next = temp1;

								Edge* te2 = new Edge();
								te2->wt = '#';
								te2->adjvex = i;
								Edge* temp2 = graph->nodes[f].edge;
								graph->nodes[f].edge = te2;
								te2->next = temp2;

								NFAstack.pop_back();
								NFAstack.push_back(graph->nodes[i]);
								OPstack.pop_back();
								break;
							}
						}

					}
					else
					{
						std::vector<bool> judge{};
						judge.push_back(false);
						int h1, t1, h2;
						for (int i = 1; i < graph->nodes.size(); i++)
						{
							judge.push_back(false);
						}
						for (int i = 1; i < graph->nodes.size(); i++)
						{
							if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
							{
								h2 = i;
								break;
							}
						}
						for (int i = 1; i < graph->nodes.size(); i++)
						{
							if (NFAstack[NFAstack.size() - 2].edge == graph->nodes[i].edge)
							{
								h1 = i;
							    t1 = deepFirst(graph->nodes, i, judge);
								Edge* tp = new Edge();
								tp->wt = '#';
								tp->adjvex = h2;
								Edge* temp = graph->nodes[t1].edge;
								graph->nodes[t1].edge = tp;
								tp->next = temp;
								graph->nodes[t1].final = false;
								graph->nodes[h2].start = false;

								NFAstack.pop_back();
								OPstack.pop_back();
								break;
							}
						}
					}

				}
				OPstack.push_back('+');
			}

		}
		//如果是*，优先级最高直接入栈
		else if (c == '*')
		{
			OPstack.push_back('*');
        }
		//如果是（，判断栈顶元素优先级进行相应运算操作
		else if (c == '(')
		{
			if (string.size() == 1 || string[string.size() - 2] == '(' || string[string.size() - 2] == '+')
			{
				OPstack.push_back('(');
			}
			else
			{
				for(; OPstack.size()!=0&&OPstack[OPstack.size() - 1] == '*';)
				{
					std::vector<bool> judge{};
					judge.push_back(false);
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						judge.push_back(false);
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
						{
							int f = deepFirst(graph->nodes, i, judge);
							Edge* te1 = new Edge();
							te1->wt = '#';
							te1->adjvex = f;
							Edge* temp1 = graph->nodes[i].edge;
							graph->nodes[i].edge = te1;
							te1->next = temp1;

							Edge* te2 = new Edge();
							te2->wt = '#';
							te2->adjvex = i;
							Edge* temp2 = graph->nodes[f].edge;
							graph->nodes[f].edge = te2;
							te2->next = temp2;

							NFAstack.pop_back();
							NFAstack.push_back(graph->nodes[i]);
							OPstack.pop_back();
							break;
						}
					}
				}
				OPstack.push_back('x');
				OPstack.push_back('(');
			}
			
		}
		//如果是），运算符栈元素出栈并对状态栈中状态执行相应运算，直到（出栈为止
		else if(c==')')
		{
			for (; OPstack[OPstack.size() - 1] != '(';)
			{
				if (OPstack[OPstack.size() - 1] == '*')
				{
					std::vector<bool> judge{};
					judge.push_back(false);
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						judge.push_back(false);
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
						{
							int f = deepFirst(graph->nodes, i, judge);
							Edge* te1 = new Edge();
							te1->wt = '#';
							te1->adjvex = f;
							Edge* temp1 = graph->nodes[i].edge;
							graph->nodes[i].edge = te1;
							te1->next = temp1;

							Edge* te2 = new Edge();
							te2->wt = '#';
							te2->adjvex = i;
							Edge* temp2 = graph->nodes[f].edge;
							graph->nodes[f].edge = te2;
							te2->next = temp2;

							NFAstack.pop_back();
							NFAstack.push_back(graph->nodes[i]);
							OPstack.pop_back();
							break;
						}
					}

				}
				else if (OPstack[OPstack.size() - 1] == 'x')
				{
					std::vector<bool> judge{};
					judge.push_back(false);
					int h1, t1, h2;
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						judge.push_back(false);
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
						{
							h2 = i;
							break;
						}
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 2].edge == graph->nodes[i].edge)
						{
							h1 = i;
							t1 = deepFirst(graph->nodes, i, judge);
							Edge* tp = new Edge();
							tp->wt = '#';
							tp->adjvex = h2;
							Edge* temp = graph->nodes[t1].edge;
							graph->nodes[t1].edge = tp;
							tp->next = temp;
							graph->nodes[t1].final = false;
							graph->nodes[h2].start = false;

							NFAstack.pop_back();
							OPstack.pop_back();
							break;
						}
					}
				}
				else 
				{
					std::vector<bool> judge{};
					judge.push_back(false);
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						judge.push_back(false);
					}
					int h1, t1, h2, t2;
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
						{
							h1 = i;
							graph->nodes[h1].start = false;
							t1= deepFirst(graph->nodes, i, judge);
							graph->nodes[t1].final = false;
							break;
						}
					}
					for (int i = 1; i < graph->nodes.size(); i++)
					{
						if (NFAstack[NFAstack.size() - 2].edge == graph->nodes[i].edge)
						{
							h2 = i;
							graph->nodes[h2].start = false;
							t2 = deepFirst(graph->nodes, i, judge);
							graph->nodes[t2].final = false;
							break;
						}
					}
					graph->nodes.push_back(*new Node());
					graph->nodes.push_back(*new Node());
					Edge* ne1 = new Edge();
					ne1->wt = '#';
					ne1->adjvex = h1;
					Edge* ne2 = new Edge();
					ne2->wt = '#';
					ne2->adjvex = h2;
					ne1->next = ne2;

					Edge* tmp = nullptr;

					Edge* te1 = new Edge();
					te1->wt = '#';
					te1->adjvex = graph->nodes.size() - 1;
					tmp = graph->nodes[t1].edge;
					graph->nodes[t1].edge = te1;
					te1->next = tmp;

					Edge* te2 = new Edge();
					te2->wt = '#';
					te2->adjvex = graph->nodes.size() - 1;
					tmp = graph->nodes[t2].edge;
					graph->nodes[t2].edge = te2;
					te2->next = tmp;

					graph->nodes[graph->nodes.size() - 2].start = true;
					graph->nodes[graph->nodes.size() - 2].edge = ne1;
					graph->nodes[graph->nodes.size() - 1].final = true;

					NFAstack.pop_back();
					NFAstack.pop_back();
					NFAstack.push_back(graph->nodes[graph->nodes.size() - 2]);
					OPstack.pop_back();
				}
			}
			OPstack.pop_back();
        }
		else
		{
		break;
        }
	}

	//将最终运算符栈中全部运算符出栈，并对状态栈中状态执行相应操作
	for (; OPstack.size() != 0;)
	{
		if (OPstack[OPstack.size() - 1] == '*')
		{
			std::vector<bool> judge{};
			judge.push_back(false);
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				judge.push_back(false);
			}
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
				{
					int f = deepFirst(graph->nodes, i, judge);
					Edge* te1 = new Edge();
					te1->wt = '#';
					te1->adjvex = f;
					Edge* temp1 = graph->nodes[i].edge;
					graph->nodes[i].edge = te1;
					te1->next = temp1;

					Edge* te2 = new Edge();
					te2->wt = '#';
					te2->adjvex = i;
					Edge* temp2 = graph->nodes[f].edge;
					graph->nodes[f].edge = te2;
					te2->next = temp2;

					NFAstack.pop_back();
					NFAstack.push_back(graph->nodes[i]);
					OPstack.pop_back();
					break;
				}
			}

		}
		else if (OPstack[OPstack.size() - 1] == 'x')
		{
			std::vector<bool> judge{};
			judge.push_back(false);
			int h1, t1, h2;
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				judge.push_back(false);
			}
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
				{
					h2 = i;
					break;
				}
			}
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				if (NFAstack[NFAstack.size() - 2].edge == graph->nodes[i].edge)
				{
					h1 = i;
					t1 = deepFirst(graph->nodes, i, judge);
					Edge* tp = new Edge();
					tp->wt = '#';
					tp->adjvex = h2;
					Edge* temp = graph->nodes[t1].edge;
					graph->nodes[t1].edge = tp;
					tp->next = temp;
					graph->nodes[t1].final = false;
					graph->nodes[h2].start = false;

					NFAstack.pop_back();
					OPstack.pop_back();
					break;
				}
			}
		}
		else
		{
			std::vector<bool> judge{};
			judge.push_back(false);
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				judge.push_back(false);
			}
			int h1, t1, h2, t2;
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				if (NFAstack[NFAstack.size() - 1].edge == graph->nodes[i].edge)
				{
					h1 = i;
					graph->nodes[h1].start = false;
					t1 = deepFirst(graph->nodes, i, judge);
					graph->nodes[t1].final = false;
					break;
				}
			}
			for (int i = 1; i < graph->nodes.size(); i++)
			{
				if (NFAstack[NFAstack.size() - 2].edge == graph->nodes[i].edge)
				{
					h2 = i;
					graph->nodes[h2].start = false;
					t2 = deepFirst(graph->nodes, i, judge);
					graph->nodes[t2].final = false;
					break;
				}
			}
			graph->nodes.push_back(*new Node());
			graph->nodes.push_back(*new Node());
			Edge* ne1 = new Edge();
			ne1->wt = '#';
			ne1->adjvex = h1;
			Edge* ne2 = new Edge();
			ne2->wt = '#';
			ne2->adjvex = h2;
			ne1->next = ne2;

			Edge* tmp = nullptr;

			Edge* te1 = new Edge();
			te1->wt = '#';
			te1->adjvex = graph->nodes.size() - 1;
			tmp = graph->nodes[t1].edge;
			graph->nodes[t1].edge = te1;
			te1->next = tmp;

			Edge* te2 = new Edge();
			te2->wt = '#';
			te2->adjvex = graph->nodes.size() - 1;
			tmp = graph->nodes[t2].edge;
			graph->nodes[t2].edge = te2;
			te2->next = tmp;

			graph->nodes[graph->nodes.size() - 2].start = true;
			graph->nodes[graph->nodes.size() - 2].edge = ne1;
			graph->nodes[graph->nodes.size() - 1].final = true;

			NFAstack.pop_back();
			NFAstack.pop_back();
			NFAstack.push_back(graph->nodes[graph->nodes.size() - 2]);
			OPstack.pop_back();
		}
	}
	//将初始状态放在nodes[0]的位置上
	for (int i = 1; i < graph->nodes.size(); i++)
	{
		if (NFAstack[0].edge == graph->nodes[i].edge)
		{
			graph->nodes[0] = graph->nodes[i];
			fnum = i;
			Edge* temp = graph->nodes[0].edge;
			for (; temp;)
			{
				if (temp->adjvex > fnum)
				{
					temp->adjvex--;
				}
				else if (temp->adjvex == fnum)
				{
					temp->adjvex = 0;
				}
				
				temp = temp->next;
			}
			graph->nodes.erase(graph->nodes.begin() + fnum);
			break;
		}
	}
	//修改指向原先初始状态的边，使其指向现在的初始状态位置
	for (int i = 1; i < graph->nodes.size(); i++)
	{
		Edge* temp = graph->nodes[i].edge;
		for (; temp;)
		{
			if (temp->adjvex > fnum)
			{
				temp->adjvex--;
			}
			else if (temp->adjvex == fnum)
			{
				temp->adjvex = 0;
			}
			temp = temp->next;
		}
	}
	return graph;
}

//NFA转DFA
Graph* ntod(Graph* graph)
{
	//创建新的节点栈用来存转化后的DFA的状态
	std::vector<Node> v{};
	//对应状态集栈的键值对集合用来对应每一个状态在数组中的脚标
	std::map<std::set<int>, int> map{};
	//创建状态集栈，用来遍历
	std::vector<std::set<int>> vector{};

	//用来记录脚标
	int x = 0;

	//创建第一个状态集其中只有初始状态
	std::set<int> set0{ 0 };
	//初始状态放在数组第一位
	map[set0] = x;
	//将状态集放入状态集栈
	vector.push_back(set0);


	//因为转化后初始状态不变，所以直接将初始状态放入节点栈
	v.push_back(graph->nodes[0]);
	//遍历状态栈中状态集
	for (int i = 0; i < vector.size(); i++)
	{
		//接收0达到的状态存入sets[0],接收1达到的状态存在sets[1]
		std::set<int> sets[2];
		for (std::set<int>::iterator j = vector[i].begin(); j != vector[i].end(); j++)
		{
			Edge* temp = graph->nodes[*j].edge;
			for (; temp;)
			{
				if (temp->wt == '0')
				{
					sets[0].insert(temp->adjvex);
				}
				else
				{
					sets[1].insert(temp->adjvex);
				}
				temp = temp->next;
			}
		}
		v[i].edge = nullptr;
		//与map中状态集比较,新状态存在则直接链接相应边，不存在则创建新状态再连接边，并加入状态栈，更新map和状态集栈
		for (int k = 0; k < 2; k++)
		{
			auto iter = map.find(sets[k]);
			if (iter == map.end())
			{
				map[sets[k]] = ++x;
				vector.push_back(sets[k]);
				Node* p = new Node();
				for (std::set<int>::iterator t = sets[k].begin(); t != sets[k].end(); t++)
				{
					if (graph->nodes[*t].final)
					{
						p->final = true;
						break;
					}
				}
				v.push_back(*p);

				Edge* p0 = new Edge();
				p0->adjvex = x;
				p0->wt = (char)(k + 48);

				Edge* tp = v[i].edge;
				v[i].edge = p0;
				p0->next = tp;

			}
			else
			{
				Edge* p0 = new Edge();
				p0->adjvex = iter->second;
				p0->wt = (char)(k + 48);

				Edge* tp = v[i].edge;
				v[i].edge = p0;
				p0->next = tp;
			}
		}
	}
	//将最终结构重新赋值给nodes,使该图表示转化后的DFA
	graph->nodes = v;
	return graph;
}