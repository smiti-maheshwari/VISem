#include <bits/stdc++.h>

using namespace std;

string key[] = { "auto", "break", "case", "char", "const", "continue", "default", "do"
				, "double", "else", "enum", "extern ", "float", "for", "goto", "if", "int"
				, "long", "register", "return", "short", "signed", "sizeof", "static"
				, "struct", "switch", "typedef", "union", "unsigned", "void", "volatile"
				, "while" , "printf", "long long", "long long int", "scanf"};

void keywordCheck(string *str)
{	
	int n = sizeof (key) / sizeof(key[0]), i;
    
	for (i = 0; i < n; i++) {
		if ((*str).find(key[i]) < (*str).length()) {
			cout<<key[i]<<" ";
		}
	}
	
	cout<<endl;
}

void delimeterCheck(string str)
{
	//char delim[] = { '{', '}', '(', ')', '[', ']', ',' , ';' , ''' };
	string delim = "{}()[],;'";
	
	int m = delim.length();
	
	int n = (str).length();
	vector <int> ind;
	//cout<<n<<" "<<m<<endl;
	for (int i = 0; i < n; i++) {
		//string x="";
		//x+=str[i];		
		//cout<<"hi"<<endl;
		for(int j = 0; j < m; j++) { 
			if (str[i]==delim[j]) {
				cout<<str[i]<<" ";	
				break;		
			}
		}
	}

	cout<<endl;
}

void puncCheck(string str)
{
	//string punc[] = { " + ", " - ", " * ", " / ", " %d", " ^", " &" };
	string punc = "+-*%&^/<>!=";
	int n = str.length();
	int m = punc.length();
	//cout<<n<<" "<<m<<endl;

	for(int i=0; i<n; i++) {	
		if(str[i] =='"') {
			i++;
			string x="";
			while(str[i] != '"') {
				x+=str[i];
				i++;		
			}
			cout<<x<<" ";
		} else {
			//cout<<"-> "<<str[i]<<endl;
			for(int j=0; j<m; j++) {
				if(str[i] == punc[j]) {
					cout<<str[i]<<" ";
				}
			}
		}
	}
	cout<<endl;
}

//Test cases
//	for (int i = 0; i < n; i++) printf("%d\n", i);
//	printf("%d", (int)x);
//	scanf(" %d\n", &an__132h);

void identCheck(string str)
{
	int n = str.length();
	int size = sizeof(key) / sizeof(key[0]);

	for(int i=0; i<n; i++) {
			string x="";
			int f1=0;
			int g=0;
			// to remove % in printf & scanf
			if(str[i] =='"') {
				i++;
				while(str[i] != '"') {
					i++;
				}
			}
			// to remove typecast			
			else if(str[i] >= 97 && str[i] <= 122 && i-1 >= 0 && str[i-1]=='(') {
					f1=1;
					string y="";
					y+=str[i];
					int j=i;					
					j++;					
					while(str[j] !=')') {
						
						y+=str[j];						
						j++;
					}
					for(int k=0; k<size; k++) {
						if(y == key[k]) {
							g=1;
							break;
						}
					}
			}
			if((f1==0) && (g==0) && (str[i] >= 65 && str[i] <= 91) || (str[i] >= 97 && str[i] <= 122)
					 || (str[i] == 95)) {
					x+=str[i];
					i++;			
					while((str[i] >= 65 && str[i] <= 91) || (str[i] >= 97 && str[i] <= 122)
					 || (str[i] == 95) || (str[i] >=48 && str[i] <=57)) {
						x += str[i];
						i++;			
					}	
			}
			//cout<<"string x -> "<<str[i]<<" "<<i<<" "<<x<<endl;
			if(x != "") {
			int f=0; 
				//cout<<"string x -> "<<x<<endl;
				for(int k=0; k<size; k++) {
					if(x == key[k]) {
						f=1;
						break;
					}
				}
				if(f==0) {
					cout<<x<<" ";
				}
			}
		}
	
	cout<<endl;
}

int main()
{
	string str;
	
	getline(cin, str);
	int l = str.find("/*");
	int r = str.find("*/");
	int p = str.find("//");
	
	if (l < str.length() && r < str.length()) str.erase(l, r);
	if (p < str.length()) str.erase(p, str.length());
	
	cout<<"Key Words: "<<endl;
	keywordCheck(&str);
	
	cout<<"Delimeters: "<<endl;
	delimeterCheck(str);
	
	cout<<"Punctuation: "<<endl;
	puncCheck(str);
	
	cout<<"Identifiers: "<<endl;
	identCheck(str);
	
	return 0;
}
