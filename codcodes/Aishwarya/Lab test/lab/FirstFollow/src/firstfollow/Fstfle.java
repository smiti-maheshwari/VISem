package firstfollow;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;
import java.util.Scanner;
import java.util.Stack;

public class Fstfle {

    static int tlen = 0, ntlen = 0;
    static char[] nont = new char[100];
    static char[] t = new char[100];
    static public String fst[], flw[], grmr[][], table[][];
    static int a[];

    static void readfile() throws FileNotFoundException {
        File f = new File("in4.txt");
        Scanner s = new Scanner(f);
        int index;
        while (s.hasNextLine()) {
            String str = s.nextLine();
            if (!str.trim().isEmpty()) {
                if (str.contains("=")) {
                    nont[ntlen] = str.charAt(0);
                    ntlen++;
                }
            }
        }
        grmr = new String[ntlen][15];
        a = new int[ntlen];
        s = new Scanner(f);
        int i = -1;
        int j = 0;
        while (s.hasNextLine()) {
            String str = s.nextLine();
            if (!str.trim().isEmpty()) {
                char temp[] = str.toCharArray();
                for (int k = 0; k < temp.length; k++) {
                    String c = String.valueOf(temp[k]);
                    if (!(new String(nont).contains(c)) && temp[k] != ' ' && temp[k] != '=' && temp[k] != '|' && !(new String(t).contains(c))) {
                        t[tlen++] = temp[k];
                    }
                }
                if (str.contains("=")) {
                    index = str.indexOf('=');
                    if (str.charAt(index + 1) == ' ') {
                        index++;
                    }
                    index++;
                    String num = String.valueOf(j);
                    //System.out.println(num);
                    i++;
                    if (i != 0) {
                        a[i - 1] = j;
                    }
                    j = 0;
                    grmr[i][j++] = str.substring(index);

                } else if (str.contains("|")) {
                    index = str.indexOf('|');
                    if (str.charAt(index + 1) == ' ') {
                        index++;
                    }
                    index++;
                    //System.out.println(i+ " " + j);
                    grmr[i][j++] = str.substring(index);
                }
            }
        }
        t[tlen++] = '$';
        a[i] = j;
    }

    static String follow(int i, int a[]) {
        char pro[], chr[];
        String temp = "", str;
        int j, k, l, m, n, index, found = 0;
        if (i == 0) {
            temp = "$";
        }
        for (j = 0; j < ntlen; j++) {
            for (k = 0; k < a[j]; k++) //entering grammar matrix
            {
                str = grmr[j][k];
                index = str.indexOf(nont[i]);
                while(index != -1) {
                    if (index == str.length() - 1) {
                        //System.out.println(grmr[j][k]);
                        if (j < i) {

                            temp = temp + flw[j];
                        }
                    } else {
                        for (m = 0; m < ntlen; m++) {
                            if (str.charAt(index + 1) == nont[m]) //first of next non-terminal otherwise (else later…)
                            {
                                chr = fst[m].toCharArray();

                                for (n = 0; n < chr.length; n++) {
                                    if (chr[n] == '!') {//if first includes epsilon
                                        if (index + 1 == str.length() - 1) {
                                            temp = temp + follow(j, a); //when non-terminal is second last
                                        } else {
                                            temp = temp + follow(m, a);
                                        }
                                    } else {
                                        temp = temp + chr[n]; //include whole first set except epsilon
                                    }
                                }
                                found = 1;
                            }
                        }
                        if (found != 1) {
                            temp = temp + str.charAt(index + 1); //follow set will include terminal(else is here)
                        }
                    }
                    
                    str = str.substring(index + 1);
                    index = str.indexOf(nont[i]);
                }
            }
        }
        return temp;
    }

    static String removeDuplicates(String str) {
        int i;
        char ch;
        boolean seen[] = new boolean[256];
        StringBuilder sb = new StringBuilder(seen.length);
        for (i = 0; i < str.length(); i++) {
            ch = str.charAt(i);
            if (!seen[ch]) {
                seen[ch] = true;
                sb.append(ch);
            }
        }
        return sb.toString();
    }

