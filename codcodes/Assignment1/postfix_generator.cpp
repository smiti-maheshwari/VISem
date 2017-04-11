#include <bits/stdc++.h>
#include <iostream>
#include <stack>
#include <iostream>
#include <fstream>

#include "generate_tree.cpp"
using namespace std;

// get weight of operators as per precedence
// higher weight given to operators with higher precedence
// for non operators, return 0 
int getWeight(char ch) {
   switch (ch) {
      case '*': return 3;
      case '.': return 2;
      case '|': return 1;
      default : return 0;
   }
}

// convert infix expression to postfix using a stack
void infix2postfix(char infix[], char postfix[], int size) {
   stack<char> s;
   int weight;
   int i = 0;
   int k = 0;
   char ch;
   // iterate over the infix expression   
   while (i < size) {
      ch = infix[i];
      if (ch == '(') {
         // simply push the opening parenthesis
         s.push(ch);
         i++;
         continue;
      }
      if (ch == ')') {
         // if we see a closing parenthesis,
         // pop of all the elements and append it to
         // the postfix expression till we encounter
         // a opening parenthesis
         while (!s.empty() && s.top() != '(') {
            postfix[k++] = s.top();
            s.pop();

         }
         // pop off the opening parenthesis also
         if (!s.empty()) {
            s.pop();
         }
         i++;
         continue;
      }
      weight = getWeight(ch);
      if (weight == 0) {
         // we saw an operand
         // simply append it to postfix expression
         postfix[k++] = ch;

      }
      else {
         // we saw an operator
         if (s.empty()) {
            // simply push the operator onto stack if
            // stack is empty
            s.push(ch);
         }
         else {
            // pop of all the operators from the stack and
            // append it to the postfix expression till we
            // see an operator with a lower precedence that
            // the current operator
            while (!s.empty() && s.top() != '(' &&
                  weight <= getWeight(s.top())) {
               postfix[k++] = s.top();
               s.pop();

            }
            // push the current operator onto stack
            s.push(ch);
         }
      }
      i++;
   }
   // pop of the remaining operators present in the stack
   // and append it to postfix expression 
   while (!s.empty()) {
      postfix[k++] = s.top();
      s.pop();
   }
   postfix[k] = 0; // null terminate the postfix expression 
}

// main
/*void preprocess_string(string inp, string *output) {

   int length = inp.length();

   //std::cout << output;

   for (int i = 0; i < length; i++) {
      //std::cout << output;
      if (!(input[i] == '|' || input[i] == '.' || input[i] == '(' || input[i] == ')')) {
         output += input[i] + ".";
      } else {
         output += input[i];
      }
   }

   //std::cout << output;

   return;
}*/


char * convert_infix_to_postfix(string input, int length) {
   char infix[10000];
   int pos = 0;

   // preproces started=

   //std::cout << output;

   for (int i = 0; i < length - 1; i++) {
      //std::cout << output;

      //std::cout << input[i];

      if (!(input[i] == '|' || input[i] == '.' || input[i] == '(' || input[i] == '#')) {
         if (!(input[i + 1] == '|' || input[i + 1] == '.'  || input[i + 1] == ')' || input[i + 1] == '*')) {
            infix[pos++] = input[i];
            infix[pos++] = '.';
         } else {
            infix[pos++] = input[i];
         }
      } else {
         //std::cout << "yes";
         infix[pos++] = input[i];
      }
   }
   
   infix[pos++] = input[length - 1];
   infix[pos++] = '.';
   infix[pos++] = '#';
   infix[pos++] = '\0';

   std::cout << infix;

   // preprocess finished

   int size = strlen(infix);
   char *postfix = (char *)malloc(size * sizeof(char));
   infix2postfix(infix,postfix,size);

   return postfix;

}

void write_to_file(string filename, string result);


/* 

"$" is considered epsilon

*/
int main() {
   //string infix = "(B*|(A|B))ABB";
   string infix = "((B*|(A|B))ABB)*((A|B)*ABB)*";
   char *postfix;
   //string infix = "(A|B)*ABB";

   postfix = convert_infix_to_postfix(infix, infix.length());
   cout<<"\nInfix Expression :: "<<infix;
   cout<<"\nPostfix Expression :: "<<postfix;
   cout<<"\nLength :: " << strlen(postfix);

   struct node *root = NULL;

   root = create_tree(postfix);

   generate_nullable(root);

   int update_pos = 1;

   update_FIRSTPOS_LASTPOS(root, update_pos);

   cout << "Numbe of leafs :: " << update_pos << endl;

   cout << "\nInner nodes correct :: \n" << check_correct(root) ;

   //cout << root -> val;
   cout<<endl;

   print_first_last(root);

   vector < vector <int> > follow_pos =  create_follow_pos(update_pos, root);

   string result = output_follow_pos(follow_pos);

   cout << result;

   string filename = "follow_pos.txt";

   write_to_file(filename, result);

   result = "";

   output_first_last(root, result);

   filename = "val_nullable_firstpos_lastpos.txt";

   write_to_file(filename, result);

   return 0;
}

void write_to_file(string filename, string result) {
  ofstream myfile;
  myfile.open(filename.c_str());
  myfile << result;
  myfile.close();
}