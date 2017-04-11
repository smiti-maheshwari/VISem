package lr;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;
import java.util.Vector;
import javafx.util.Pair;

public class ff {
    static converter ob;
    static HashMap<String, Integer> mp = new HashMap<String, Integer>();
    static HashMap<Integer, String> rev = new HashMap<Integer, String>();
    static HashMap<String, Vector> first = new HashMap<String, Vector>();
    static HashMap<String, Integer> vis = new HashMap<String, Integer>();
    static HashMap<String, Integer> isvis = new HashMap<String, Integer>();
    static HashMap<String, Vector> follow = new HashMap<String, Vector>();
    static Vector mat[] = new Vector[100];
    static Vector all = new Vector();
    static Vector rr = new Vector();
    static Vector ll = new Vector();
    static int m;
    static String fsym;
    
    static void fin(String s) {
        //   System.out.println("With" + s);
        vis.put(s, 1);
        int j, len;
        len = s.length();
        //  System.out.println("Length: " + len);
        for (j = 0; j < len; j++) {
            if (((s.charAt(j) >= 'a') && (s.charAt(j) <= 'z')) || (s.charAt(j) == '#') || (s.charAt(j) == '+' || s.charAt(j) == '-' || s.charAt(j) == '*' || s.charAt(j) == '/')) {
                continue;
            } else {
                break;
            }
        }
        if (first.get(s) == null) {
            first.put(s, new Vector());
        }
        if (j == len) {
            first.get(s).add(s.charAt(0));
        } else if (len == 1) {
            //     System.out.println("I am here with " + s);
            int ind = 0;
            if (mp.containsKey(s)) {
                ind = (int) mp.get(s);
            }
            for (j = 0; j < mat[ind].size(); j++) {
         //       System.out.println("fnding ");
                //         System.out.println(mat[ind].get(j));
                if (!vis.containsKey(mat[ind].get(j))) {
                    //         System.out.println("Checking: " + mat[ind].get(j));
                    fin((String) mat[ind].get(j));
                }
                for (int k = 0; k < first.get(mat[ind].get(j)).size(); k++) {
                    if (j > 0) {
                        int l;
                        for (l = 0; l < first.get(s).size(); l++) {
                            if (first.get(s).get(l) == first.get(mat[ind].get(j)).get(k)) {
                                break;
                            }
                        }
                        if (l == first.get(s).size()) {
                            first.get(s).add(first.get(mat[ind].get(j)).get(k));
                        }
                    } else {
                        first.get(s).add(first.get(mat[ind].get(j)).get(k));
                    }
                }
            }
        } else {
            // System.out.println("yoyo: ");
            String ss;
            int flag;
            for (j = 0; j < len; j++) {
                flag = 0;
                ss = "";
                ss = ss + (char) s.charAt(j);
                //   System.out.println("after: " + ss);
                if (!vis.containsKey(ss)) {
                    //     System.out.println("Checking: " + ss);
                    fin(ss);
                }
                for (int k = 0; k < first.get(ss).size(); k++) {
                    if ((char) first.get(ss).get(k) != '#') {
                        int l;
                        for (l = 0; l < first.get(s).size(); l++) {
                            if (first.get(s).get(l) == first.get(ss).get(k)) {
                                break;
                            }
                        }
                        if (l == first.get(s).size()) {
                            first.get(s).add(first.get(ss).get(k));
                        }
                    } else {
                        flag = 1;
                        if (j == len - 1) {
                            int l;
                            for (l = 0; l < first.get(s).size(); l++) {
                                if ((char) first.get(s).get(l) == '#') {
                                    break;
                                }
                            }
                            if (l == first.get(s).size()) {
                                first.get(s).add('#');
                            }
                        }
                    }
                }
                if (flag == 1) {
                    continue;
                } else {
                    //   System.out.println("breaked");
                    break;
                }
            }
        }
    }