    static String func(int i, int j) {
        int k, l, m, found = 0;
        String temp = "", str = "";
        for (k = 0; k < grmr[i][j].length(); k++, found = 0) //when nonterminal has epsilon production
        {
            for (l = 0; l < ntlen; l++) //finding nonterminal
            {
                if (grmr[i][j].charAt(k) == nont[l]) //for nonterminal in first set
                {
                    for (m = 0; m < a[l]; m++) {
                        //System.out.println(l + " " + m + " " + a[l]);
                        str = func(l, m);
                        if (!(str.length() == 1 && str.charAt(0) == '!')) {
                            temp = temp + str;
                        }
                    }
                    found = 1;
                    break;
                }
            }
            if (found == 1) {
                if (str.contains("!")) {
                    continue;
                }
            } else //if first set includes terminal
            {
                temp = temp + grmr[i][j].charAt(k);
            }
            break;
        }
        return temp;
    }

    static void parse(String str) {
        Stack<Character> st = new Stack<Character>();
        str = str + "$";
        int i = 0, row = 0, col = 0, m;
        String rule;
        st.push('$');
        st.push(nont[0]);
        while (i < str.length()) {
            char c = str.charAt(i);
            if (c == st.peek()) {
                //System.out.println(nont[row] + "->" + table[row][col]);
                st.pop();
                i++;
            } else if (st.size() != 0) {
                row = new String(nont).indexOf(st.peek());
                col = new String(t).indexOf(c);
                //System.out.println(c + " " + st.peek());
                if ((row == -1) || (col == -1)) {
                    System.out.println("String is not accepted by give grammer." + "Error has occured as value of (" + st.peek() + "," + c + ") is not found.");
                    break;
                }
                rule = table[row][col];
                if ((rule == null)) {
                    System.out.println("String is not accepted by give grammer." + "Error has occured as value of (" + st.peek() + "," + c + ") is empty.");
                    break;
                }
                st.pop();
                if (rule != "!") {
                    System.out.println(nont[row] + "->" + rule);
                    for (m = rule.length() - 1; m >= 0; m--) {
                        //System.out.print(rule.charAt(m) + " " );
                        st.push(rule.charAt(m));
                    }
                } else {
                    System.out.println(nont[row] + "->" + "!");
                }

            }
        }

    }

    public static void main(String args[]) throws IOException {
        int i, j, k;
        readfile();
        fst = new String[ntlen];
        flw = new String[ntlen];
        int index = 0;
        String temp = "";
        table = new String[ntlen][tlen];
        for (i = 0; i < ntlen; i++) {
            temp = "";
            for (j = 0; j < a[i]; j++) {
                String c = func(i, j);
                for (k = 0; k < c.length(); k++) {
                    index = new String(t).indexOf(c.charAt(k));
                    table[i][index] = grmr[i][j];
                }
                temp = temp + c;
            }
            fst[i] = temp;
        }
        //System.out.println("------------------First-------------------------");
        for (i = 0; i < ntlen; i++) {
            fst[i] = removeDuplicates(fst[i]);
        }
       // System.out.println("---------------Follow-----------------------------");
        for (i = 0; i < ntlen; i++) {
            flw[i] = follow(i, a);
        }
        for (i = 0; i < ntlen; i++) {
            flw[i] = removeDuplicates(flw[i]);
        }
        for (i = 0; i < ntlen; i++) {
            if (fst[i].indexOf('!') != -1) {
                for (k = 0; k < flw[i].length(); k++) {
                    index = new String(t).indexOf(flw[i].charAt(k));
                    table[i][index] = "!";
                }
            }
        }
       /* System.out.println("--------------Parsing Table-----------------------");
        System.out.print("   ");
        for (i = 0; i < tlen; i++) {
            System.out.print(t[i] + "\t");
        }

        //for printing the table
        System.out.println("");
        for (i = 0; i < ntlen; i++) {
            System.out.print(nont[i] + " ");
            for (j = 0; j < tlen; j++) {
                if (table[i][j] == null) {
                    System.out.print("-" + "\t");
                } else {
                    System.out.print(table[i][j] + "\t");
                }
            }
            System.out.println(" ");
        }
        Scanner s = new Scanner(System.in);
        System.out.println("\nEnter the string");
        String input = s.next();
        System.out.println("------------------Now parsing the input-------------------------");
        parse(input);*/
    }
}
