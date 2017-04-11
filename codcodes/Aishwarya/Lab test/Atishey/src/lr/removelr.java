package lr;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.StringTokenizer;
import java.util.Vector;

public class removelr {

    static int done[] = new int[26];
    static int used[] = new int[26];
    static Map<String, Vector> mp = new HashMap<String, Vector>();

    public removelr() throws FileNotFoundException, IOException {
        System.out.println("Removing left recursion.");
        FileReader fr = new FileReader("converted.txt");
        BufferedReader br = new BufferedReader(fr);

        Arrays.fill(done, 0);
        Arrays.fill(used, 0);

        String s;

        while ((s = br.readLine()) != null) {
            done[s.charAt(0) - 'A'] = 1;
        }

        br.close();
        fr.close();
        
  /*      for(int i =0;i<26;i++)
         System.out.print(done[i] + " ");
        System.out.println();
*/
        fr = new FileReader("converted.txt");
        br = new BufferedReader(fr);

       

        int i, j, flag;

        while ((s = br.readLine()) != null) {
     //       System.out.println("Producton: " + s);
            char non = s.charAt(0);
            flag = 0;
            Vector v = new Vector();
            StringTokenizer st = new StringTokenizer(s.substring(2), ",");
            while (st.hasMoreTokens()) {
                String pro = st.nextToken();
       //         System.out.println("For: " + pro);
                if (pro.charAt(0)>='A' && pro.charAt(0)<='Z' && (used[pro.charAt(0) - 'A'] == 1)) {
       //             System.out.println("haha");
                    Vector repwith = mp.get("" + pro.charAt(0));
                    for (i = 0; i < repwith.size(); i++) {
                        String rep = (String) repwith.get(i);
                        String newpro = "";
                        if (!rep.equals("#")) {
                            newpro = repwith.get(i) + pro.substring(1);
                        } else {
                            newpro = pro.substring(1);
                        }

                        if (newpro.equals("")) {
                            flag = 1;
                        } else {
                            v.add(newpro);
                        }
                    }
                } else {
                    v.add(pro);
                }
            }
            if (flag == 1) {
                v.add("#");
            }
     //       System.out.println("After substituting: " + v);

            int sz = v.size();

            Vector beta = new Vector();
            Vector alpha = new Vector();

            for (i = 0; i < sz; i++) {
                String pro = (String) v.get(i);

                if (pro.charAt(0) == non) {
                    String ad = pro.substring(1);
                    //if(ad.equals("")) ad = "#";
                    alpha.add(ad);
                } else {
                    beta.add(pro);
                }
            }

   //         System.out.println("Alphas: " + alpha);
    //        System.out.println("Betas: " + beta);
            if (alpha.size() != 0) {

                char use = 0;
                for (i = 0; i < 26; i++) {
                    if (done[i] == 0) {
                        done[i] = 1;
                        use = (char) (i + 'A');
                        System.out.println("used " + use + " i: " + i);
                        break;
                    }
                }

                Vector vv = new Vector();
                for (i = 0; i < beta.size(); i++) {
                    String toadd = (String) beta.get(i) + use;
                    vv.add(toadd);
                }
                mp.put("" + non, vv);

   //             System.out.println("For " + non + " : " + vv);
                
                vv = new Vector();
                for (i = 0; i < alpha.size(); i++) {
                    String toadd = (String) alpha.get(i) + use;
                    vv.add(toadd);
                }
                vv.add("#");
                mp.put("" + use, vv);
   //             System.out.println("For " + use + " : " + vv);
            } else {
                Vector vv = new Vector();
                for (i = 0; i < beta.size(); i++) {
                    String toadd = (String) beta.get(i);
                    vv.add(toadd);
                }
                mp.put("" + non, vv);
   //             System.out.println("For " + non + " : " + vv);
            }

            used[non - 'A'] = 1;
        }
        br.close();
        fr.close();

        FileWriter fw = new FileWriter("recursed.txt");
        BufferedWriter bw = new BufferedWriter(fw);
        
        for (Map.Entry<String, Vector> entry : mp.entrySet()) {
            Vector v = entry.getValue();
            bw.write(entry.getKey() + "\t");
            for(i=0;i<v.size();i++) {
                if(i==0) bw.write((String) v.get(i));
                else bw.write("," + v.get(i));
            }
            bw.write("\r\n");
  //          System.out.println("Key = " + entry.getKey() + ", Value = " + entry.getValue());
        }
        
        bw.close();
        fw.close();
        
        System.out.println("Left recursion removed.");
    }
}