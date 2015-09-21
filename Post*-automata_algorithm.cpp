// Post_Vector_Vector.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <math.h>
#include <map>
#include <set>
#include <list>
#include <queue>
#include <ctime>
#include <functional>
#include "mem.h"
using namespace std;

class Trans {
public:    //   a
	vector<int> p; // p-->q
	vector<int> a;
	vector<int> q;
	Trans() {}
	Trans(vector<int> p, vector<int> a, vector<int> q)
	{
		this->p = p;
		this->a = a;
		this->q = q;
	}
	bool operator<(const Trans& t) const
	{
		return (p < t.p || (p == t.p && a < t.a) ||
			(p == t.p && a == t.a && q < t.q));
	}
};

class Right {  //right part of a rule
public:
	int tag;
	vector<int> q;
	vector<int> a1;
	vector<int> a2;
	bool v;
	//simple pop: p,\gamma -> q, \epsilon
	Right(int tag, vector<int> q, bool v) {
		this->tag = tag;
		this->q = q;
		//this->a1 = -1;
		for (vector<int>::iterator it = this->a1.begin(); it != this->a1.end(); ++it)
		{
			*it = -1;
		}
		//this->a2 = -1;
		for (vector<int>::iterator it = this->a2.begin(); it != this->a2.end(); ++it)
		{
			*it = -1;
		}
		this->v = v;
	}
	//internal rule: p, \gamma -> q, a1
	//non-standard pop: p, \gamma1\gamma2 -> q,a1
	//simple-push: p, \epsilon -> q, a1
	Right(int tag, vector<int> q, vector<int> a1, bool v) {
		this->tag = tag;
		this->q = q;
		this->a1 = a1;
		//this->a2 = -1;
		for (vector<int>::iterator it = (this->a2).begin(); it != (this->a2).end(); ++it)
		{
			*it = -1;
		}
		this->v = v;
	}
	//push: p, \gamma -> q, a1 a2
	Right(int tag, vector<int> q, vector<int> a1, vector<int> a2, bool v) {
		this->tag = tag;
		this->q = q;
		this->a1 = a1;
		this->a2 = a2;
		this->v = v;
	}
};

//internal rule: p, \gamma -> q, a1
function<Right(vector<int>, vector<int>, vector<int>)> computeRule0(const function< vector<int>(vector<int>)>& computeq, const function<vector<int>(vector<int>)>& computea1)
{
	return [=](vector<int> p, vector<int> y, vector<int> notuse) {
		vector<int> q = computeq(p);
		vector<int> a1 = computea1(y);
		/*if (q >= 0 && a1 >= 0) {
		return Right(0, q, a1, true);
		}
		return Right(0, q, a1, false);*/
		bool v = true;
		//if (q < 0)
		//	v = false;
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
		//if (v) 
		//	return Right(0, q, a1, true);
		//return Right(0, q, a1, false);
		return Right(0, q, a1, v);
	};
}
//simple pop: p,\gamma -> q, \epsilon
function<Right(vector<int>, vector<int>, vector<int>)> computeRule1(const function<vector<int>(vector<int>)>& computeq)
{
	return [=](vector<int> p, vector<int> y, vector<int> notuse) {
		vector<int> q = computeq(p);
		/*if (q >= 0) {
			return Right(1, q, true);
		}
		return Right(1, q, false);*/
		bool v = true;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
		return Right(1, q, v);
	};
}
//non-standard pop: p, \gamma1\gamma2 -> q,a1
// q??p????–‘∫????¨a1?? \gamma1\gamma2????–‘∫???

function<Right(vector<int>, vector<int>, vector<int>)> computeRule2(const function<vector<int>(vector<int>)>& computeq, const function<vector<int>(vector<int>, vector<int>)>& computea1)
{
	return [=](vector<int> p, vector<int> g1, vector<int> g2) {
		vector<int> q = computeq(p);
		vector<int> a1 = computea1(g1, g2);
		/*if (q >= 0 && a1 >= 0) {
		return Right(2, q, a1, true);  //use a2 to store g1
		}
		return Right(2, q, a1, false);*/
		bool v = true;
		//if (q < 0)
		//	v = false;
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
		return Right(2, q, a1, v);
	};
}

