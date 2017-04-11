#include <bits/stdc++.h>

using namespace std;

struct node {
  char symbol;
  struct node *left;
  struct node *right;
  bool nullable;
  int position;
  set<int> firstpos;
  set<int> lastpos;
};

struct transition {
  int istate;
  char symbol;
  int fstate;
};

char postfixExpr[100];
char expr[100];
int followpos[100][100];
set<char> symbolSet;
struct node *allNodes[100];//all node from bottom up manner with count node Count
static int nodeCount = 0;
map<int,char> mp;//node correspond to which symbol
map<int,set<int> > states;// collect set as integer

struct transition transTable[100];

static int transCount = 0;
/*inserting concat . between symbol of regular expression*/

void preprocess() {
  char intermediate[100];
  int s = 0,i;
  for(i = 0; i < strlen(expr)-1; i++) {
    if('a' <= expr[i] && expr[i] <= 'z') {
      intermediate[s++] = expr[i];
      if(('a' <= expr[i+1] && expr[i+1] <= 'z') || expr[i+1] == '(') {
        intermediate[s++] = '.';
      }
    } else if(expr[i] == '(' || expr[i] == '|') {
      intermediate[s++] = expr[i]; 
    } else if(expr[i] == ')' || expr[i] == '*') {
      intermediate[s++] = expr[i];
      if(expr[i] == '*')
        intermediate[s++] = '$';
      if(('a' <= expr[i+1] && expr[i+1] <= 'z') || expr[i+1] == '(') {
        intermediate[s++] = '.';
      }
    }
  }
  intermediate[s++] = expr[i];
  if(expr[i] == '*')
        intermediate[s++] = '$';
  intermediate[s++] = '.';
  intermediate[s++] = '#';
  intermediate[s++] = '\0';
  strcpy(expr,intermediate);
  return;
}

int getWeight(char ch) {
   switch (ch) {
      case '*': return 3;
      case '.': return 2;
      case '|': return 1;
      default : return 0;
   }
}

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
            while (!s.empty() && s.top() != '(' && weight <= getWeight(s.top())) {
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
   postfix[k] = '\0'; // null terminate the postfix expression 
}

int countSymbol() {
  int count = 0;
  for(int i = 0; i < strlen(expr); i++) {
    if((expr[i]  >= 'a' && expr[i] <= 'z') || expr[i] == '#') {
      count++;
      if(expr[i] != '#')symbolSet.insert(expr[i]);
    }
  }
  return count;
}

struct node * createTree() {
  struct node *head, *root;
  stack<struct node *> stk;
  int j = 0;
  for(int i = 0; i < strlen(postfixExpr); i++) {
      if((postfixExpr[i]  >= 'a' && postfixExpr[i] <= 'z') || postfixExpr[i] == '#' || postfixExpr[i] == '$') {
        if(postfixExpr[i] == '$') {
          head = new node;
          head->symbol = postfixExpr[i];
        } else {
          head= new node;
          head->symbol = postfixExpr[i];
          head->position = j+1;
          head->nullable = false;
          head->right = NULL;
          head->left = NULL;
          head->firstpos.insert(j+1);
          head->lastpos.insert(j+1);
          j++;
        }
        stk.push(head);
      } else if(postfixExpr[i] == '|' ) { 
        root = new node;
        root->symbol = postfixExpr[i];
        root->position = -1;
        root->right = stk.top();
        stk.pop();
        root->left = stk.top();
        stk.pop();
        if(root->right != NULL  && root->right->nullable) {
          root->nullable = true;  
        } else if(root->left != NULL && root->left->nullable){
          root->nullable = true;
        }
        if(root->left != NULL) {
          root->firstpos.insert(root->left->firstpos.begin(),root->left->firstpos.end());
          root->lastpos.insert(root->left->lastpos.begin(),root->left->lastpos.end());
        }
        if(root->right != NULL) {
          root->firstpos.insert(root->right->firstpos.begin(),root->right->firstpos.end());
          root->lastpos.insert(root->right->lastpos.begin(),root->right->lastpos.end());
        }
        stk.push(root);
        //root->nullable = root->left->nullable || root->right->nullable;
        //printf("%d or done\n", i);
      } else if(postfixExpr[i] == '.') {
        root = new node;
        root->symbol = postfixExpr[i];
        root->position = -1;
        root->right = stk.top();
        stk.pop();
        root->left = stk.top();
        stk.pop();
        if(root->right != NULL && root->left != NULL && root->right->nullable && root->left->nullable) {
          root->nullable = true;  
        } else {
          root->nullable = false;
        }
        
        if(root->left->nullable == true) {
          if(root->left != NULL) {
            root->firstpos.insert(root->left->firstpos.begin(),root->left->firstpos.end());
          }
          if(root->right != NULL) {
            root->firstpos.insert(root->right->firstpos.begin(),root->right->firstpos.end());
          }
        } else {
          if(root->left != NULL) {
            root->firstpos.insert(root->left->firstpos.begin(),root->left->firstpos.end());
           }
        }
        if(root->right->nullable == true) {
          if(root->left != NULL) {
            root->lastpos.insert(root->left->lastpos.begin(),root->left->lastpos.end());
          }
          if(root->right != NULL) {
            root->lastpos.insert(root->right->lastpos.begin(),root->right->lastpos.end());
          }
        } else {
          if(root->right != NULL) {
            root->lastpos.insert(root->right->lastpos.begin(),root->right->lastpos.end());
          }
        }
        stk.push(root);
        //printf("%d and done\n", i);
      } else if(postfixExpr[i] == '*') {
        root = new node;
        root->symbol = postfixExpr[i];
        root->position = -1;
        root->nullable = true;
        root->right = stk.top();
        stk.pop();
        root->left = stk.top();
        stk.pop();
        root->right = NULL;
        root->firstpos.insert(root->left->firstpos.begin(),root->left->firstpos.end());
        root->lastpos.insert(root->left->lastpos.begin(),root->left->lastpos.end());
        stk.push(root);
        //printf("%d * done\n", i);
      }
  }
  root = stk.top();
  stk.pop();
  if(stk.empty())
    cout << "hhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n";
  return root;
}
/*
   start with root node
   for each i in lastpos(root->left) add firstpos(root->right) to followpos(i) if . node
   for each i in lastpos(root) add firstpos(root) to followpos(root) if * node
*/
void calculateFollowPos(struct node *root) {
  if(root == NULL)
    return;
  set<int>::iterator it, it2;
  if(root->symbol == '.') {
    for(it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++) {
      for(it2 = root->right->firstpos.begin(); it2 != root->right->firstpos.end(); it2++) {
        followpos[*it][*it2] = 1;
      }
    }
  } else if(root->symbol == '*') {
    for(it = root->left->lastpos.begin(); it != root->left->lastpos.end(); it++) {
      for(it2 = root->left->firstpos.begin(); it2 != root->left->firstpos.end(); it2++) {
        followpos[*it][*it2] = 1;
      }
    }
  }
  calculateFollowPos(root->left);//recursively calculate for both nodes
  calculateFollowPos(root->right);
  allNodes[nodeCount++] = root;
  if(root->position != -1) 
    mp[root->position] = root->symbol;// pair (i, symbol) , for ease in counting what are the node corresponding to this symbol
}

