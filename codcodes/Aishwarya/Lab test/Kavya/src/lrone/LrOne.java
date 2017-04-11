package lrOne;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Scanner;
import java.util.Set;
import java.util.Stack;
import java.util.StringTokenizer;
import java.util.Vector;

public class LrOne {

    static LinkedHashMap<String, List<String>> input = new LinkedHashMap<String, List<String>>();
    static LinkedHashMap<String, List<String>> itemset = new LinkedHashMap<String, List<String>>();
    static LinkedHashMap<String, HashMap<String, String>> links = new LinkedHashMap<String, HashMap<String, String>>();
    static Vector<String> term = new Vector<String>();
    static Vector<String> non = new Vector<String>();
    static Vector<String> toVisit = new Vector<String>();
    static String action[][] = new String[1000][1000];
    static String Goto[][] = new String[1000][1000];
    static String array[] = new String[1000];
    static LinkedHashMap<String, List<String>> lookahead = new LinkedHashMap<String, List<String>>();
    static int ind = 0;
    static int sno = 0;
    static int flag = 0;

    public static void main(String[] args) throws FileNotFoundException, IOException {
        //   table[] t = new table[100];
        FileReader fileReader = new FileReader(new File("parse1.txt"));
        BufferedReader br = new BufferedReader(fileReader);
        String line = "";
        //  int p = 0;

        Scanner in = new Scanner(System.in);
        System.out.println("input the start symbol");
        String p = in.nextLine();

        List<String> jk = new ArrayList<String>();
        jk.add(p);
        String h = "Z'";
        input.put(h, jk);

        while ((line = br.readLine()) != null) {
            array[ind++] = line;
            StringTokenizer st = new StringTokenizer(line, "->");
            List<String> l = new ArrayList<String>();
            String s = st.nextToken();
            if (input.containsKey(s)) {
                l = input.get(s);
            } else {
                non.add(s);
            }
            l.add(st.nextToken());
            input.put(s, l);
        }

        for (String ss : input.keySet()) {
            List<String> l = new ArrayList<String>();
            l = input.get(ss);

            for (int i = 0; i < l.size(); i++) {
                String q = l.get(i);
                for (int j = 0; j < (l.get(i)).length(); j++) {
                    String y = "";
                    y += q.charAt(j);
                    if (!non.contains(y) && !term.contains(y)) {
                        term.add(y);
                    }
                }
            }
        }

 
        itemsets();

    }

