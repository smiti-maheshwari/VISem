
package llparser;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;
import java.util.Set;
import java.util.Stack;
import java.util.StringTokenizer;

class RemoveLeftRecursion {
    int nNonTerminal; //no of non terminals
    int nterminal; //no of terminals
    String terminal[];
    String nonTerminal[];
    ArrayList <String> grammar[] = new ArrayList[100];
    HashMap <String, Integer> hm = new HashMap <String, Integer> ();//NonTerminals map
    HashMap <String, Integer> terminalMap = new HashMap <String, Integer> (); //terminalMap
    HashMap <Integer, String> terMap = new HashMap <Integer, String> ();
    HashMap <Integer, String> mh = new HashMap <Integer, String> ();
    void inputGrammar() throws FileNotFoundException
    {
        Scanner s = new Scanner(new File("input.txt"));
        String s1;
        System.out.println("Enter number of non Terminals");
        nNonTerminal = s.nextInt();
        nonTerminal = new String [nNonTerminal];
        System.out.println("Enter non Terminals");
        for (int i = 0; i < nNonTerminal; i++) {
            nonTerminal[i] = s.next();
            
        }
        for (int i = 0; i < nNonTerminal; i++) {
            hm.put(nonTerminal[i], i);
            mh.put(i, nonTerminal[i]);
        }
        System.out.println("Enter number of Terminals");
        nterminal = s.nextInt();
        terminal = new String[nterminal];
        System.out.println("Enter terminals");
        for (int i = 0; i < nterminal; i++) {
            terminal[i] = s.next();
            terminalMap.put(terminal[i], i);
            terMap.put(i, terminal[i]);
        }
        int n;
        //nNonTerminal = nonTerminal.length;
        // nterminal = terminal.length;
        for (int i = 0; i < 100; i++) {
            grammar[i] = new ArrayList();
        }
        String s2;
        for (int i = 0; i < nNonTerminal; i++) {
            System.out.println("Enter no of Prduction for" + nonTerminal[i]);
            n = s.nextInt();
            System.out.println("Enter the productions");
            for (int j = 0; j < n; j++) {
                s2 = s.next();
                grammar[i].add(s2);
            }
        }
    }
    
    void leftfactoring()
    {
        int cnt,p;
        String fir, sec;
        String prefix = "";
        for (int i = 0; i < nNonTerminal; i++) {
            for (int j = 0; j < grammar[i].size(); j++) {
                for (int k = j+1; k < grammar[i].size(); k++) {
                    cnt = 0;
                    p = -1;
                    fir = grammar[i].get(j);
                    sec = grammar[i].get(k);
                    prefix = "";
                    while (cnt < fir.length() && cnt < sec.length() && fir.charAt(cnt) == sec.charAt(cnt)) {
                        prefix += fir.charAt(cnt);
                        cnt++;
                        p++;
                    }
                    if (p != -1) {
                        String rem1 = "", rem2 = "", nont = "";
                        
                        if (cnt < fir.length())
                            rem1 = fir.substring(cnt);
                        else {
                            rem1 = "@";
                            prefix = prefix + ".";
                            rem2 = sec.substring(cnt+1);
                        }
                        
                        if (cnt < sec.length())
                            rem2 = sec.substring(cnt);
                        else {
                            rem2 = "@";
                            prefix = prefix + ".";
                            rem1 = fir.substring(cnt+1);
                        }
                        
                        nont = mh.get(i) + "F'";
                        grammar[nNonTerminal].add(rem1);
                        grammar[nNonTerminal].add(rem2);
                        hm.put(nont, nNonTerminal);
                        mh.put(nNonTerminal, nont);
                        nNonTerminal++;
                        prefix = prefix + nont;
                        grammar[i].set(j,prefix);
                        grammar[i].remove(k);
                        k--;
            
                    }
                }
                
            }
        }
    }
    
    void eliminateImmediateLeftRecursion(int i)
    {
        ArrayList <String> alpha = new ArrayList();
        ArrayList <String> beta = new ArrayList();
        
        boolean flag = false;
        String nt=""; //non terminal
        
        for (int j = 0; j < grammar[i].size(); j++) {
            if (match(i, i, j) == true) {
                if (flag == false) {
                    nt = mh.get(i) + "'";
                    flag = true;
                }
                alpha.add(grammar[i].get(j).substring(mh.get(i).length()+1));
            } else {
                beta.add(grammar[i].get(j));
            }
        }
        
        if (flag == true) {
            grammar[i].clear();
            
            hm.put(nt, nNonTerminal);
            mh.put(nNonTerminal, nt);
            for (int j = 0; j < beta.size(); j++) {
                String tmp = beta.get(j);
                tmp = tmp + "."+nt;
                grammar[i].add(tmp);
            }
            for (int j = 0; j < alpha.size(); j++) {
                String tmp = alpha.get(j);
                tmp = tmp + "."+nt;
                grammar[nNonTerminal].add(tmp);
            }
            grammar[nNonTerminal].add("@");
            nNonTerminal++;
        }
    }
    
