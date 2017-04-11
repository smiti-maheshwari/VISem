void output_vec(vector <int> arr, string &result) {

	result += "\t::\t";

	for (int i = 0; i < arr.size(); i++) {
		result += arr[i] + " ";
	}
}

void output_first_last(struct node *root, string &root) {
	result += root -> val;
	result += "\t::\t" + to_string(root -> nullable);
	output_vec(root -> firstpos);
	//result += "\t::\t";
	output_vec(root -> lastpos);
	result += endl;

	if (!root -> leaf) {
		

		if (root -> val == '*') {
			print_first_last(root -> right);
		} else {
			print_first_last(root -> right);
			print_first_last(root -> left);
		}
	}


}

void generate_output_first_last(struct node *root, string &result) {
	// first variable is always root

	output_first_last(root, result);
}