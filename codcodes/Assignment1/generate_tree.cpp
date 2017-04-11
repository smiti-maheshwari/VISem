#include <bits/stdc++.h>
#define pb push_back
using namespace std;

int traversed_position;

struct node {
	struct node *left;
	struct node *right;
	char val;
	bool leaf;
	int correspondance;
	vector <int> firstpos;
	vector <int> lastpos;
	bool nullable;
	//here variables are to be added for FIRSTPOS, LASTPOS
};

struct node * new_node(char c, bool l) {
	struct node * NN = new struct node();

	NN -> left = NULL;
	NN -> right = NULL;

	NN -> val = c;
	NN -> leaf = l;

	NN -> nullable = false;
	NN -> correspondance = -1;

	return NN;
}


struct node * create_root(char *postfix) {	
	char last_elem = postfix[strlen(postfix) - 1];

	return new_node(last_elem, false);
}


// returns label if the node is LEAF
bool class_of_variable(char current_location) {
	if (!(current_location == '|' || current_location == '.' || current_location == '*'))
		return true;

	return false;
}


void recursive_build(char *postfix, struct node * parent) {

	char current_location = postfix[traversed_position];

	//cout << parent -> val << endl;
	
	if (parent -> leaf || traversed_position <= 0)
		return;


	//venturing to the right
	char position_right = postfix[--traversed_position];

	bool label = class_of_variable(position_right);

	struct node * NN = new_node(position_right, label);

	parent -> right = NN;

	recursive_build(postfix, NN);

	//venturing to the left in NON LEAF.. if it is NOT KLEEN STAR
	
	if (current_location != '*') {
		char position_left = postfix[--traversed_position];

		label = class_of_variable(position_left);

		NN = new_node(position_left, label);

		parent -> left = NN;

		recursive_build(postfix, NN);

	}

	return;

}

struct node * create_tree(char *postfix) {

	struct node *root = create_root(postfix);

	traversed_position = strlen(postfix) - 1;

	recursive_build(postfix, root);

	return root;

}

	

bool check_correct(struct node *root) {
	cout << root -> val;
	cout << " " << root -> nullable << endl;

	if (!root -> leaf) {
		if (root -> val == '*') {
			if (root -> right == NULL)
				return false;
			else
				check_correct(root -> right);
		} else if (root -> right ==  NULL || root -> left == NULL)
			return false;
		else {
			return check_correct(root -> right) && check_correct(root -> left);
		}
	}

	return true;

}

//ASSUMPTION - for our program $ is used to represent epsilon
void generate_nullable(struct node * root) {

	if(root -> leaf && root -> val == '$') {
		root -> nullable = true;
		return;
	}

	if (root -> val == '*') {
		root -> nullable = true;

		generate_nullable(root -> right);

		return;
	}

	if (root -> val == '|') {
		generate_nullable(root -> right);

		generate_nullable(root -> left);

		root -> nullable = root -> left -> nullable || root -> right -> nullable;
		
		return;
	}

	if (root -> val == '.') {
		generate_nullable(root -> right);

		generate_nullable(root -> left);

		root -> nullable = root -> left -> nullable && root -> right -> nullable;
	}

	return;
}


vector <int> union_vector(vector <int> v1, vector <int> v2) {
	set<int> all;

	all.insert(v1.begin(), v1.end());
	all.insert(v2.begin(), v2.end());

	vector<int> result(all.begin(), all.end());

	return result;
}


void update_FIRSTPOS_LASTPOS(struct node *root, int &update_pos) {
	
	if(root -> leaf) {
		if (root -> val == '$') {
		} else {
			cout << root -> val << " " << update_pos << endl;

			root -> correspondance = update_pos;
			
			(root -> firstpos).pb(update_pos);
			(root -> lastpos).pb(update_pos++);
		}
		return;
	}

	if (root -> val == '*') {

		update_FIRSTPOS_LASTPOS(root -> right, update_pos);

		root -> firstpos = root -> right -> firstpos;

		root -> lastpos = root -> right -> lastpos;

		return;
	}

	if (root -> val == '|') {
		update_FIRSTPOS_LASTPOS(root -> left, update_pos);

		update_FIRSTPOS_LASTPOS(root -> right, update_pos);

		root -> firstpos = union_vector(root -> left -> firstpos, root -> right -> firstpos);

		root -> lastpos = union_vector(root -> left -> lastpos, root -> right -> lastpos);

		return;
	}

	if (root -> val == '.') {
		update_FIRSTPOS_LASTPOS(root -> left, update_pos);

		update_FIRSTPOS_LASTPOS(root -> right, update_pos);


		if (root -> left -> nullable)
			root -> firstpos = union_vector(root -> left -> firstpos, root -> right -> firstpos);
		else 
			root -> firstpos = root -> left -> firstpos;

		if (root -> right -> nullable)
			root -> lastpos = union_vector(root -> left -> lastpos, root -> right -> lastpos);
		else 
			root -> lastpos = root -> right -> lastpos;
		
		return;
	}
	return;	
}