// push: p, \gamma -> q, a1 a
function<Right(vector<int>, vector<int>, vector<int>)> computeRule3(const function<vector<int>(vector<int>)>& computeq, const function<vector<int>(vector<int>)>& computea1,
	const function<vector<int>(vector<int>)>& computea2)
{
	return [=](vector<int> p, vector<int> y, vector<int> notuse) {
		vector<int> q = computeq(p);
		vector<int> a1 = computea1(y);
		vector<int> a2 = computea2(y);
		/*if (q >= 0 && a1 >= 0 && a2 >= 0) {
		return Right(3, q, a1, a2, true);
		}
		return Right(3, q, a1, a2, false);*/
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

		return Right(3, q, a1, a2, v);

	};
}
//simple push: p, \epsilon -> q, a
function<Right(vector<int>, vector<int>, vector<int>)> computeRule4(const function<vector<int>(vector<int>)>& computeq, const vector<int>& a)
{
	return [=](vector<int> p, vector<int> y, vector <int>  notuse) {
		vector<int> q = computeq(p);
		/*if (q >= 0) {
			return Right(4, q, a, true);
		}
		return Right(4, q, a, false);*/
		bool v = true;
		for (vector<int>::iterator it = q.begin(); it != q.end(); ++it)
		{
			if (*it < 0)
			{
				v = false;
				break;
			}
		}
		return Right(4, q, a, v);

	};
}

