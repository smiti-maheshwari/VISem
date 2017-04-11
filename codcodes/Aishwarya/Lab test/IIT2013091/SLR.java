/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package cod;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.*;

/**
 *
 * @author Megha
 */
public class SLR {

    static LinkedHashMap<String, List<String>> h = new LinkedHashMap();
    static LinkedHashMap<String, List<String>> hDup = new LinkedHashMap();
    static Vector<String> terms = new Vector<String>();
    static Vector<String> non = new Vector<String>();
    static HashMap<String, HashMap<String, String>> Action = new HashMap();
    static HashMap<String, HashMap<String, String>> Goto = new HashMap();
    static HashMap<String, List<String>> itemset = new HashMap();
    static HashMap<String, HashMap<String, String>> links = new HashMap();
    static HashMap<Integer, String> rules = new HashMap();
    static HashMap<String, String> error = new HashMap();
    static HashMap<String, List<String>> first = new HashMap();
    static HashMap<String, List<String>> follow = new HashMap();
    static List<String> newTerms = new ArrayList<String>();

    static String start;
    static String epsi = "ε";
    static int Conflict = 0;

    public static void main(String args[]) throws FileNotFoundException {

        FileInputStream in = new FileInputStream("slr2.txt");
        Scanner sc = new Scanner(in);

        String line;

        while (sc.hasNext()) {
            line = sc.nextLine();
            List<String> l = new ArrayList<String>();
            String items[] = line.split(" -> ");
            if (h.containsKey(items[0])) {
                l = h.get(items[0]);

            }
            l.add(items[1]);
            h.put(items[0], l);
            hDup.put(items[0], l);
        }

        Set<String> s = h.keySet();

        for (String i : s) {
            System.out.print(i + " -> ");
            terms.add(i);
            List<String> l = h.get(i);
            for (int j = 0; j < l.size(); j++) {
                System.out.print(l.get(j) + " \\ ");
            }
            System.out.println();
        }
        
        for (String i : s) {
            List<String> l = h.get(i);
            for (int j = 0; j < l.size(); j++) {
                String temp[] = l.get(j).split(" ");
                for (int k = 0; k < temp.length; k++) {
                    if (!terms.contains(temp[k]) && !non.contains(temp[k])) {
                        non.add(temp[k]);
                    }
                }
            }

        }

        System.out.println("Enter start symbol");
        Scanner sc3 = new Scanner(System.in);
        String sho = sc3.nextLine();
        start = sho;

        // check and remove left recursion
           LRec();
        // Set<String> s = h.keySet();

        for (String i : hDup.keySet()) {
            System.out.print(i + " -> ");

            List<String> l = hDup.get(i);
            for (int j = 0; j < l.size(); j++) {
                System.out.print(l.get(j) + " \\ ");
            }
            System.out.println();
        }
      
         
        //find first
        for (int i = 0; i < terms.size(); i++) {

            firstFind(terms.get(i));
            //System.out.println(terms.get(i) + "\t" + first.get(terms.get(i)));
        }

        System.out.println();
        System.out.println("First");
        for (int i = 0; i < terms.size(); i++) {
            List<String> l = first.get(terms.get(i));
            System.out.print(terms.get(i) + " : ");
            for (int j = 0; j < l.size(); j++) {
                System.out.print(l.get(j) + " , ");

            }
            System.out.println();
        }
//remove terms added after left recursion was done
        for (int i = 0; i < newTerms.size(); i++) {
            terms.remove(newTerms.get(i));
        }
        
        //find follow
        // adding end to start symbol
        List<String> temp = new ArrayList<String>();
        temp.add("$");
        follow.put(sho, temp);

        // till no new addition occurs in follow
        int flag = 0;
        while (true) {
            flag = 0;
            for (int i = 0; i < terms.size(); i++) {
                flag = followFind(terms.get(i));
            }
            if (flag == 0) {
                break;
            }
        }

        System.out.println();
        System.out.println("Follow");
        for (int i = 0; i < terms.size(); i++) {
            List<String> l = follow.get(terms.get(i));
            System.out.print(terms.get(i) + " : ");
            for (int j = 0; j < l.size(); j++) {
                System.out.print(l.get(j) + " , ");

            }
            System.out.println();
        }

        // adding S' -> S  for parsing
        String shonew = sho + "'";
        while (terms.contains(shonew)) {
            shonew += "'";
        }
        terms.add(shonew);
        List<String> shh = new ArrayList<String>();
        shh.add(sho);
        h.put(shonew, shh);

        canonical();
        System.out.println();
        System.out.println();
        
        parseTable(shonew);
        
        if (Conflict == 1) {
            return;
        }
        errorDefine();
        fillError();

        System.out.println();
        System.out.println();
        System.out.println("Enter string to parse");
        String str;
        Scanner sc2 = new Scanner(System.in);
        str = sc2.nextLine();
        str = str + " $";
        parse(str);

    }

