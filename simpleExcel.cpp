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
#include<queue>
using namespace std;

const int maxn=1003, maxm=29;
int n, m;

bool check(const string&);//检查是否为字符串
double tod(const string&);//转成double
void strToUpper(string&);//所有小写变大写
bool Top();

class Cell{
public:
	string content;
	double value;
	bool isValue;//是数值或者输出时是数值就为true
	void evaluate();//递归下降法，返回表达式的结果
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

class EVA{
public:
	int Index;
	string str;
	double EA();
	double EB();
	double EC();
	double ED();
	double EE();
	double EF();
};

int inDegree[maxn*maxm];
queue<int> qe;//记录表达式的拓扑序

int main(){
//	ios::sync_with_stdio(false);
	freopen("in.csv", "r", stdin);
	cin >>n >>m; getchar(); getchar();//\r\n
	for(int i = 1; i<=n; i++){
		string s; getline(cin, s);
		s.erase(s.length()-1, 1);//去掉windows多出的\r回车符
		istringstream strin(s);
		int x = 1;
		while(getline(strin, table[i][x++].content,',')){//公式中可能含逗号，要合并
			Cell &cx = table[i][x-1];
			if(cx.content != "" && cx.content[0]=='=' && cx.content.find('(')!=-1){
				while(cx.content[cx.content.length()-1] != ')'){
					string cs; getline(strin, cs, ',');
					cx.content += ","+cs;
				}
			}
		}
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
/*
	cout <<"test graph----->" <<endl;
	for(int i=1; i<=n*m; i++){
		cout <<i<<":";
		for(int j=head[i]; edges[j].from ==i && j<=n*m; j++){
			cout <<edges[j].to <<" ";
		}
		cout <<endl;
	}
*/
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

	//evaluate
	while(!qe.empty()){
		int z = qe.front(); qe.pop();
		Cell &x = table[(z-1)/m+1][(z-1)%m+1];
		x.evaluate();
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
		if(table[(p-1)/m+1][(p-1)%m+1].content[0] == '=') qe.push(p);//记录表达式计算顺序
		if(head[p]==-1) continue;//出度为0,跳过

		for(int i=head[p]; edges[i].from == p && i<cnt; i++){
			int x = edges[i].to;
			if((--inDegree[x])==0) sk.push(x);
		}
	}
	return k==0?true:false;
}

double SUM(string s){
	double ans=0;
	for(int i=0; i<s.length()-1; i++){
		int x1=0,y1=0,x2=0,y2=0;
		if(isalpha(s[i]) && isdigit(s[i+1])){
			y1 = s[i]-64, x1=0;
			while(i<s.length()-1 && isdigit(s[i+1])){
				x1=x1*10+(s[++i]-'0');
			}

			if(i<s.length()-1 &&  s[i+1] == ':'){
				i+=2;//a1:b1
				y2 = s[i]-64, x2=0;
				while(i<s.length()-1 && isdigit(s[i+1])){
					x2=x2*10+(s[++i]-'0');
				}
				if(x1>x2) swap(x1,x2);
				if(y1>y2) swap(y1,y2);
				for(int j=x1; j<=x2; j++){
					for(int k=y1;k<=y2;k++){
						ans+=table[j][k].value;
					}
				}
			}else
				ans+=table[x1][y1].value;
		}
	}
	return ans;
}

double AVG(string s){
	double ans=0; int xcount=0;
	for(int i=0; i<s.length()-1; i++){
		int x1=0,y1=0,x2=0,y2=0;
		if(isalpha(s[i]) && isdigit(s[i+1])){
			y1 = s[i]-64, x1=0;
			while(i<s.length()-1 && isdigit(s[i+1])){
				x1=x1*10+(s[++i]-'0');
			}

			if(i<s.length()-1 &&  s[i+1] == ':'){
				i+=2;//a1:b1
				y2 = s[i]-64, x2=0;
				while(i<s.length()-1 && isdigit(s[i+1])){
					x2=x2*10+(s[++i]-'0');
				}
				if(x1>x2) swap(x1,x2);
				if(y1>y2) swap(y1,y2);
				for(int j=x1; j<=x2; j++){
					for(int k=y1;k<=y2;k++){
						ans+=table[j][k].value;
						xcount++;
					}
				}
			}else
				ans+=table[x1][y1].value, xcount++;
		}
	}
	return ans/xcount;
}

double MAX(string s){
	double ans=1e-308;
	for(int i=0; i<s.length()-1; i++){
		int x1=0,y1=0,x2=0,y2=0;
		if(isalpha(s[i]) && isdigit(s[i+1])){
			y1 = s[i]-64, x1=0;
			while(i<s.length()-1 && isdigit(s[i+1])){
				x1=x1*10+(s[++i]-'0');
			}

			if(i<s.length()-1 &&  s[i+1] == ':'){
				i+=2;//a1:b1
				y2 = s[i]-64, x2=0;
				while(i<s.length()-1 && isdigit(s[i+1])){
					x2=x2*10+(s[++i]-'0');
				}
				if(x1>x2) swap(x1,x2);
				if(y1>y2) swap(y1,y2);
				for(int j=x1; j<=x2; j++){
					for(int k=y1;k<=y2;k++){
						ans = max(ans, table[j][k].value);
					}
				}
			}else
				ans = max(ans, table[x1][y1].value);
		}
	}
	return ans;
}

double MIN(string s){
	double ans=1e308;
	for(int i=0; i<s.length()-1; i++){
		int x1=0,y1=0,x2=0,y2=0;
		if(isalpha(s[i]) && isdigit(s[i+1])){
			y1 = s[i]-64, x1=0;
			while(i<s.length()-1 && isdigit(s[i+1])){
				x1=x1*10+(s[++i]-'0');
			}

			if(i<s.length()-1 &&  s[i+1] == ':'){
				i+=2;//a1:b1
				y2 = s[i]-64, x2=0;
				while(i<s.length()-1 && isdigit(s[i+1])){
					x2=x2*10+(s[++i]-'0');
				}
				if(x1>x2) swap(x1,x2);
				if(y1>y2) swap(y1,y2);
				for(int j=x1; j<=x2; j++){
					for(int k=y1;k<=y2;k++){
						ans = min(ans, table[j][k].value);
					}
				}
			}else
				ans = min(ans, table[x1][y1].value);
		}
	}
	return ans;
}


double EVA::EA(){
	double a, b;
	a=EB();

	while(str[Index] == '+' || str[Index]=='-'){
		char op=str[Index++];

		b=EB();
		switch(op){
			case '+':
				a+=b;
				break;
			case '-':
				a-=b;
				break;
			default:
				break;
		}
	}
	if(str[Index]==')') Index++;
	return a;
}

double EVA::EB(){
	double a, b;
	a = EC();

	while(str[Index]=='*' || str[Index]=='/'){
		char op=str[Index++];
		
		b=EC();
		switch(op){
			case '*':
				a*=b;
				break;
			case '/':
				a/=b;
				break;
			default:
				break;
		}
	}
	return a;
}

double EVA::EC(){
	double a, b;
	a = ED();

	while(str[Index]=='%'){
		Index++;
		b=ED();
		int c = a, d = b;
		a=double(c%d);
	}
	return a;
}

double EVA::ED(){
	double a, b;
	a=EE();
	while(str[Index]=='^'){
		Index++;
		b=EE();
		a = pow(a, b);
	}
	return a;
}

double EVA::EE(){
	int x=0;
	while(str[Index]=='-'){
		 x++;Index++;
	}
	if(x&1) return EF()*-1.0;
	else return EF();
}

double EVA::EF(){
	if(str[Index]=='('){
		Index++;
		return EA();
	}
	char c=str[Index];
	switch(c){
		case 'A':
			if(str[Index+1]=='C'){//acos()
				Index+=5;
				return acos(EA());
			}
			else if(str[Index+1]=='S'){//asin()
				Index+=5;
				return asin(EA());
			}
			else if(str[Index+1]=='T'){//atan()
				Index+=5;
				return atan(EA());
			}else if(str[Index+1]=='B'){//abs()
				Index+=4;
				return fabs(EA());
			}else if(str[Index+1]=='V'){
				int p=Index+4;
				int loc=str.find(')', p);
				Index = loc+1;
				return AVG(str.substr(p, loc-p));
			}
		case 'C':
			if(str[Index+3]=='H'){//cosh()
				Index+=5;
				return cosh(EA());
			}
			else if(str[Index+3]=='('){//cos()
				Index+=4;
				return cos(EA());
			}
		case 'E'://exp()
			if(str[Index+1]=='X'){
				Index+=4;
				return exp(EA());
			}
		case 'L':
			if(str[Index+3]=='1'){//log10()
				Index+=6;
				return log10(EA());
			}
			else if(str[Index+3]=='G'){//log()
				Index+=4;
				return log(EA());
			}
		case 'M':
			if(str[Index+1]=='A'){//max()
				int p=Index+4;
				int loc=str.find(')', p);
				Index = loc+1;
				return MAX(str.substr(p, loc-p));
			}
			else if(str[Index+1]=='I'){//min()
				int p=Index+4;
				int loc=str.find(')', p);
				Index = loc+1;
				return MIN(str.substr(p, loc-p));
			}
		case 'P'://pow10()
			if(str[Index+1]=='O'){
				Index+=6;
				return pow(10, EA());
			}
		case 'S':
			if(str[Index+1]=='I'){
				if(str[Index+3]=='H'){//sinh()
					Index+=5;
					return sinh(EA());
				}
				else if(str[Index+3]=='('){//sin()
					Index+=4;
					return sin(EA());
				}
			}
			else if(str[Index+1]=='Q'){
				if(str[Index+3]=='T'){//sqrt()
					Index+=5;
					return sqrt(EA());
				}
				else if(str[Index+1]=='Q'){//sqr()
					Index+=4;
					return pow(EA(),2);
				}
			}else if(str[Index+1]=='U'){//sum()
				int p=Index+4;
				int loc=str.find(')', p);
				Index = loc+1;
				return SUM(str.substr(p, loc-p));
			}
		case 'T':
			if(str[Index+3]=='H'){//tanh()
				Index+=5;
				return tanh(EA());
			}
			else if(str[Index+1]=='A'){//tan()
				Index+=4;
				return tan(EA());
			}
		default:
			break;
	}
	char *endptr;
	const char *ss = str.c_str();
	if(isalpha(c)){
		int y=c-64;
		int x=strtol(ss+Index+1, &endptr, 10);
		Index=endptr-ss;
		return table[x][y].value;
	}
	double a = strtod(ss+Index, &endptr);
	Index = endptr - ss;
	return a;
}

void Cell::evaluate(){
	EVA e;
	e.str = content.substr(1);
	e.Index = 0;
	value = e.EA();
}