    boolean match(int i, int j, int k)
    {
        String tmp = mh.get(j);
        String tmp1 = grammar[i].get(k);
        for (int x = 0; x < tmp.length(); x++) {
            if (x >= tmp1.length()) {
                return false;
            }
            if (tmp.charAt(x) != tmp1.charAt(x)) {
                return false;
            }
        }
        return true;
    }
    
    void removeLeftRecursion()
    {
        ArrayList<String> rem[] = new ArrayList[100];
        for (int i = 0; i < 100; i++) {
            rem[i] = new ArrayList();
        }
        for (int i = 0; i < nNonTerminal; i++) {
            for (int j = 0; j < i; j++) {
                for (int k = 0; k < grammar[i].size(); k++) {
                    if (match(i, j, k) == true) {
                        String add = "";
                        String s = grammar[i].get(k).substring(mh.get(j).length());
                        System.out.println("s:" + s);
                                
                        for (int l = 0; l < grammar[j].size(); l++) {
                            String s1 = grammar[j].get(l);
                            s1 = s1 + s;
                            grammar[i].add(s1);
                        }
                        String tmp = grammar[i].get(k);
                        System.out.println("tmp:" + tmp);
                        grammar[i].remove(k);
                        k--;
                    } else {
                    }
                }
            }
            eliminateImmediateLeftRecursion(i);
        }
    }
    
    void print()
    {
        for (int i = 0; i < nNonTerminal; i++) {
            System.out.print(mh.get(i) + "->");
            for (int j = 0; j < grammar[i].size(); j++) {
                System.out.print(grammar[i].get(j) + "|");
            }
            System.out.println();
        }
    }
}
class FirstFollow {
    HashSet<String> [][] firstProd = new HashSet[100][100];
    RemoveLeftRecursion lr = new RemoveLeftRecursion();
    ArrayList <String> [] grammar = new ArrayList[100];
    ArrayList <String> [] first = new ArrayList[100];
    ArrayList <String> [] follow = new ArrayList[100];
    Set <String> firstSet[] = new HashSet[100];
    Set <String> followSet[] = new HashSet[100];
    HashMap <String, Integer> Terminal = new HashMap (); 
    HashMap <String, Integer> nTerminalMap = new HashMap ();
    int nonTerminal;
    int terminal;
    void preProcess() throws FileNotFoundException 
    {
        lr.inputGrammar();
        lr.removeLeftRecursion();
        lr.leftfactoring();
        lr.print();
        grammar = lr.grammar;
        nonTerminal = lr.nNonTerminal;
        terminal = lr.nterminal;
        Terminal = lr.terminalMap;
        nTerminalMap = lr.hm;
        init();
    }
    