    static void itemsets() {

        //Vector<String> Visited = new Vector<String>();
        String s = "I" + Integer.toString(sno);

        List<String> l1 = new ArrayList<String>();
        List<String> la = new ArrayList<String>();
        for (String ss : input.keySet()) {
            List<String> l = new ArrayList<String>();

            l = input.get(ss);

            for (int i = 0; i < l.size(); i++) {
                String k = l.get(i);
                String k1 = ss + "->." + k;
                l1.add(k1);
            }
        }
        la.add("$");

        itemset.put(s, l1);
        lookahead.put(s, la);
        toVisit.add(s);
        lookA(s);
        String d;

        for (int i = 0; i < toVisit.size(); i++) {
            d = toVisit.get(i);
            //    if (d == null) {
            //      continue;
            // }
            //System.out.println("to visit" + d);

            List<String> llk = new ArrayList<String>();
            List<String> lla = new ArrayList<String>();
            llk = itemset.get(d);
            lla = lookahead.get(d);
            //int sno1 = sno;
            //    System.out.println("size" + llk.size());
            for (int k = 0; k < non.size(); k++) {
                List<String> l2 = new ArrayList<String>();
                List<String> l3 = new ArrayList<String>();
                for (int j = 0; j < llk.size(); j++) {
                    String y = llk.get(j);
                    String k1 = lla.get(j);
                    int h = y.indexOf(".");
                    String hp = "";
                    if (h == y.length() - 1) {
                        continue;
                    }

                    //System.out.println(y + " " + h);
                    if (h + 1 < y.length()) {
                        hp += y.charAt(h + 1);
                    }
                    String s2 = "";
                    String look = "";
                    if (hp.equals(non.get(k))) {
                        //String s1 = "I" + Integer.toString(sno++);
                        look += k1;
                        char c1 = y.charAt(h + 1);
                        for (int m = 0; m < y.length(); m++) {

                            if (m == h) {
                                s2 += y.charAt(h + 1);
                            } else if (m == h + 1) {
                                s2 += ".";
                            } else {
                                s2 += y.charAt(m);
                            }

                        }
                    }
                    //System.out.println(s2);
                    if (!s2.equals("")) {
                        l2.add(s2);
                        l3.add(look);
                    }
                }
                if (l2.size() > 0) {
                    String s1 = "I" + Integer.toString(++sno);
                    itemset.put(s1, l2);
                    lookahead.put(s1, l3);
                    HashMap<String, String> h = new HashMap<String, String>();
                    if (links.containsKey(non.get(k))) {
                        h = links.get(non.get(k));
                    }
                    h.put(d, s1);
                    links.put(non.get(k), h);
                    //System.out.println(s1);
                    toVisit.add(s1);

                    closure(s1, non.get(k));

                   // lookA(s1);
                    /*  for (int n = 0; n < l2.size(); n++) {
                     System.out.println(s1 + "      " + l2.get(n) + "    non : " + non.get(k));
                     }
                     */
                }
            }

            for (int k = 0; k < term.size(); k++) {
                List<String> l2 = new ArrayList<String>();
                List<String> l3 = new ArrayList<String>();
                for (int j = 0; j < llk.size(); j++) {
                    String y = llk.get(j);
                    String k1 = lla.get(j);
                    int h = y.indexOf(".");
                    String hp = "";
                    if (h == y.length() - 1) {
                        continue;
                    }

                    if (h + 1 < y.length()) {
                        hp += y.charAt(h + 1);
                    }
                    String s2 = "";
                    String look = "";
                    if (hp.equals(term.get(k))) {
                        //String s1 = "I" + Integer.toString(sno++);
                        look += k1;
                        char c1 = y.charAt(h + 1);
                        for (int m = 0; m < y.length(); m++) {

                            if (m == h) {
                                s2 += y.charAt(h + 1);
                            } else if (m == h + 1) {
                                s2 += ".";
                            } else {
                                s2 += y.charAt(m);
                            }

                        }
                    }
                    if (!s2.equals("")) {
                        l2.add(s2);
                        l3.add(look);
                    }
                }
                if (l2.size() > 0) {
                    sno++;
                    String s1 = "I" + Integer.toString(sno);

                    itemset.put(s1, l2);
                    lookahead.put(s1, l3);
                    HashMap<String, String> h = new HashMap<String, String>();
                    if (links.containsKey(term.get(k))) {
                        h = links.get(term.get(k));
                    }
                    h.put(d, s1);
                    links.put(term.get(k), h);
                    // System.out.println(s1);
                    toVisit.add(s1);

                    closure(s1, term.get(k));

                   // lookA(s1);
                    /* for (int n = 0; n < l2.size(); n++) {
                     System.out.println(s1 + "      " + l2.get(n) + "    term : " + term.get(k));
                     }
                     */
                }
            }

        }

        for (String kk : itemset.keySet()){
            List<String> lp = new ArrayList<String>();
            List<String> lpp = new ArrayList<String>();
            lp = itemset.get(kk);
            System.out.println(kk + ":    ");
            lpp = lookahead.get(kk);
            for (int b = 0; b < lp.size(); b++) {
                System.out.println(lp.get(b) + " , "+ lpp.get(b));
            }
        }

     /*   for (String kk : lookahead.keySet()) {
            List<String> lp = new ArrayList<String>();
            lp = lookahead.get(kk);
            System.out.println(kk + ":    ");
            for (int b = 0; b < lp.size(); b++) {
                System.out.println(lp.get(b));
            }
      }*/
 
        actionGoto();
        if (flag != 1)
            parsing();
    }

