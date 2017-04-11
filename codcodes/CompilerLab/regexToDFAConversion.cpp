#include <bits/stdc++.h>

using namespace std;

struct node
{
	char val;
	struct node *left, *right;
	vector <int> firstPos;
	vector <int> lastPos;
	bool null;
};

struct node *createLeaf(char key, int ind, bool ch)
{
	struct node *newNode;
	
	newNode = new node;

	newNode->val = key;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->null = ch;

	if (ind > 0) {
		newNode->firstPos.push_back(ind);
		newNode->lastPos.push_back(ind);
	}

	return newNode;
}

struct node *createNode(char key, struct node *left, struct node *right)
{
	struct node *newNode;
	
	if (key == '|') newNode = createLeaf(key, 0, left->null || right->null);
	else if (key == '.') newNode = createLeaf(key, 0, left->null && right->null);
	else if (key == '*') newNode = createLeaf(key, 0, true);
	
	newNode->left = left;
	newNode->right = right;

	return newNode;
}

bool isUnique(vector <char>& numToSym, char c)
{
	for (int i = 0; i < numToSym.size(); i++) {
		if (c == numToSym[i]) {
			return false;
		}
	}
	return true;
}

struct node *createTree(string post, vector <char> &numToSym, map <char, int> &symToNum, int *leafCount, vector <char> &symList)
{
	stack <struct node * > s;
	struct node *root, *newNode, *left, *right;
	int len = post.length();
	int i, ind = 1;
	char c;
	
	for (i = 0; i < len; i++) {
		c = post[i];
		if (c == '.' || c == '|') {
			right = s.top();
			s.pop();
			left = s.top();
			s.pop();
			newNode = createNode(c, left, right);
			s.push(newNode);
		} else if (c == '*') {
			left = s.top();
			s.pop();
			newNode = createNode(c, left, NULL);
			s.push(newNode);
		} else {
			if (c == 'E') newNode = createLeaf(c, 0, true);
			else {
				newNode = createLeaf(c, ind++, false);
				if (c != '#') {
					numToSym.push_back(c);
					symToNum[c] = (*leafCount);
					(*leafCount)++;
					
					if (isUnique(symList, c) == true) symList.push_back(c);
				}
			}
			s.push(newNode);
		}
	}
	
	return s.top();
}

void inOrder(struct node *root)
{
	if (root == NULL) return;
	else {	
		inOrder(root->left);
		cout << root->val << " ";
		inOrder(root->right);
	}
}

int rank(char a)
{
	if (a == '|') return 1;
	if (a == '.') return 2;
	if (a == '*') return 3;
	return -1;
}

string postfix(string str, int len)
{
	vector <char> v;
	int i;
	stack <char> s;
	
	for (i = 0; i < len; i++) {
		if ((str[i] >= 'a' && str[i] <= 'z') || str[i] == '#' || (str[i] >= '0' && str[i] <= '9') || str[i] == 'E') {
			v.push_back(str[i]);
		} else if (str[i] == '(') {
			s.push(str[i]);
		} else if (str[i] == ')') {
			while (s.empty() == 0 && s.top() != '(') {
				v.push_back(s.top());
				s.pop();
			}
			s.pop();
		} else {
			while (s.empty() == 0 && rank(str[i]) <= rank(s.top())) {
				v.push_back(s.top());
				s.pop();
			}
			s.push(str[i]);
		}
	}
	
	while (s.empty() == 0) {
		v.push_back(s.top());
		s.pop();
	}
	
	string post = "";
	for (i = 0; i < v.size(); i++) post += v[i];
	
	return post;
}

void makeUnion(vector <int> v1, vector <int> v2, vector <int> &v)
{
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	
	int i = 0, j = 0;
	
	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j]) {
			v.push_back(v1[i]);
			i++;
		} else if (v1[i] > v2[j]) {
			v.push_back(v2[j]);
			j++;
		} else {
			v.push_back(v1[i]);
			i++;
			j++;
		}
	}
	
	while (i < v1.size()) {
		v.push_back(v1[i]);
		i++;
	}
	
	while (j < v2.size()) {
		v.push_back(v2[j]);
		j++;
	}
}

void makeCopy(vector <int> v1, vector <int> &v2)
{
	for (int i = 0; i < v1.size(); i++) v2.push_back(v1[i]);
}

void findFirstAndLastPos(struct node *root)
{
	if (root != NULL && (root->left != NULL || root->right != NULL)) {
		findFirstAndLastPos(root->left);
		findFirstAndLastPos(root->right);
		
		if (root->val == '|') {
			makeUnion(root->left->firstPos, root->right->firstPos, root->firstPos);
			makeUnion(root->left->lastPos, root->right->lastPos, root->lastPos);
		} else if (root->val == '.') {
			if (root->left->null == true) {
				makeUnion(root->left->firstPos, root->right->firstPos, root->firstPos);
			} else makeCopy(root->left->firstPos, root->firstPos);
			if (root->right->null == true) {
				makeUnion(root->left->lastPos, root->right->lastPos, root->lastPos);
			} else makeCopy(root->right->lastPos, root->lastPos);
		} else if (root->val == '*') {
			makeCopy(root->left->firstPos, root->firstPos);
			makeCopy(root->left->lastPos, root->lastPos);
		}
	}
}

void printVector(vector <int> &v)
{
	for (int i = 0; i < v.size(); i++) cout<<v[i];
	cout<<"\t";
}

