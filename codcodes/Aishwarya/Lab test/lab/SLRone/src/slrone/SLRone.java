package slrone;

import firstfollow.Fstfle;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;
import java.util.Stack;

public class SLRone {

    static int i, j, k, m, n = 0, o, p = -1, ns = 0, tn = 0, rr = 0, ch = 0;
    static char[][] read, dfa = null;
    static String[] gr, tempr = null;
    static char[] gl, templ, temp2, nont, t = null;
    static char temp, str1 = 0;
    static int tlen = 0, ntlen = 0, augs = -1;
    static String[][] action, Goto = null;

    static String swap(String s, int a, int b) {
        char[] carr = s.toCharArray();
        char tmp = carr[a];
        carr[a] = carr[b];
        carr[b] = tmp;
        return new String(carr);
    }

    public static class states {

        char[] lhs = null;
        String[] rhs = null;
        int n;

        states() {
            lhs = new char[15];
            rhs = new String[15];
            n = 0;
        }
    }
    static states[] I = null;

    static int compstruct(states s1, states s2) {
        int t, q;
        if (s1.n != s2.n) {
            return 0;
        }
        for (q = 0; q < s1.n; q++) {
            if (!(s1.lhs[q] == s2.lhs[q])) {
                return 0;
            }
        }
        for (t = 0; t < s1.n; t++) {
            if (!(s1.rhs[t].equals(s2.rhs[t]))) {
                return 0;
            }
        }
        return 1;
    }

    static void moreprod() {
        int r, s, t, l1 = 0, rr1 = 0;
        char[][] read1 = new char[15][10];

        for (r = 0; r < I[ns].n; r++) {
            t = I[ns].rhs[l1].indexOf('.');
            if (t + 1 == I[ns].rhs[l1].length()) {
                l1++;
                continue;
            }
            temp = I[ns].rhs[l1].charAt(t + 1);
            l1++;
            for (s = 0; s < rr1; s++) {
                if (temp == read1[s][0]) {
                    break;
                }
            }
            if (s == rr1) {
                read1[rr1][0] = temp;
                rr1++;
            } else {
                continue;
            }

            for (s = 0; s < n; s++) {
                if (gl[s] == temp) {
                    I[ns].rhs[I[ns].n] = "";
                    I[ns].rhs[I[ns].n] = I[ns].rhs[I[ns].n].concat(".");
                    I[ns].rhs[I[ns].n] = I[ns].rhs[I[ns].n].concat(gr[s]);
                    I[ns].lhs[I[ns].n] = gl[s];
                    I[ns].lhs[I[ns].n + 1] = '\0';
                    I[ns].n++;
                }
            }
        }
    }

    static void canonical(int l) {
        int t1;
        char[][] read1 = new char[15][10];
        temp2 = new char[2];
        templ = new char[15];
        tempr = new String[15];
        char rr1 = 0;
        int index;
        for (i = 0; i < I[l].n; i++) {
            temp2[0] = '.';
            index = I[l].rhs[i].indexOf('.');
            if (index + 1 == I[l].rhs[i].length()) {
                continue;
            }
            temp2[1] = I[l].rhs[i].charAt(index + 1);
            String tep = new String(temp2);
            for (j = 0; j < rr1; j++) {
                if (read1[j].equals(tep)) {
                    break;
                }
            }

            if (j == rr1) {
                read1[rr1] = tep.toCharArray();
                rr1++;
            } else {
                continue;
            }

            for (j = 0; j < I[l].n; j++) {
                boolean t = I[l].rhs[j].contains(tep);
                if (t == true) {
                    templ[tn] = I[l].lhs[j];
                    tempr[tn] = I[l].rhs[j];
                    tn++;
                }
            }

            for (j = 0; j < tn; j++) {
                p = tempr[j].indexOf('.');
                tempr[j] = swap(tempr[j], p, (p + 1));
                I[ns].lhs[I[ns].n] = templ[j];
                I[ns].lhs[I[ns].n + 1] = '\0';
                I[ns].rhs[I[ns].n] = tempr[j];
                I[ns].n++;
            }

            moreprod();
            for (j = 0; j < ns; j++) {
                index = -1;
                if (compstruct(I[ns], I[j]) == 1) {
                    I[ns].lhs[0] = '\0';
                    for (k = 0; k < I[ns].n; k++) {
                        I[ns].rhs[k] = null;
                    }
                    I[ns].n = 0;
                    dfa[l][j] = temp2[1];
                    //if (l == 5) {
                       // System.out.println(3 + " " + temp2[1]);
                    //}
                    break;
                }
            }
            if (j < ns) {
                tn = 0;
                for (j = 0; j < 15; j++) {
                    templ[j] = '\0';
                    tempr[j] = null;
                }
                continue;
            }

            dfa[l][j] = temp2[1];
            System.out.println("-------------------I[" + ns + "]----------------------------");
            for (j = 0; j < I[ns].n; j++) {
                if (I[ns].lhs[j] == 'Z' && I[ns].rhs[j].equals(str1 + ".")) {
                    augs = ns;
                }

                System.out.println("\t" + I[ns].lhs[j] + " -> " + I[ns].rhs[j]);
            }
            //getchar();
            ns++;
            tn = 0;
            for (j = 0; j < 15; j++) {
                templ[j] = '\0';
                tempr[j] = null;
            }
        }
    }