    static void lookA(String s) {
        List<String> ll = new ArrayList<String>();
        List<String> l = new ArrayList<String>();
        List<String> l2 = new ArrayList<String>();
        ll = lookahead.get(s);
        l = itemset.get(s);
        for (int i =0; i < l.size();i++) {
            if(i < ll.size()) {
                l2.add(ll.get(i));
            } else  {
                l2.add("");
            }
        }
        lookahead.put(s, l2);
        ll = l2;
       // System.out.println(s);

       //System.out.println(l.size());
        for (int i = 0; i < l.size(); i++) {

            String w = l.get(i); // rule 
            
            String q = ll.get(i); // lookahead
            int k = w.indexOf(".");
            String d = "";
            //  System.out.println("w : "+w);
            if (k == w.length() - 1) {
                // ll.add(q);
                continue;
            }

            d += w.charAt(k + 1); 
           
           if(term.contains(d)) continue;
            String f = "";
            String g = "";

            if (k + 2 < w.length()) {
                for (int j = k + 2; j < w.length(); j++) {
                    f += w.charAt(j);
                }

                String h = first(f);

                if (h.contains("#")) {

                    for (int j = 0; j < h.length(); j++) {
                        g += h.charAt(j) + "/";
                    }
                    g += q;
                } else {
                    for (int j = 0; j < h.length(); j++) {
                        if (j == h.length() - 1) {
                            g += h.charAt(j);
                        } else {
                            g += h.charAt(j) + "/";
                        }
                    }

                }
                //ll.add(g);
            } else {
                g = "";
                g += q;
                //ll.add(g);
            }
         //  System.out.println(s+ "   rule"+w);
            for (int m = 0; m < l.size(); m++) {
                String v = ll.get(m); 
                if (!v.equals(""))
                    v += "/" + g;
                else 
                    v += g;
                
                
                if (l.get(m).startsWith(d) && l.get(m).charAt(3) == '.') {
                //    System.out.println("v: "+v + " d "+ d);
                    ll.set(m, v);
                }
            }

            /*       System.out.println("lookahead of " + s);
             for (int j = 0; j < ll.size(); j++) {
             System.out.println(ll.get(j));
             }
             */
        }

        lookahead.put(s, ll);

        /*   System.out.println(s);
         List<String> lp = new ArrayList<String> ();
         lp = lookahead.get(s);
         for (int n = 0; n < lp.size(); n++) {
         System.out.println(" " + lp.get(n));
         }
         */
    }

    static void closure(String s, String s1) {

        List<String> l = itemset.get(s);

        for (int i = 0; i < l.size(); i++) {
            String k = l.get(i);
            int h = k.indexOf(".");

            if (h == k.length() - 1) {
                continue;
            }

            String g = "";

            if (h + 1 < k.length()) {
                g += k.charAt(h + 1);
            }

            if (non.contains(g)) {
                List<String> b = input.get(g);

                for (int j = 0; j < b.size(); j++) {
                    String ss = b.get(j);

                    String sq = g + "->." + ss;

                    if (!l.contains(sq)) {
                        l.add(sq);
                    }
                }

            }
        }

        lookA(s);
        for (String p : itemset.keySet()) {
            List<String> ll = itemset.get(p);
            Set<Object> set1 = new HashSet<Object>();
            Set<Object> set2 = new HashSet<Object>();
            set1.addAll(ll);
            set2.addAll(l);
            if (set1.equals(set2) && p != s) {
                List<String> l1 = lookahead.get(p);
                List<String> l3 = lookahead.get(s);
                Set<Object> set3 = new HashSet<Object>();
                Set<Object> set4 = new HashSet<Object>();
                set3.addAll(l1);
                set4.addAll(l3);
                if (!set3.equals(set4)) {
                    continue;
                }
                HashMap<String, String> h1 = new HashMap<String, String>();
                h1 = links.get(s1);
                for (String q : h1.keySet()) {
                    if (h1.get(q).equals(s)) {
                        h1.put(q, p);
                    }
                }
                links.put(s1, h1);
                itemset.remove(s);
                toVisit.remove(s);
                lookahead.remove(s);
                sno--;
                break;
            }
        }

    }