    ArrayList <String> recFirst(int idx)
    {
        String tmp = "";
        boolean flag;
        for (int i = 0; i < grammar[idx].size(); i++) {
            tmp = grammar[idx].get(i);
            ArrayList <String> ar = new ArrayList();
            //System.out.println("tmp = " + tmp);
            flag = true;
            //tmp = tmp + '.';
            StringTokenizer s = new StringTokenizer(tmp, ".");
            while (s.hasMoreTokens()) {
                String temp = s.nextToken();
               // System.out.println(temp);
                if (Terminal.containsKey(temp) || "@".equals(temp)) {
                  //  System.out.println("temprec " + temp);
                    first[idx].add(temp);
                    ar.add(temp);
                    flag = false;
                    break;
                } else {
                    flag = false;
                    ArrayList <String> tcp  = new ArrayList();
                    tcp = recFirst(nTerminalMap.get(temp));
                   // System.out.println("tcp.size" + tcp.size());
                    for (int j = 0; j < tcp.size(); j++) {
                        if ("@".equals(tcp.get(j))) {
                            flag = true;
                        } else {
                            first[idx].add((String) tcp.get(j));
                            ar.add((String) tcp.get(j));
                        }
                     //   System.out.println("tcp.get(j)" +(String)tcp.get(j));
                    }
                    if (flag == false) {
                        break;
                    }
                }
            }
            if (flag == true) {
                ar.add("@");
                first[idx].add("@");
            }
            for (int l = 0; l < ar.size(); l++) {
                firstProd[idx][i].add(ar.get(l));
            }
        }
        //System.out.println("abhishek");
        /*for (int i = 0; i < first[idx].size(); i++) {
            System.out.println(first[idx].get(i));
        }*/
        return first[idx];
    }
    void init()
    {
        for (int i = 0; i < 100; i++) {
            first[i] = new ArrayList();
            follow[i] = new ArrayList();
            firstSet[i] = new HashSet();
            followSet[i] = new HashSet();
        }
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                firstProd[i][j] = new HashSet();
            }
        }
    }
    
    void calFirst()
    {
        
        for (int i = 0; i < nonTerminal; i++) {
            first[i] = recFirst(i);
            for (int j = 0; j < first[i].size(); j++) {
                firstSet[i].add(first[i].get(j));
            }
        }
    }
    
    void print()
    {
        System.out.println("First");
        for (int i = 0; i < nonTerminal; i++) {
            System.out.print(lr.mh.get(i) + "->");
            for (String b:firstSet[i]) {
                System.out.print(b + ",");
            }
            System.out.println();
        }
        System.out.println("Follow");
        for (int i = 0; i < nonTerminal; i++) {
            System.out.print(lr.mh.get(i) + "->");
            for (String b:followSet[i]) {
                System.out.print(b + ",");
            }
            System.out.println();
        }
        for (int i = 0; i < nonTerminal; i++) {
            for (int j = 0; j < grammar[i].size(); j++) {
                System.out.print(grammar[i].get(j) + "->");
                for (String k: firstProd[i][j]) {
                    System.out.print(k + " ");
                }
                System.out.println("");
            }
        }
    }
    
    void calFollow()
    {
        follow[0].add("$");
        for (int i = 0; i < nonTerminal; i++) {
            ArrayList <String> ar = new ArrayList();
            
            ar = recFollow(i);
            follow[i] = ar;
            
            if (i == 0)
                follow[i].add("$");
            for (int j = 0; j < follow[i].size(); j++) {
                //System.out.print(ar.get(j) + " ");
                followSet[i].add(follow[i].get(j));
            }
            
        }
    }
    
    ArrayList <String> recFollow(int idx)
    {
        boolean flag; //checking for epsilon
        for (int i = 0; i < nonTerminal; i++) {
            for (int j = 0; j < grammar[i].size(); j++) {
                String temp1 = grammar[i].get(j);
                while (temp1.contains(lr.mh.get(idx))) {
                   // System.out.println("temp1 " + grammar[i].get(j) + " " + temp1);
                    int ind = temp1.indexOf(lr.mh.get(idx));
                    int le = lr.mh.get(idx).length();
                    if ((ind == 0 || temp1.charAt(ind - 1) == '.') && ((ind+le) == temp1.length() || temp1.charAt(ind+le) == '.')) 
                    {
                        
                    } else {
                        temp1 = temp1.substring(ind);
                        if (temp1.contains(".")) {
                            ind = temp1.indexOf(".");
                            temp1 = temp1.substring(ind+1);
                        } else {
                            break;
                        }
                    }
                    
                    String tmp;
                    if (ind+lr.mh.get(idx).length()+1 < temp1.length()) {
                        tmp = temp1.substring(ind+lr.mh.get(idx).length()+1);
                    } else {
                        tmp = "";
                    }
                    
                    flag = true;
                    StringTokenizer s = new StringTokenizer(tmp, ".");
                    while (s.hasMoreTokens()) {
                        String temp = s.nextToken();
                        if (Terminal.containsKey(temp)) {
                            follow[idx].add(temp);
                            flag = false;
                            break;
                        } else {
                           // System.out.println("temp = " + temp + " " + temp1);
                            int n = nTerminalMap.get(temp);
                            flag = false;
                            for (String k: firstSet[n]) {
                                if (!"@".equals(k)) {
                                    follow[idx].add(k);
                                } else {
                                    flag = true;
                                }
                            }
                            if (flag == false) {
                                break;
                            }
                        }
                    }
                    
                  //  System.out.println("abhishek");
                    
                    if (flag == true) {
                        if (lr.mh.get(i).equals(lr.mh.get(idx))) {
                            
                        } else {
                            ArrayList <String> tcp = new ArrayList();
                            if (follow[i].size() == 0) {
                                tcp = recFollow(i);
                            } else {
                                tcp = follow[i];
                            }
                                
                            for (int k = 0; k < tcp.size(); k++) {
                                  follow[idx].add(tcp.get(k));
                            }
                        }
                    }
                    temp1 = tmp;
                }
            }
        }
        return follow[idx];
    }
    
}

class LL1 {
    FirstFollow f = new FirstFollow();
    Set <String> [][] table = new HashSet[100][100];
    