    static void parse(String str) {
        Stack<String> st = new Stack<String>();
        str = str + "$";
        st.push("0");
        String next_state = " ";
        int i = 0;
        while (!next_state.equals("acc")) {
            char c = str.charAt(i);
            if (new String(t).indexOf(c) == -1) {
                System.out.println("Error has occured as (" + st.peek() + "," + c + ") is not present.");
                break;
            }
            next_state = action[Integer.parseInt(st.peek())][new String(t).indexOf(c)];
            if (next_state == null) {
                System.out.println("Error has occured at (" + st.peek() + "," + c + ") as this place in table is empty.");
            }
            if (next_state.charAt(0) == 'S') {
                st.push(Character.toString(str.charAt(i)));
                st.push(Character.toString(next_state.charAt(1)));
                i++;
            } else if (next_state.charAt(0) == 'R') {
                int r_state = Integer.parseInt(Character.toString(next_state.charAt(1)));
                System.out.println(gl[r_state] + " -> " + gr[r_state]);
                int popped_len = 2 * gr[r_state].length();
                while (popped_len != 0) {
                    st.pop();
                    popped_len--;
                }
                int q = Integer.parseInt(st.peek());
                st.push(Character.toString(gl[r_state]));
                int r = new String(nont).indexOf(gl[r_state]);
                st.push(Goto[q][r]);
            }
        }
    }