    static void actionGoto() {  
        term.add("$");

        int t = term.size();
        int nt = non.size();
        int items = itemset.size();

        //System.out.println(action[1][2]);
        
        for (int i = 0; i < items; i++) {
            for (int j = 0; j < t; j++) {
                action[i][j] = "-";
            }
            for (int j = 0; j <nt; j++) {
                Goto[i][j] = "-";
            }
        }
        //shift goto
        
        for (int i = 0; i < items; i++) {
            if (i == 1) {
            //    continue;
            }
            for (int j = 0; j < t; j++) {
                String o = "";
                String y = "";
                if (links.containsKey(term.get(j))) {
                    HashMap<String, String> h = new HashMap();
                    h = links.get(term.get(j));

                    if (h.containsKey("I" + Integer.toString(i))) {
                        o = h.get("I" + Integer.toString(i));
                        for (int kk = 1; kk < o.length(); kk++) {
                            y += o.charAt(kk);
                        }
                        o = "s" + y;
                    }
                }
                // System.out.println(o + " " + i + " " + j);
                if (action[i][j].equals("-")) {
                    if (!o.equals("")) {
                        action[i][j] = o;
                    }
                } else {
                    flag = 1;
                    break;
                }

            }

            if (flag == 1) {
                break;
            }
            for (int j = 0; j < nt; j++) {
                String o = "";
                String y = "";
                if (links.containsKey(non.get(j))) {
                    HashMap<String, String> h = new HashMap();
                    h = links.get(non.get(j));

                    if (h.containsKey("I" + Integer.toString(i))) {
                        o = h.get("I" + Integer.toString(i));
                        for (int kk = 1; kk < o.length(); kk++) {
                            y += o.charAt(kk);
                        }
                        o = y;
                    }
                }

                if (!Goto[i][j].equals("-")) {
                    flag = 1;
                    break;
                } else { 
                    if (!o.equals("")) 
                        Goto[i][j] = o;
                }
            }
            
            if (flag == 1) {
                break;
            }
        }

        //reduce
        int b = 0;

        for (String kk : itemset.keySet()) {
            List<String> l = new ArrayList<String>();
            l = itemset.get(kk);
            int f = 0;
            String s = "";
            int id;

            List<String> look = new ArrayList<String>();
            look = lookahead.get(kk);
            for (int i = 0; i < l.size(); i++) {
                String d = l.get(i);
                int k = d.indexOf(".");
                if (k == d.length() - 1) {
                    String o = l.get(i);
                    String h = "";
                    for (int c = 0; c < o.length() - 1;c++) {
                        h += o.charAt(c);
                    }
                    int g = 0;
                    for (int u = 0; u < ind; u++) {
                        if (h.equals(array[u])) {
                            g = u + 1;
                            break;
                        }
                    }
                    h = "r" + Integer.toString(g);
                    String luk = look.get(i);
                    String[] bb = luk.split("/");

                    for (int y = 0; y < bb.length; y++) {
                        for (int z = 0; z < term.size(); z++) {
                            if (bb[y].equals(term.get(z))) {
                                if (action[b][z].equals("-")) {
                                     if (!h.equals(""))
                                        action[b][z] = h;
                                        break;
                                } else {    
                                    flag = 1;
                                    break;
                                }
                            }
                        }
                        if (flag == 1) {
                            break;
                        }
                    }

                }

            }
            b++;
        }
        action[1][term.size() - 1] = "Accept";
        
        if (flag == 1) {
            System.out.println("Grammar is not lr(1)");
            return;
        }

        for (int i = 0; i < itemset.size(); i++) {
            for (int j = 0; j < term.size(); j++) {
                System.out.print(action[i][j] + " ");
            }

            System.out.println();
        }

        for (int i = 0; i < itemset.size(); i++) {
            for (int j = 0; j < non.size(); j++) {
                System.out.print(Goto[i][j] + " ");
            }

            System.out.println();
        }

    }

