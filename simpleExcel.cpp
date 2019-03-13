#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
#include<cmath>
#include<sstream>
using namespace std;

const int maxn=1003, maxm=29;
int n, m;

bool check();//检查是否为字符串
void tod();//转成double
bool Top();

struct Cell{
	string content;
	double value;
	bool isValue;//是数值或者输出时是数值就为true
	Cell(){
		isValue = false;//默认是字符
	}
	double evaluate();//递归下降法，返回表达式的结果
} table[maxn][maxm];//存储excel

//前向星图，用于拓扑排序
struct Edge{
	int from, to, next;//to = (x-1)*m + y;
} edges[maxn*maxm];//数量？？
int head[maxn*maxm], cnt;//头指针和边的数量


int main(){
//	ios::sync_with_stdio(false);
	freopen("in.csv", "r", stdin);
	cin >>n >>m; getchar(); getchar();//\r\n
	for(int i = 1; i<=n; i++){
		string s, ss; getline(cin, s);
		s.erase(s.length()-1, 1);//去掉windows多出的\r换行符
		stringstream strin(s);
		int x = 1;
		while(getline(strin, table[i][x++].content,','));
	}
	for(int i = 1; i<=n; i++)
		for(int j = 1;j<=m; j++)
			if(table[i][j].content == "") table[i][j].content="0";
	//test input()
	for(int i = 1; i<=n; i++){
		for(int j = 1;j<=m; j++){
			cout <<table[i][j].content <<" ";
		}
		cout <<endl;
	}

	for(int i = 1; i<=n; i++){
		for(int j = 1;j<=m; j++){
			Cell &x = table[i][j];//make a alias
			if(x.isValue) cout <<x.value <<",";
			else cout <<x.content <<",";
		}
		cout <<endl;
	}
	return 0;
}

double Cell::evaluate(){return 1;}
bool check(){return 1;}
void tod(){}

bool top(){return 1;}
