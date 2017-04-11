#include <bits/stdc++.h>
#define ll long long int
#define alphabet 256
#define dfa_size 1000
using namespace std;

set <ll> hash[alphabet];

set < set <ll> > final_states;

struct node {
	ll pos;
	ll left;
	ll right;
	ll nullable;
	set <ll> lpos;
	set <ll> rpos;
	char ch;
};

struct states {
	ll trans[alphabet];
	set <ll> vs;
};

ll count_nodes;
ll count_states;

struct states dfa[dfa_size];
struct node n[dfa_size];

ll operand(char c) {
	if(c=='*'||c=='('||c==')'||c=='|'||c=='.')
		return 0;
	return 1;
}

ll prec(char c) {
	switch(c) {
		case '|':
			return 1;
		case '.':
			return 2;
		case '*':
			return 3;
	}
	return -1;
}

string to_postfix(string inp) {
	string out="";
	stack <char> st;
	ll i;
	
	for(i=0;i<inp.size();i++){
		if(operand(inp[i])==1)
			out=out+inp[i];
		else if(inp[i]=='(')
			st.push('(');
		else if(inp[i]==')') {
			while(!st.empty() && st.top()!='('){
				out=out+st.top();
				st.pop();
			}
			st.pop();
		}else {
			while(!st.empty() && prec(inp[i])<=prec(st.top())) {
				out=out+st.top();
				st.pop();		
			}
			st.push(inp[i]);
		}
	}
	
	while(!st.empty()){
		out=out+st.top();
		st.pop();
	}
	out=out+"#.";
	return out;
}

void initialise(struct node n) {
	n.left=0;
	n.right=0;
	n.nullable=0;
}

ll build_tree(string inp) {
	ll i;
	stack <ll> st;
	count_nodes=1;
	for(i=0;i<inp.length();i++){
		initialise(n[count_nodes]);
		n[count_nodes].ch=inp[i];
		if(inp[i]=='|'||inp[i]=='.') {
			ll l,r;
			r=st.top();
			st.pop();
			l=st.top();
			st.pop();
			if(inp[i]=='|'){
				if(n[l].nullable==1 || n[r].nullable==1)
					n[count_nodes].nullable=1;
			}
			if(inp[i]=='.'){
				if(n[l].nullable==1 && n[r].nullable==1)
					n[count_nodes].nullable=1;
			}
			n[count_nodes].left=l;
			n[count_nodes].right=r;
		}
		if(inp[i]=='*'){
			ll t;
			t=st.top();
			st.pop();
			n[count_nodes].nullable=1;
			n[count_nodes].left=t;
		}
		st.push(count_nodes);
		count_nodes++;
	}
	ll head=st.top();
	st.pop();
	return head;
}

void level(ll head) {
	queue <ll> q;
	q.push(head);
	while(!q.empty()){
		ll tmp=q.front();
		cout << n[tmp].ch<<endl;
		q.pop();
		if(n[tmp].left!=0)
			q.push(n[tmp].left);
		if(n[tmp].right!=0)
			q.push(n[tmp].right);
	}
	cout << endl<<endl;
}

void assign_leaf(ll head, ll *cnt){
	if(head==0)
		return;
	if(n[head].left==0 && n[head].right==0) {
		*cnt=*cnt+1;
		n[head].lpos.insert(*cnt);
		n[head].rpos.insert(*cnt);
		cout << n[head].ch << "  "<< *cnt<<endl;
		hash[n[head].ch].insert(*cnt);
		return;
	}
	if(n[head].left!=0)
		assign_leaf(n[head].left,cnt);
	if(n[head].right!=0)
		assign_leaf(n[head].right,cnt);
}

set <ll> unions(set <ll> a, set <ll> b) {
	if(a.size()==0)
		return b;
	if(b.size()==0)
		return a;
	set <ll> :: iterator sti;
	for(sti=b.begin();sti!=b.end();sti++){
		a.insert(*sti);
	}
	return a;
}

void print_set(set <ll> a) {
	set <ll> :: iterator sti;
	for(sti=a.begin();sti!=a.end();sti++){
		cout<< *sti <<"->";
	}
	cout<<endl;
}

void print(ll h) {
	if(h==0)
		return;
	print_set(n[h].lpos);
	print_set(n[h].rpos);
	cout << endl<<endl;
}

void print_all(ll h) {
	print(h);
	if(n[h].left!=0)
		print_all(n[h].left);
	if(n[h].right!=0)
	print_all(n[h].right);
}

