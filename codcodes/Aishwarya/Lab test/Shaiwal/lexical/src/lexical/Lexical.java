package lexical;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class Lexical {

    public static void main(String[] args) throws FileNotFoundException, IOException {
        BufferedReader in = new BufferedReader(new FileReader("in.c"));
        BufferedWriter out = new BufferedWriter(new FileWriter("out.c"));

        String a = "", text = "";
        int c, f = 0;

        while ((c = in.read()) != -1) {
            a = a + (char) c;
        }

        int l = a.length();

        for (int i = 0; i < l; i++) {
            if (a.charAt(i) == '"') {
                //System.out.println("found");
                i += 2;
                while (a.charAt(i - 1) != '"') {
                    i++;
                }
            }
            if (a.charAt(i) == '/' && a.charAt(i + 1) == '*') {
                i += 2;
                while (!(a.charAt(i) == '*' && a.charAt(i + 1) == '/')) {
                    if (a.charAt(i) == '\n') {
                        text = text + a.charAt(i);
                    }
                    i++;
                }
                i += 1;
            } else if (a.charAt(i) == '/' && a.charAt(i + 1) == '/') {
                i += 2;
                while (a.charAt(i) != '\n') {
                    i++;
                }
                i--;
            } else {
                if (a.charAt(i) == '<' || a.charAt(i) == '>' || a.charAt(i) == '(' || a.charAt(i) == ')' || a.charAt(i) == ',' || a.charAt(i) == ';' || a.charAt(i) == ':' || a.charAt(i) == '{' || a.charAt(i) == '}') {
                    f = 1;
                }

                if (a.charAt(i) == '^' || a.charAt(i) == '+' || a.charAt(i) == '*' || a.charAt(i) == '/' || a.charAt(i) == '-' || a.charAt(i) == '=' || a.charAt(i) == '%' || a.charAt(i) == '&' || a.charAt(i) == '|' || a.charAt(i) == '~') {
                    f = 2;
                }

                if (f == 1 || f == 2) {
                    text = text + ' ' + a.charAt(i) + ' ';
                } else {
                    text = text + a.charAt(i);
                }
                f = 0;
            }
        }
        //System.out.println(text);
        out.write(text);
        out.close();

        String[] keyword = {"#include", "scanf", "return", "auto", "if", "else", "case", "switch", "extern", "for", "while", "do", "struct", "class", "goto", "enum", "sizeof", "static", "union", "int", "float", "double", "unsigned", "printf", "scanf"};
        String[] pun = {",", ".", ",", "(", ")", "{", "}", ";", "<", ">"};
        String[] op = {"+", "-", "&", "&&", "|", "||", "*", "/", "^", "%", "="};
        //File fi = new File("out.c"); 
        //Scanner s = new Scanner(fi);
        a = "";
        in = new BufferedReader(new FileReader("out.c"));
        while ((c = in.read()) != -1) {
            a = a + (char) c;
        }
        l = a.length();
        //System.out.println(a);
        String s = "";
        for (int i = 0; i < l; i++) {

            if (a.charAt(i) != ' ' && a.charAt(i) != '\n' && a.charAt(i) != '\t' && a.charAt(i) != '\r') {

                s += a.charAt(i);
            } else if (a.charAt(i) == ' ' || a.charAt(i) == '\n' || a.charAt(i) == '\t' || a.charAt(i) == '\r') {
                boolean flag;
                flag = false;
                if (s.length() == 0) {
                    continue;
                }
                //System.out.println(s);
                for (String keyword1 : keyword) {
                    if (s.equals(keyword1)) {
                        System.out.println("(\" Keyword " + ", " + keyword1 + ")");
                        flag = true;
                    }
                }

                for (String pun1 : pun) {
                    if (s.equals(pun1)) {
                        System.out.println("(\" Punctuation " + ", " + pun1 + ")");
                        flag = true;
                    }
                }

                for (String op1 : op) {
                    if (s.equals(op1)) {
                        System.out.println("(\" Operator " + ", " + op1 + ")");
                        flag = true;
                    }
                }
                if (flag == false) {

                    boolean fl = true;

                    for (int z = 0; z < s.length(); z++) {
                        int x = s.charAt(z);
                        if (x >= 48 && x <= 57) {
                            fl = true;
                        } else {
                            fl = false;
                            break;
                        }
                    }
                    if (fl) {
                        System.out.println("Constant = " + s);
                    } else {
                        System.out.println("Identifier " + ", " + s + ")");
                    }
                    //     System.out.println(s);
                }
                s = "";
            }
        }
    }
}
