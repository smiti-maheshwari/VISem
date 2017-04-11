package lr;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.Arrays;
import static java.util.Collections.sort;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;
import java.util.StringTokenizer;
import java.util.Vector;
import javafx.util.Pair;

public class SLR {
  //  static HashMap<String, Vector> follow = new HashMap<String, Vector>();
    static Vector pro = new Vector();
    static Vector aug = new Vector();
    static Vector items = new Vector();
    static Vector gs = new Vector();
    static Vector term = new Vector();
    static Vector nonterm = new Vector();
    static int augsize, numI, numS;
    static ff ob1;
    static converter ob2;
    static String augment;
    
    static Vector closure(Vector v) {
        int i, j, k, len, sz, flag = 1;
        Vector toret = v;
        while (flag == 1) {
            sz = toret.size();
            flag = 0;
            for (i = 0; i < sz; i++) {
                String each = (String) v.get(i);
                int dot = each.indexOf('.');
                if (dot < each.length() - 1) {
                    char afterdot = each.charAt(dot + 1);

                    for (j = 0; j < augsize; j++) {
                        String prod = (String) aug.get(j);
                        String toadd = "" + afterdot + " .";
 //                       System.out.println("For production: " + prod);
                        len = prod.length();
                        if (prod.charAt(0) == afterdot) {
                            for (k = 0; k < len; k++) {
                                if (prod.charAt(k) == ' ') {
                                    break;
                                }
                            }
                            for (k = k + 1; k < len; k++) {
                                toadd = toadd + prod.charAt(k);
                            }
                        }
    //                    System.out.println("to add is: " + toadd);
                        if (!toadd.equals("" + afterdot + " .") && !toret.contains(toadd)) {
                            flag = 1;
                            toret.add(toadd);
                        } else if((prod.charAt(0) == afterdot) && toadd.equals("" + afterdot + " .") && (len==2) && !toret.contains(toadd)) {
                            flag = 1;
                            toret.add(toadd);
                        }
                    }
                }
            }
        }
        return toret;
    }

    // check contains method;
    static void find_items(String fn) {
        Vector v = new Vector();
        v.add(augment+" ." + fn);
        items.add(closure(v));

        int i, j, k, l, sz, szz = gs.size(), flag = 1;
        char c;
        Vector I;

        while (flag == 1) {
            flag = 0;
            sz = items.size();
            for (i = 0; i < sz; i++) {
                I = (Vector) items.get(i);
                for (j = 0; j < szz; j++) {
                    c = (char) gs.get(j);
                    int yesfc = 0;
                    int si = I.size();
                    for (k = 0; k < si; k++) {
                        String prod = (String) I.get(k);
                        int dot = prod.indexOf('.');
                        if (dot < prod.length() - 1) {
                            char afterdot = prod.charAt(dot + 1);

                            if (afterdot == c) {
                                yesfc = 1;
                                break;
                            }
                        }
                    }
                    if (yesfc == 1) {
                        Vector vv = new Vector();
                        for (k = 0; k < si; k++) {
                            String prod = (String) I.get(k);
                            int dot = prod.indexOf('.');
                            if (dot < prod.length() - 1) {
                                char afterdot = prod.charAt(dot + 1);
                                if (afterdot == c) {
                                    String next = "";
                                    for (l = 0; l < prod.length(); l++) {
                                        if (prod.charAt(l) == '.') {
                                            next = next + afterdot + '.';
                                            l++;
                                        } else {
                                            next = next + prod.charAt(l);
                                        }
                                    }
                                    vv.add(next);
                                }
                            }
                        }
                        vv = closure(vv);
                        if (!items.contains(vv)) {
                            flag = 1;
                            items.add(vv);
                        }
                    }
                }
            }
        }
    }

    static boolean isterminal(char c) {
        if (((c >= 'a') && (c <= 'z')) || (c == '#') || (c == ')' || c == '(' || c == '$' || c == '+' || c == '-' || c == '*' || c == '/')) {
            return true;
        }
        return false;
    }

    static boolean faltu(char c) {
        if ((c!='\t') && (c != ' ') && (c != ',') && (c!='#')) {
            return false;
        }
        return true;
    }

    static boolean inkernel(Vector vv, int k) {
        int j, sz = vv.size(), cnt = 0;
        for (j = 0; j < sz; j++) {
            String s = (String) vv.get(j);
            Vector v = (Vector) items.get(k);
            String f = (String) v.get(0);
            int dot = f.indexOf('.');

            char predotchar = f.charAt(dot - 1);

            for (int i = 0; i < v.size(); i++) {
                f = (String) v.get(i);
                dot = f.indexOf('.');
                char dotchar = f.charAt(dot - 1);

                if (dotchar != predotchar) {
                    break;
                }

                if (s.equals(f)) {
                    cnt++;
                }
            }
        }

        if (cnt == sz) {
            return true;
        } else {
            return false;
        }
    }

