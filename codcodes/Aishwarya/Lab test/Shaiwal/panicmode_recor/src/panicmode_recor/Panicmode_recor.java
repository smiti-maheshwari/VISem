package panicmode_recor;

import java.util.Map;
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
    Boolean mainflag = false;
    ArrayList <String> grammar[] = new ArrayList[100];
    HashMap <String, Integer> hm = new HashMap <String, Integer> ();//NonTerminals map
    HashMap <String, Integer> terminalMap = new HashMap <String, Integer> (); //terminalMap
    HashMap <Integer, String> terMap = new HashMap <Integer, String> ();
    HashMap <Integer, String> mh = new HashMap <Integer, String> ();
    void inputGrammar() throws FileNotFoundException
    {
        Scanner s = new Scanner(new File("inp.txt"));
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
                mainflag = true;
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
                        for (int l = 0; l < grammar[j].size(); l++) {
                            String s1 = grammar[j].get(l);
                            s1 = s1 + s;
                            grammar[i].add(s1);
                        }
                        String tmp = grammar[i].get(k);
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
        //lr.leftfactoring();
        //lr.print();
        grammar = lr.grammar;
        nonTerminal = lr.nNonTerminal;
        terminal = lr.nterminal;
        Terminal = lr.terminalMap;
        nTerminalMap = lr.hm;
        init();
    }
    
    ArrayList <String> recFirst(int idx) throws Exception
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
            try {
            first[i] = recFirst(i);
            } catch (Exception e) {
                System.out.println("NOT CORRECT");
            }
            for (int j = 0; j < first[i].size(); j++) {
                firstSet[i].add(first[i].get(j));
            }
        }
    }
    
    void print()
    {
       /* System.out.println("First");
        for (int i = 0; i < nonTerminal; i++) {
            System.out.print(lr.mh.get(i) + "->");
            for (String b:firstSet[i]) {
                System.out.print(b + ",");
            }
            System.out.println();
        }*/
        System.out.println("Follow");
        for (int i = 0; i < nonTerminal; i++) {
            System.out.print(lr.mh.get(i) + "->");
            for (String b:followSet[i]) {
                System.out.print(b + ",");
            }
            System.out.println();
        }
       /* for (int i = 0; i < nonTerminal; i++) {
            for (int j = 0; j < grammar[i].size(); j++) {
                System.out.print(grammar[i].get(j) + "->");
                for (String k: firstProd[i][j]) {
                    System.out.print(k + " ");
                }
                System.out.println("");
            }
        }*/
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

class follow {
    
    FirstFollow x = new FirstFollow();
    Set <String> followpos[] = new HashSet[100];
    int mflag = 0;
    
    void compute() throws FileNotFoundException {
        x.preProcess();
        
        if (x.lr.mainflag == true) {
            mflag = 1;
            return;
        }
        try{
            x.calFirst();
            x.calFollow();
        }
        catch(Exception e){
            
        }
        // x.print();
        
        for (int i = 0; i < 100; i++) {
            followpos[i] = new HashSet <String>();
        }
        
        for (int i = 2; i <= 99; i++) {
            followpos[i] = x.followSet[i-2];
        }
       /* System.out.println(x.lr.mh.get(0) + " -> " + x.followSet[0].size());
       
        for (String s: followpos[1]) {
            System.out.print(s);
        }*/
        
    }
    
   // public static void main(String args[]) throws FileNotFoundException {
    //    follow f = new follow();
    //    f.compute();
    //}
    
}

public class Panicmode_recor {
    
    int nt;
    int te;
    int states;
    int sy;
    int prcnt;
    int finst;
    int mainflag = -1;
    follow f = new follow();
    Set <String> st[] = new HashSet[100];
    ArrayList <String> pr[] = new ArrayList[100];
    ArrayList <String> nonter = new ArrayList();
    ArrayList <String> ter = new ArrayList();
    Map < Set<String> , Integer> ma = new HashMap();
    Map <String,Integer> nont = new HashMap();
    Map <String,Integer> mkitemma = new HashMap();
    Set <String> sym = new HashSet();
    Set <String> table[][] = new HashSet[100][100];
    Map <String, Integer> symmap = new HashMap();
    Map <String, Integer> prmap = new HashMap();
    Map <Integer, String> left = new HashMap();
    Map <Integer, String> right = new HashMap();
    Set <String> onter = new HashSet <String>();
    
    void init()
    {
        for (int i = 0; i < 100; i++) {
            st[i] = new HashSet();
            pr[i] = new ArrayList <String> ();
        }
        
        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                table[i][j] = new HashSet();
            }
        }
        
    }
    
    void addaugmented(String inp)
    {
        String te = inp + "'";
        nonter.add(te);
        sy++;
        symmap.put(te,sy);
        sym.add(te);
        nont.put(te,0);
        pr[0].add(inp);
        prcnt++;
        prmap.put(te + "->" + inp,prcnt);
        right.put(prcnt, inp);
        left.put(prcnt, te);
    }
    
    void input() throws FileNotFoundException
    {
        init();
        sy = 0;
        prcnt = 0;
        Scanner s = new Scanner(new File("inp.txt"));
        System.out.println("enter no. of non-terminals");
     
        nt = s.nextInt();
        
        String inp;
        System.out.println("enter non-terminals");
        for (int i = 1; i <= nt; i++) {
            inp = s.next();
            if (i == 1) {
                addaugmented(inp);
            }
            nonter.add(inp);
            sy++;
            symmap.put(inp, sy);
            sym.add(inp);
            onter.add(inp);
            nont.put(inp,i);
        }
        
        System.out.println("enter no. of terminals");
        te = s.nextInt();
        
        System.out.println("enter terminals");
        for (int i = 1; i <= te; i++) {
            inp = s.next();
            ter.add(inp);
            sy++;
            symmap.put(inp, sy);
            sym.add(inp);
        }
        
        for (int i = 1; i <= nt; i++) {
            System.out.println("enter no.of productions of " + nonter.get(i));
            int p = s.nextInt();
            System.out.println("enter productions (Use dot(.) for Concatenation of Symbols)");
            for (int j = 0; j < p; j++) {
                inp = s.next();
   //             ma.put(inp,1);
                pr[i].add(inp);
                prcnt++;
               // System.out.println(nonter.get(i) + "->" + inp);
                prmap.put(nonter.get(i) + "->" + inp,prcnt);
                right.put(prcnt, inp);
                left.put(prcnt, nonter.get(i));
            }
        }
        nt++;
        
        sy++;
        symmap.put("$", sy);
        sym.add("$");
       
    }
    
    
    void makeitem(String tmp)
    {
        String temp = "",temp1 = "";
        StringTokenizer tk = new StringTokenizer(tmp,".");
        if (tk.hasMoreElements()) {
            temp = tk.nextToken();
        }
        
        if (mkitemma.containsKey(temp)) {
            return;
        }
        
        mkitemma.put(temp,1);
        
        if (nont.containsKey(temp)) {
            int ind = nont.get(temp);
            for (int i = 0; i < pr[ind].size(); i++) {
                temp1 = temp + "->" + pr[ind].get(i);
                st[states].add(temp1);
                makeitem(pr[ind].get(i));
            }
        }
        
    }
    
    void makeset()
    {
        states = 0;
        String temp = "";
        if (states == 0) {
            temp = nonter.get(0) + "->" + pr[0].get(0);
            st[states].add(temp);
        }
        mkitemma.clear();
        makeitem(pr[0].get(0));
        
      /*  for (String s : st[0]) {
            System.out.println(s);
        }*/
        
        states++;
        ma.put(st[0],1);
        int cur = 0;
        
        int rdfl = 0,rdind = 0;
        
        while (cur < states) {
            
            for (String symbol : sym) { 
            
                for (String s : st[cur]) {

                    String inp = "";
                    String red = "";
                    rdfl = 0;
                    rdind = 0;
                    
                    String epsi = "";
                    
                    StringTokenizer a = new StringTokenizer(s, "->");
                    while (a.hasMoreElements()) {
                        temp = a.nextToken();
                        epsi = temp;
                        if (a.hasMoreElements()) {
                            inp = inp + temp;
                            inp = inp + "->";
                        }
                    }
                    
                   // System.out.println(inp);
                    a = new StringTokenizer(temp,"#");
                    while (a.hasMoreElements()) {
                        temp = a.nextToken();

                        if (a.hasMoreElements()) {
                            inp = inp + temp;
                            inp = inp + "#";
                        }
                    }
                    
                    if (s.charAt(s.length() - 1) == '#' || epsi.equals("@")) {
                        
                        red = "";
                        for (int j = 0; j < s.length() - 1; j++) {
                            if (s.charAt(j) == '#') {
                                red = red + ".";
                            } else {
                                red = red + s.charAt(j);
                            }
                        }
                        if (epsi.equals("@")) {
                            red = red + "@";
                        }
                        rdfl = 2;
                       // System.out.println(s + "hello " + red);
                        rdind = prmap.get(red);
                        
                        
                        if (rdfl == 2) {
                            for (String k : sym) {
                                if (!nont.containsKey(k)) {
                                    String ins = "R" + rdind;
                                    
                                    if (rdind == 1 && symmap.get(k) == '$') {
                                        ins = "Accept";
                                    }
                                    
                                    if (rdind != 1) {
                                        for (String z : sym) {
                                            if (!nont.containsKey(z)) {
                                                table[cur][symmap.get(z)].add(ins);
                                            }
                                        }
                                       // System.out.println(cur + " -> " + k + " -> " + ins);
                                    } else {
                                        //System.out.println(cur + " hi " + prcnt);
                                        finst = cur;
                                    }
                                }
                            }
                        }
                        
                        continue;
                   
                    }

                    a = new StringTokenizer(temp, ".");
                    if (a.hasMoreElements()) {
                        temp = a.nextToken();
                        inp = inp + temp;
                    }
                    
                   /* if (cur == 2) {
                        System.out.println(symbol + " hello " + temp + " full " + s);
                    }*/
                    
                    if (!temp.equals(symbol))
                        continue;

                    inp = inp + "#";
                    String temp1 = "";

                    while (a.hasMoreElements()) {
                        temp = a.nextToken();
                        temp1 = temp1 + temp;
                        inp = inp + temp;
                        if (a.hasMoreElements()) {
                            temp1 = temp1 + ".";
                            inp = inp + ".";
                        }
                    }
                    
                    

                    st[states].add(inp);

                    mkitemma.clear();
                    if (temp1.length() != 0)
                        makeitem(temp1);
                   


                }
                
                String ins = "";
                if (!nont.containsKey(symbol)) {
                    ins = ins + "S";
                }
                
                int tempst = states;
                int ind = 0;
                int flag = 0;
  

                if (ma.containsKey(st[states])) {
                     ind = ma.get(st[states]);
                     ind--;
                     flag = 1;
                     st[states].clear();

                } else {
                    if (st[states].size() != 0) {
                        ma.put(st[states], states+1);
                        ind = states;
                        flag = 1;
                        states++;
                    }
                }
                
                if (flag == 1) {
                  //  System.out.println("hello");
                    
                    ins = ins + ind;
                    table[cur][symmap.get(symbol)].add(ins);
                   // System.out.println(cur + " -> " + symbol + " -> " + ins);
                }
                
                
            }
            
            cur++;
        }
        
    }
    
    void accpt() {
        table[finst][sy].add("accept");
    }
    
    Boolean notconflict()
    {
        for (int i = 0; i < states; i++) {
            for (String s : sym) {
                if (table[i][symmap.get(s)].size() > 1) {
                    System.out.println("Here is a conflict");
                    return false;
                }
            }
        }
        return true;
    }
    
    
    Stack <String> errorrec(Stack <String> x)
    {
        while (!x.empty()) {
            int ind = Integer.parseInt(x.peek());
            for (String s : onter) {
                int j = symmap.get(s);
                if (table[ind][j].size() != 0) {
                    return x;
                } 
            }
            x.pop();
            x.pop();
        }
        return x;
    }
    
    int removeinp(String inp, int ind, String nonterminal)
    {
        int j = symmap.get(nonterminal);
        String temp = "";
        int org = ind;
        while (!ter.contains(temp) && !temp.equals("$")) {
                ind++;
                if (ind == inp.length()) {
                    return ind;
                }
                temp = temp + inp.charAt(ind);
               // System.out.println("temp " + temp + ter.contains(temp));
         }
        //System.out.println(temp);
        
        for (String s : f.followpos[j]) {
            //System.out.println(temp + " h " + s + " kh " + nonterminal);
            if (s.equals(temp)) {
                //System.out.println("remove " + s + " jf " + org);
                return org;
            }
        }
        return removeinp(inp, ind, nonterminal);
    }
    
    void parse(String inp) throws FileNotFoundException
    {
        inp = inp + "$";
        Stack <String> st = new Stack <String> ();
        st.push("0");
        
        int si = inp.length();
        int i = -1,org;
        String cur = "";
        while (true) {
            
            if (i == si || st.empty()) {
                System.out.println("NOT CORRECT");
                return;
            }
            String temp = cur,temp1 = "";
            org = i;
            while (!ter.contains(temp) && !temp.equals("$")) {
                i++;
                if (i == si) {
                    System.out.println("NOT CORRECT");
                    return;
                }
                temp = temp + inp.charAt(i);
            }
            
            //System.out.println(i + " hello " + temp);
          
            cur = temp;
            int ind = symmap.get(temp);
            int tp = Integer.parseInt(st.peek());
            System.out.println(st.peek() + " parse " + temp);
            //System.out.println(temp + " hello " + tp);
            //System.out.println(st.peek() + " parse " + temp);
            
            if (table[tp][ind].size() == 0) {
                
                if (mainflag == -1) {
                    try {
                        f.compute();
                        mainflag = f.mflag;
                    } catch (Exception e) {
                        
                    }
                    
                }
                
                if (mainflag == 1){
                    System.out.println("NOT CORRECT");
                    return;
                }
                
                st = errorrec(st);
                if (st.empty()) {
                    System.out.println("NOT CORRECT");
                    return;
                }
                int top = Integer.parseInt(st.peek());
                for (String s : onter) {
                    int j = symmap.get(s);
                    if (table[top][j].size() != 0) {
                        i = removeinp(inp, org, s);
                        st.push(s);
                        for (String U : table[top][j]) {
                            st.push(U);
                        }
                        break;
                    }
                }
                cur = "";
                if (i == si) {
                    System.out.println("NOT CORRECT");
                    return;
                }
                System.out.println("USED PANIC MODE RECOVERY");
                continue;
            }
                
            
            for (String s : table[tp][ind]) {
                temp1 = s;
            }
            System.out.println(temp1);
            //System.out.println(i + "hello " + temp1);
            if (temp1.equals("accept")) {
                System.out.println("Successfully Parsed");
                return;
            }
            char c = 'a';
            if (temp1.charAt(0) == 'R' || temp1.charAt(0) == 'S') {
                c = temp1.charAt(0);
                temp1 = temp1.substring(1);
            }
            
            if (c == 'S') {
                st.push(temp);
                cur = "";
                st.push(temp1);
            } else {
                int j = Integer.parseInt(temp1);
                String l = left.get(j);
                String r = right.get(j);
                
                StringTokenizer k = new StringTokenizer(r,".");
                int cnt = 0;
                while (k.hasMoreTokens()) {
                    r = k.nextToken();
                    if (r.equals("@"))
                        break;
                    cnt++;
                }
                cnt = cnt * 2;
                while (cnt > 0) {
                    if (st.empty()) {
                       System.out.println("NOT CORRECT");
                        return; 
                    }
                    //System.out.println(st.peek());
                    st.pop();
                    cnt--;
                }
                
                if (st.empty()) {
                   System.out.println("NOT CORRECT");
                   return; 
                }
                //System.out.println("hello");
                ind = symmap.get(l);
                tp = Integer.parseInt(st.peek());
                if (table[tp][ind].size() == 0) {
                    System.out.println("NOT CORRECT");
                    return;
                }
               
                for (String s : table[tp][ind]) {
                    temp1 = s;
                }
                //System.out.println(temp1);
                st.push(l);
                st.push(temp1);
            }
            
        }
        
    }
    
    void print()
    {
        System.out.println("States = " + states);
        
        for (int i = 0; i < states; i++) {
            System.out.println("state no. = " + i);
            for (String k : st[i]) {
                System.out.println(k);
            }
            System.out.println("");
        }
        
        System.out.println("table ------------");
        
        
        
        for (int i = 0; i < states; i++) {
            for (String s : sym) {
                System.out.print("state " + i + " symbol " + s + " -> ");
                for (String k : table[i][symmap.get(s)])
                    System.out.println(k);
                System.out.println();
            }
        }
        
    }
    
    public static void main(String[] args) throws FileNotFoundException {
        
        Scanner s = new Scanner(System.in);
        Panicmode_recor obj = new Panicmode_recor();
        obj.input();
        obj.makeset();
        obj.accpt();
        
       if (obj.notconflict()) {
            String inp;
            inp = s.next();
            obj.parse(inp);
        }
        obj.print();
    }
    
}

