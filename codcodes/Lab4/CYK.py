import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
import write_to_file

import copy

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

rev_graph = {}

set_terminal = ()

set_non_terminal = ()

terminals_mark = []

table = {}

hash_table = {}

dic = {}

root = ""

'''
  Objective of the code :

  1. This CODE takes input of the DOT graph ..

  2. Then it calls creates calls DOMINATOR GRAPH ..

  3. Then it calls the CYCLE finding routine 

'''



def read_file(input_file) :
  fHandle = open(input_file, 'r')
  data = fHandle.read()
  fHandle.close()

  return data
#input file ends

def get_data(input_file) :
  global graph, set_terminal, set_non_terminal, table

  data = read_file(input_file)
  #print data
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  preprocess_terminals()

  find_first(root)

  #print segment_production_into_terminals_and_non_terminals(graph["E'"][0])
  
  result_str = prepare_output()

  #print table

  return graph, set_terminal, set_non_terminal, table, root
  #write_to_file.write_output(filename = output_file, inp = result_str, typer="str")
#end method


''' Assumption the first NON-TERMINAL in line1 is the root'''
def construct_graph(data) :
  dic = {}
  rev_dic = {}
  hash_table = {}

  count = 0


  for line in data.split('\n') :
    line = line.strip()

    if len(line) <= 1:
      continue

    #print line[:-1]

    a, b = line.split('->')

    #print line
    a = a.strip()
    b = b.strip()


    if count == 0:
      root = a
      count = 1


    #print a
    #print b
    b_split = b.split('|')

    #print b
    #print b_split

    b_split = [elem.strip() for elem in b_split]

    for prod_rule in b_split :
      if prod_rule  == '' :
        continue

      if prod_rule in hash_table :
        if not a in hash_table[prod_rule] : 
          hash_table[prod_rule] += [a]
      else :
        hash_table[prod_rule] = [a]

      if a in dic :
        dic[a] += [prod_rule]
      else :
        dic.update({a : [prod_rule]})
    #end inner loop
  #end loop

  #print dic
  print hash_table

  return dic, hash_table, root

# end of funciton generate enhanced productions



def upper_case_lower_case(char_data) :
  '''
    2 -> means terminal
    1 -> means non terminal
    0 -> charecters that are neither terminals nor non terminals
    -1 -> special charecter "'"
  '''

  if char_data.isupper() :
    return 1

  if char_data == "'" :
    return -1

  if char_data == ' ' or char_data == '\t' or char_data == '|' :
    return 0


  if char_data.islower() :
    return 2

  return 2
#end of method



def classify_terminals_non_terminals(data) :
  global set_terminal, set_non_terminal

  non_terminal_list = []
  terminal_list = []

  split_data = data.split('\n')

  lock = False
  construct_non_terminal = ""

  for line in split_data :
    a = line.split('->')
    #non_terminal_list += [a[0].strip()]

    for elem in a[1].strip() :
      class_elem = upper_case_lower_case(elem)

      if class_elem == -1 and lock :
        construct_non_terminal += "'"
        continue
      
      if class_elem == 1:
        if lock == True :
          non_terminal_list += [construct_non_terminal]
        
        lock = True
        construct_non_terminal = elem

        #non_terminal_list += [elem]
      elif class_elem == 2 :
        if lock ==True :
          non_terminal_list += [construct_non_terminal]
          lock = False

        terminal_list += [elem]
        #end elif
      elif lock :
        non_terminal_list += [construct_non_terminal]
        construct_non_terminal = ""
        lock = False 

    #end inner loop

    #print non_terminal_list

    if lock == True :
      non_terminal_list += [construct_non_terminal]
      construct_non_terminal = ""
      lock = False

    if construct_non_terminal.strip().islower():
      non_terminal_list += [construct_non_terminal]



  #end outer loop

  set_terminal = set(terminal_list)
  set_non_terminal = set(non_terminal_list)


#classification functions ends


def segment_production_into_terminals_and_non_terminals(prod) :
  result_list = []

  result_str = ""

  '''logic - when NEW TERMINAL OR NON-TERMINAL is found..
      the last one is added..

      as the NON TERMINALS may be of the form - A''
  '''
  #print prod

  count = 0

  for char_elem in prod :
    if count == 0:
      result_str = char_elem
      count = 1
      continue

    #print char_elem
    if char_elem == "'" :
      result_str += char_elem
    elif not result_str == "":
      result_list += [result_str]
      result_str = char_elem
  #end loop

  ''' last update being added to the LIST'''

  result_list += [result_str]

  #print result_list

  return result_list

# method ends here


def initialize_string(data_str) :
  global hash_table

  length_string = len(data_str)

  dic_struct = {}

  dic[0] = {}

  for i in range(length_string) :
    if not i in dic[0] :
      dic[0][i] = []

    for elem in hash_table[data_str[i]] :
        dic[0][i] += [elem]
    #end inner loop
  #end outer loop

  return dic

#end definition

def product_lists(lista, listb) :

  result_list = []

  for elem_a in lista :
    for elem_b in listb :
      result_list += [elem_a + elem_b]
    #end loop
  #end outer loop

  return result_list

#end definition


def compute_location(row, col) :
  global dic, hash_table

  p1_r = 0
  p1_c = col
  p2_r = row - 1
  p2_c = col + 1
  
  result_list = []

  while not p1_r == row : 
    intermediate_list = product_lists(dic[p1_r][p1_c], dic[p2_r][p2_c])

    for elem in intermediate_list :
      if elem in hash_table :
        result_list += hash_table[elem]

    p1_r += 1
    p2_r -= 1
    p2_c += 1

  #end while

  return list(set(result_list))

#end definition


def cyk_algorithm(data_str) :
  global graph, set_terminal, set_non_terminal, table, hash_table, dic, root

  length_string = len(data_str)

  dic = {}

  dic = initialize_string(data_str)

  print dic
  print "kklklk"

  len_data_str = len(data_str)
  print len_data_str
  print root

  for i in range(1, len_data_str) :
    dic[i] = {}
    for j in range(len_data_str - i) :
      print("%d %d" %(i, j))
      dic[i][j] = compute_location(i, j)
    #end innner loop
  #end outer loop

  if root in dic[len_data_str - 1][0] :
    print 'yayayaya'
    return True
  else :
    return False


#end method definition



def executer(input_file, output_file, test_strings) :
  global graph, set_terminal, set_non_terminal, table, hash_table, root

  data = read_file(input_file)
  #print data
  result_str = ""


  classify_terminals_non_terminals(data)

  graph, hash_table, root = construct_graph(data)

  print root

  data_str = read_file(test_strings)

  for line in data_str.split() :
    line = line.strip()
    if line == "" :
      continue
    
    bool_result = cyk_algorithm(line)

    print dic
    print bool_result

    if bool_result :
      result_str += line + " " + "Yes\n"
    else :
      result_str += line + " " + "No\n"

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")
#end method





def main() :
  args = sys.argv[1:]

  if not args:
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>] [--test_strings <test_strings>]'
    #print 'usage2: [old_filename]'
    sys.exit(1)

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_file" :
    input_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>] [--test_strings <test_strings>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--output_file" :
    output_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>] [--test_strings <test_strings>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--test_strings" :
    test_strings = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>] [--test_strings <test_strings>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------

  executer(input_file = input_file, output_file = output_file, test_strings = test_strings) 

if __name__ == '__main__' :
  main()