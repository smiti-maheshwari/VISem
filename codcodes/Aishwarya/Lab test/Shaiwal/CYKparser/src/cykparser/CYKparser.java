
package cykparser;

import java.io.*;
import java.util.*;

public class CYKparser {

    int nt;
    int te;
    int strt;
    ArrayList <String> pr[] = new ArrayList[100];
    ArrayList <String> nonter = new ArrayList();
    ArrayList <String> ter = new ArrayList();
    ArrayList <String> start = new ArrayList();
    Set <String> table[][] = new HashSet[1000][1000];
 //   HashMap <String,Integer> ma = new HashMap();
  
    void init()
    {
        for (int i = 0; i < 100; i++)
            pr[i] = new ArrayList <String>();
            
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
                table[i][j] = new HashSet <String>();
            }
        }
    }
    
    void reset()
    {
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
                table[i][j].clear();
            }
        }
        
    }
    
    void input()
    {
        init();
        
        Scanner s = null;
        
        try {
        
            s = new Scanner(new File("input.txt"));
        
        } catch (Exception e) {
            System.out.println(e);
        }
       // BufferedReader s = new BufferedReader(new FileReader("input.txt"));
        
        
        System.out.println("enter no. of non-terminals");
     
        nt = s.nextInt();
        
        String inp;
        System.out.println("enter non-terminals");
        for (int i = 1; i <= nt; i++) {
            inp = s.next();
            nonter.add(inp);
        }
        
        System.out.println("enter no. of terminals");
        te = s.nextInt();
        
        System.out.println("enter terminals");
        for (int i = 1; i <= te; i++) {
            inp = s.next();
            ter.add(inp);
        }
        
        for (int i = 0; i < nt; i++) {
            System.out.println("enter no.of productions of " + nonter.get(i));
            int p = s.nextInt();
            System.out.println("enter productions (Use dot(.) for Concatenation of Symbols)");
            for (int j = 0; j < p; j++) {
                inp = s.next();
   //             ma.put(inp,1);
                pr[i].add(inp);
            }
        }
        
        
        System.out.println("enter no. of Start Symbols");
        strt = s.nextInt();
        System.out.println("enter Start Symbol non-terminals");
        for (int i = 1; i <= strt; i++) {
            inp = s.next();
            start.add(inp);
        }
        
    }
    
    void parseString(String input)
    {
        int i,j,k;
        int si = input.length();
        
        String temp = "";
        
        for (i = 0; i < si; i++) {
            temp = "";
            temp = temp + input.charAt(i);
            for (j = 0; j < nt; j++) {
                for (k = 0; k < pr[j].size(); k++) {
                    if (temp.equals(pr[j].get(k))) {
                        table[1][i+1].add(nonter.get(j));
                    }
                }
            }
        }
        
        for (i = 2; i <= si; i++) {
            for (j = 1; j <= si-i+1; j++) {
                for (k = 1; k <= i-1; k++) {
                    
                    for (String m : table[k][j]) {
                        for (String l : table[i-k][j+k]) {
                            temp = m + "." + l;
                            
                            
                            for (int x = 0; x  < nt; x++) {
                                for (int y = 0; y < pr[x].size(); y++) {
                                    if (temp.equals(pr[x].get(y))) {
                                        table[i][j].add(nonter.get(x));
                                    }
                                }
                            }
                            
                            
                        }
                    }
                    
                    
                }
            }
        }
        Boolean flag = false;
        
        for (String l : table[si][1]) {
            for (i = 0; i < strt; i++) {
                if (l.equals(start.get(i))) {
                    flag = true;
                    break;
                }
            }
            if (flag)
                break;
        }
        
        if (flag) {
            System.out.println("STRING ACCEPTED");
        } else {
            
            System.out.println("STRING NOT ACCEPTED");
        }
        
        reset();
        
    }
    
    public static void main(String[] args) {
        CYKparser obj = new CYKparser();
        obj.input();
        
        Scanner take = new Scanner(System.in);
        String inp;
        int t;
        
        System.out.println("enter no. of Strings");
        t = take.nextInt();
        while (t > 0) {
            System.out.println("enter string to parse");
            inp = take.next();
            obj.parseString(inp);
            t--;
        }
    }
    
}