/* lastest addition follow pos */

bool update_follow_recursive(struct node *root, vector< vector <int> > &follow_pos) {
	cout << root -> val;
	cout << " " << root -> nullable << endl;

	if (!root -> leaf) {
		cout << "yes";
		if (root -> val == '*') {
			int i =0, j = 0;

			for(i = 0; i < (root -> lastpos).size(); i++) {
				for (j = 0; j < (root -> firstpos).size(); j++) {
					cout << root -> firstpos[j];
					follow_pos[root -> lastpos[i]].pb(root -> firstpos[j]);
				}
			}


		} else if (root -> val == '.') {
			int i =0, j = 0;

			for(i = 0; i < (root -> left -> lastpos).size(); i++) {
				for (j = 0; j < (root -> right -> firstpos).size(); j++) {
					follow_pos[root -> left -> lastpos[i]].pb(root -> right -> firstpos[j]);
				}
			}

			
		}

		if (root -> val == '*') {
			update_follow_recursive(root -> right, follow_pos);
		} else {
			update_follow_recursive(root -> left, follow_pos);
			update_follow_recursive(root -> right, follow_pos);
		}
	}



	return true;

}

void remove_duplicates(vector < vector <int> > &follow_pos);

vector <int> convert_to_set_to_list(vector <int> v1);

vector < vector <int> >  create_follow_pos(int &no_of_leafs_1, struct node *root) {

	vector < vector <int> > follow_pos(no_of_leafs_1);
	int i;
	int last_pos = no_of_leafs_1 - 1;

	follow_pos[last_pos].pb(-1);

	update_follow_recursive(root, follow_pos);

	remove_duplicates(follow_pos);

	return follow_pos;
}


void remove_duplicates(vector < vector <int> > &follow_pos) {
	int i, j;

	for (i = 0; i < follow_pos.size(); i++) {
		follow_pos[i] = convert_to_set_to_list(follow_pos[i]);
	}
}


vector <int> convert_to_set_to_list(vector <int> v1) {
	set<int> all;

	all.insert(v1.begin(), v1.end());

	vector<int> result(all.begin(), all.end());

	return result;
}


/*to_string( int value );*/
/* stoi (strng) -> string to  integer*/

/* OUTPUT FOLLOW_POS */

string output_follow_pos(vector < vector <int> > &arr ) {

	string result = "";

	for (int i = 0; i < arr.size(); i++) {
		result += to_string(i) + " :: ";
		for (int j = 0; j < arr[i].size(); j++) {
			result += to_string(arr[i][j])  + " ,";
		}

		result += '\n';
	}

	return result;
}

void print_vec(vector <int> arr) {

	cout << "\t\t";

	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i] << " ,";
	}
}


void print_first_last(struct node *root) {
	cout << root -> val;
	cout << "\t" << root -> nullable;
	print_vec(root -> firstpos);
	cout << "\t\t";
	print_vec(root -> lastpos);
	cout << endl;

	if (!root -> leaf) {
		

		if (root -> val == '*') {
			print_first_last(root -> right);
		} else {
			print_first_last(root -> right);
			print_first_last(root -> left);
		}
	}


}

/*OUTPUT FIRST_POS LAST_POS NULLABLE*/

void output_vec(vector <int> arr, string &result) {

	result += "\t::\t";

	for (int i = 0; i < arr.size(); i++) {
		result += to_string(arr[i]) + " ,";
	}
}

void output_first_last(struct node *root, string &result) {
	result += root -> val;
	result += "\t::\t" + to_string(root -> correspondance);
	result += "\t::\t" + to_string(root -> nullable);
	output_vec(root -> firstpos, result);
	//result += "\t::\t";
	output_vec(root -> lastpos, result);
	result += "\n";

	if (!root -> leaf) {
		

		if (root -> val == '*') {
			output_first_last(root -> right, result);
		} else {
			output_first_last(root -> right, result);
			output_first_last(root -> left, result);
		}
	}


}

void generate_output_first_last(struct node *root, string &result) {
	// first variable is always root

	output_first_last(root, result);
}