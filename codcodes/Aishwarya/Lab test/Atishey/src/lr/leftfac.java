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
import static lr.removelr.done;
import static lr.removelr.mp;

public class leftfac {
    
    public leftfac() throws FileNotFoundException, IOException {
        System.out.println("Doing left factoring.");
        FileReader fr = new FileReader("recursed.txt");
        BufferedReader br = new BufferedReader(fr);

        String s;
        int done[] = new int[26];
        Arrays.fill(done, 0);

        Vector pro = new Vector();

        while ((s = br.readLine()) != null) {
            pro.add(s);
            done[s.charAt(0) - 'A'] = 1;
        }

        br.close();
        fr.close();

        int i, j=0;
        Vector fin = new Vector();
        while (j < pro.size()) {
            s = (String) pro.get(j);
     //       System.out.println("For production: " + s + " " + pro.size());
            char non = s.charAt(0);
            s = s.substring(2);
            int flag = 0;
            StringTokenizer st = new StringTokenizer(s, ",");
            Map<String, Vector> mp = new HashMap<String, Vector>();
            while (st.hasMoreTokens()) {
                String p = st.nextToken();
    //            System.out.println("For pro : " + p);
                if (!mp.containsKey("" + p.charAt(0))) {
                    mp.put("" + p.charAt(0), new Vector());
                    mp.get("" + p.charAt(0)).add(p.substring(1));
                } else {
                    flag = 1;
                    mp.get("" + p.charAt(0)).add(p.substring(1));
                }
            }
            if (flag == 1) {
                String add = non + " ";
                for (Map.Entry<String, Vector> entry : mp.entrySet()) {
                    String key = entry.getKey();
                    Vector v = entry.getValue();
                    if (v.size() > 1) {
                        char use = 0;
                        for (i = 0; i < 26; i++) {
                            if (done[i] == 0) {
                                done[i] = 1;
                                use = (char) (i + 'A');
                                break;
                            }
                        }
                        if (add.equals(non + " ")) {
                            add = add + key + use;
                        } else {
                            add = add + "," + key + use;
                        }
                        String toadd = use + " ";
                        for (i = 0; i < v.size(); i++) {
                            if (i == 0) {
                                if (v.get(i).equals("")) {
                                    toadd = toadd + "#";
                                } else {
                                    toadd = toadd + v.get(i);
                                }
                            } else {
                                if (v.get(i).equals("")) {
                                    toadd = toadd + ",#";
                                } else {
                                    toadd = toadd + "," + v.get(i);
                                }
                            }
                        }
                        pro.add(toadd);
                    } else {
                        if (add.equals(non + " ")) {
                            add = add + key + v.get(0);
                        } else {
                            add = add + "," + key + v.get(0);
                        }
                    }
                }
                fin.add(add);
            } else {
                fin.add(pro.get(j));
            }
            j++;
        }
  //      System.out.println(fin);
        FileWriter fw = new FileWriter("left_factored.txt");
        BufferedWriter bw = new BufferedWriter(fw);
        
        for(i=0;i<fin.size();i++) {
            bw.write(fin.get(i) + "\r\n");
        }
        
        bw.close();
        fw.close();
        
        System.out.println("Left factoring done.");
    }
}