    public static void main(String[] args) throws FileNotFoundException, IOException {
        ob2 = new converter();
        ob1 = new ff();
       /* FileReader fr = new FileReader("follows.txt");
        BufferedReader br = new BufferedReader(fr);

        String ss;
        while ((ss = br.readLine()) != null) {
            char of = ss.charAt(0);

            follow.put("" + of, new Vector());

            int i = ss.indexOf('>');

            ss = ss.substring(i + 2);

            StringTokenizer st = new StringTokenizer(ss, ",[] ");
            while (st.hasMoreTokens()) {
                String tk = st.nextToken();
                System.out.println("token was: " + tk);
                (follow.get("" + of)).add(tk);
            }
        }

        fr.close();
        br.close();*/

        //////////////////////////////////////////////
        FileReader fr1 = new FileReader("left_factored.txt");
        BufferedReader br1 = new BufferedReader(fr1);

        int i, j, k, n;
        String s,fn=ob1.fsym;
        
        for(i=0;i<26;i++) {
            if(ob2.done2[i]==0) {
                augment = "" + (char)(i+'A');
                break;
            }
        }
        
        char c;
        while ((s = br1.readLine()) != null) {
            while(s.contains("#")) s = s.replace("#","");
            n = s.length();
            String non = "";
            String lhs = "";
            for (i = 0; i < n; i++) {
                c = s.charAt(i);
                if (!faltu(c) && !gs.contains(c)) {
                    gs.add(c);
                }
                if (isterminal(c)) {
                    if (!faltu(c) && !term.contains(c)) {
                        term.add(c);
                    }
                } else {
                    if (!faltu(c) && !nonterm.contains(c)) {
                        nonterm.add(c);
                    }
                }
                if ((s.charAt(i) == '\t') || (s.charAt(i)==' ')) {
                    break;
                } else {
                    non = non + s.charAt(i);
                }
            }
            if (fn == "") {
                fn = non;
            }
            for (i = i + 1; i < n; i++) {
                c = s.charAt(i);
                if (!faltu(c) && !gs.contains(c)) {
                    gs.add(c);
                }
                if (isterminal(c)) {
                    if (!faltu(c) && !term.contains(c)) {
                        term.add(c);
                    }
                } else {
                    if (!faltu(c) && !nonterm.contains(c)) {
                        nonterm.add(c);
                    }
                }
                if (s.charAt(i) == ',') {
                    pro.add(non + " " + lhs);
                    aug.add(non + " " + lhs);
                    lhs = "";
                } else {
                    lhs = lhs + s.charAt(i);
                }
            }
            pro.add(non + " " + lhs);
            aug.add(non + " " + lhs);
        }
        System.out.println("Productions : " + pro);
        br1.close();
        fr1.close();

        System.out.println("Adding augmented rule " + augment+" -> " + fn);
        aug.add(augment + " " + fn);
/*
        ob1.follow.put("S", new Vector());
        ob1.follow.get("S").add("$");
  */      
        System.out.println("Augmented grammar : " + aug);
        augsize = aug.size();

        gs.add('$');
        numS = gs.size();
        System.out.println("Grammar Symbols: " + gs);
        System.out.println("Terminals: " + term);
        System.out.println("Non Terminals: " + nonterm);

        find_items(fn);
        System.out.println("Items are: " + items);

        numI = items.size();

        Vector mat[][] = new Vector[numI][numS];
        String conflict[][] = new String[numI][numS];
        
        for (i = 0; i < numI; i++) {
            for (j = 0; j < numS; j++) {
                mat[i][j] = new Vector();
                conflict[i][j] ="";
            }
        }
        int done[] = new int[numS];
        for (i = 0; i < numI; i++) {
            Vector I = (Vector) items.get(i);
            int sz = I.size();

            Arrays.fill(done, 0);
//            System.out.println("Checking item : " + I);

            for (k = 0; k < numS; k++) {
                char sym = (char) gs.get(k);
                Vector v = new Vector();

                for (j = 0; j < sz; j++) {
                    String prod = (String) I.get(j);
                    int dot = prod.indexOf('.');
                    if (dot < prod.length() - 1) {
                        char afterdot = prod.charAt(dot + 1);
                        if (afterdot == sym) {
                            char cc[] = prod.toCharArray();
                            char temp = cc[dot];
                            cc[dot] = cc[dot + 1];
                            cc[dot + 1] = temp;
                            prod = new String(cc);
                            v.add(prod);
                        }
                    }
                }
                if (!v.isEmpty()) {
//                    System.out.println("checking for symbols: " + v);
                    for (j = 0; j < numI; j++) {
                        if (inkernel(v, j)) {
                            if (!mat[i][gs.indexOf(sym)].contains("s " + j)) {
                                mat[i][gs.indexOf(sym)].add("s " + j);
                            }
                            break;
                        }
                    }
                }
            }
            for (j = 0; j < sz; j++) {
                String prod = (String) I.get(j);

//                System.out.println("For string: " + prod);
                if (prod.equals(augment+ " " + fn + ".")) {
                    mat[i][gs.indexOf('$')].add("acc");
                } else {
                    int dot = prod.indexOf('.');
                    if (dot == prod.length() - 1) {
                        s = "" + prod.charAt(0);
                        Vector v = ob1.follow.get(s);
                        for (k = 0; k < v.size(); k++) {
                            mat[i][gs.indexOf(v.get(k))].add("r " + prod);
                        }
                    } else {
                        
                        /*char cc[] = prod.toCharArray();
                         char temp = cc[dot];
                         cc[dot] = cc[dot + 1];
                         cc[dot + 1] = temp;
                         temp = cc[dot];
                         prod = new String(cc);
                         System.out.println("Prod " + prod);
                         for (k = 0; k < numI; k++) {
                         if (inkernel(prod,k)) {
                         if (!mat[i][gs.indexOf(temp)].contains("s " + k)) {
                         mat[i][gs.indexOf(temp)].add("s " + k);
                         }
                         break;
                         }
                         }*/
                    }
                }
            }
        }
        System.out.print("      ");
        for (j = 0; j < numS; j++) {
            System.out.print(gs.get(j) + "      ");
        }
        System.out.println();

        for (i = 0; i < numI; i++) {
            System.out.print(i + " :    ");
            for (j = 0; j < numS; j++) {
                System.out.print(mat[i][j] + "     ");
            }
            System.out.println();
        }

        for(i=0;i<numI;i++) {
            for(j=0;j<numS;j++) {
                if(mat[i][j].size()>1) {
                    System.out.println("There is a conflict.");
                    String act1 = (String) mat[i][j].get(0);
                    String act2 = (String) mat[i][j].get(1);
                    if(act1.charAt(0)=='s' && act2.charAt(0)=='s') {
                        System.out.println("shift-shift conflict");
                        conflict[i][j] = "shift-shift";
                    } else if((act1.charAt(0)=='r' && act2.charAt(0)=='s') || (act1.charAt(0)=='s' && act2.charAt(0)=='r')) {
                        System.out.println("shift-reduce conflict");
                        conflict[i][j] = "shift-reduce";
                    } else if(act1.charAt(0)=='r' && act2.charAt(0)=='r'){
                        System.out.println("reduce-reduce conflict");
                        conflict[i][j] = "reduce-reduce";
                    }
                }
            }
        }
        
        System.out.println("Enter string to be checked : ");
        Scanner scan = new Scanner(System.in);
        String sss = scan.next();
        sss = ob2.convert(sss);
        sss = sss + '$';
        System.out.println("String is: " + sss);
        Stack st = new Stack();
        st.push("c$");
        st.push("n0");

        j = 0;
        int state = 0;
        char stt, cur = 0;
        try {
        while (true) {
            s = (String) st.peek();
            cur = sss.charAt(j);
            System.out.println("Top : " + s + " cur: " + cur);

            if (s.charAt(0) == 'c') {
                stt = s.charAt(1);
            } else {
                state = Integer.parseInt(s.substring(1));
                System.out.println("State: " + state);
            }
            if(!conflict[state][gs.indexOf(cur)].equals("")) System.out.println("There is a " + conflict[state][gs.indexOf(cur)] + " conflict.");
            String act = (String) mat[state][gs.indexOf(cur)].get(0);
            System.out.println("Act: " + act);

            if (act.contains("r ")) {
                char pre = 0;
                int b = 0, len = act.length() - 1;
                for (i = 0; i < len; i++) {
                    if (act.charAt(i) == ' ') {
                        break;
                    }

                }
                i++;
                for (; i < len; i++) {
                    if (act.charAt(i) == ' ') {
                        break;
                    }
                    pre = act.charAt(i);
                }
                i++;
  //              System.out.println("Char at i : " + i + " " + act.charAt(i) + " len: " + len);

                b = len - i;
                b *= 2;
  //              System.out.println("B " + b);
                while (b != 0) {
                    b--;
                    st.pop();
                }
                s = (String) st.peek();
                cur = sss.charAt(j);
                if (s.charAt(0) == 'c') {
                    stt = s.charAt(1);
                } else {
                    state = Integer.parseInt(s.substring(1));
                }
                System.out.println("New state : " + state + " pre: " + pre);
                st.push("c" + pre);
                if(!conflict[state][gs.indexOf(pre)].equals("")) System.out.println("There is a " + conflict[state][gs.indexOf(pre)] + " conflict.");
                act = (String) mat[state][gs.indexOf(pre)].get(0);
                 System.out.println("Act: " + act);
                st.push("n" + act.substring(2));
            } else if (act.contains("s ")) {
                st.push("c" + cur);
                st.push("n" + act.substring(2));
                j++;
            } else if (act.equals("acc") && (j >= sss.length() - 1)) {
                System.out.println("Accepted");
                break;
            } else {
                System.out.println("Error at symbol "  + ob2.rev1.get(""+cur));
                System.out.println("Rejected.");
                break;
            }
        }
       } catch(Exception e) {
           System.out.println("Error at symbol "  + ob2.rev1.get(""+cur));
            System.out.println("Rejected.");
        }
    }
}