int main(int argc, const char * argv[]) {
	vector<function<Right(vector<int>, vector<int>, vector<int>)> > rules; //delta
	vector<function<Right(vector<int>, vector<int>, vector<int>)> > rulesp; //delta'
	vector<Trans> trans;
	set<Trans> rel;
	// for checking
	//map<int, list<int> > relp; //rel graph as a adj list
	//queue<int> relq; //for bfs
	//set<int> rels;  //for bfs
	//now state changes to vector!!!!!!
	map<vector<int>, list<vector<int>> > relp; //rel graph as a adj list
	queue<vector<int>> relq; //for bfs
	set<vector<int>> rels;

	map<Trans, list<Trans> > dep; //
	queue<Trans> depq; //for bfs
	set<Trans> deps; //for bfs

	int generator = -3;
	Trans t; //temp use
	time_t start, end;
	//sturct timeeval start, end;


	ifstream in;
	in.open("cases.txt", ios::in);
	//int tag, initq, coeffq, consq, coeffa1, consa1, coeffa2, consa2;
	//int cons1, cons2;
	int tag, coeffq, coeffa1, coeffa2;
	vector<int> consa1, consa2, cons1, cons2;
	vector<int> initq, consq;//change state from int to vector<int>
	int dimension1, dimension2;//dimension1 is the dimension of state, dimension2 is the dimension of stack symbol
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
	//the new node, their state!!!!!!!! not -2, -3, but (-2, -2, ...)
	vector<int> newnode1, big_state_vector,zero_state;
	for (int j = 0; j < dimension1; ++j)
	{
		
		newnode1.push_back(-2);
		big_state_vector.push_back(100000);
		zero_state.push_back(0);
	}
	trans.push_back(Trans(initq, cons1, newnode1));
	rel.insert(Trans(newnode1, cons2, big_state_vector));

	// rules.push_back(computeRule4([](int n){return n-1;}, 1));
	// trans.push_back(Trans(1000, -1, 1000000));
	// rules.push_back(computeRule4([](int n){return n-1;}, 1));
	// trans.push_back(Trans(1000, -1, 1000000));
	/*rules.push_back(computeRule4([](int n){return n-2;}, 9));
	rules.push_back(computeRule1([](int n) {return n + 8; }));
	rules.push_back(computeRule1([](int n) {return n - 4; }));

	trans.push_back(Trans(4, -1, 1000000));*/

	//rules.push_back(computeRule1([](int n) {return n + 1; }));

	//rules.push_back(computeRule3([](int n) {return n + 1; }, [](int n) {return n + 1; }, [](int n) {return n + 2; }));
	//trans.push_back(Trans(1, 1, -2));
	//rel.insert(Trans(-2, 2, 1000000));

	/*
	rules.push_back(computeRule1([](int n) {return n + 3; }));
	rules.push_back(computeRule4([](int n) {return n - 1; }, 8));
	rules.push_back(computeRule3([](int n) {return n - 1; }, [](int n) {return n - 2; }, [](int n) {return n + 9; }));
	trans.push_back(Trans(2, 9, -2));
	rel.insert(Trans(-2, 1, 1000000));
	*/

	//reach is faster than post on this case
	/*rules.push_back(computeRule3([](int n) {return n - 1; }, [](int n) {return n + 9; }, [](int n) {return n + 6; }));
	rules.push_back(computeRule3([](int n) {return n - 1; }, [](int n) {return n - 3; }, [](int n) {return n - 9; }));
	//trans.push_back(Trans(16, 5, -2));
	//rel.insert(Trans(-2, 1, 1000000));
	trans.push_back(Trans(10, 5, -2));
	rel.insert(Trans(-2, 1, 1000000));*/

	//should be yes, but no
	/*rules.push_back(computeRule1([](int n) {return n + 9; }));
	rules.push_back(computeRule2([](int n) {return n + 7; }, [](int n1, int n2) {return n1 + n2 + 3; }));
	rules.push_back(computeRule4([](int n) {return n - 9; }, 3));
	rules.push_back(computeRule2([](int n) {return n + 7; }, [](int n1, int n2) {return n1 + n2 + 5; }));
	trans.push_back(Trans(3, 6, -2));
	rel.insert(Trans(-2, 3, 1000000));*/
	//should yes, but no
	/*rules.push_back(computeRule0([](int n) {return n - 10; }, [](int n) {return n + 5; }));

	rules.push_back(computeRule0([](int n) {return n + 2; }, [](int n) {return n - 1; }));
	rules.push_back(computeRule2([](int n) {return n + 3; }, [](int n1, int n2) {return n1 + n2 - 2; }));
	trans.push_back(Trans(11, 4, -2));
	rel.insert(Trans(-2, 10, 1000000));*/
	//should yes, but no
	/*rules.push_back(computeRule4([](int n) {return n - 9; }, 3));
	rules.push_back(computeRule1([](int n) {return n + 9; }));
	trans.push_back(Trans(10, 1, -2));
	rel.insert(Trans(-2, 1, 1000000));*/

	//should yes, but no, in the else, add if t.p>=t.q, then YES1, then rectify
	/*rules.push_back(computeRule4([](int n) {return n - 9; }, 3));
	rules.push_back(computeRule1([](int n) {return n + 9; }));
	trans.push_back(Trans(17, -1, 100000));*/


	//doesn't satisfy yes1 and yes2, cannot stop, but in fact it has an infinite run, so it goes on and on.
	//rectify, pop, dependency, yes2
	/*rules.push_back(computeRule1([](int n) {return n + 7; }));
	rules.push_back(computeRule3([](int n) {return n + 3; }, [](int n) {return n - 7; }, [](int n) {return n + 6; }));
	rules.push_back(computeRule3([](int n) {return n + 7; }, [](int n) {return n - 10; }, [](int n) {return n - 10; }));
	trans.push_back(Trans(5, 1, -2));
	rel.insert(Trans(-2, 8, 1000000));*/
	vector<int> epsilon, zero, big_vector;
	for (int j = 0; j < dimension2; ++j)
	{
		epsilon.push_back(-1);
		zero.push_back(0);
		big_vector.push_back(10000);
	}
	int count = 0;
	start = clock();
	while (!trans.empty()) {
		t = trans.back();
		trans.pop_back();
		//checking
		count++;
		boolean b = rel.find(t) == rel.end();
		if (b) {
			rel.insert(t); //for result
			relp[t.p].push_back(t.q); //for checking
		}
		//checking...
		rels.clear();
		relq.push(t.q);
		while (!relq.empty()) {
			vector<int> e = relq.front();
			relq.pop();
			if (e >= zero_state && e <= t.p) {
				cout << "Yes1 ";
				end = clock();
				cout << end - start << " useconds ";
				//cout << getCurrentRSS() << " bytes" << endl;
				cout << getCurrentRSS() << " bytes ";
				cout << count << endl;
				exit(0);
			}
			rels.insert(e);
			list<vector<int>> adj = relp[e];
			for (list<vector<int>>::iterator it = adj.begin(); it != adj.end(); ++it) {
				if (rels.find(*it) == rels.end()) { //not visited
					relq.push(*it);
				}
			}
		}

		deps.clear();
		list<Trans> adj = dep[t];
		for (list<Trans>::iterator it = adj.begin(); it != adj.end(); ++it) {
			depq.push(*it);
		}
		while (!depq.empty()) {
			Trans e = depq.front();
			depq.pop();
			if (e.p >= zero_state && t.p >= zero_state && e.p <= t.p && e.a >= zero && e.a <= t.a) {
				cout << "Yes2 ";
				end = clock();
				//cout << (double((end - start) * pow(10, 6))) / CLOCKS_PER_SEC << " useconds ";
				cout << end - start << " useconds ";
				//cout << getCurrentRSS() << " bytes" << endl;
				cout << getCurrentRSS() << " bytes ";
				cout << count << endl;
				exit(0);
			}
			deps.insert(e);
			list<Trans> adj = dep[e];
			for (list<Trans>::iterator it = adj.begin(); it != adj.end(); ++it) {
				if (deps.find(*it) == deps.end()) { //not visited
					depq.push(*it);
				}
			}
		}

		//end chekcing
		if (b) {
			if (t.a != epsilon) {
				//t.a = -1 means epsilon
				for (vector<function<Right(vector<int>, vector<int>, vector<int>)> >::iterator itf = rules.begin(); itf != rules.end(); ++itf) {
					Right r = (*itf)(t.p, t.a, big_vector);

					if (!r.v) continue;
					if (r.tag == 0) {
						Trans temp = Trans(r.q, r.a1, t.q);
						trans.push_back(temp);
						dep[temp].push_back(t);
						//new added if
						if (r.q >= zero_state && t.q >= zero_state && r.q >= t.q) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
					}
					else if (r.tag == 1) {
						trans.push_back(Trans(r.q, epsilon, t.q));
						//new added if
						if (r.q >= zero_state && t.q >= zero_state && r.q >= t.q) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
						for (set<Trans>::iterator it = rel.begin(); it != rel.end(); ++it) {

							if (it->p == t.q && it->a != epsilon) {

								Trans temp = Trans(r.q, it->a, it->q);
								//trans.push_back(temp);
								dep[temp].push_back(*it);
								//new added if
								if (r.q >= zero_state && it->q >= zero_state && r.q >= it->q) {
									cout << "Yes1 ";
									end = clock();
									cout << end - start << " useconds ";
									cout << getCurrentRSS() << " bytes ";
									cout << count << endl;
									exit(0);
								}
							}
						}
					}
					else if (r.tag == 2) {

						for (set<Trans>::iterator it = rel.begin(); it != rel.end(); ++it) {
							//t is the second
							if (it->p >= zero_state && it->q == t.p && it->a != epsilon) {
								Right r2 = (*itf)(it->p, it->a, t.a);
								if (r2.v) {
									Trans temp = Trans(r2.q, r2.a1, t.q);
									trans.push_back(temp);
									dep[temp].push_back(t);
									//new added if
									if (r2.q >= zero_state && t.q >= zero_state && r2.q >= t.q) {
										cout << "Yes1 ";
										end = clock();
										cout << end - start << " useconds ";
										cout << getCurrentRSS() << " bytes ";
										cout << count << endl;
										exit(0);
									}
								}
							}
							// t is the first
							if (it->p == t.q && it->a != epsilon) {
								Right r3 = (*itf)(t.p, t.a, it->a);
								if (r3.v) {
									Trans temp = Trans(r3.q, r3.a1, it->q);
									trans.push_back(temp);
									//dep[temp].push_back(Trans(it->p, it->a, it->q));
									dep[temp].push_back(*it);
									if (r3.q >= zero_state && it->q >= zero_state && r3.q >= it->q) {
										cout << "Yes1 ";
										end = clock();
										cout << end - start << " useconds ";
										cout << getCurrentRSS() << " bytes ";
										cout << count << endl;
										exit(0);
									}
								}

							}
						}



						//another case.............
						/*
						rulesp.push_back(Rule(0, t.q, r.a2, r.q, r.a3));
						for (set<Trans>::iterator it = rel.begin(); it != rel.end(); ++it) {
						if (it->p == t.q && it->a == r.a2) {
						Trans temp  = Trans(r.q,r.a3,it->q);
						trans.push_back(temp);
						//    dep[temp].push_back(t);
						dep[temp].push_back(*it);
						}
						}
						*/
					}
					else if (r.tag == 3) {
						int nnode = generator--;
						vector<int> newnode;
						for (int j = 0; j < dimension1; ++j)
						{
							newnode.push_back(nnode);					
						}
						Trans temp1, temp2;
						temp1 = Trans(r.q, r.a1, newnode);
						temp2 = Trans(newnode, r.a2, t.q);
						trans.push_back(temp1);
						//trans.push_back(temp2);
						rel.insert(temp2);
						relp[newnode].push_back(t.q);//this is important!!!!
												   //new added if
						if (r.q >= zero_state && t.q >= zero_state && r.q >= t.q) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
						dep[temp1].push_back(t);
						dep[temp2].push_back(t);
						for (set<Trans>::iterator it = rel.begin(); it != rel.end(); ++it) {
							if (it->a == epsilon && it->q == newnode) {
								trans.push_back(Trans(it->p, r.a2, t.q));
								//new added if
								if (it->p >= zero_state && t.q >= zero_state && it->p >= t.q) {
									cout << "Yes1 ";
									end = clock();
									cout << end - start << " useconds ";
									cout << getCurrentRSS() << " bytes ";
									cout << count << endl;
									exit(0);
								}
							}
						}
					}
					else {
						//r.tag == 4
						trans.push_back(Trans(r.q, r.a1, t.p));
						if (r.q >= zero_state && t.p >= zero_state && r.q >= t.p) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
					}
				}

			}
			else {
				if (t.p >= t.q && t.q >= zero_state) {
					cout << "Yes1 ";
					end = clock();
					cout << end - start << " useconds ";
					cout << getCurrentRSS() << " bytes" << endl;
					exit(0);
				}
				for (set<Trans>::iterator it = rel.begin(); it != rel.end(); ++it) {
					if (it->p == t.q && it->a != epsilon) {
						trans.push_back(Trans(t.p, it->a, it->q));
						//new added if
						if (t.p >= zero_state && it->q >= zero_state && t.p >= it->q) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
					}
				}


				for (vector<function<Right(vector<int>, vector<int>, vector<int>) > >::iterator itf = rules.begin(); itf != rules.end(); ++itf) {
					Right r = (*itf)(t.p, epsilon, big_vector);
					if (!r.v) continue;
					if (r.tag == 4) {
						Trans temp = Trans(r.q, r.a1, t.p);
						trans.push_back(temp);
						if (r.q >= zero_state && t.p >= zero_state && r.q >= t.p) {
							cout << "Yes1 ";
							end = clock();
							cout << end - start << " useconds ";
							cout << getCurrentRSS() << " bytes ";
							cout << count << endl;
							exit(0);
						}
					}
				}

			}
		}
	}
	end = clock();
	cout << "No ";
	cout << end - start << " useconds ";
	//cout << getCurrentRSS() << " bytes " << rel.size() << endl;
	cout << getCurrentRSS() << " bytes ";
	cout << count << endl;
	return 0;
}
