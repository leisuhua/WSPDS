// Reach_Vector_Vector.cpp : 定义控制台应用程序的入口点。
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stack>
#include <vector>
#include <time.h>
#include "mem.h"
#include <functional>
using namespace std;

//状态是整数，栈字符从整数变为向量

class Node {
public:
	vector<int> p; //state
	stack< vector<int> > s; //change1: stack symbol, now vector
	Node* n; //father node
	bool v; // valid node ?
	Node(vector<int> p, stack< vector<int> > s, Node* n, bool v)
	{
		this->p = p;
		this->s = s;
		this->n = n;
		this->v = v;
	}
};

//internal rule
function<Node(Node)> computeRule0(const function<vector<int>(vector<int>)>& computeq, const function< vector<int>(vector<int>)>& computea)
{
	//n.p, n.s.top() -> q, a 
	return [=](Node n) {
		if (n.s.empty())
			return Node(n.p, n.s, NULL, false);//to ensure this is a invalid node, just set v = false
		vector<int> q = computeq(n.p);
		stack< vector<int> > s = n.s; //copy stack
		vector<int>  a = computea(s.top());
		s.pop();
		s.push(a);
		bool v = true;
		/*if (q < 0 || a < 0)
		v = false;*/
		//判断一个vector是不是大于0， 写一个循环？
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//if (q < 0)
		//	v = false;
		if (v) {
			for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
			{
				if (*it < 0)
				{
					v = false;
					break;
				}
			}
		}
		if (v) {
			for (vector<int>::iterator it = a.begin(); it != a.end(); ++it)
			{
				if (*it < 0)
				{
					v = false;
					break;
				}
			}
		}


		return Node(q, s, new Node(n.p, n.s, n.n, n.v), v);
	};
}

//pop rule
//n.p, n.s.pop() -> q, \epsilon
function<Node(Node)> computeRule1(const function<vector<int>(vector<int>)>& computeq)
{
	return [=](Node n) {
		if (n.s.empty())
			return Node(n.p, n.s, NULL, false);
		vector<int> q = computeq(n.p);
		stack<vector<int> > s = n.s;
		s.pop();
		bool v = true;
		//if (q < 0)
			//v = false;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
	
		return Node(q, s, new Node(n.p, n.s, n.n, n.v), v);
	};
}


function<Node(Node)> computeRule2(const function<vector<int>(vector<int>)>& computeq, const function<vector<int>(vector<int>, vector<int>)>& computea)
{
	//n.p, n.s.top,n.s.top -> q, a
	return [=](Node n) {
		if (n.s.size() < 2)
			return Node(n.p, n.s, NULL, false);
		vector<int> q = computeq(n.p);
		stack<vector<int> > s = n.s;
		vector<int> a1 = s.top();
		s.pop();
		vector<int> a2 = s.top();
		s.pop();
		vector<int> a = computea(a1, a2);
		s.push(a);
		bool v = true;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
		if (v) {
			for (vector<int>::iterator it = a.begin(); it != a.end(); ++it)
			{
				if (*it < 0)
				{
					v = false;
					break;
				}
			}
		}

		return Node(q, s, new Node(n.p, n.s, n.n, n.v), v);
	};
}
//push rule
//n.p, n.s.pop() -> q, a1 a2
function<Node(Node)> computeRule3(const function<vector<int>(vector<int>)>& computeq, const function<vector<int>(vector<int>)>& computea1,
	const function<vector<int>(vector<int>)>& computea2)
{
	return [=](Node n) {
		if (n.s.empty())
			return Node(n.p, n.s, NULL, false);
		vector<int> q = computeq(n.p);
		stack<vector<int>> s = n.s;
		vector<int> a = s.top();
		s.pop();
		vector<int> a1 = computea1(a);
		vector<int> a2 = computea2(a);
		//s.push(a1);
		//s.push(a2);
		s.push(a2);
		s.push(a1);
		bool v = true;
		//if (q < 0) v = false;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
		if (v) {
			for (vector<int>::iterator it = a1.begin(); it != a1.end(); ++it)
			{
				if (*it < 0)
				{
					v = false;
					break;
				}
			}
		}

		if (v) {
			for (vector<int>::iterator it = a2.begin(); it != a2.end(); ++it)
			{
				if (*it < 0)
				{
					v = false;
					break;
				}
			}
		}

		return Node(q, s, new Node(n.p, n.s, n.n, n.v), v);
	};
}