    static void fin_fol(String s) {
     //   System.out.println("finding follow of : " + s);
        if (!follow.containsKey(s)) {
            follow.put(s, new Vector());
        }
        isvis.put(s, 1);
        int i, j, k, l, flag;
        for (i = 0; i < m; i++) {
            for (j = 0; j < mat[i].size(); j++) {
                String r = (String) mat[i].get(j);
                for (k = 0; k < r.length(); k++) {
                    if (r.charAt(k) == s.charAt(0)) {
                        //          cout<<"k: "<<k<<endl;
                        flag = 0;
                        if (k + 1 < r.length()) {
                            String ss = "";
                            ss = ss + r.charAt(k + 1);
                    //        System.out.println("Strng : " + ss);
                            //       cout<<"ss: "<<ss<<endl;
                            if (first.get(ss) == null) {
                                fin(ss);
                            }
                            for (l = 0; l < first.get(ss).size(); l++) {
                                if ((char) first.get(ss).get(l) == '#') {
                                    flag = 1;
                                    continue;
                                }
                                int kk;
                                for (kk = 0; kk < follow.get(s).size(); kk++) {
                                    if (follow.get(s).get(kk) == first.get(ss).get(l)) {
                                        break;
                                    }
                                }
                                if (kk == follow.get(s).size()) {
                                    follow.get(s).add(first.get(ss).get(l));
                                }
                            }
                        } else {
                            flag = 1;
                        }

                        if (flag == 1) {
                            if (!isvis.containsKey(rev.get(i))) {
                                fin_fol(rev.get(i));
                            }
                            String ss = rev.get(i);
                            for (l = 0; l < follow.get(ss).size(); l++) {
                                if ((char) follow.get(ss).get(l) == '#') {
                                    flag = 1;
                                    continue;
                                }
                                int kk;
                                for (kk = 0; kk < follow.get(s).size(); kk++) {
                                    if (follow.get(s).get(kk) == follow.get(ss).get(l)) {
                                        break;
                                    }
                                }
                                if (kk == follow.get(s).size()) {
                                    follow.get(s).add(follow.get(ss).get(l));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    static boolean isterminal(char c) {
        if (((c >= 'a') && (c <= 'z')) || (c == '#') || (c == '$' || c == '+' || c == '-' || c == '*' || c == '/')) {
            return true;
        }
        return false;
    }

    public ff() throws FileNotFoundException, IOException {
        System.out.println("Finding firsts and follows.");
        Scanner scan = new Scanner(System.in);
        System.out.println("Enter start symbol: ");
        fsym = scan.next();
        fsym = converter.conv2.get(fsym);
        File file = new File("left_factored.txt");
        FileInputStream fr = new FileInputStream(file);
        for (int i = 0; i < 100; i++) {
            mat[i] = new Vector();
        }
        char c = (char) fr.read();
        int flag = 0;
        String lhs = new String("");
        String rhs = new String("");
        m = 0;
        int j;
        while (true) {
            if ((c == '\t')||(c==' ')) {
                mp.put(lhs, m);
                rev.put(m, lhs);
                //    System.out.println("lhs before space:" + lhs);
                all.add(lhs);
                ll.add(lhs);
                flag = 1;
                lhs = "";
            } else if (c == '\r') {
                //  System.out.println("rhs before newline: " + rhs);
                mat[m].add(rhs);
                all.add(rhs);
                rr.add(rhs);
                flag = 0;
                rhs = "";
                if (fr.available() > 0) {
                    c = (char) fr.read();
                } else {
                    break;
                }
                // System.out.println("Size : " + mat[m].size() + " " + mat[m].get(0) + " " + mat[m].get(1));
             /*   for (int k = 0; k < mat[m].size(); k++) {
                 // System.out.println("fdjgnkanrlkven");
                 //  System.out.println("k: " + k);
                 System.out.println(mat[m].get(k));
                 }
                 */ // System.out.println("Size : " + mat[m].size() + " " + mat[m].get(0) + " " + mat[m].get(1));
                m++;
            } else if (c == ',') {
                //System.out.println("rhs before comma: " + rhs);
                mat[m].add(rhs);
                rr.add(rhs);
                all.add(rhs);
                /* System.out.println("Size : " + mat[m].size() + " " + mat[m].get(0));
                 for(int k = 0;k<mat[m].size();k++) {
                 // System.out.println("fdjgnkanrlkven");
                 //  System.out.println("k: " + k);
                 System.out.println("k: " + k + " " +mat[m].get(k) + " ");
                 }
                 System.out.println("Size : " + mat[m].size() + " " + mat[m].get(0));
                 */
                rhs = "";
            } else if (flag == 0) {
                lhs = lhs + c;
            } else if (flag == 1) {
                rhs = rhs + c;
            }
            if (fr.available() > 0) {
                c = (char) fr.read();
            } else {
                break;
            }
        }

   /*     System.out.println("Map is: ");
        for (Map.Entry<String, Integer> entry : mp.entrySet()) {
            System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
        }

        System.out.println("Rev Map is: ");
        for (Map.Entry<Integer, String> entry : rev.entrySet()) {
            System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
        }

        System.out.println("m: " + m);
     */   fr.close();

        int i;

    /*    System.out.println("All.");
        for (i = 0; i < all.size(); i++) {
            System.out.println(all.get(i));
        }
        System.out.println("All over.");

        System.out.println("Table is : ");

        for (i = 0; i < m; i++) {
            System.out.println(rev.get(i) + " ");

            for (j = 0; j < mat[i].size(); j++) {
                System.out.println(mat[i].get(j));
            }
        }
        System.out.println();
*/
        for (i = 0; i < all.size(); i++) {
            if (!vis.containsKey(all.get(i))) {
                fin((String) all.get(i));
            }
        }

        String output = "";

  //      System.out.println("Firsts: ");
        //output = output + "Firsts: \n";
        for (i = 0; i < all.size(); i++) {
  //          System.out.println("For : " + all.get(i));
            output = output + all.get(i) + " -> ";
            for (j = 0; j < first.get(all.get(i)).size(); j++) {
                //cout<<first[all[i]][j]<<" ";
  //              System.out.print(first.get(all.get(i)).get(j) + " ");
                output = output + first.get(all.get(i)).get(j) + " ";
            }
            output = output + "\r\n";
  //          System.out.println();
        }

        for (Map.Entry<String, String> entry : ob.rev1.entrySet()) {
            while (output.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                output = output.replace(entry.getKey(), entry.getValue());
            }
        }
        for (Map.Entry<String, String> entry : ob.rev2.entrySet()) {
            while (output.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                output = output.replace(entry.getKey(), entry.getValue());
            }
        }

        FileWriter fw = new FileWriter("firsts.txt");
        BufferedWriter bw = new BufferedWriter(fw);

        bw.write("Firsts: \r\n");
        bw.write(output);
        
        bw.close();
        fw.close();
        
        follow.put(fsym, new Vector());
        follow.get(fsym).add('$');
        fin_fol(fsym);

        String ss = "";

        for (i = 0; i < all.size(); i++) {
            for (j = 0; j < ((String) all.get(i)).length(); j++) {
                if (((char) ((String) all.get(i)).charAt(j) >= 'a') && (((char) ((String) all.get(i)).charAt(j) <= 'z')) || ((char) ((String) all.get(i)).charAt(j) == '#') || ((char) ((String) all.get(i)).charAt(j) == '+' || (char) ((String) all.get(i)).charAt(j) == '-' || (char) ((String) all.get(i)).charAt(j) == '*' || (char) ((String) all.get(i)).charAt(j) == '/')) {
                    continue;
                } else {
                    ss = ss + (char) ((String) all.get(i)).charAt(j);
                    //         cout<<ss<<endl;
                    if (!isvis.containsKey(ss)) {
                        fin_fol(ss);
                    }
                    ss = "";
                }
            }
        }

        output = "";
        System.out.println("Follows:");
        for (Map.Entry<String, Vector> entry : follow.entrySet()) {
            System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
            output = output + entry.getKey() + " -> " + entry.getValue() + "\r\n";
        }

        for (Map.Entry<String, String> entry : ob.rev1.entrySet()) {
            while (output.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                output = output.replace(entry.getKey(), entry.getValue());
            }
        }
        for (Map.Entry<String, String> entry : ob.rev2.entrySet()) {
            while (output.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                output = output.replace(entry.getKey(), entry.getValue());
            }
        }

        fw = new FileWriter("follows.txt");
        bw = new BufferedWriter(fw);
        
        bw.write("Follows: \r\n");
        bw.write(output);

        bw.close();
        fw.close();
        
        System.out.println("firsts,follows done.");
    }
    
    public void parse() throws IOException {
        System.out.println("LL1 parsing.");
        String output = "";
        Map<String, Vector> parse = new HashMap<String, Vector>();

        int i,j;
        char c;
        for (i = 0; i < ll.size(); i++) {
    //        System.out.println("For : " + ll.get(i));
            parse.put((String) ll.get(i), new Vector());
            output = output + ll.get(i) + " : ";
            int k = mp.get(ll.get(i));
            for (j = 0; j < first.get(ll.get(i)).size(); j++) {
                c = (char) first.get(ll.get(i)).get(j);
                if (c != '#') {
      //              System.out.println("At " + c + " goto ");

                    output = output + "(" + c + ",";

                    for (int l = 0; l < mat[k].size(); l++) {
                        if (first.get(mat[k].get(l)).contains(c)) {
        //                    System.out.print(mat[k].get(l) + " ");
                            parse.get(ll.get(i)).add(new Pair(c, mat[k].get(l)));
                            output = output + ll.get(i) + "->" + mat[k].get(l) + " ";
                        }
                    }
                    output = output + ")";

      //              System.out.println();
                } else {
                    for (int t = 0; t < follow.get(ll.get(i)).size(); t++) {
                        char cc = (char) follow.get(ll.get(i)).get(t);
        //                System.out.println("At " + cc + " goto ");
                        output = output + "(" + cc + ",";
                        for (int l = 0; l < mat[k].size(); l++) {
                            if (first.get(mat[k].get(l)).contains('#')) {
    //                            System.out.print(mat[k].get(l) + " ");
                                parse.get(ll.get(i)).add(new Pair(cc, mat[k].get(l)));
                                output = output + ll.get(i) + "->" + mat[k].get(l) + " ";
                            }
                        }
                        output = output + ")";
      //                  System.out.println();
                    }
                }
            }
            output = output + "\r\n";
        }

        FileWriter fw1 = new FileWriter("used_parse_table.txt");
        BufferedWriter bw1 = new BufferedWriter(fw1);

        bw1.write(output);
        bw1.close();
        fw1.close();

        System.out.println("Enter the string to be checked: ");
        Scanner scan = new Scanner(System.in);
        String sss = scan.next();

        sss = ob.convert(sss);

        sss = sss + '$';

        System.out.println("String to be parsed: " + sss);

        Stack st = new Stack();
        st.push('$');

   //     System.out.println("All: " + all.get(0));
        st.push(fsym.charAt(0));

        i = 0;
        String topush = null;
        while (!st.empty()) {
            char top = (char) st.pop();
            System.out.println("Top of stack " + top + " string character " + sss.charAt(i));
            if (isterminal(top)) {
                if (top == sss.charAt(i)) {
                    i++;
                    System.out.println("Match");
                } else {
                    System.out.println("Rejected due to " + ob.rev1.get("" + top));
                    break;
                }
            } else {
                if (parse.containsKey("" + top)) {
                    Vector v = parse.get("" + top);
                    for (j = 0; j < v.size(); j++) {
                        if (sss.charAt(i) == (char) ((Pair) (v.get(j))).getKey()) {
                            topush = (String) ((Pair) (v.get(j))).getValue();
                            break;
                        }
                    }
                    if (j == v.size()) {
                        System.out.println("Rejected due to " + ob.rev1.get("" + top) + " no production rule.");
                        break;
                    } else {
                        String tp = "" + top, tpush = topush;
                        for (Map.Entry<String, String> entry : ob.rev1.entrySet()) {
                            while (tp.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                                tp = tp.replace(entry.getKey(), entry.getValue());
                            }
                        }
                        for (Map.Entry<String, String> entry : ob.rev2.entrySet()) {
                            while (tp.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                                tp = tp.replace(entry.getKey(), entry.getValue());
                            }
                        }
                        for (Map.Entry<String, String> entry : ob.rev1.entrySet()) {
                            while (tpush.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                                tpush = tpush.replace(entry.getKey(), entry.getValue());
                            }
                        }
                        for (Map.Entry<String, String> entry : ob.rev2.entrySet()) {
                            while (tpush.contains(entry.getKey()) && (!entry.getKey().equals(entry.getValue()))) {
                                tpush = tpush.replace(entry.getKey(), entry.getValue());
                            }
                        }
                        System.out.println(tp + " -> " + tpush);
                        if (!topush.equals("#")) {
                            for (j = topush.length() - 1; j >= 0; j--) {
                                st.push(topush.charAt(j));
                            }
                        }
                    }
                } else {
                    System.out.println("Rejected due to " + ob.rev1.get("" + top) + " no production rule.");
                    break;
                }
            }
        }

        if (i == sss.length()) {
            System.out.println("Accepted");
        } else {
            System.out.println("Rejected");
        }
        
        System.out.println("Parsing completed.");
    }
    
    public static void main(String args[]) throws FileNotFoundException, IOException {
        ob = new converter();
        ff obj = new ff();
        obj.parse();
    }
}