    public static void main(String[] args) throws FileNotFoundException, IOException {
        File file = new File("example.txt");
        Scanner s = new Scanner(file);
        I = new states[15];
        int index = 0;
        for (i = 0; i < I.length; i++) {
            I[i] = new states();
        }
        gl = new char[15];
        gr = new String[15];
        read = new char[15][10];
        dfa = new char[25][25];
        nont = new char[25];
        t = new char[25];
        Goto = new String[25][25];
        action = new String[25][25];
        //read, tempr, dfa
        while (s.hasNextLine()) {
            String str = s.nextLine();
            if (n == 0) {
                str1 = str.charAt(0);
            }
            String c = String.valueOf(str.charAt(0));
            if (!(new String(nont).contains(c))) {
                nont[ntlen++] = str.charAt(0);
            }
            gl[n] = str.charAt(0);
            gr[n] = str.substring(str.indexOf("= ") + 2);
            n++;
        }
        s = new Scanner(file);
        while (s.hasNextLine()) {
            String str = s.nextLine();
            if (!str.trim().isEmpty()) {
                char tempu[] = str.toCharArray();
                for (int k = 0; k < tempu.length; k++) {
                    String c = String.valueOf(tempu[k]);
                    if (!(new String(nont).contains(c)) && tempu[k] != ' ' && tempu[k] != '=' && tempu[k] != '|' && !(new String(t).contains(c))) {
                        t[tlen++] = tempu[k];
                    }
                }
            }
        }
        t[tlen++] = '$';
        int l;
        System.out.println("THE GRAMMAR IS AS FOLLOWS");
        for (i = 0; i < n; i++) {
            System.out.println(gl[i] + " -> " + gr[i]);
        }
        System.out.println("--------------------------------------");
        I[0].lhs[0] = 'Z';
        I[0].rhs[0] = "." + Character.toString(str1);
        I[0].n++;
        l = 0;

        for (i = 0; i < n; i++) {
            temp = I[0].rhs[l].charAt(1);
            l++;
            for (j = 0; j < rr; j++) {
                if (temp == read[j][0]) {
                    break;
                }
            }
            if (j == rr) {
                read[rr][0] = temp;
                rr++;
            } else {
                continue;
            }
            for (j = 0; j < n; j++) {
                if (gl[j] == temp) {
                    I[0].rhs[I[0].n] = "";
                    I[0].rhs[I[0].n] = I[0].rhs[I[0].n].concat(".");
                    I[0].rhs[I[0].n] = I[0].rhs[I[0].n].concat(gr[j]);
                    I[0].lhs[I[0].n] = gl[j];
                    I[0].n++;
                }
            }
        }
        ns++;

        System.out.println("I[" + (ns - 1) + "]");
        for (i = 0; i < I[0].n; i++) {
            System.out.println("\t" + I[0].lhs[i] + " -> " + I[0].rhs[i]);
        }

        for (l = 0; l < ns; l++) {
            canonical(l);
        }

        for (i = 0; i < ns; i++) {
            for (j = 0; j < ns; j++) {
                if (dfa[i][j] != '\0') {
                    index = new String(nont).indexOf(dfa[i][j]);
                    if (index != -1) {
                        Goto[i][index] = Integer.toString(j);
                    }
                    index = new String(t).indexOf(dfa[i][j]);
                    if (index != -1) {
                        action[i][index] = "S" + Integer.toString(j);
                    }
                }
            }
        }
        index = new String(t).indexOf('$');
        action[augs][index] = "acc";

        //LR0
        /*for (i = 0; i < ns; i++) {
         for (j = 0; j < I[i].n; j++) {
         if (I[i].rhs[j].indexOf('.') == I[i].rhs[j].length() - 1) {
         for (k = 0; k < n; k++) {
         String spr = I[i].rhs[j].substring(0, I[i].rhs[j].length() - 1);
         if (gl[k] == I[i].lhs[j] && gr[k].equals(spr) && i != augs) {
         for (int p = 0; p < tlen; p++) {
         action[i][p] = "R" + Integer.toString(k);
         }
         }
         }
         }
         }
         }*/
        //SLR1
        Fstfle f = new Fstfle();
        f.main(args);
        for (i = 0; i < ns; i++) {
            for (j = 0; j < I[i].n; j++) {
                if (I[i].rhs[j].indexOf('.') == I[i].rhs[j].length() - 1) {
                    for (k = 0; k < n; k++) {
                        String spr = I[i].rhs[j].substring(0, I[i].rhs[j].length() - 1);
                        if (gl[k] == I[i].lhs[j] && gr[k].equals(spr) && i != augs) {
                            String follow = f.flw[new String(nont).indexOf(gl[k])];
                            char[] tp = follow.toCharArray();
                            for (int p = 0; p < tp.length; p++) {
                                index = new String(t).indexOf(tp[p]);
                                // System.out.println(i +" " + j +" " + k);
                                if (action[i][index] == null) {
                                    action[i][index] = "R" + Integer.toString(k);
                                } else {
                                    System.out.println("Error");
                                }
                            }
                        }
                    }
                }
            }
        }

        System.out.println("------------------------TABLE----------------------------------");
        System.out.print("\t");
        for (i = 0; i < tlen; i++) {
            System.out.print(t[i] + "\t");
        }
        System.out.println("");
        for (i = 0; i < ns; i++) {
            System.out.print(i + "\t");
            for (j = 0; j < tlen; j++) {
                if (action[i][j] == null) {
                    System.out.print("-" + "\t");
                } else {
                    System.out.print(action[i][j] + "\t");
                }
            }
            System.out.println("");
        }
        System.out.println("------------------------GOTO----------------------------------");
        System.out.print("\t");
        for (i = 0; i < ntlen; i++) {
            System.out.print(nont[i] + "\t");
        }
        System.out.println("");
        for (i = 0; i < ns; i++) {
            System.out.print(i + "\t");
            for (j = 0; j < ntlen; j++) {
                if (Goto[i][j] == null) {
                    System.out.print("-" + "\t");
                } else {
                    System.out.print(Goto[i][j] + "\t");
                }
            }
            System.out.println("");
        }

        s = new Scanner(System.in);
        String ip = s.next();
        parse(ip);
    }
}
