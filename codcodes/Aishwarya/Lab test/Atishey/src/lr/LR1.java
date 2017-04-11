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

public class LR1 {
   // static HashMap<String, Vector> first = new HashMap<String, Vector>();
    static Vector pro = new Vector();
    static Vector aug = new Vector();
    static Vector items = new Vector();
    static Vector gs = new Vector();
    static Vector term = new Vector();
    static Vector nonterm = new Vector();
    static int augsize, numI, numS;
    static converter ob1;
    static ff ob2;
    static String augment;
    
    static Vector find_first(String s)
    {
   //     System.out.println("String is : " + s);
        int i,j,len = s.length();
        Vector v = new Vector();
        
        for(i = 0;i<len;i++) {
            if(isterminal(s.charAt(i))) {
                v.add(""+s.charAt(i));
                break;
            } else {
                int flag=0;
                Vector vv = ob2.first.get(""+s.charAt(i));
                
                for(j = 0;j<vv.size();j++) {
                    String sv = "" + vv.get(j);
                    if(!sv.equals("#")) v.add(sv);
                    else flag=1;
                }
                if(flag==1) {
                    if(i==len-1) {
                        v.add('#');
                        break;
                    }
                } else break;
            }
        }
 //       System.out.println(v);
        return v;
    }
    