void makeDFA(struct node *root, int count) {
  int t = 0;
  states[t] = root->firstpos;//initial state 
  map<int,int> unamrked;
  map<int,int>::iterator itm;
  unamrked[t] = 1;
  while(!(unamrked.empty())) {
    itm = unamrked.begin();
    for(set<char>::iterator it = symbolSet.begin(); it != symbolSet.end(); it++) {
      set<int> temp, tmp2;
      tmp2 = states[itm->first];
      for(set<int>::iterator it2 = tmp2.begin(); it2 != tmp2.end(); it2++) {
        if(mp[*it2] == *it) {
          for(int i = 1; i <= count; i++) {
            if(followpos[*it2][i]) 
              temp.insert(i);
          }
        }
      }
      bool flag = true;
      for(map<int,set<int> >::iterator it2 = states.begin(); it2 != states.end(); it2++) {
        if(it2->second == temp) {
          transTable[transCount].istate = itm->first;
          transTable[transCount].symbol = *it;
          transTable[transCount].fstate = it2->first;
          transCount++;
          flag = false;
          break;
        }    
      }

      if(flag && !(temp.empty())) {
        t++;
        states[t] = temp;
        unamrked[t] = 1;
        transTable[transCount].istate = itm->first;
        transTable[transCount].symbol = *it;
        transTable[transCount].fstate = t;
        transCount++;
      }
    }
    unamrked.erase(itm);
  }
  
  return;
}

int main() {
  int count;
  printf("Enter regular expression with small english alphabets only:");
  scanf("%s", expr); 
  count = strlen(expr);
  preprocess();//to insert . between symbols
  printf("%s\n", expr);
  infix2postfix(expr,postfixExpr,strlen(expr));
  count = countSymbol();
  printf("%s\n", postfixExpr);
  struct node *temp = createTree();//firstpos and lastpos are calculated in createTree() function
  memset(followpos,0,sizeof(followpos));
  calculateFollowPos(temp);// send root node of tree
  makeDFA(temp,count);
  for(int i = 0; i < nodeCount; i++) {
    cout << allNodes[i]->symbol << " " << allNodes[i]->position << endl << "Firstpos: ";
    for(set<int>::iterator it2 = allNodes[i]->firstpos.begin(); it2 != allNodes[i]->firstpos.end(); it2++) {
      cout << *it2 << ' ';
    }
    cout << endl << "Lastpos: ";
    for(set<int>::iterator it2 = allNodes[i]->lastpos.begin(); it2 != allNodes[i]->lastpos.end(); it2++) {
      cout << *it2 << ' ';
    }
    cout << endl;
  }
  cout << "Follwpos" << endl;
  for(int i = 1; i <= count; i++) {
    cout << i << " --> ";
    for(int j = 1; j <= count ;j++) {
      if(followpos[i][j]) cout <<  j << " ";
    }
    cout << endl;
  }
  cout << "Node set" << endl;
  for(map<int,set<int> >::iterator it2 = states.begin(); it2 != states.end(); it2++) {
    cout << it2->first << " -> ";
    for(set<int>::iterator it = it2->second.begin(); it != it2->second.end(); it++) {
      cout << *it << " ";
    } 
    cout << endl;    
  }
  printf("%d\n",transCount);
  for(int i = 0; i < transCount; i++) {
    printf("move(%d,%c) = %d\n", transTable[i].istate, transTable[i].symbol, transTable[i].fstate);
  }
  return 0;
}
/*
(a|b)*baac
(a|b)*abb
(a(aa|b)*ab|b)((ba|a)(aa|b)*ab|bb)*((ba|a)(aa|b)*)*|a(aa|b)*
*/
