package lr;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

public class converter {
    static int done1[] = new int[26];
    static int done2[] = new int[26];
    static Vector term = new Vector();
    static Vector nonter = new Vector();
    static Map <String,String> conv1 = new HashMap <String,String> ();
    static Map <String,String> rev1 = new HashMap <String,String> ();
    static Map <String,String> conv2 = new HashMap <String,String> ();
    static Map <String,String> rev2 = new HashMap <String,String> ();
    static removelr ob1;
    static leftfac ob2;
    
    String convert(String s) {
        for(int i=0;i<term.size();i++) {
            String ter = (String) term.get(i);
            while(s.contains(ter) && !conv1.get(ter).equals(ter)) s = s.replace(ter, conv1.get(ter));
        }
        for(int i=0;i<nonter.size();i++) {
            String nter = (String) nonter.get(i);
            while(s.contains(nter) && !conv2.get(nter).equals(nter)) s = s.replace(nter, conv2.get(nter));
        }
        return s;
    }
    
    public converter()  throws IOException {
        System.out.println("Converter called.");
        
        Arrays.fill(done1, 0);
        Arrays.fill(done2, 0);

        FileReader fr = new FileReader("terminals.txt");
        BufferedReader br = new BufferedReader(fr);

        String s;

        while ((s = br.readLine()) != null) {
            term.add(s);
            for(int i=0;i<s.length();i++) {
                if (s.charAt(i) >= 'a' && s.charAt(i) <= 'z') {
                    done1[s.charAt(i) - 'a'] = 1;
                }
            }
        }

        br.close();
        fr.close();

        fr = new FileReader("nonterminals.txt");
        br = new BufferedReader(fr);

        while ((s = br.readLine()) != null) {
            nonter.add(s);
            for(int i=0;i<s.length();i++) {
                if (s.charAt(i) >= 'A' && s.charAt(i) <= 'Z') {
                    done2[s.charAt(i) - 'A'] = 1;
                }
            }
        }

        br.close();
        fr.close();

        int i,j;
       
        for(i=0;i<term.size();i++) {
            String ter = (String) term.get(i);
            
            if((ter.length()==1) && ((ter.charAt(0) >= 'a' && ter.charAt(0) <= 'z')||(ter.charAt(0)=='#'))) {
                conv1.put(ter, ter);
                rev1.put(ter,ter);
            } else {
                for(j=0;j<26;j++) {
                    if(done1[j]==0) {
                        done1[j] = 1;
                        break;
                    }
                }
                conv1.put(ter,""+(char)(j+'a'));
                rev1.put(""+(char)(j+'a'),ter);
            }
        }
        
        for(i=0;i<nonter.size();i++) {
            String nter = (String) nonter.get(i);
            
            if((nter.length()==1) && nter.charAt(0) >= 'A' && nter.charAt(0) <= 'Z') {
                conv2.put(nter, nter);
                rev2.put(nter,nter);
            } else {
                for(j=0;j<26;j++) {
                    if(done2[j]==0) {
                        done2[j] = 1;
                        break;
                    }
                }
                conv2.put(nter,""+(char)(j+'A'));
                rev2.put(""+(char)(j+'A'),nter);
            }
        }
        
        fr = new FileReader("grammar9.txt");
        br = new BufferedReader(fr);

        FileWriter fw = new FileWriter("converted.txt");
        BufferedWriter bw = new BufferedWriter(fw);
        
        while ((s = br.readLine()) != null) {
            for(i=0;i<term.size();i++) {
                String ter = (String) term.get(i);
                while(s.contains(ter) && !conv1.get(ter).equals(ter)) s = s.replace(ter, conv1.get(ter));
            }
            for(i=0;i<nonter.size();i++) {
                String nter = (String) nonter.get(i);
                while(s.contains(nter) && !conv2.get(nter).equals(nter)) s = s.replace(nter, conv2.get(nter));
            }
            bw.write(s + "\r\n");
        }
        br.close();
        fr.close();
        bw.close();
        fw.close();
        
        System.out.println("File Converted.");
        
        ob1 = new removelr();
        ob2 = new leftfac();
    }
}