    static Vector closure(Vector v) {
        int i, j, k, len, sz, flag = 1;
        Vector toret = v;
        while (flag == 1) {
            sz = toret.size();
            flag = 0;
  //          System.out.println("gagahkasjdk");
            for (i = 0; i < sz; i++) {
                String each = (String) v.get(i);
   //             System.out.println("For production: " + each);
                int dot = each.indexOf('.');
                int comma = each.indexOf(',');
                String forf = "";
                for(j=dot+2;j<each.length();j++){
                    if(each.charAt(j)!=',') forf = forf + each.charAt(j);
                }
                Vector vv = find_first(forf);
                if (dot < each.length() - 3) {
                    char afterdot = each.charAt(dot + 1);

                    for (j = 0; j < augsize; j++) {
                        String prod = (String) aug.get(j);
                        
                        String toadd = "" + afterdot + " .";
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
                       
                        if (!toadd.equals("" + afterdot + " .")) {
                            String add;
                            for(k=0;k<vv.size();k++) {
                                add = toadd;
                                add = add + "," + vv.get(k);
                                
                                if(!toret.contains(add)) {
                                    toret.add(add);
                                    flag = 1;
  //                                  System.out.println("added: " + add);
                                }
                            }
                        } else if((prod.charAt(0) == afterdot) && toadd.equals("" + afterdot + " .") && (len==2)) {
                            String add;
                            for(k=0;k<vv.size();k++) {
                                add = toadd;
                                add = add + "," + vv.get(k);
                                
                                if(!toret.contains(add)) {
                                    toret.add(add);
                                    flag = 1;
 //                                   System.out.println("added: " + add);
                                }
                            }
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
        v.add(augment+ " ." + fn + ",$");
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
        if ((c!='\t') &&(c != ' ') && (c != ',')) {
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
    
    /*static boolean inkernel(String s,int k)
    {
        Vector v = (Vector) items.get(k);
        String f = (String) v.get(0);
        int dot= f.indexOf('.');
        
        char predotchar = f.charAt(dot-1);
        
        for(int i=0;i<v.size();i++) {
            f = (String) v.get(i);
            dot= f.indexOf('.');
            char dotchar = f.charAt(dot-1);
            
            if(dotchar!=predotchar) break;
            
            if(s.equals(f)) return true;
        }
        
        return false;
    }*/

    public static void main(String[] args) throws FileNotFoundException, IOException {
        ob1 = new converter();
        ob2 = new ff();
        /*FileReader fr = new FileReader("firsts.txt");
        BufferedReader br = new BufferedReader(fr);

        String ss;
        while((ss = br.readLine()) != null) {
            char of = ss.charAt(0);
            
            first.put(""+of, new Vector());
            
            int i = ss.indexOf('>');
            
            ss = ss.substring(i+2);
            
            StringTokenizer st = new StringTokenizer(ss,",[] ");
            while(st.hasMoreTokens()) {
                String tk = st.nextToken();
                System.out.println("token was: " + tk);
                (first.get(""+of)).add(tk);
            }
        }
        
        fr.close();
        br.close();
        */
        
        //////////////////////////////////////////////
        FileReader fr1 = new FileReader("left_factored.txt");
        BufferedReader br1 = new BufferedReader(fr1);

        int i,j,k, n;
        String s, fn=ob2.fsym;
        
        for(i=0;i<26;i++) {
            if(ob1.done2[i]==0) {
                augment = "" + (char)(i+'A');
                break;
            }
        }
       /* System.out.println("Enter start symbol: ");
        Scanner scan = new Scanner(System.in);
        fn = scan.next();
        fn = ob1.rev2.get(fn);
        */char c;
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

        System.out.println("Adding augmented rule "+augment + " -> " + fn);
        aug.add(augment + " " + fn);

        System.out.println("Augmented grammar : " + aug);
        augsize = aug.size();

        gs.add('$');
        term.add('$');
        numS = gs.size();
        System.out.println("Grammar Symbols: " + gs);
        System.out.println("Terminals: " + term);
        System.out.println("Non Terminals: " + nonterm);

        find_items(fn);
        System.out.println("items : " + items);
        
        numI = items.size();

        Vector mat[][] = new Vector[numI][numS];
        String conflict[][] = new String[numI][numS];
        
        for (i = 0; i < numI; i++) {
            for (j = 0; j < numS; j++) {
                mat[i][j] = new Vector();
                 conflict[i][j] = "";
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
                    if (dot < prod.length() - 3) {
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
                if (prod.equals(augment + " " + fn + ".,$")) {
                    mat[i][gs.indexOf('$')].add("acc");
                } else {
                    int dot = prod.indexOf('.');
                    if (dot == prod.length() - 3) {
                        //s = "" + prod.charAt(prod.length()-1);
                        
                        mat[i][gs.indexOf(prod.charAt(prod.length()-1))].add("r " + prod.substring(0, prod.length()-2));
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
                        System.out.println("reducr-reduce conflict");
                        conflict[i][j] = "reduce-reduce";
                    }
                }
            }
        }
        
        System.out.println("Enter string to be checked : ");
        Scanner scan = new Scanner(System.in);
        String sss = scan.next();
        sss = ob1.convert(sss);
        sss = sss + '$';
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
            System.out.println("Top : " + s  + " cur: " + cur);
            
            if (s.charAt(0) == 'c') {
                stt = s.charAt(1);
            } else {
                state = Integer.parseInt(s.substring(1));
                System.out.println("State: " + state);
            }
           if(!conflict[state][gs.indexOf(cur)].equals("")) System.out.println("There is a " + conflict[state][gs.indexOf(cur)] + " conflict.");
            String act = (String) mat[state][gs.indexOf(cur)].get(0);
            System.out.println("Act: "  + act);
            
            if (act.contains("r ")) {
                char pre = 0;
                int b = 0, len = act.length()-1;
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
    //            System.out.println("Char at i : " + i + " " + act.charAt(i) + " len: " + len);
                
                b = len-i;
                b *= 2;
   //             System.out.println("B " + b);
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
            } else if (act.equals("acc") && (j >= sss.length()-1)) {
                System.out.println("Accepted");
                break;
            } else {
                System.out.println("Error at symbol "  + ob1.rev1.get(""+cur));
                System.out.println("Rejected.");
                break;
            }
        }
        } catch(Exception e) {
            System.out.println("Error at symbol "  + ob1.rev1.get(""+cur));
            System.out.println("Rejected." + e);
        }
        
    }
}
