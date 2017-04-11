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

''' Assumption the first NON-TERMINAL in line1 is the root'''
def construct_graph(data) :
  dic = {}
  rev_dic = {}

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

      if a in dic :
        dic[a] += [prod_rule]
      else :
        dic.update({a : [prod_rule]})
    #end inner loop
  #end loop

  print dic

  return dic, root

# end of funciton generate enhanced productions



def upper_case_lower_case(char_data) :
  if char_data.isupper() :
    return 1

  if char_data.islower() :
    return 2

  if char_data == "'" :
    return -1

  return 0
#end of method



def classify_terminals_non_terminals(data) :
  global set_terminal, set_non_terminal

  non_terminal_list = []
  terminal_list = []

  split_data = data.split('\n')

  lock = False
  construct_non_terminal = ""

  for line in split_data :
    for elem in line :
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

    #end inner loop

    if lock == True :
      non_terminal_list += [construct_non_terminal]
      construct_non_terminal = ""
      lock = False

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

  for char_elem in prod :
    if char_elem == "'" :
      result_str += char_elem
    else :
      result_list += [result_str]
      result_str = char_elem
  #end loop

  ''' last update being added to the LIST'''

  result_list += [result_str]

  return result_list

# method ends here


def check_first_available_for_production(prod) :
  global table

  list_term_non_term = segment_production_into_terminals_and_non_terminals(prod)

  if elem[0] in list_term_non_term :
    return 1

  return 0

# ends here


def update_first(current_node, list_term_non_term) :
  global table, set_terminal

  bool_epsilon = True

  result_list = []

  for elem in list_term_non_term :
    if bool_epsilon and not '$' in table[elem] :
      bool_epsilon = False

    result_list += copy.copy(table[elem])

    if elem in set_terminal :
      bool_epsilon = False
      break
  #end for

  ''' That all non terminals have epsilon'''
  if bool_epsilon :
    result_list += ['$']

  graph[elem] = (set(result_list).intersection(graph[elem]))




# update first ends here


def dfs(current_node) :
  global table

  if current_node in table :
    return

  ''' segementing the production into 
    a list'''
  list_term_non_term = segment_production_into_terminals_and_non_terminals(prod)

  for prod in graph[current_node] :
    check_available = check_first_available_for_production(prod)

    if check_available ==  1 :
      update_first(current_node, list_term_non_term)
    else :

      for elem in list_term_non_term :
        dfs(elem)

      update_first(current_node, list_term_non_term)
    # else block ends here
  # loop ends here

  return
# dfs ends here




def find_first(root) :
  global graph, set_terminal, set_non_terminal

  dfs(root)
  
  for terminals in graph :
    dfs(terminals)

# find_first method ends here



def preprocess_terminals() :
  global table, set_terminal

  for elem in set_terminal :
    table.update({elem : elem})
  #loop ends here

  # for our program '$' is the epsilon
  table.update({'$' : '$'})


#method ends here




def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, table

  data = read_file(input_file)
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  preprocess_terminals()

  find_first(root)
#end method






def main() :
  args = sys.argv[1:]

  if not args:
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>]'
    #print 'usage2: [old_filename]'
    sys.exit(1)

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_file" :
    input_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--output_file" :
    output_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--output_file <cycles>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------
  executer(input_file = input_file, output_file = output_file) 

if __name__ == '__main__' :
  main()