    public static void canonical() {
        int ind = 0;
        List<String> l = new ArrayList<String>();
        for (String s : h.keySet()) {
            List<String> tem = h.get(s);
            for (int i = 0; i < tem.size(); i++) {

                String y = s + " -> .";
                if (!tem.get(i).equals(epsi)) {
                    y += " " + tem.get(i);
                }
                l.add(y);
            }
        }

        /*for (int i = 0; i < l.size(); i++) {
         System.out.println(l.get(i));
         }*/
        itemset.put("I" + Integer.toString(ind), l);

        Vector<String> toVisit = new Vector<String>();
        Vector<String> Visited = new Vector<String>();
        toVisit.add("I" + Integer.toString(ind));
        ind++;

        while (toVisit.size() > 0) {
            String s = toVisit.firstElement();
            Visited.add(s);

            links.put(s, new HashMap<String, String>());
            List<String> now = itemset.get(s);

            HashMap<String, String> curLinks = links.get(s); // getting the links for this itemset

            for (int j = 0; j < now.size(); j++) {

                curLinks = links.get(s);
                String nextItem = "";
                String v = now.get(j);
                // A -> B .

                if (v.charAt(v.length() - 1) == '.') {
                    continue;
                }

                String nextE = nextElem(v);

                Vector<String> sameNext = new Vector<String>();
                sameNext.add(v);
                for (int kl = j + 1; kl < now.size(); kl++) {
                    String tempo = now.get(kl);
                    if (tempo.charAt(tempo.length() - 1) == '.') {
                        continue;
                    }

                    if (nextElem(tempo).equals(nextE)) {
                        sameNext.add(tempo);
                    }
                }
                List<String> m = null;
                // already shiftT element has been seen once

                if (curLinks.containsKey(nextE)) {
                    nextItem = curLinks.get(nextE);
                    m = itemset.get(nextItem);

                } else {
                    m = new ArrayList<String>();

                }
                for (int hu = 0; hu < sameNext.size(); hu++) {
                    String kn = shiftDot(sameNext.get(hu));
                    if (!m.contains(kn)) {
                        m.add(kn);
                    }
                }

                //String nextE2 = nextElem(kn);
                // find closure if next element is a terminal
                for (int w = 0; w < m.size(); w++) {
                    String nextE2 = nextElem(m.get(w));
                    if (terms.contains(nextE2)) {
                        List<String> lk = h.get(nextE2);
                        for (int jk = 0; jk < lk.size(); jk++) {
                            String y = nextE2 + " -> .";
                            if (!lk.get(jk).equals(epsi)) {
                                y += " " + lk.get(jk);
                            }

                            if (!m.contains(y)) {
                                m.add(y);
                            }
                        }
                    }

                }
                // check if the itemset already exists
                int flagh = 0;

                for (String hj : itemset.keySet()) {

                    Set<String> set1 = new HashSet<String>();
                    set1.addAll(m);
                    Set<String> set2 = new HashSet<String>();
                    set2.addAll(itemset.get(hj));

                    if (set1.equals(set2)) {
                        nextItem = hj;
                        flagh = 1;
                        break;
                    }

                }

                //System.out.println(flagh + " " + s + "  " + nextItem);
                if (flagh == 0) {

                    nextItem = "I" + Integer.toString(ind);
                    itemset.put(nextItem, m);

                    ind++;
                }

                curLinks.put(nextE, nextItem);

                if (!toVisit.contains(nextItem) && !Visited.contains(nextItem)) {
                    toVisit.add(nextItem);
                }
                links.put(s, curLinks);
            }
            toVisit.remove(0);

        }

        System.out.println("***********");
        for (String st : itemset.keySet()) {
            System.out.println(st + " : " + itemset.get(st));
        }

        System.out.println("***********");
        System.out.println();
        for (String s : links.keySet()) {
            System.out.println(s + " : " + links.get(s));
        }
        System.out.println("***********");
    }

