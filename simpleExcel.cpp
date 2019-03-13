#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
#include<cmath>
#include<sstream>
#include<cctype>
#include<cstdlib>
#include<cstring>
#include<stack>
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
cout <<"test input---->" <<endl;
	for(int i = 1; i<=n; i++){
		for(int j = 1;j<=m; j++){
			cout <<table[i][j].content <<" ";
		}
		cout <<endl;
	}

	//在判断类型的同时，建图
	for(int i=1; i<=n; i++){
		for(int j=1; j<=m; j++){
			Cell &x = table[i][j];
			if(x.content == "") x.content="0";//1.处理空白符 
			else if(x.content[0] == '='){//2.处理函数
				x.isValue = true;
				strToUpper(x.content);//都变成大写
				addEdges((i-1)*m+j, x.content);//根据公式向图中加入边
			}else if(check(x.content)){//3.处理数值
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
cout <<"test graph----->" <<endl;
	for(int i=1; i<=n*m; i++){
		cout <<i<<":";
		for(int j=head[i]; edges[j].from ==i && j<=n*m; j++){
			cout <<edges[j].to <<" ";
		}
		cout <<endl;
	}
//test top
/*
cout <<"test Top()----->" <<endl;
	if(Top()) cout << "YES" <<endl;
	else cout <<"ERROR!" << endl;
*/
	if(!Top()){
		cout <<"error!" <<endl;
		return 0;
	}
	//output
cout <<"test output----->" <<endl;
	for(int i = 1; i<=n; i++){
		for(int j = 1;j<=m; j++){
			Cell &x = table[i][j];//make a alias
			if(x.isValue) printf("%.2lf", x.value);
			else cout <<x.content;
			if(j<m) printf(",");
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
		int x1=0,y1=0,x2=0,y2=0;
		if(isalpha(str[i]) && isdigit(str[i+1])){
			y1 = str[i]-64, x1=0;
			while(i<str.length()-1 && isdigit(str[i+1])){
				x1=x1*10+(str[++i]-'0');
			}

			if(i<str.length()-1 &&  str[i+1] == ':'){
				i+=2;//a1:b1
				y2 = str[i]-64, x2=0;
				while(i<str.length()-1 && isdigit(str[i+1])){
					x2=x2*10+(str[++i]-'0');
				}
			cout <<x1 <<" "<<y1 <<" " <<x2 <<" " <<y2 <<endl;
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
			}else{
				Edge &x = edges[cnt];
				x.from=(x1-1)*m+y1;
				x.to = f;
				cnt++;
				inDegree[f]++;
			}
		}
	}
}
bool Top(){
	stack<int> sk;
	for(int i=1; i<=n*m; i++)
		if(inDegree[i]==0)
			sk.push(i);

	int k=n*m;
	while(!sk.empty()){
		int p = sk.top(); sk.pop(); k--;
		if(head[p]==-1) continue;//出度为0,跳过

		for(int i=head[p]; edges[i].from == p && i<cnt; i++){
			int x = edges[i].to;
			if((--inDegree[x])==0) sk.push(x);
		}
	}
	return k==0?true:false;
}

double Cell::evaluate(){return 1;}
