#include <bits/stdc++.h>

using namespace std;

int main()
{
	string tmp = "cat";
	char tab2[1024];
	strcpy(tab2, tmp.c_str());

	cout << tab2;

	char arr[ ] = "This is a test";

	string str(arr);

	string str1 = string(arr);

	cout << str1 << endl;

	cout << str << endl;
}