    public static String shiftDot(String k) {

        String ite[] = k.split(" ");
        String kn = "";
        for (int y = 0; y < ite.length; y++) {
            if (ite[y].equals(".")) {
                kn += ite[y + 1] + " . ";
                y += 1;
            } else {
                kn += ite[y] + " ";
            }
        }
        kn = kn.substring(0, kn.length() - 1);
        return kn;
    }

    public static String nextElem(String v) {
        String ele[] = v.split(" ");
        String shiftT = ""; // A -> B . C gives C so to become A -> B C .
        for (int p = 0; p < ele.length - 1; p++) {
            if (ele[p].equals(".")) {
                shiftT = ele[p + 1];
                break;
            }
        }
        return shiftT;
    }

    public static void parseTable(String shonew) {
        if (!non.contains("$")) {
            non.add("$");
        }
        // numbering productions
        int ind = 0;
        for (String s : h.keySet()) {
            List<String> l = h.get(s);
            for (int i = 0; i < l.size(); i++) {
                rules.put(ind, s + " -> " + l.get(i));
                ind++;
            }
        }
        System.out.println(rules);

        for (String s : itemset.keySet()) {
            List<String> l = itemset.get(s);

            HashMap<String, String> ac = new HashMap<String, String>();
            HashMap<String, String> go = new HashMap<String, String>();

            for (int i = 0; i < l.size(); i++) {
                // check if reduction present
                if (l.get(i).charAt(l.get(i).length() - 1) == '.') {
                    String ty[] = l.get(i).split(" -> ");
                    if (!ty[1].equals(".")) {
                        // find the rule
                        String x = l.get(i); // of the form A -> B .

                        if (x.equals(shonew + " -> " + start + " .")) { // for accepting state
                            ac.put("$", "Accept");

                        } else {
                            x = x.substring(0, x.length() - 2); // removing dot at last
                            int rulNo = -1;
                            for (int y = 0; y < rules.size(); y++) {
                                if (rules.get(y).equals(x)) {
                                    rulNo = y;
                                    break;
                                }
                            }
                            String temp[] = x.split(" -> ");
                            List<String> followKey = follow.get(temp[0]);
                            for (int r = 0; r < followKey.size(); r++) {
                                if (!ac.containsKey(followKey.get(r)) || ac.get(followKey.get(r)).equals("Reduce " + Integer.toString(rulNo))) {
                                ac.put(followKey.get(r), "Reduce " + Integer.toString(rulNo));
                                } else {
                                    System.out.println("Conflict while filling parse table at " + s + " on " + followKey.get(r));
                                    Conflict = 1;
                                    return;
                                }
                            }
                        }
                    }
                } else {
                    HashMap<String, String> transfer = links.get(s);
                    for (String p : transfer.keySet()) {
                        if (non.contains(p)) {
                            if (!ac.containsKey(p) || ac.get(p).equals("Shift " + transfer.get(p))) {
                            ac.put(p, "Shift " + transfer.get(p));
                            } else {
                                System.out.println("Conflict while filling parse table at " + s + " on " + p);
                                Conflict = 1;    
                                return;
                            }
                        } else {
                            if (!go.containsKey(p) || go.get(p).equals(transfer.get(p))) {
                            go.put(p, transfer.get(p));
                            } else {
                                System.out.println("Conflict while filling parse table at " + s + " on " + p);
                                Conflict = 1;    
                                return;
                            }
                        }
                    }
                }
            }

            Action.put(s, ac);
            Goto.put(s, go);
        }

        System.out.println("Action and goto table :");

        for (int i = 0; i < non.size(); i++) {
            System.out.print("    " + non.get(i) + " \t\t ");
        }
        for (int i = 0; i < terms.size(); i++) {
            System.out.print("    " + terms.get(i) + " \t\t ");
        }
        System.out.println();
        for (String s : itemset.keySet()) {
            System.out.print(s + "  ");
            HashMap<String, String> tem = Action.get(s);
            for (int i = 0; i < non.size(); i++) {
                if (tem.containsKey(non.get(i))) {
                    System.out.print(tem.get(non.get(i)) + " \t ");
                } else {
                    System.out.print("\t\t");
                }
            }
            tem = Goto.get(s);
            for (int i = 0; i < terms.size(); i++) {
                if (tem.containsKey(terms.get(i))) {
                    System.out.print(tem.get(terms.get(i)) + "\t\t");
                } else {
                    System.out.print("\t\t ");
                }
            }
            System.out.println();
        }

    }

