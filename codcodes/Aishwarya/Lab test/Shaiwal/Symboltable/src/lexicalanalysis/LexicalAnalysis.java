/**
 * *
 * // Geekdon
 *
 * lexicalAnalysis() ---> here i did lexical analysis process stringAdd(int index, String line) ---> this is a method where i save string in a vector
 *
 * vector ----> a string type vector lexemes ----> a 1-D array to store lexemes
 * tokensName ----> a 1-D array to store tokens 
 * attributeValue----> a 1-D array to store lexemes 
 * attribute values symbolTable ----> a 2-D array to store symbol table entry
 */
package lexicalanalysis;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.Scanner;
import java.util.Vector;

public class LexicalAnalysis {

    boolean isNumericCheck(String s) {
        boolean flag = false;
        if (s.matches("[0-9]*.?[0-9]+")) {
            return true;
        }
        
        return flag;
    }

    boolean checkkeyword(String n) {
        boolean flag = false;
        String keyword[] = {"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "else if"};
        for (String keyword1 : keyword) {
            if (n.equals(keyword1)) {

                flag = true;
            }
        }
        return flag;
    }

    static Vector<String> vector = new Vector<String>();
    static int position;

    String line;
    int index;

    public void lexicalAnalysis() {

        try {
            BufferedReader in = new BufferedReader(new FileReader("input.txt"));
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
                 text = text + a.charAt(i);
         /*       if (a.charAt(i) == '<' || a.charAt(i) == '>' || a.charAt(i) == '(' || a.charAt(i) == ')' || a.charAt(i) == ',' || a.charAt(i) == ';' || a.charAt(i) == ':' || a.charAt(i) == '{' || a.charAt(i) == '}') {
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
            */
                 }
            
        }
        //System.out.println(text);
        out.write(text);
        out.close();
            
            
            Scanner scanner = new Scanner(new File("out.c")); // input from file

            File outputfile = new File("output.txt"); // output file  
            FileWriter filewriter = new FileWriter(outputfile.getAbsoluteFile());
            BufferedWriter bufferwriter = new BufferedWriter(filewriter);

            while (scanner.hasNextLine()) {

                position = 0;
                line = scanner.nextLine();
                line = line.trim();

                // multi line comment check
                if (line.startsWith("/*")) {
                    if (line.endsWith("*/")) {
                        continue;
                    }

                    while (scanner.hasNextLine()) {
                        line = scanner.nextLine();

                        if (line.endsWith("*/")) {
                            break;
                        }
                    }
                    continue;
                } // single single line comment check
                else if (line.equals("") || line.startsWith("//") || line.startsWith("#")) {
                    continue;
                } // here found lexemes 
                else {
                    for (index = 0; index < line.length(); index++) {
                        String check = "" + line.charAt(index);

                        // remove line comments
                        if (check.equals("/")) {
                            check = check + line.charAt(index + 1);
                            if (check.equals("//")) {
                                break;
                            }
                        } else if (check.equals(" ")) {
                            if (position == index) {
                                position = index + 1;
                                continue;
                            } // add string
                            else {
                                stringAdd(index, line);
                            }
                        } else if (check.equals("*") || check.equals("/") || check.equals("%") || check.equals("(")
                                || check.equals(")") || check.equals("{") || check.equals("}")
                                || check.equals("[") || check.equals("]") || check.equals(",") || check.equals(";")) {
                            stringAdd(index, line);

                            String s = "" + line.charAt(index);
                            vector.add(s);
                        } else if (check.equals("=") || check.equals("+") || check.equals("-") || check.equals("&")
                                || check.equals("|") || check.equals("!") || check.equals("<") || check.equals(">")) {
                            String s = "" + line.charAt(index + 1);

                            // Again check for another expression
                            if (s.equals("=") || s.equals("+") || s.equals("-") || s.equals("&") || s.equals("|")) {
                                stringAdd(index, line);
                                s = "" + line.charAt(index) + line.charAt(index + 1);
                                vector.add(s);
                                position = index + 2;
                                index = index + 1;
                            } else {
                                stringAdd(index, line);
                                s = "" + line.charAt(index);
                                vector.add(s);
                            }
                        }
                    }
                    
                    stringAdd(index, line);
                }
            }

            String[] lexemes = new String[vector.size()];
            lexemes = vector.toArray(lexemes);

            String[] tokensName = new String[lexemes.length];
            String[] attributeValue = new String[lexemes.length];
            String[][] symbolTable = new String[lexemes.length][50];
            int counter = 0;
            boolean checked = false;

            for (int i = 0; i < lexemes.length; i++) {

                checked = false;

                // Syntax checking
                if (checkkeyword(lexemes[i])) {
                    tokensName[i] = new String(lexemes[i]);
                    attributeValue[i] = new String("keyword");
                } // checks lexeme is a number or not
                else if (isNumericCheck(lexemes[i])) {
                  
                    tokensName[i] = new String("number");
                    attributeValue[i] = new String("Constant");
                } // Operator checking
                else if (lexemes[i].equals("||") || lexemes[i].equals("&&") || lexemes[i].equals("!")) {
                    tokensName[i] = new String("operator");
                    attributeValue[i] = new String("Logical Operator");
                } else if (lexemes[i].equals("==")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Equality");
                } else if (lexemes[i].equals("!=")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Not Equal");
                } else if (lexemes[i].equals("<")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Less than");
                } else if (lexemes[i].equals(">")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Greater than");
                } else if (lexemes[i].equals("<=")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Less than equal to");
                } else if (lexemes[i].equals(">=")) {
                    tokensName[i] = new String("Relational Operator");
                    attributeValue[i] = new String("Greater than equal to");
                } else if (lexemes[i].equals("=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Simple assignment");
                }else if (lexemes[i].equals("+=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Add and assignment");
                }else if (lexemes[i].equals("-=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Subtract and  assignment");
                }else if (lexemes[i].equals("/=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Divide and assignment");
                }else if (lexemes[i].equals("*=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Multiply and  assignment");
                }else if (lexemes[i].equals("<<=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Left Shift and assignment");
                } else if (lexemes[i].equals(">>=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Right Shift and assignment");
                }else if (lexemes[i].equals("&=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Bitwise and assignment");
                }else if (lexemes[i].equals("^=")) {
                    tokensName[i] = new String("Assingment Operator");
                    attributeValue[i] = new String("Bitwise Exclusive OR and assignment");
                }else if (lexemes[i].equals("|=")) {
                    tokensName[i] = new String("Assignment operator");
                    attributeValue[i] = new String("Bitwise Inclusive OR and assignment");
                } else if (lexemes[i].equals("+")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("addition");
                } else if (lexemes[i].equals("-")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("subtraction");
                } else if (lexemes[i].equals("*")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("Multiplication");
                } else if (lexemes[i].equals("/")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("Division");
                } else if (lexemes[i].equals("%")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("Modulus");
                } else if (lexemes[i].equals("^")) {
                    tokensName[i] = new String("Bitwise operator");
                    attributeValue[i] = new String("Power");
                } else if (lexemes[i].equals("++")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("increment");
                } else if (lexemes[i].equals("--")) {
                    tokensName[i] = new String("Arithmetic operator");
                    attributeValue[i] = new String("decrement");
                } else if (lexemes[i].equals("<<")) {
                    tokensName[i] = new String("Bitwise operator");
                    attributeValue[i] = new String("left shift");
                } else if (lexemes[i].equals(">>")) {
                    tokensName[i] = new String("Bitwise operator");
                    attributeValue[i] = new String("right shift");
                } else if (lexemes[i].equals("&")) {
                    tokensName[i] = new String("operator");
                    attributeValue[i] = new String("Bit Wise And");
                } else if (lexemes[i].equals("|")) {
                    tokensName[i] = new String("operator");
                    attributeValue[i] = new String("Bit Wise Or");
                } else if (lexemes[i].equals("~")) {
                    tokensName[i] = new String("operator");
                    attributeValue[i] = new String("Bit Wise Complement");
                }// Checks special symbol
                else if (lexemes[i].equals("(")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("opening braces");
                } else if (lexemes[i].equals(")")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("closing braces");
                } else if (lexemes[i].equals("{")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("left curly braces");
                } else if (lexemes[i].equals("}")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("right curly braces");
                } else if (lexemes[i].equals("[")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("opening bracket");
                } else if (lexemes[i].equals("]")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("closing bracket");
                } else if (lexemes[i].equals(",")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("comma");
                } else if (lexemes[i].equals(";")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("semicolon");
                } else if (lexemes[i].equals(":")) {
                    tokensName[i] = new String("Punctuation");
                    attributeValue[i] = new String("colon");
                } else if (lexemes[i].equals("main")) {
                    tokensName[i] = new String("id");
                    attributeValue[i] = "" + counter;
                    symbolTable[counter][0] = "main";
                    symbolTable[counter][1] = new String();
                    counter++;
                } else {
                    tokensName[i] = new String("id");
                    int j;

                    for (j = 0; j < counter; j++) {
                        if (symbolTable[j][0].equals(lexemes[i])) {
                            checked = true;
                            attributeValue[i] = "" + j;
                            break;
                        }
                    }
                    if (checked != true) {
                        symbolTable[j][0] = new String(lexemes[i]);
                        int index = i;
                        counter++;
                        while (index > 0) {
                            if (tokensName[index].equals(lexemes[index])) {
                                symbolTable[j][1] = tokensName[index];
                                attributeValue[i] = "" + j;
                                break;
                            }
                            index--;
                        }
                    }
                }
            }

            // Print output
            System.out.println("Lexemes\t\t\tToken Type\t\t\tAttribute Value");
            bufferwriter.write("Token Table");
            bufferwriter.newLine();
            bufferwriter.write("______________________________________________________________________________________________________________");
            bufferwriter.newLine();
            bufferwriter.write("Lexemes\t\t\tToken Name\t\t\tAttribute Value");
            bufferwriter.newLine();

            bufferwriter.write("______________________________________________________________________________________________________________");
            bufferwriter.newLine();
            System.out.println("______________________________________________________________________________________________________________");
            System.out.println();

            for (int i = 0; i < lexemes.length; i++) {
                if (tokensName[i].equals("id") || tokensName[i].equals("number")) {
                    System.out.println(lexemes[i] + "\t\t\t" + tokensName[i] + "\t\t\t\t" + attributeValue[i]);

                    bufferwriter.write(lexemes[i] + "\t\t\t" + tokensName[i] + "\t\t\t" + attributeValue[i]);
                    bufferwriter.newLine();
                } else {
                    System.out.println(lexemes[i] + "\t\t\t" + tokensName[i] + "\t\t\t" + attributeValue[i]);

                    bufferwriter.write(lexemes[i] + "\t\t\t" + tokensName[i] + "\t\t\t" + attributeValue[i]);
                    bufferwriter.newLine();

                }
            }

            bufferwriter.newLine();
            bufferwriter.newLine();
            bufferwriter.newLine();
            bufferwriter.newLine();

            System.out.println();

            bufferwriter.write("Symbol Table");
            bufferwriter.newLine();
            bufferwriter.write("______________________________________________________________________________________________________________");
            bufferwriter.newLine();

            System.out.println("Symbol\t\t\tToken\t\t\tData Type\t\t\tPointer to Symbol Table Entry");
            bufferwriter.write("Symbol\t\t\tToken\t\t\tData Type\t\t\tPointer to Symbol Table Entry");
            bufferwriter.newLine();
            System.out.println("______________________________________________________________________________________________________________");
            bufferwriter.write("______________________________________________________________________________________________________________");
            bufferwriter.newLine();
            for (int i = 0; i < counter; i++) {
                for (int j = 0; j < 2; j++) {
                    System.out.print(symbolTable[i][j] + "\t\t\t");

                    bufferwriter.write(symbolTable[i][j] + "\t\t\t");
                    //bufferwriter.newLine();
                }
                System.out.println("id\t\t\t\t" + i);
                bufferwriter.write("id\t\t\t\t" + i);
                bufferwriter.newLine();
            }

            scanner.close();
            bufferwriter.close();
        } catch (Exception e) {

            e.printStackTrace();

        }

    }

    public void stringAdd(int index, String line) {

        String str = new String();
        for (int j = position; j < index; j++) {
            str = str + line.charAt(j);
        }
        if (str.length() > 0) {
            str = str.trim();
            vector.add(str);
        }

        position = index + 1;
    }

    public static void main(String[] args) {

        LexicalAnalysis la = new LexicalAnalysis();

        la.lexicalAnalysis();
    }
}