    void init() throws FileNotFoundException
    {
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                table[i][j] = new HashSet();
            }
        }
        f.preProcess();
        f.calFirst();
        f.calFollow();
        f.print(); //print function for first and follow set and first productions
    }
    
    void calTable()
    {
        //init();
        boolean flag;
        for (int i = 0; i < f.nonTerminal; i++) {
            for (int j = 0; j < f.grammar[i].size(); j++) {
                flag = false;
                String tmp = f.grammar[i].get(j);
                for (String k: f.firstProd[i][j]) {
                    if (!"@".equals(k)) {
                        //System.out.println(k + " " + f.Terminal.get(k));
                        table[i][f.Terminal.get(k)].add(tmp); //Terminal map
                    } else {
                        flag = true; //no of terminals in the grammar
                    }
                }
                
                if (flag == true) {
                    for (String k: f.follow[i]) {
                        if (!"$".equals(k)) {
                            table[i][f.Terminal.get(k)].add(tmp);
                        } else {
                            table[i][f.terminal].add(tmp);
                        }
                    }
                }
                
            }
        }
    }
    
    void print()
    {
        boolean f1 = false;
        for (int i = 0; i < f.nonTerminal; i++) {
            for (int j = 0; j <= f.terminal; j++) {
                if (table[i][j].size() > 1) {
                    System.out.println("Conflict");
                    f1 = true;
                    break;
                }
            }
            if (f1 == true) break;
        }
        
        
        if (f1 == false) {
            for (int i = 0; i < f.nonTerminal; i++) {
                for (int j = 0; j < f.terminal; j++) {
                       System.out.print(f.lr.mh.get(i) + " " + f.lr.terMap.get(j) + "->" );
                       if (table[i][j].size() != 0) {
                           for (String k: table[i][j]) {
                               System.out.print(k);
                           }
                       }
                       System.out.println("");
                }
               System.out.print(f.lr.mh.get(i) + " " + "$" + "->" );
               if (table[i][f.terminal].size() != 0) {
                    for (String k: table[i][f.terminal]) {
                        System.out.print(k);
                    }
                }
                System.out.println("");
            }
        }      
    }
    
    void stringMatch(String s)
    {
        int i = 0;
        s = s + "$";
       // System.out.println("abh" + s.length());
        Stack <String> st = new Stack<String>();
        st.push("$");
        st.push(f.lr.nonTerminal[0]);
        while (!st.empty()) {
            
            if (i >= s.length()) {
                System.out.println("Invalid Input.");
                break;
            }
            
            String tmp = st.peek();
            st.pop();
            System.out.println("i = " + i + " " + tmp);
            if ("$".equals(tmp)) {
                if (s.charAt(i) == '$') {
                    i++;
                    continue; //stack top $ and Current charactre in the string is $
                } else {
                    System.out.println("Invalid Input.");
                    break;
                }
            }
            if (f.Terminal.containsKey(tmp)) {
                int j = 0;
                while (i < s.length() && j < tmp.length()) {
                    if (s.charAt(i) != tmp.charAt(j)) {
                        break;
                    } 
                    i++;
                    j++;
                }
                if (j < tmp.length()) {
                    System.out.println("Invalid Input.");
                    break;
                }
            } else {
               
                String temp = "";
                int z = i;
                boolean f1 = false;
                while (z < s.length()) {
                    temp += s.charAt(z);
                    z++;
                    if ("$".equals(temp) || f.Terminal.containsKey(temp)) {
                        f1 = true;
                        break;
                    }
                }
                if (f1 == false) {
                    System.out.println("temp = " + temp);
                    System.out.println("Invalid Input.");
                    break;
                }
                String str="";
                int x, y;
                if (temp.equals("$")) {
                    x = f.terminal;
                    y = f.lr.hm.get(tmp);
                } else {
                   // System.out.println("temp = " + temp + " i =" + i);
                    x = f.lr.terminalMap.get(temp);
                    y = f.lr.hm.get(tmp);
                }
                if (table[y][x].size() == 0) {
                    System.out.println("Invalid Input.");
                    break;
                }
                for (String k: table[y][x]) {
                      str = k;
                }
                ArrayList <String> a = new ArrayList();
                if (!"@".equals(str)) {
                    StringTokenizer stk = new StringTokenizer(str, ".");
                    while (stk.hasMoreTokens()) {
                        a.add(stk.nextToken());
                    }
                    for (int l = a.size()-1; l >= 0; l--) {
                        st.push(a.get(l));
                    }
                } 
            }
        }
        
        if (st.empty() && i == s.length()) {
            System.out.println("Parsing done Successfully.");
        }
    }
}


public class LLPARSER {
    public static void main(String args[]) throws FileNotFoundException {
        LL1 l = new LL1();
        l.init();
        l.calTable();
        l.print();
        String s;
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter string to parse:");
        s = sc.next();
        l.stringMatch(s);
    }
}