   public static void firstFind(String s) {

        List<String> l = hDup.get(s);
        List<String> m = null;
        if (first.containsKey(s)) {
            m = first.get(s);
        } else {
            m = new ArrayList<String>();
        }

        for (int i = 0; i < l.size(); i++) {
            String t = l.get(i);
            String temp[] = t.split(" ");
            int cnt = 0;
            if (non.contains(temp[0]) && !m.contains(temp[0]) && !m.equals("ε")) {
                m.add(temp[0]);
            } else if (temp[0].equals("ε")) {
                cnt++;
            } else if (terms.contains(temp[0])) {
                for (int j = 0; j < temp.length; j++) {
                    
                    if (non.contains(temp[j])) {
                        m.add(temp[j]);
                        break;
                    }
                    firstFind(temp[j]);

                    List<String> t2 = first.get(temp[j]);
                    for (int p = 0; p < t2.size(); p++) {
                        String hl = t2.get(p);
                        if (!m.contains(hl) && !hl.equals("ε")) {
                            m.add(hl);
                        }
                    }
                    if (first.get(temp[j]).contains("ε")) {
                        cnt++;
                    } else {
                        break;
                    }

                }
                if (cnt == temp.length) {
                    m.add("ε");
                }
            }
        }
        first.put(s, m);
    }


    public static int followFind(String s) {

        int flag = 0;
        List<String> m = null;
        if (follow.containsKey(s)) {
            m = follow.get(s);
        } else {
            m = new ArrayList<String>();
        }

        Set<String> key = h.keySet();
        for (String i : key) {
            List<String> l = h.get(i);
            for (int j = 0; j < l.size(); j++) {
                String temp[] = l.get(j).split(" ");

                for (int k = 0; k < temp.length; k++) {
                    if (temp[k].equals(s)) {
                        // rule A -> aBb
                        if (k < temp.length - 1) {
                            //if non terminal
                            if (non.contains(temp[k + 1])) {
                                if (!m.contains(temp[k + 1])) {
                                    m.add(temp[k + 1]);
                                    flag = 1;
                                }
                            } else { // if terminal
                                List<String> tt = first.get(temp[k + 1]);
                                for (int yy = 0; yy < tt.size(); yy++) {
                                    if (!m.contains(tt.get(yy)) && !tt.get(yy).equals(epsi)) {
                                        m.add(tt.get(yy));
                                        flag = 1;
                                    }
                                }

                                // A -> aBb and first of b contains ε
                                List<String> tt3 = first.get(temp[k + 1]);
                                for (int yy = 0; yy < tt3.size(); yy++) {
                                    if (tt3.get(yy).equals(epsi) && follow.containsKey(i)) {
                                        List<String> tt2 = follow.get(i);
                                        for (int yy2 = 0; yy2 < tt2.size(); yy2++) {
                                            if (!m.contains(tt2.get(yy2)) && !tt2.get(yy2).equals(epsi)) {
                                                m.add(tt2.get(yy2));
                                                flag = 1;
                                            }
                                        }
                                    }
                                }
                            }

                        } else // rule A -> aB
                        {
                            if (follow.containsKey(i)) {
                                List<String> tt = follow.get(i);
                                for (int yy = 0; yy < tt.size(); yy++) {
                                    if (!m.contains(tt.get(yy)) && !tt.get(yy).equals(epsi)) {
                                        m.add(tt.get(yy));
                                        flag = 1;
                                    }
                                }
                            }
                        }

                    }
                }
            }

        }
        follow.put(s, m);
        return flag;
    }