    static void parsing() {
        Scanner inn = new Scanner(System.in);
        System.out.println("Enter string to be parsed");

        String h = inn.nextLine();

        h += "$";

        Stack st = new Stack();
        st.push("0");

        int flag1 = 0;
        int cnt = 0;
        String state = "";
        //int yyy = 0;
        int flag2 = 0;
        while (!st.empty()) {
            String top = (String) st.lastElement();
            int t = Integer.parseInt(top);

            /*for (int m = 0; m < st.size(); m++) {
                System.out.print(st.elementAt(m) + " ");
            }
            System.out.println();
            */
            // System.out.println(st.elements());
            // System.out.println(top);
            for (int c = 0; c < term.size(); c++) {
                if (h.charAt(cnt) == (term.get(c)).charAt(0)) {
                    //  System.out.println(h.charAt(cnt) + " " + (term.get(c)).charAt(0));
                    state = action[t][c];
                    if (state.equals("-")) {
                        flag2 = 1;
                    }
                    break;
                }
            }

            if (flag2 == 1) {
                break;
            }
            //    System.out.println(state);
            if (state.startsWith("s")) {
                // System.out.println(state);
                String yy = "";
                yy += h.charAt(cnt);
                st.push(yy);
                yy = "";
                for (int kk = 1; kk < state.length(); kk++) {
                            yy += state.charAt(kk);
                 }
                st.push(yy);

                // System.out.println(cnt);
                cnt++;
            } else if (state.startsWith("r")) {
                String yy = "";
                for (int kk = 1; kk < state.length(); kk++) {
                            yy += state.charAt(kk);
                }

                String rule = array[Integer.parseInt(yy) - 1];
                String hj[] = rule.split("->");
                // top = (String) st.lastElement();
                //System.out.println(hj[1]);
                int size = hj[1].length();
                size *= 2;

                while (size > 0) {
                    st.pop();
                    size--;
                }

                String top1 = (String) st.lastElement();
//                System.out.println(top1);
                st.push(hj[0]);
                for (int c = 0; c < non.size(); c++) {
                    if (hj[0].charAt(0) == (non.get(c)).charAt(0)) {
                        state = Goto[Integer.parseInt(top1)][c];
                        break;
                    }
                }

                st.push(state);

            } else if (state.equals("Accept")) {
                flag1 = 1;
                break;
            }

        }

        if (flag1 == 1) {
            System.out.println("String accepted");
        } else if (flag2 == 1 || st.empty()) {
            System.out.println("not acc");
        }

    }

    static String first(String z) {
        String tmp = "";
        int flag = 0;
        int cnt = 0;
        String p = "";
        //    System.out.println((prodRule.get(z)).size()); 

        String yy = "";
        
        yy += z.charAt(0);
        //System.out.println("z   : "+z+"  yy:  "+yy); 
        if (term.contains(yy)) {
            return yy;
        }

        //System.out.println("z " + );
        for (int i = 0; i < (input.get(yy)).size(); i++) { // no. of productions of A ka size
            flag = 0;
            //flag1 = 1;
            if ((input.get(yy)).get(i).charAt(0) == yy.charAt(0)) {
                continue;
            }
            for (int j = 0; j < ((input.get(yy)).get(i)).length(); j++) { // A ke first production ka size
                // System.out.println("hdzkvn" + (prodRule.get(z)).get(i));
                flag = 0;
                //flag1 = 1;
                //System.out.println("chekc    " +(prodRule.get(z)).get(i));
                for (int k = 0; k < non.size(); k++) { // non terminals me checking
                    if ((non.get(k)).charAt(0) == ((input.get(yy)).get(i)).charAt(j)) {

                        p = first(non.get(k));
                        // System.out.println("p  " + p);

                        if (!(p.equals("#"))) {
                            // flag = 1;
                            for (int q = 0; q < p.length(); q++) {
                                if (p.charAt(q) != '#' && tmp.indexOf(p.charAt(q)) == -1) {
                                    tmp += p.charAt(q);
                                }
                            }

                            if (p.contains("#")) {
                                cnt++;
                            }
                            //System.out.println("cnt   " + cnt);
                            // flag1 = 1;
                            //flag1 = 0;
                        } else {
                            cnt++;
                            if (cnt == non.size() - 1 && !(tmp.contains("#")) && j == ((input.get(yy)).get(i)).length() - 1) {
                                tmp += "#";
                                cnt = 0;
                            }
                            //System.out.println("cnt   " + cnt);
                        }

                        flag = 1;
                        break;
                    }
                }
                //   System.out.println("flag " + flag);
                if (flag == 1) {
                    if (p.contains("#")) {
                        continue;
                    }
                } else {
                    //     System.out.println("bfr   " + tmp);
                    if (tmp.indexOf(((input.get(yy)).get(i)).charAt(j)) == -1) {
                        //   System.out.println(((prodRule.get(z)).get(i)).charAt(j) + " tmp " + tmp);
                        tmp += ((input.get(yy)).get(i)).charAt(j);
                        // System.out.println("after" + tmp);
                    }
                    //   System.out.println("aftfr   " + tmp);
                    // System.out.println(tmp);
                }

                /*if (cnt == nt - 1 && !(tmp.contains("#"))) {
                 tmp += "#";
                 cnt = 0;
                 }*/
                break;
            }
        }

        return tmp;
    }

}
