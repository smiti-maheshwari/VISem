package slrone;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class leftrecursion {
    static String[] gr= null;
    static char[] gl,nont, t = null;
    static int ntlen;

 static int isleft(char[] left,String[] right)

    {
       int f=0;
       for(int i=0;i<left.length;i++)
       {
           for(int j=0;j<right.length;j++)

           {
               if(left[i]==right[j].charAt(0))
               {
                   System.out.println("Grammar is left recursive");
                   f=1;
               }

           }
       }
          return f;   
    }
    public static void main(String[] args) throws FileNotFoundException {
        // TODO code application logic here
        String[] left=new String[10];
        String[] right=new String[10];
        Scanner sc=new Scanner(System.in);
        File file = new File("leftrec.txt");
        Scanner s = new Scanner(file);
        
        gl = new char[15];
        gr = new String[15];
        nont = new char[25];
         
        int n =0;
        while (s.hasNextLine()) {
            String str = s.nextLine();
            String c = String.valueOf(str.charAt(0));
            if (!(new String(nont).contains(c))) {
                nont[ntlen++] = str.charAt(0);
            }
            gl[n] = str.charAt(0);
            gr[n] = str.substring(str.indexOf("= ") + 2);
            n++;
        }

        System.out.println("the productions are");
        for(int i=0;i<n;i++)
        {
            System.out.println(gl[i]+"->"+gr[i]);
        }
        int flag=0;
       flag=isleft(gl,gr);
           if(flag==1)
           {
               System.out.println("Removing left recursion");
           }
           else
           {
               System.out.println("No left recursion");
           }       
    }
}