void printFirstAndLastPos(struct node *root)
{
	if (root == NULL) return;
	
	printFirstAndLastPos(root->left);
	cout<<root->val<<"\t";
	printVector(root->firstPos);
	printVector(root->lastPos);
	cout<<endl;
	printFirstAndLastPos(root->right);
}

void makeFollow(map <int, vector <int> > &m, vector <int> v, int ind)
{
	vector <int> temp;
	int i, j;
	
	for (i = 0; i < m[ind].size(); i++) temp.push_back(m[ind][i]);
	m[ind].clear();
	
	sort(v.begin(), v.end());
	sort(temp.begin(), temp.end());
	
	i = 0, j = 0;
	
	while (i < v.size() && j < temp.size()) {
		if (v[i] < temp[j]) {
			m[ind].push_back(v[i]);
			i++;
		} else if (v[i] > temp[j]) {
			m[ind].push_back(temp[j]);
			j++;
		} else {
			m[ind].push_back(v[i]);
			i++;
			j++;
		}
	}
	
	while (i < v.size()) {
		m[ind].push_back(v[i]);
		i++;
	}
	
	while (j < temp.size()) {
		m[ind].push_back(temp[j]);
		j++;
	}
}

void computeFollowPos(struct node *root, map <int, vector <int> > &m)
{
	if (root != NULL) {
		computeFollowPos(root->left, m);
		if (root->val == '.' && root->left != NULL && root->right != NULL) {
			int size = root->left->lastPos.size();
			for (int i = 0; i < size; i++) {
				makeFollow(m, root->right->firstPos, root->left->lastPos[i]);
			}
		} else if (root->val == '*') {
			int size = root->lastPos.size();
			for (int i = 0; i < size; i++) {
				makeFollow(m, root->firstPos, root->lastPos[i]);
			}
		} else if (root->val == '#') {
			m[root->firstPos[0]].push_back(-1);
		}
		computeFollowPos(root->right, m);
	}
}

void printFollowPos(map <int, vector <int> > m)
{
	for (int i = 1; i <= m.size(); i++) {
		cout<<i<<"\t";
		for (int j = 0; j < m[i].size(); j++) cout<<m[i][j]<<" ";
		cout<<endl;
	}
}

void mapLeafNodes(struct node *root, map <int, char> &m)
{
	if (root == NULL) return;
	
	mapLeafNodes(root->left, m);
	if (root->left == NULL && root->right == NULL) m[root->firstPos[0]] = root->val;
	mapLeafNodes(root->right, m);
}

vector < vector <int> > vec;

void makeUnion(vector <int> &v1, vector <int> v2)
{
	vector <int> temp;
	temp = v1;
	v1.clear();
	
	sort(temp.begin(), temp.end());
	sort(v2.begin(), v2.end());
	
	int i = 0, j = 0;
	
	while (i < temp.size() && j < v2.size()) {
		if (temp[i] < v2[j]) {
			v1.push_back(temp[i]);
			i++;
		} else {
			v1.push_back(v2[j]);
			if (temp[i] == v2[j]) i++;
			j++;
		}
	}
	
	while (i < temp.size()) {
		v1.push_back(temp[i]);
		i++;
	}
	
	while (j < v2.size()) {
		v1.push_back(v2[j]);
		j++;
	}
}

void computeDFA(struct node *root, map <int, vector <int> > &follow, map <int, char> m, vector <char> symList)
{
	int i, ind;
	char c;
 	vector <int> s0;
 	s0 = root->firstPos;
 	map <vector <int>, int > Dstates;
 	map <int, char>::iterator it;
 	vector <int> temp;
 	int cx = 0;
 	int mat[100][100];
 	
 	Dstates[s0] = cx++;
 	queue <vector <int> > q;
 	vec.push_back(s0);
 	q.push(s0);
 	bool ch = false;
	
	while (q.empty() != 1) {
		s0 = q.front();
		q.pop();
		
		for (i = 0; i < symList.size(); i++) {
			c = symList[i];
			temp.clear();
			
			for (it = m.begin(); it != m.end(); it++) {
				if (it->second == c) {
					ind = it->first;
					ch = false;
					for (int j = 0; j < s0.size(); j++) {
						if (ind == s0[j]) {
							ch = true;
						}
					}
					if (ch == true) makeUnion(temp, follow[ind]);
				}
			}
			if (Dstates.find(temp) == Dstates.end()) {
				vec.push_back(temp);
				q.push(temp);
				Dstates[temp] = cx++;
			}
			
			if (temp.size() != 0) mat[Dstates[s0]][Dstates[temp]] = c;
		}
	}
	
	cout<<"DFA is "<<endl<<endl;
	for (i = 0; i < cx; i++) {
		for (int j = 0; j < cx; j++) cout<<mat[i][j]<<" ";
		cout<<endl;
	}
}

int main()
{
	string post = "", in;
	struct node *root;
	vector <char> numToSym;
	vector <char> symList;
	map <char, int> symToNum;
	int leafCount = 0;
	     
	cin>>in;
	in += '.';
	in += '#';
	post = postfix(in, in.length());
	cout<<post<<endl;
	
	root = createTree(post, numToSym, symToNum, &leafCount, symList);

	findFirstAndLastPos(root);
	cout<<"symbol\tf pos\tl pos\n"<<endl;
	printFirstAndLastPos(root);
	cout<<endl;
	
	map <int, vector <int> > followPos;
	
	computeFollowPos(root, followPos);
	cout<<"\tfollowPos"<<endl;
	printFollowPos(followPos);
	
	map <int, char> leafMap;
	
	mapLeafNodes(root, leafMap);	
	computeDFA(root, followPos, leafMap, symList);
	
	return 0;
}
