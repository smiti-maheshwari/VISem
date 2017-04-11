#include <bits/stdc++.h>

using namespace std;

string key[] = { "auto", "break", "case", "char", "const", "continue", "default", "do"
				 , "double", "else", "enum", "extern ", "float", "for", "goto", "if", "int"
				 , "long", "register", "return", "short", "signed", "sizeof", "static"
				 , "struct", "switch", "typedef", "union", "unsigned", "void", "volatile"
				 , "while" , "printf", "long long", "long long int", "scanf"
			   };

vector <pair <int, string> > keyword;
vector <pair <int, string> > delim;
vector <pair <int, string> > integer;
vector <pair <int, string> > punc;
vector <pair <int, string> > id;
int index = 0;

void keywordCheck(string& str) {
	char c;
	string::size_type ptr;
	string::size_type keyLen;
	string::size_type prevMatchInd;
	int n = sizeof (key) / sizeof(key[0]), i;

	prevMatchInd = 0;
	for (i = 0; i < n;) {
		//cout << "check for " << key[i] << " at ind " << prevMatchInd << ":\t";
		keyLen = key[i].length();
		if ((ptr = str.find(key[i], prevMatchInd)) < str.length()) {
			prevMatchInd = ptr + keyLen;

			if (ptr > 1) {
				// check if previous character is not A-Z, a-z, 0-9 or _
				c = str[ptr - 1];
				//cout << "prev char = '" << c << "' ";
				if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
					// do nothing
					//cout << "no match\n";
					continue;
				}
			}

			// check if next character not A-Z, a-z, 0-9 or _
			if (ptr + keyLen < str.length() - 1) {
				c = str[ptr + keyLen];
				//cout << "next char = '" << c << "' ";
				if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
					// do nothing
					//cout << "no match\n";
					continue;
				}
			} else {
				i++;
				prevMatchInd = 0;
			}

			// print the output
			cout<<key[i]<<" ";
			keyword.push_back(make_pair(index++, key[i]));
			string::iterator itL = str.begin() + ptr;
			string::iterator itR = str.begin() + ptr + keyLen;
			str.erase(itL, itR);
			//cout << "match found\n";
		} else {
			i++;
			prevMatchInd = 0;
			//cout << "no match\n";
		}
	}

	cout<<endl;
}

void delimeterCheck(string& str) {
	//char delim[] = { '{', '}', '(', ')', '[', ']', ',' , ';' , ''' };
	string delim = "{}()[],;'\"";
	int m = delim.length();
	vector <int> ind;
	//cout<<n<<" "<<m<<endl;
	for (int i = 0; i < str.length(); i++) {
		//string x="";
		//x+=str[i];
		//cout<<"hi"<<endl;
		for(int j = 0; j < m; j++) {
			if (str[i]==delim[j]) {
				cout<<str[i]<<" ";
				string tmp = "" + str[i];
				keyword.push_back(make_pair(index++, tmp));
				string::iterator it = str.begin() + i;
				str.erase(it);
				i--;
				break;
			}
		}
	}

	cout<<endl;
}

void puncCheck(string& str) {
	//string punc[] = { " + ", " - ", " * ", " / ", " %d", " ^", " &" };
	string punc = "+-*%&^/<>!=";
	int n = str.length();
	int m = punc.length();
	//cout<<n<<" "<<m<<endl;

	for(int i=0; i<str.length(); i++) {
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
					string tmp = "" + str[i];
					keyword.push_back(make_pair(index++, tmp));
					string::iterator it = str.begin() + i;
					str.erase(it);
					i--;
				}
			}
		}
	}
	cout<<endl;
}

//Test cases
//	for (long intint = 0; intint < "n"; intint++) printf("%d\n", intint);
// for (int aint = 0; inta < intint; int_int++);
//	printf("%d", (int)x);
//	scanf(" %d\n", &an__132h);
// printf("int"); // long long int " \" " /* */
// /* printf("%d", (int) 5); */ cout << "hello";
// a 2.29, a7689 890b22 ,0.343, -3345.44, ,908f, ,890,
// printf(" /* int long \" a \" %d %f */", " \" ", "\"", "", " ", (int) a);