//simple-push rule
function<Node(Node)> computeRule4(const function<vector<int>(vector<int>)>& computeq, vector<int> a)
{
	return [=](Node n) {
		vector<int> q = computeq(n.p);
		stack<vector<int>> s = n.s;
		s.push(a);
		bool v = true;
		//if (q < 0)
			//v = false;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
		return Node(q, s, new Node(n.p, n.s, n.n, n.v), v);
	};
}


int main(int argc, const char * argv[]) {
	vector<function<Node(Node)> > rules;
	stack<vector<int> > is;
	stack<Node> ns; //for dfs

	time_t start, end;

	ifstream in;
	in.open("cases.txt", ios::in);
	//int tag, initq, coeffq, consq, coeffa1, consa1, coeffa2, consa2;
	//int cons1, cons2;
	int tag, coeffq, coeffa1, coeffa2;
	vector<int> consa1, consa2, cons1, cons2;
	vector<int> initq,consq;//change state from int to vector<int>
	int dimension1,dimension2;//dimension1 is the dimension of state, dimension2 is the dimension of stack symbol
	in >> dimension1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!先读dimension 
	in >> dimension2;
	in >> tag;
	while (tag != 5) {
		switch (tag) {
		case 0:
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			in >> coeffq >> coeffa1;
			//>> consq 
			consq.clear(); //每次用之前需要清空
							//读consq vector
			for (int j = 0; j < dimension1; ++j)
			{
				int velement;
				in >> velement;
				consq.push_back(velement);
			}

			consa1.clear(); //每次用之前需要清空
							//读consa1 vector
			for (int j = 0; j < dimension2; ++j)
			{
				int velement;
				in >> velement;
				consa1.push_back(velement);
			}

			rules.push_back(computeRule0([=](vector <int> n) { vector<int> res1;
			for (int j = 0; j < dimension1; ++j)
				res1.push_back(n[j] * coeffq + consq[j]);
			return res1;  },
				[=](vector<int> n)
			{ vector<int> res2;
			for (int j = 0; j < dimension2; ++j)
				res2.push_back(n[j] * coeffa1 + consa1[j]);
			return res2; }));
			break;
		case 1:
			in >> coeffq;
			//>> consq;
			consq.clear(); //每次用之前需要清空
						   //读consq vector
			for (int j = 0; j < dimension1; ++j)
			{
				int velement;
				in >> velement;
				consq.push_back(velement);
			}
			rules.push_back(computeRule1([=](vector <int> n) { vector<int> res1;
			for (int j = 0; j < dimension1; ++j)
				res1.push_back(n[j] * coeffq + consq[j]);
			return res1;  }));
			break;

		case 2:
			in >> coeffq >> coeffa1 >> coeffa2;
			//>> consq 
			consq.clear(); //每次用之前需要清空
						   //读consq vector
			for (int j = 0; j < dimension1; ++j)
			{
				int velement;
				in >> velement;
				consq.push_back(velement);
			}
			
			consa1.clear(); //每次用之前需要清空
							//读consa1 vector
			for (int j = 0; j < dimension2; ++j)
			{
				int velement;
				in >> velement;
				consa1.push_back(velement);
			}
			rules.push_back(computeRule2([=](vector <int> n) { vector<int> res1;
			for (int j = 0; j < dimension1; ++j)
				res1.push_back(n[j] * coeffq + consq[j]);
			return res1; },
				[=](vector<int> n1, vector<int> n2)
			{ vector<int> res;
			for (int j = 0; j < dimension2; ++j)
				res.push_back(n1[j] * coeffa1 + n2[j] * coeffa2 + consa1[j]);
			return res; }));
			break;

		case 3:
			in >> coeffq >> coeffa1 >> coeffa2;
			//>> consq
			consq.clear(); //每次用之前需要清空
						   //读consq vector
			for (int j = 0; j < dimension1; ++j)
			{
				int velement;
				in >> velement;
				consq.push_back(velement);
			}
			
			consa1.clear(); //每次用之前需要清空
							//读consa1 vector
			for (int j = 0; j < dimension2; ++j)
			{
				int velement;
				in >> velement;
				consa1.push_back(velement);
			}
			
			consa2.clear(); //每次用之前需要清空
							//读consa1 vector
			for (int j = 0; j < dimension2; ++j)
			{
				int velement;
				in >> velement;
				consa2.push_back(velement);
			}
			rules.push_back(computeRule3([=](vector <int> n) { vector<int> resq;
			for (int j = 0; j < dimension1; ++j)
				resq.push_back(n[j] * coeffq + consq[j]);
			return resq; },
				[=](vector<int> n)
			{ vector<int> res1;
			for (int j = 0; j < dimension2; ++j)
				res1.push_back(n[j] * coeffa1 + consa1[j]);
			return res1; },
				[=](vector<int> n)
			{ vector<int> res2;
			for (int j = 0; j < dimension2; ++j)
				res2.push_back(n[j] * coeffa2 + consa2[j]);
			return res2; }));
			break;
		case 4:
			in >> coeffq;
			//>> consq;
			consq.clear(); //每次用之前需要清空
						   //读consq vector
			for (int j = 0; j < dimension1; ++j)
			{
				int velement;
				in >> velement;
				consq.push_back(velement);
			}
			// consa1;
			consa1.clear(); //每次用之前需要清空
							//读consa1 vector
			for (int j = 0; j < dimension2; ++j)
			{
				int velement;
				in >> velement;
				consa1.push_back(velement);
			}
			rules.push_back(computeRule4([=](vector <int> n) { vector<int> res1;
			for (int j = 0; j < dimension1; ++j)
				res1.push_back(n[j] * coeffq + consq[j]);
			return res1; }, consa1));
			break;
		default:
			cout << "invalid rule" << endl;
			exit(-1);
			break;
		}
		in >> tag;
	}
	//in >> initq;
	initq.clear(); //每次用之前需要清空

	for (int j = 0; j < dimension1; ++j)
	{
		int velement;
		in >> velement;
		initq.push_back(velement);
	}
	// cons1 
	cons1.clear(); //每次用之前需要清空

	for (int j = 0; j < dimension2; ++j)
	{
		int velement;
		in >> velement;
		cons1.push_back(velement);
	}
	//cons2;
	cons2.clear(); //每次用之前需要清空
	for (int j = 0; j < dimension2; ++j)
	{
		int velement;
		in >> velement;
		cons2.push_back(velement);
	}
	is.push(cons2);
	is.push(cons1);

	ns.push(Node(initq, is, NULL, true));
	int count = 0;
	start = clock();
	while (!ns.empty()) {
		Node curn = ns.top();
		ns.pop();
		for (vector<function<Node(Node)> >::iterator it = rules.begin(); it != rules.end(); ++it) {
			Node child = (*it)(curn);
			if (child.v)
				ns.push(child);
		}

		//checking
		count++;
		Node* ancesp = curn.n;
		int minLeng = (int)curn.s.size(); //min stack length in the path
		while (ancesp != NULL) {
			Node ancesn = *ancesp;
			int len = (int)ancesn.s.size();
			if (minLeng >= len) {
				if (curn.p >= ancesn.p && ((!ancesn.s.empty() && curn.s.top() >= ancesn.s.top())
					|| ancesn.s.empty()))
				{
					cout << "Yes ";
					end = clock();
					//cout << (double(end - start)) / CLOCKS_PER_SEC << " seconds ";
					cout << end - start << " useconds ";
					//cout << getCurrentRSS() << " bytes" << endl;
					cout << getCurrentRSS() << " bytes ";
					cout << count << endl;
					exit(0);
				}
				minLeng = len;
			}
			ancesp = ancesn.n;
		}
	}

	end = clock();
	cout << "No ";
	//cout << (double(end - start)) / CLOCKS_PER_SEC << " seconds ";
	cout << end - start << " useconds ";
	//cout << getCurrentRSS() << " bytes" << endl;
	cout << getCurrentRSS() << " bytes ";
	cout << count << endl;
	return 0;
}
