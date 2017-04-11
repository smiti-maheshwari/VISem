// set_union example
#include <iostream>     // std::cout
#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector

#define pb push_back

using namespace std;

void send(int &p) {
	cout << p++ << endl;

	cout << p++ << endl;

	return;


}

void send2(vector< vector <int> > &arr) {

	arr[8].pb(26);
	arr[3].pb(26);
	arr[6].pb(26);
	arr[8].pb(26);
}

vector <vector <int> > send3 () {
	int p = 26, i, j;

	send(p);

	vector< vector <int> > arr(10);

	arr[0].pb(10);

	arr[1].pb(10);

	arr[1].pb(12);

	send2(arr);

	

	cout << p << endl;

	return arr;
}


int main() {

	vector < vector <int> > arr = send3();

	for (int i = 0; i < arr.size(); i++) {
		cout << i << " :: ";
		for (int j = 0; j < arr[i].size(); j++) {
			cout << arr[i][j] << " " << " ";
		}

		cout << endl;
	}
}