void numCheck(string& str) {
	char prev = ';';
	for (int i = 0; i < str.length();) {
		if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_') {
			while ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_' || (str[i] >= '0' && str[i] <= '9')) {
				prev = str[i];
				i++;
			}
			prev = str[i];
			i++;
			continue;
		}

		if (!((prev >= 'a' && prev <= 'z') || (prev >= 'A' && prev <= 'Z') || prev == '_') && str[i] >= '0' && str[i] <= '9') {
			string::iterator itL = str.begin() + i;
			string tmp = "";
			tmp += str[i++];
			int dotCount = 0;

			while (i < str.length() && (str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
				if (str[i] == '.') {
					dotCount++;
				}
				if (dotCount > 1) {
					break;
				}
				tmp += str[i];
				i++;
			}

			if (i < str.length() && ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_')) {
				prev = str[i];
				continue;
			}

			if (dotCount <= 1) {
				string::iterator itR = str.begin() + i;
				int delta = itR - itL;
				i = i - delta + 1;
				str.erase(itL, itR);
				integer.push_back(make_pair(index++, tmp));
				cout << tmp << " ";
			}
		} else {
			prev = str[i++];
		}
	}
	cout << endl;
}

void identCheck(string& str) {
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
				id.push_back(make_pair(index++, x));
			}
		}
	}

	cout<<endl;
}

void removeComments(string& str) {
	int l = str.find("/*");
	int r = str.find("*/");
	int p = str.find("//");

	if (l < str.length() && r < str.length()) str.erase(l, r);
	if (p < str.length()) str.erase(p, str.length());
}

string removeAllComments(string prgm) {
	int n = prgm.length();
	string res;

	// Flags to indicate that single line and multpile line comments
	// have started or not.
	bool s_cmt = false;
	bool m_cmt = false;


	// Traverse the given program
	for (int i=0; i<n; i++) {
		// If single line comment flag is on, then check for end of it
		if (s_cmt == true && prgm[i] == '\n')
			s_cmt = false;

		// If multiple line comment is on, then check for end of it
		else if  (m_cmt == true && prgm[i] == '*' && prgm[i+1] == '/')
			m_cmt = false,  i++;

		// If this character is in a comment, ignore it
		else if (s_cmt || m_cmt)
			continue;

		// Check for beginning of comments and set the approproate flags
		else if (prgm[i] == '/' && prgm[i+1] == '/')
			s_cmt = true, i++;
		else if (prgm[i] == '/' && prgm[i+1] == '*')
			m_cmt = true,  i++;

		// If current character is a non-comment character, append it to res
		else  res += prgm[i];
	}
	return res;
}

void removeStrings(string& str, int pos) {
	//cout << "\n\n==================================\n";
	//cout << "removeString(str, " << pos << ")\n";
	if (pos < str.length()) {
		string::size_type l = str.find('"', pos);

		if (l < string::npos) {
			// if a double quote is present
			//cout << "l = " << l << endl;
			string::size_type r = str.find('"', l + 1);
			string::size_type prevInd;
			char c;

			if (r < str.length()) {
				// if a pair of double quotes is present
				prevInd = r;
				c = str[r - 1];
				//cout << "r = " << r << ", prev char = " << c << endl;

				// make sure that r stores index of last double quote
				while (c == '\\' && r < string::npos) {
					prevInd = r;
					r = str.find('"', r + 1);
					c = str[r - 1];
					//cout << "r = " << r << ", prev char = " << c << endl;
				}
				if (r == string::npos) {
					r = prevInd;
				}
				//cout << "final r = " << r << ", prev char = " << str[r - 1] << endl;

				// erase entire string
				if (r - l > 1) {
					string::iterator itL = str.begin() + l + 1;
					string::iterator itR = str.begin() + r;
					//str.erase(l + 1, r - 1);
					str.erase(itL, itR);
					//cout << "\nerased string:\t" << str;
				}

				// make recursive call to remove remaining strings (if any)
				if (r < string::npos) {
					int delta = (r - 1) - (l + 1) + 1;
					removeStrings(str, r - delta + 1);
				}
			}
		}
	}
}

int main() {
	string str, temp;
	int n, i;


	cin>>n;
	char aa;
	cin>>aa;


	str = "";
	for (i = 0; i < n; i++) {
		if (i == 0) str += aa;

		getline(cin, temp);
		str += temp;
		str += '\n';
	}
	cout<<"input programme is : \n"<<str<<endl;

	str = removeAllComments(str);
	cout << "\nafter removing comments: '" << str << "'";

	removeStrings(str, 0);
	cout << "\nafter removing strings:  '" << str << "'";
	//removeComments(str);

	cout<<"\nKey Words: "<<endl;
	keywordCheck(str);
	cout << "\nafter removing keyword:  '" << str << "'";

	cout<<"\nDelimeters: "<<endl;
	delimeterCheck(str);
	cout << "\nafter removing delim:   '" << str << "'";

	cout<<"\nPunctuation: "<<endl;
	puncCheck(str);
	cout << "\nafter removing punc:    '" << str << "'";

	cout << "\nNumbers: " << endl;
	numCheck(str);
	cout << "\nafter removing numbers:  '" << str << "'";

	cout<<"\nIdentifiers: "<<endl;
	identCheck(str);


	return 0;
}
