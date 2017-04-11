#include<stdio.h>
#include<ctype.h>
#include <bits/stdc++.h>
using namespace std;

string FIRST(char c);
void addToResultSet(string &Result,char val);
int n;  //numOfProductions
string productionSet[10];
map<char, string> first;
map<char, string> follow;

void addToResultSet(string &Result,char val)
{
    int k;
    for(k=0 ;k<Result.length();k++)
        if(Result[k]==val)
            return;
    Result+=val;
}

string FIRST(char c) {
	int i,j,k;
	bool foundEpsilon, foundTerminal;
	string result="";
	
	if(!(isupper(c)))
    {
    	result+=c;
    	return result;
    }
    
    for(i=0;i<n;i++) {
    	if(productionSet[i][0]==c) {
    		foundTerminal=false;
    		if(productionSet[i][2]=='@') {
    			result+='@';
    		}else {
    			j=2;
    			while(j<productionSet[i].length()) {
    				foundEpsilon=false;
    				string subResult="";
    				if(first.find(productionSet[i][j])==first.end()) {
    					first[productionSet[i][j]] = FIRST(productionSet[i][j]);
    		
    				}
    				subResult = first[productionSet[i][j]];
    				
	                for(k=0;k<subResult.length();k++) {
	                	if(subResult[k]=='@')
	                    {
	                        foundEpsilon=true;
	                    }else {
	                    	if(islower(subResult[k])) {
	                    		foundTerminal=true;
	                    	}
	                    	addToResultSet(result, subResult[k]);
	                    }
	                	
	                }
	            	
	                if(foundEpsilon==false) {
	                    break;
	                }
	                j++;
	                if(j == productionSet[i].length()){
	                	result+="@";
	                }

    			}
    		}
    	}
    }
    return result;
}

string findFollow(char c) {
	string result="";
	bool foundEpsilon;
	
	if(productionSet[0][0]==c) {
		result+='$';
	}
	for(int i=0;i<n;i++) {
	//	if(productionSet[i][0]==c) {
	//		continue;
	//	}
		for(int j=2; j<productionSet[i].length(); j++) {
			if(productionSet[i][j] == c) {
				int p=j;
				while(p<productionSet[i].length()) {
					
			
					if((p+1)==productionSet[i].length()) {
						if(productionSet[i][0]==c) {
							p++;
							continue;
							}
						if(follow.find(productionSet[i][0])==follow.end()) {
							follow[productionSet[i][0]] = findFollow(productionSet[i][0]);
						}
						
							string subResult = follow[productionSet[i][0]];
					//	cout<<"adding follow of " <<productionSet[i][0] << "to:"<<c<<endl;
						for(int k=0;k<subResult.length();k++) {
							addToResultSet(result, subResult[k]);	
						
						}
	    	               
					}else {
						foundEpsilon = false;
						if(first.find(productionSet[i][p+1])==first.end()) {
							first[productionSet[i][p+1]] = FIRST(productionSet[i][p+1]);
						}
						string subResult = first[productionSet[i][p+1]];
					//	cout<<"adding first of " <<productionSet[i][p+1] << "to:"<<c<<endl;
						for(int k=0;k<subResult.length();k++) {
							if(subResult[k]== '@') {
								foundEpsilon=true;
							}else {
								addToResultSet(result, subResult[k]);	
							}
						}
						if(foundEpsilon==false) {
							break;
						}
					}
					p++;
				}
			} 
		}
	}
	return result;
}

int main() {
	cout<<"Enter no of productions:\n";
	cin>>n;
	int i;
	for(i=0;i<n; i++)//read production string eg: E=E+T
    {
        printf("Enter productions Number %d : ",i+1);
        cin>>productionSet[i];
    }
    
    string b = "";
    for(i=0;i<n;i++) {
		int c=0;
		for(int j=0;j <= i;j++) {
			if(productionSet[i][0] == b[j]) {
				c=1;    
				break;
		    }
        }
	    if(c !=1) {
	      b += productionSet[i][0];
	    
	    }               
    }
    cout<<"b is " << b<<endl;
    for(i=0;i<b.length();i++) {
    	if(first.find(b[i])==first.end()) {
    		first[b[i]] = FIRST(b[i]);
    	}
    //	cout<<b[i]<<"\t\t"<<first[b[i]]<<endl;
    }
    for(i=0;i<b.length();i++) {
    	if(follow.find(b[i])==follow.end()) {
    		follow[b[i]] = findFollow(b[i]);
    	}
    	cout<<b[i]<<"\t\t"<<first[b[i]]<<"\t\t"<<follow[b[i]]<<endl;
    }
    
	return 0;
}
