// Generator_Vector_Vector.cpp : 定义控制台应用程序的入口点。

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main(int argc, const char * argv[]) {
	ofstream out, record;
	out.open("cases.txt", ios::out | ios::trunc);
	record.open("record.xls", ios::out | ios::app);
	srand((unsigned int)time(0));
	//int rulenum = rand() % 10;
	//int dimension = rand() % 3 + 1;//[1, 3]
	int dimension1 = 3;//dimension of state
	int dimension2 = 3;//dimension of stack symbols
	int rulenum = rand() % 10 + 1;
	int tag, coeffq, constq, coeffa, consta, coeffg, constg, coeffa1, coeffa2, consta1, consta2;
	record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
		<< "<th>case</th>"
		<< "<th>" << argv[1] << "</th>"
		<< "</tr></table>";
	out << dimension1 << " " << dimension2 << endl;  

	for (int i = 0; i < rulenum; ++i) {
		tag = rand() % 5;
		out << tag << " ";
		switch (tag) {
		case 0:
			coeffq = 1, coeffa = 1;
			//先写一部分
			out << coeffq << " " << coeffa;
			record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
				<< "<th>" << tag << "</th>"
				<< "<th>" << coeffq << "</th>"
				<< "<th>" << coeffa << "</th>";
			//用for 写constq
			for (int j = 0; j < dimension1; ++j)
			{
				constq = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << constq;
				record << "<th>" << constq << "</th>";
			}
			//用for写consta vector
			for (int j = 0; j < dimension2; ++j)
			{
				consta = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << consta;
				record << "<th>" << consta << "</th>";
			}
			//写剩下的
			out << endl;
			record << "</tr></table>";

			break;
		case 1:
			coeffq = 1;
			out << coeffq;
			//<< " " << constq;
			record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
				<< "<th>" << tag << "</th>"
				<< "<th>" << coeffq << "</th>";
				
			//用for 写constq
			for (int j = 0; j < dimension1; ++j)
			{
				constq = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << constq;
				record << "<th>" << constq << "</th>";
			}
			//写剩下的
			out << endl;
			record << "</tr></table>";
			break;
		case 2:
			coeffq = 1,
				coeffa1 = 1, coeffa2 = 1;
			out << coeffq << " " << coeffa1 << " " << coeffa2;
			record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
				<< "<th>" << tag << "</th>"
				<< "<th>" << coeffq << "</th>"
				<< "<th>" << coeffa1 << "</th>"
				<< "<th>" << coeffa2 << "</th>";
			//用for 写constq
			for (int j = 0; j < dimension1; ++j)
			{
				constq = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << constq;
				record << "<th>" << constq << "</th>";
			}
			//consta
			for (int j = 0; j < dimension2; ++j)
			{
				consta = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << consta;
				record << "<th>" << consta << "</th>";
			}
			//写剩下的
			out << endl;
			record << "</tr></table>";
			break;
		case 3:
			//consta1, consta2 是 dimension纬向量
			coeffq = 1, coeffa1 = 1,coeffa2 = 1;
			out << coeffq << " " << coeffa1 << " " << coeffa2;
			record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
				<< "<th>" << tag << "</th>"
				<< "<th>" << coeffq << "</th>"
				<< "<th>" << coeffa1 << "</th>"
				<< "<th>" << coeffa2 << "</th>";

			//用for 写constq
			for (int j = 0; j < dimension1; ++j)
			{
				constq = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << constq;
				record << "<th>" << constq << "</th>";
			}

			//consta1
			for (int j = 0; j < dimension2; ++j)
			{
				consta1 = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << consta1;
				record << "<th>" << consta1 << "</th>";
			}
			
			//consta2
			for (int j = 0; j < dimension2; ++j)
			{
				consta2 = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << consta2;
				record << "<th>" << consta2 << "</th>";
			}
			//写剩下的
			out << endl;
			record << "</tr></table>";

			break;
		case 4:
			//consta 是 dimension纬向量，该怎么随机生成？
			coeffq = 1;
			out << coeffq;
			record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
				<< "<th>" << tag << "</th>"
				<< "<th>" << coeffq << "</th>";

				//用for 写constq
			for (int j = 0; j < dimension1; ++j)
			{
				constq = (rand() % (10 - (-10) + 1)) - 10;
				out << " " << constq;
				record << "<th>" << constq << "</th>";
			}
			//consta
			for (int j = 0; j < dimension2; ++j)
			{
				consta = rand() % 11;
				out << " " << consta;
				record << "<th>" << consta << "</th>";
			}
			//写剩下的
			out << endl;
			record << "</tr></table>";

			break;
		default:
			cout << "error" << endl;
			exit(-1);
			break;
		}
	}


	int initp, consts1, consts2;

	//out << "5 " << initp; // tag 5 indicate initial state
	out << "5";


	record << "<table width='100%' border='1' cellspacing='1' cellpadding='1' borderColor=#0000FF><tr>"
		<< "<th>initialp</th>";
		//<< "<th>" << initp << "</th>";
	for (int j = 0; j < dimension1; ++j)
	{
		initp = rand() % 16;
		out << " " << initp;
		record << "<th>" << initp << "</th>";
	}
	for (int j = 0; j < dimension2; ++j)
	{
		consts1 = rand() % 11;
		out << " " << consts1;
		record << "<th>" << consts1 << "</th>";
	}
	for (int j = 0; j < dimension2; ++j)
	{
		consts2 = rand() % 11;
		out << " " << consts2;
		record << "<th>" << consts2 << "</th>";
	}
	out << endl;
	record << "</tr></table>";

	out.close();
	record.close();
	return 0;
}
