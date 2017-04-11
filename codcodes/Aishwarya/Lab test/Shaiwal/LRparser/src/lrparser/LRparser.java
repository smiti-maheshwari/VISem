
package lrparser;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.Stack;
import java.util.StringTokenizer;

public class LRparser {
    
    int nt;
    int te;
    int states;
    int sy;
    int prcnt;
    int finst;
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
    
    void parse(String inp)
    {
        inp = inp + "$";
        Stack <String> st = new Stack <String> ();
        st.push("0");
        
        int si = inp.length();
        int i = -1;
        String cur = "";
        while (true) {
            
            if (i == si || st.empty()) {
                System.out.println("NOT CORRECT");
                return;
            }
            String temp = cur,temp1 = "";
            
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
           // System.out.println(temp + " hello " + tp);
            
            if (table[tp][ind].size() == 0) {
                System.out.println("NOT CORRECT");
                return;
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
        LRparser obj = new LRparser();
        obj.input();
        obj.makeset();
        obj.accpt();
        
       if (obj.notconflict()) {
            String inp;
            inp = s.next();
            obj.parse(inp);
        }
        //obj.print();
    }
    
}
