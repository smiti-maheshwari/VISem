import java.util.*;
import java.io.*;

class Ques1 {

    ArrayList< HashMap<String, Integer> > database = new ArrayList< HashMap<String, Integer> >();

    ArrayList< HashMap< Set<String>, Integer > > store = new ArrayList< HashMap< Set<String>, Integer> >();

    HashMap< Set<String>, Set<String>> selectedAssociations = new HashMap<Set<String>, Set<String>>();

    double supportPercent = 0.8;

    int support; /*Min support count*/
    double confidence = 0.9; /*Min confidence count*/

    int maxItems = 75;
    int totalTransactions = 0;

    void run() {
        getInput("data/chess.dat");
        support = (int)(supportPercent * totalTransactions);
        generateL1();
        int lNumber = 2;
        while (true) {
            boolean isEmpty = generateL(lNumber);
            if (isEmpty == true) {
                break;
            }
            lNumber++;
        }
        createAssociations(lNumber);
        displayAssociations();
    }
    
    void generateL1() {
        HashMap< Set<String>, Integer> l1 = new HashMap< Set<String>, Integer>();
        /*Not for general cases*/
        for (int i = 1; i <= maxItems; i++) {
            int count = 0;
            String item = "" + i;
            
            Set<String> itemset = new TreeSet<String>(new Comp());
            itemset.add(item);

            count += getFrequency(itemset);

            if (count >= support) {
                l1.put(itemset, count);
                System.out.println(itemset + " " + count);
            }
        }
        store.add(l1);
    }

    boolean generateL(int lNumber) {
        boolean isEmpty = true;
        HashMap<Set<String>, Integer> current = new HashMap<Set<String>, Integer>();
        HashMap< Set<String>, Integer> previous = store.get(lNumber - 2);

        ArrayList< Set<String>> entryArr = new ArrayList<Set<String>>(previous.keySet());

        for (int i = 0; i < entryArr.size(); i++) {
            for (int j = i + 1; j < entryArr.size(); j++) {
                Set<String> first = entryArr.get(i);
                Set<String> second = entryArr.get(j);

                if (isCompitable(first, second)) {
                    Set<String> joined = join(first, second);

                    if (isApriori(joined, previous)) {

                        int count = getFrequency(joined);
                        if (count >= support) {
                            isEmpty = false;
                            current.put(joined, count);
                            System.out.println(joined + " " + count);
                        }
                    }
                }
            }
        }
        if (!isEmpty) {
            store.add(current);
        }
        return isEmpty;
    }

    void createAssociations(int lNumber) {
        for (Set<String> currentSet : store.get(lNumber - 2).keySet()) {
            ArrayList<Set<String>> allSubsets = createAllSubsets(currentSet);
            for (int i = 0; i < allSubsets.size(); i++) {
                Set<String> currentSubset = allSubsets.get(i);
                Set<String> difference = getDifference(currentSet, currentSubset);
                double currConfidence = (double)getSupport(currentSet) / getSupport(currentSubset);
                System.out.print(currentSubset + " " + difference);
                System.out.println(" -> " + getSupport(currentSet) + " / " + getSupport(currentSubset) + " = " + currConfidence);
                if (currConfidence >= confidence) {
                    selectedAssociations.put(currentSubset, difference);
                }
            }
        }
    }

    void displayAssociations() {
        System.out.println("");
        System.out.println("Support: " + supportPercent + " Confidence: " + confidence);
        for (Set<String> key : selectedAssociations.keySet()) {
            System.out.println(key + " -> " + selectedAssociations.get(key));
        }
    }

    boolean isApriori(Set<String> itemset, HashMap<Set<String>, Integer> previous) {
        ArrayList<Set<String>> subsets = createSubsets(itemset);

        for (int i = 0; i < subsets.size(); i++) {
            Set<String> currentSubset = subsets.get(i);
            if (previous.get(currentSubset) == null) {
                return false;
            }
        }
        return true;
    }

    int getFrequency(Set<String> itemset) {
        int frequency = 0;
        for (HashMap<String, Integer> transaction : database) {
            int minimum = 10000;
            for (String item : itemset) {
                if (transaction.get(item) != null) {
                   if (transaction.get(item) < minimum) {
                       minimum = transaction.get(item);
                   }
                } else {
                    minimum = 0;
                }
            }
            frequency += minimum;
        }
        return frequency;
    }

    int getSupport(Set<String> set) {
        int length = set.size();
        return store.get(length - 1).get(set);
    }

    boolean isCompitable(Set<String> first, Set<String> second) {
        for (int i = 0; i < first.size() - 1; i++) {
            if (!get(first, i).equals(get(second, i))) {
                return false;
            }
        }
        return true;
    }

    Set<String> join(Set<String> first, Set<String> second) {
        Set<String> result = new TreeSet<String>(new Comp());
        for (int i = 0; i < first.size() - 1; i++) {
            result.add(get(first, i));
        }
        result.add(get(first, (first.size() - 1)));
        result.add(get(second, (second.size() - 1)));
        return result;
    }

    ArrayList<Set<String>> createSubsets(Set<String> set) {
        ArrayList<Set<String>> subsets = new ArrayList<Set<String>>();
        for (int i = 0; i < set.size(); i++) {
            Set<String> currentSubset = new TreeSet<String>(new Comp());
            for (int j = 0; j < set.size(); j++) {
                if (j == i) {
                    continue;
                }
                currentSubset.add(get(set, j));
            }
            subsets.add(currentSubset);
        }
        return subsets;
    }

    ArrayList<Set<String>> createAllSubsets(Set<String> set) {
        ArrayList<Set<String>> allSubsets = new ArrayList<Set<String>>();
        int maxSize = (int)Math.pow(2, set.size());

        for (int i = 1; i < maxSize - 1; i++) {
            Set<String> current = new TreeSet<String>(new Comp());
            for (int j = 0; j < set.size(); j++) {
                if ((i & (1 << j)) != 0) {
                    current.add(get(set, j));
                }
            }
            allSubsets.add(current);
        }
        return allSubsets;
    }

    Set<String> getDifference(Set<String> first, Set<String> second) {
        Set<String> result = new TreeSet<String>(new Comp());
        for (int i = 0; i < first.size(); i++) {
            String element = get(first, i);
            if (!second.contains(element)) {
                result.add(element);
            }
        }
        return result;
    }

    String get(Set<String> set, int index) {
        int count = 0;
        for (String ans : set) {
            if (count == index) {
                return ans;
            }
            count++;
        }
        return null;
    }

    void fillDatabase(String input) {
        String[] split = input.split(" ");
        HashMap<String, Integer> hm = new HashMap<String, Integer>();
        for (String item : split) {
            if (hm.get(item) == null) {
                hm.put(item, 1);
            } else {
                hm.put(item, hm.get(item) + 1);
            }
        }
        database.add(hm);
    }

    void getInput(String filename) {
        try {
            BufferedReader br = new BufferedReader(new FileReader(filename));
            String input;
            while ((input = br.readLine()) != null) {
                totalTransactions++;
                fillDatabase(input);
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private class Comp implements Comparator<String> {
        public int compare(String a1, String a2) {
            return Integer.parseInt(a1) - Integer.parseInt(a2);
        }
    }


    public static void main(String[] args) {
        new Ques1().run();
    }
}
