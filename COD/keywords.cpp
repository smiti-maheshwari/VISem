#include <bits/stdc++.h>
using namespace std;
struct trie{
	trie *child[26];
	bool isleaf;
};
trie * newnode()
{
	trie * node = (struct trie *)malloc(sizeof(struct trie));
	for (int i=0;i<26;i++){
		node->child[i] = NULL;
	}
	return node;
}
void insert(trie* root, char key)
{
	trie *crawl = root;
	int index;
	int len = sizeof(key)/sizeof(key[0]);
	for (int i=0;i<len;i++){
		index = key[i] - 'a';
		if (!crawl->child[index])
			crawl->child[index] = newnode();
		crawl = crawl->child[index];
	}
	crawl->isleaf = true;
}

int main()
{
	char keys[][10] = {"if", "else", "while", "for", "do"};
	trie *root = newnode();
	int len = sizeof(keys)/sizeof(keys[0]);
	for (int i=0;i<len;i++){
		insert(root, keys[i]);
	}
}