    public static void LRec() {

        
        // for all Ai
        for (int i = 0; i < terms.size(); i++) {

            List<String> newI = new ArrayList<String>();
            // for all Aj, j < i

            for (int j = 0; j < i; j++) {
                // for every production of form Ai -> Aj #
                List<String> l = hDup.get(terms.get(i));

                for (int k = 0; k < l.size(); k++) {
                    String tem = l.get(k);
                    String items[] = tem.split(" ");
                    // finding # from Ai -> Aj #
                    String rem = "";
                    if (tem.indexOf(items[0]) + items[0].length() + 1 < tem.length()) {
                        rem = tem.substring(tem.indexOf(items[0]) + items[0].length());
                    }

                    if (items[0].equals(terms.get(j))) {

                        List<String> p = hDup.get(terms.get(j));

                        for (int y = 0; y < p.size(); y++) {
                            String r = p.get(y) + rem;
                            newI.add(r);

                        }

                    } else if (!newI.contains(tem)) {
                        newI.add(tem);
                    }
                }
            }
            if (i != 0) {
                hDup.put(terms.get(i), newI);
            }
        }

        
        for (int i = 0; i < terms.size(); i++) {

            List<String> l = hDup.get(terms.get(i));
            List<String> newT = new ArrayList<String>();
            List<String> oldT = new ArrayList<String>();

            String termnew = terms.get(i) + "'";
            while (terms.contains(termnew)) {
                termnew += "'";
            }
            int flag2 = 0;
            for (int j = 0; j < l.size(); j++) {
                String s = l.get(j);
                String items[] = s.split(" ");

                if (items[0].equals(terms.get(i))) {
                    flag2 = 1;
                    String temp = s.substring(s.indexOf(items[0]) + items[0].length() + 1);
                    temp += " " + termnew;
                    newT.add(temp);
                } else {
                    String temp;
                    if (items[0].equals(epsi)) {
                        temp = termnew;
                    } else {
                        temp = s + " " + termnew;
                    }
                    oldT.add(temp);
                }
            }
            newT.add(epsi);
            if (flag2 == 1) {
                if (!non.contains(epsi)) {
                    non.add(epsi);
                }
                hDup.replace(terms.get(i), oldT);
                hDup.put(termnew, newT);
                newTerms.add(termnew);
            }

        }
        for (int i = 0; i < newTerms.size(); i++) {
            terms.add(newTerms.get(i));
        }
        

    }

