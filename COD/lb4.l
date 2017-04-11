letter[a-zA-Z]
digit[0-9]

%%

{digit}+("E"("+"|"-")?{digit}+)? printf("\n%s\t real number",yytext);  
{digit}+"."{digit}+("E"("+"|"-")?{digit}+)? printf("\n%s\t floating pt no ",yytext);  
"if"|"else"|"int"|"char"|"scanf"|"printf"|"switch"|"return"|"struct"|"do"|"while"|"void"|"for"|"float" printf("\n%s\t keyword",yytext);  
"\a"|"\\n"|"\\b"|"\t"|"\\t"|"\b"|"\\a" printf("\n%s\t Escape sequence",yytext);  
{letter}({letter}|{digit})* printf("\n%s\t identifier",yytext);  
"&&"|"<"|">"|"<="|">="|"="|"+"|"-"|"?"|"*"|"/"|"%"|"&"|"||" printf("\n%s\toperator ",yytext);  
"{"|"}"|"["|"]"|"("|")"|"#"|"'"|"."|"\""|"\\"|";"|","|"!" printf("\n%s\tspecial character",yytext);  
"%d"|"%s"|"%c"|"%f"|"%e" printf("\n%s\tformat specifier",yytext); 
%%

int yywrap()  
{  
 return 1;  
}  
int main(int argc,char *argv[])  
{
	yyin=fopen(argv[1],"r");  
 	yylex();  
 	fclose(yyin);  
 	return 0;  
}  
 