ll check(ll head) {
	set <ll> a1,a2,b1,b2;
	a1=n[n[head].left].lpos;
	a2=n[n[head].left].rpos;
	
	if(n[head].ch=='*') {
		n[head].lpos=a1;
		n[head].rpos=a2;
		return head;
	}
	
	b1=n[n[head].right].lpos;
	b2=n[n[head].right].rpos;
	
	if(n[head].ch=='|') {
		set <ll> tmp1,tmp2;
		n[head].lpos=unions(a1,b1);
		n[head].rpos=unions(a2,b2);
	}
	if(n[head].ch=='.') {
		set <ll> tmp1,tmp2;
		if(n[n[head].left].nullable==1)
			n[head].lpos=unions(a1,b1);
		else 
			n[head].lpos=a1;
		
		if(n[n[head].right].nullable==1)
			n[head].rpos=unions(a2,b2);
		else 
			n[head].rpos=b2;	
	}
	return head;
}

void call_set(ll head) {
	if(head==0)
		return;
	call_set(n[head].left);
	call_set(n[head].right);
	head=check(head);
}

ll check_follow(ll h, set <ll> follow[]) {
	if(n[h].ch=='.') {
		set <ll> :: iterator sti;
		set <ll> tmp=n[n[h].left].rpos;
		for(sti=tmp.begin();sti!=tmp.end();sti++){
			ll j=*sti;
			follow[j]=unions(follow[j],n[n[h].right].lpos);
			print_set(follow[j]);
		}
	}
	if(n[h].ch=='*') {
		set <ll> :: iterator sti;
		set <ll> tmp=n[h].rpos;
		for(sti=tmp.begin();sti!=tmp.end();sti++){
			ll j=*sti;
			follow[j]=unions(follow[j],n[h].lpos);
			print_set(follow[j]);
		}
	}
}

void calc_follow(ll h, set <ll> adj[]) {
	if(h==0)
		return;
	if(n[h].left!=0)
		calc_follow(n[h].left,adj);
	if(n[h].right!=0)
		calc_follow(n[h].right,adj);
	h=check_follow(h,adj);
}

void print_follow(set <ll> adj[],ll count) {
	cout <<endl<<"print follow"<<endl;
	ll i;
	for(i=1;i<count;i++) {
		set <ll> :: iterator sti;
		if(adj[i].size()==0)
			continue;
		cout << i<<endl;
		for(sti=adj[i].begin();sti!=adj[i].end();sti++){
			cout << *sti<<"-->";
		}
		cout <<endl;
	}
	cout<<endl<<endl;
}

ll check_state(ll count,set <ll> st){
	ll i;
	for(i=1;i<=count;i++) {
		if(dfa[i].vs==st)
			return i;
	}
	return -1;
}

int main() {
	string inp,out;
	cin >> inp;
	out=to_postfix(inp);
	//cout << out<<endl;
	ll head=0;
	head=build_tree(out);
	level(head);
	
	ll count=0;
	assign_leaf(head,&count);	
	cout <<endl<<endl;
	
	call_set(head);
//	print(head);
	print_all(head);
//	cout <<count<<endl;
	
	set <ll> follow[count+1];
	calc_follow(head,follow);
	print_follow(follow,count+1);
	
	cout << "DFA printing"<<endl;
	count_states=1;
	ll at_pos=1;
	// dfa[0] left blank
	dfa[1].vs=n[head].lpos;
	struct states current=dfa[1];
	
	ll i;
	while(1) {
		for(i=0;i<alphabet;i++) {
			if(hash[i].size()==0) 
				continue;
			set <ll> calc;
			set <ll> :: iterator sti;
			// for all alphabets check follow pos union 
			for(sti=hash[i].begin();sti!=hash[i].end();sti++){
				if(current.vs.find(*sti)!=current.vs.end()){
					calc=unions(calc,follow[*sti]);
				}
			}
			if(calc.size()==0)
				continue;
			// check state already exists or not
			ll vals=check_state(count_states,calc);
			if(vals!=-1){
				current.trans[i]=vals;
				// transition to existing state
			}else{
				count_states++;
				// transition to new state
				current.trans[i]=count_states;
				dfa[count_states].vs=calc;
			}
			print_set(calc);
		}
		if(at_pos==count_states)
			break;
		if(at_pos<count_states)
			at_pos++;
		current=dfa[at_pos];
	}
	return 0;
}





