#include <bits/stdc++.h>
using namespace std;

int main()
{
	string keywords[] = {"auto", "else","long" , "switch","break","enum","register","typedef", "case", "extern", "return","union","char","float","short","unsigned",
						"const","for","signed","void","continue","goto","sizeof","volatile","default","if","static","while","do","int","struct","_Packed","double"
						};
	sort(keywords, keywords+32);
	int i;
	// for (i=0;i<32;i++)
	// 	cout<<", "<<keywords[i];
	// cout<<"\n";
	string input;
	cin >> input;
	for (i=0;i<input.length();i++){
		cout << input[i];
	}
	return 0;

}