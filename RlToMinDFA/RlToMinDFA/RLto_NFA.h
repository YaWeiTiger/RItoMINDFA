#include<iostream>
#include<vector>
#include "class.h"
//�������������������������ֹ״̬
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

//������ʽת����ת�Ƶ�NFA
Graph* rto_n()
{
	Graph* graph = new Graph();
	graph->nodes.push_back(*new Node());
	//����״̬ջ
	std::vector<Node> NFAstack{};
	//����������ջ
	std::vector<char> OPstack{};
	std::cout << "����������ʽ��";
	//�ַ�ջ������������ʽ��
	std::vector<char> string{};
	char c;
	int fnum=-1;
	for (; ; )
	{
		c = std::cin.get();
		string.push_back(c);
		//�����ǰ���յ��ַ���1����0����Ҫ�ж�ǰһ���ַ����Ӷ��ж��Ƿ�Ҫ��ӳ����������x��ʾ��
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
			//����ǣ���+����ջΪ������Ҫ���
			if (string.size() == 1 || string[string.size() - 2] == '(' || string[string.size() - 2] == '+')
			{
				NFAstack.push_back(graph->nodes[graph->nodes.size() - 2]);
			}
			//������ӣ���Ҫ�ж������ջջ��Ԫ�����ȼ�
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
		//�����+���ж�ջ��Ԫ�����ȼ�������Ӧ�������
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
		//�����*�����ȼ����ֱ����ջ
		else if (c == '*')
		{
			OPstack.push_back('*');
        }
		//����ǣ����ж�ջ��Ԫ�����ȼ�������Ӧ�������
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
		//����ǣ��������ջԪ�س�ջ����״̬ջ��״ִ̬����Ӧ���㣬ֱ������ջΪֹ
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

	//�����������ջ��ȫ���������ջ������״̬ջ��״ִ̬����Ӧ����
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
	//����ʼ״̬����nodes[0]��λ����
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
	//�޸�ָ��ԭ�ȳ�ʼ״̬�ıߣ�ʹ��ָ�����ڵĳ�ʼ״̬λ��
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

//NFAתDFA
Graph* ntod(Graph* graph)
{
	//�����µĽڵ�ջ������ת�����DFA��״̬
	std::vector<Node> v{};
	//��Ӧ״̬��ջ�ļ�ֵ�Լ���������Ӧÿһ��״̬�������еĽű�
	std::map<std::set<int>, int> map{};
	//����״̬��ջ����������
	std::vector<std::set<int>> vector{};

	//������¼�ű�
	int x = 0;

	//������һ��״̬������ֻ�г�ʼ״̬
	std::set<int> set0{ 0 };
	//��ʼ״̬���������һλ
	map[set0] = x;
	//��״̬������״̬��ջ
	vector.push_back(set0);


	//��Ϊת�����ʼ״̬���䣬����ֱ�ӽ���ʼ״̬����ڵ�ջ
	v.push_back(graph->nodes[0]);
	//����״̬ջ��״̬��
	for (int i = 0; i < vector.size(); i++)
	{
		//����0�ﵽ��״̬����sets[0],����1�ﵽ��״̬����sets[1]
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
		//��map��״̬���Ƚ�,��״̬������ֱ��������Ӧ�ߣ��������򴴽���״̬�����ӱߣ�������״̬ջ������map��״̬��ջ
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
	//�����սṹ���¸�ֵ��nodes,ʹ��ͼ��ʾת�����DFA
	graph->nodes = v;
	return graph;
}