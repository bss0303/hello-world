#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
#include<cmath>
#include<sstream>
#include<cctype>
#include<cstdlib>
#include<cstring>
using namespace std;

const int maxn=1003, maxm=29;
int n, m;

bool check(const string&);//检查是否为字符串
double tod(const string&);//转成double
void strToUpper(string&);//所有小写变大写
bool Top();

struct Cell{
	string content;
	double value;
	bool isValue;//是数值或者输出时是数值就为true
	double evaluate();//递归下降法，返回表达式的结果
} table[maxn][maxm];//存储excel

//前向星图，用于拓扑排序
struct Edge{
	int from, to;//to = (x-1)*m + y;
	bool operator<(const Edge& x){
		return (from==x.from)?(to<x.to):from<x.from;
	}
} edges[maxn*maxm];//数量？？
int head[maxn*maxm], cnt;//头指针和边的数量
void addEdges(int, string);

int inDegree[maxn*maxm];

int main(){
//	ios::sync_with_stdio(false);
	freopen("in.csv", "r", stdin);
	cin >>n >>m; getchar(); getchar();//\r\n
	for(int i = 1; i<=n; i++){
		string s; getline(cin, s);
		s.erase(s.length()-1, 1);//去掉windows多出的\r换行符
		istringstream strin(s);
		int x = 1;
		while(getline(strin, table[i][x++].content,','));
	}
//test input
/*	for(int i = 1; i<=n; i++){
		for(int j = 1;j<=m; j++){
			cout <<table[i][j].content <<" ";
		}
		cout <<endl;
	}
*/
	//在判断类型的同时，建图
	for(int i=1; i<=n; i++){
		for(int j=1; j<=m; j++){
			Cell &x = table[i][j];
			if(x.content == "") x.content="0"; 
			else if(x.content[0] == '='){
				x.isValue = true;
				strToUpper(x.content);//都变成大写
				addEdges((i-1)*m+j,x.content);//根据公式向图中加入边
			}else if(check(x.content)){
				x.isValue=1;
				x.value=tod(x.content);
			}
		}
	}
	sort(edges, edges+cnt);
	memset(head,-1,sizeof(head));
	head[edges[0].from] = 0;
	for(int i=1; i<cnt; i++){
		if(edges[i].from != edges[i-1].from){
			head[edges[i].from] = i;
		}
	}
//test graph
	for(int i=1; i<=n*m; i++){
		cout <<i<<":";
		for(int j=head[i]; edges[j].from ==i && j<=n*m; j++){
			cout <<edges[j].to <<" ";
		}
		cout <<endl;
	}

	//output
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

bool check(const string& str){
	istringstream strin(str);
	double b; char p;
	if(!(strin>>b) || strin >>p) return false;
	return true;
}

double tod(const string& str){
	istringstream iss(str);
	double num;
	iss>>num;
	return num;
}
void strToUpper(string& str){//所有小写变大写
	for(int i = 0; i<str.length(); i++)
		if(str[i]<= 122 && str[i]>=97) str[i]-=32;
}

void addEdges(int f, string str){
	for(int i=0; i<str.length()-1; i++){
		int x1=0, y1=0, x2=0, y2=0;
		if(isalpha(str[i]) && isdigit(str[i+1])){
			int y1 = str[i]-64, x1=0;
			while(i<str.length()-1 && isdigit(str[i+1])){
				x1=x1*10+(str[++i]-'0');
			}
		}
		if(str[i+1] == ':'){
			i+=2;//a1:b1
			int y2 = str[i]-64, x2=0;
			while(i<str.length()-1 && isdigit(str[i+1])){
				x2=x2*10+(str[++i]-'0');
			}
			if(x1>x2) swap(x1,x2);
			if(y1>y2) swap(y1,y2);
			for(int j=x1; j<=x2; j++){
				for(int k=y1;k<=y2;k++){
					Edge &x = edges[cnt];
					x.from=(j-1)*m+k;
					x.to = f;
					cnt++;
					inDegree[f]++;
				}
			}
			continue;
		}
		Edge &x = edges[cnt];
		x.from=(x1-1)*m+y1;
		x.to = f;
		cnt++;
		inDegree[f]++;
	}
}
bool Top(){
}

double Cell::evaluate(){return 1;}