    public static void parse(String str) {
        Stack<String> stack = new Stack<String>();
        int ind = 0;

        String items[] = str.split(" ");
        stack.push("I0");
        int n = items.length;
        while (!stack.empty() && ind < n) {
            String s = stack.peek();
            String a = items[ind];
            //System.out.println(s + " " + a);
            if (!Action.get(s).containsKey(a)) {

                System.out.println("Error");

                /* String er = stack.peek();
                int flag = 0;
                
                while (!terms.contains(er)) {
                    stack.pop();
                    er = stack.peek();
                }
                while (!follow.get(er).contains(items[ind])) {
                    ind++;
                
                }
                System.out.println("error " + er + "  " + items[ind]); */
                return;

            } else {
                String tab = Action.get(s).get(a);

                if (tab.contains("Shift")) {
                    String t[] = tab.split(" ");
                    // t[1] is our new state
                    String sd = t[1]; // s'
                    stack.push(a);
                    stack.push(sd);
                    ind++;
                    System.out.println(tab);

                } else if (tab.contains("Reduce")) {
                    String t[] = tab.split(" ");

                    // t[1] is our new state
                    String r = rules.get(Integer.parseInt(t[1])); // s'
                    String sdit[] = r.split(" ");
                    String A = sdit[0];// A -> Beta
                    String beta = r.substring(r.indexOf("->") + 3);
                    String ite[] = beta.split(" ");
                    int mod = ite.length;
                    for (int j = 0; j < 2 * mod; j++) {
                        stack.pop();
                    }
                    String sd = stack.peek();
                    stack.push(A);
                    stack.push(Goto.get(sd).get(A));
                    System.out.println(r);

                } else if (tab.contains("Accept")) {
                    System.out.println("Accepted");
                    return;
                } else if (tab.contains("Error")) {
                    System.out.println(tab);
                    System.out.println(error.get(tab));
                    if (tab.equals("Error1")) {

                        stack.push("id");
                        String k = Action.get(s).get("id");
                        if (k.contains("Shift")) {
                            String kl[] = k.split(" ");
                            stack.push(kl[1]);
                        } else {
                            
                            
                            
                            System.out.println("ERROR : no shift for id for state " + s);
                            
                            return;
                        }
                    } else if (tab.equals("Error2")) {
                        ind++;
                    } else if (tab.equals("Error3")) {
                        stack.push("+");
                        String k = Action.get(s).get("+");
                        if (k.contains("Shift")) {
                            String kl[] = k.split(" ");
                            stack.push(kl[1]);
                        } else {
                            System.out.println("ERROR : no shift for operator + for state " + s);
                            
                            return;
                        }
                    } else if (tab.equals("Error4")) {
                        
                        stack.push(")");
                       String k = Action.get(s).get(")");
                       
                        if (k.contains("Shift")) {
                            String kl[] = k.split(" ");
                            stack.push(kl[1]);
                        } else {
                            System.out.println("ERROR : no shift for ) for state " + s);
                            
                            return;
                        }
                    }
                }

            }
        }
    }

    public static void errorDefine() {
        error.put("Error1", "Missing operand");
        error.put("Error2", "Unbalanced right paranthesis");
        error.put("Error3", "Missing operator");
        error.put("Error4", "Missing right paranthesis");
    }

    public static void fillError() {

        for (String s : Action.keySet()) {
            HashMap<String, String> temp = Action.get(s);

            if (!temp.containsKey("+") && !temp.containsKey("*") && !temp.containsKey("$")) {
                temp.put("+", "Error1");
                temp.put("*", "Error1");
                temp.put("$", "Error1");
            }
            if (!temp.containsKey(")")) {
                temp.put(")", "Error2");
            }
            if (!temp.containsKey("(") || !temp.containsKey("id")) {
                temp.put("(", "Error3");
                temp.put("id", "Error3");

            }
            if (!temp.containsKey("$")) {
                temp.put("$", "Error4");
            }
            Action.put(s, temp);
        }

        System.out.println("Action and goto table :");

        for (int i = 0; i < non.size(); i++) {
            System.out.print("    " + non.get(i) + " \t\t ");
        }
        for (int i = 0; i < terms.size(); i++) {
            System.out.print("    " + terms.get(i) + " \t\t ");
        }
        System.out.println();
        for (String s : itemset.keySet()) {
            System.out.print(s + "  ");
            HashMap<String, String> tem = Action.get(s);
            for (int i = 0; i < non.size(); i++) {
                if (tem.containsKey(non.get(i))) {
                    System.out.print(tem.get(non.get(i)) + " \t ");
                } else {
                    System.out.print("\t\t");
                }
            }
            tem = Goto.get(s);
            for (int i = 0; i < terms.size(); i++) {
                if (tem.containsKey(terms.get(i))) {
                    System.out.print(tem.get(terms.get(i)) + "\t\t");
                } else {
                    System.out.print("\t\t ");
                }
            }
            System.out.println();
        }
    }
    
}
