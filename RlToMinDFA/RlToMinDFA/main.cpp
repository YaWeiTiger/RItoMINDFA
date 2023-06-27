#include <stdio.h>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include "RLto_NFA.h"
#include "minDFA.h"
#include "removeabasolong.h"





int main() {
	std::cout << "是否输出正则文法（是1，否0）";
	char out=std::cin.get();
	std::cin.get();
	Graph* p = rto_n();
	deleteEmpty(p);
	Graph* p2 = ntod(p);
	Graph* p3 = Div(p2);
	std::cout << std::endl;
	if (out == '1')
		PrintRE(p3);
	std::cin.get();
}