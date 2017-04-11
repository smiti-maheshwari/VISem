 #include<bits/stdc++.h>
 using namespace std;
 set <char> symb;
 set <int> digits; 
 set <string> lits;
 set <string> keys;
 bool issymb(char c)
 {
 	if (c==','||c=='|'||c=='('||c==')'||c==';')
 		return 1;
 	else return 0;
 }
bool iskey(char str[])
 {
 	if(strcmp("for",str)==0||strcmp("while",str)==0||strcmp("do",str)==0||strcmp("int",str)==0||strcmp("float",str)==0||strcmp("char",str)==0||strcmp("double",str)==0||strcmp("static",str)==0||strcmp("switch",str
)==0||strcmp("case",str)==0||strcmp("return",str)==0||strcmp("int",str)==0)
return 1;
	else
		return 0;

 }
 bool isdigit(char c)
 {
 	if ((c-'0'>=0)&&(c-'0'<=9))
 		return 1;
 	else
 		return 0;
 }
 bool ischar(char c){
 	if (((c>='a')&&(c<='z'))||(c=='_'))
 		return 1;
 	else
 		return 0;
 }
 int main()
 {
 	char c;
 	int dig;
 	FILE * f1 = fopen("helloworld.c","r");
 	while ((c = getc(f1))!=EOF){
 		
 		if (c=='/'){
 			
 			c = getc(f1);
 			if (c!=EOF && c=='/'){
 				while ((c=getc(f1))!=EOF){
 					if (c=='\n')
 						break;
				 }
			 }else if (c !=EOF && c=='*'){
			 		while ((c=getc(f1))!=EOF){
	 					if (c=='/')
	 						break;
				 	}
			 }else{
			 	fseek(f1,-1,SEEK_CUR);
			 	symb.insert(c);
			 }
			 
		 }else if (isdigit(c)){
		 	
 			dig = c-'0';
 			while (((c = getc(f1))!=EOF )&& (isdigit(c))){
 				dig = 10*dig+c-'0';
			 }
			 digits.insert(dig);
			 
		 }else if (issymb(c)){
		 	symb.insert(c);
		 }else if (ischar(c)){
		 
		 	char yo[10];
		 	yo[0] = c;
		 	int i=1;
		 	while((c=getc(f1))!=EOF){
		 		if (c==' '){
		 			break;
		 		}
		 		if (issymb(c)){
		 			symb.insert(c);
		 			break;
				 }
		 		yo[i++] = c;
			 }
			 yo[i] = '\0';
			// cout<<"heyy: "<<yo<<"\n";
			 string str = yo;
			
			 if (iskey(yo)){
			 	
			 	keys.insert(yo);
			 }else{
			 	lits.insert(yo);
			 }	
		 }else if (c=='\"'){
		 	char yo[10];
		 	int i=0;
		 	while((c=getc(f1))!=EOF){
		 		if (c=='\"')
		 			break;
		 		yo[i++] = c;
			 }
			 yo[i] = '\0';
			 lits.insert(yo);
		 }
 		
	 }
	 set<char>::iterator iter;
	 set<string>::iterator iter1;
	 set<int>::iterator iter2;
	 cout<<"\nSymbols:\n";
	 for(iter=symb.begin(); iter!=symb.end();++iter) {
	 	cout<<*iter<<" ";
	 }
	 cout<<"\nDigits\n";
	 for(iter2=digits.begin(); iter2!=digits.end();++iter2) {
	 		cout<<*iter2<<" ";
	 }
	 cout<<"\nLiterals\n";
	  for(iter1=lits.begin(); iter1!=lits.end();++iter1) {
	 		cout<<*iter1<<" ";
	 }
	 cout<<"\nKeywords\n";
	  for(iter1=keys.begin(); iter1!=keys.end();++iter1) {
	 		cout<<*iter1<<" ";
	 }
 	fclose(f1);
 	return 0;
 }
