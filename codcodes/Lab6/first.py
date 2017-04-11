import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
import write_to_file
import elem_left_recursion

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

serial_productions  = {}

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
  global graph, set_terminal, set_non_terminal, table, serial_productions

  data = read_file(input_file)
  #print data
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  preprocess_terminals()

  find_first(root)

  #print segment_production_into_terminals_and_non_terminals(graph["E'"][0])
  
  result_str = prepare_output()

  #print table

  return graph, set_terminal, set_non_terminal, table, root, serial_productions
  #write_to_file.write_output(filename = output_file, inp = result_str, typer="str")
#end method


''' Assumption the first NON-TERMINAL in line1 is the root'''
def construct_graph(data) :
  global serial_productions
  dic = {}
  rev_dic = {}

  count = 0

  count_productions = 1

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
      if a in serial_productions :
        serial_productions[a].update({prod_rule : count_productions})
        count_productions += 1
      else :
        serial_productions.update({a : {prod_rule : count_productions}})
        count_productions += 1


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

    if not a[0].strip() in non_terminal_list :
      k = a[0].strip()
      non_terminal_list += [copy.copy(k)]

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

'''That is ALL ELEMENTS in that particular production is in FIRST TABLE'''
def check_first_available_for_production(prod) :
  global table, set_non_terminal

  list_term_non_term = segment_production_into_terminals_and_non_terminals(prod)

  ''' that is the all ELEMENTS of PRODUCTION has been found in the table'''

  #print prod
  #print set_terminal
  if prod[0] in set_terminal or prod[0] == '@' :
    return 1

  #print "yoyo"

  for elem in list_term_non_term :
    if not elem in table : #uf atleast one is not found then it has to be updated and hence 0 is returned
      return 0

  return 1

# ends here


def update_first(current_node, list_term_non_term) :
  global table, set_terminal

  bool_epsilon = True

  result_list = []

  for elem in list_term_non_term :
    if bool_epsilon and not '@' in table[elem] :
      bool_epsilon = False

    result_list += copy.copy(table[elem])

    ''' not bool_epsilon epsilon

        is important below as ..

        1. if we scan a NON-TERMINAL in a PRODUCTION and it has not epsilon in its production...
        2. Then the next NON-TERMINALs (if present) will not contribute to the first

        3. we add EPSILON (@) to the CURRENT_NODE's productions if all the NON-TERMINALs in the production... 
          contains an EPSILON (@)

    '''
    if elem in set_terminal or not bool_epsilon :
      bool_epsilon = False
      break
  #end for

  ''' That all non terminals have epsilon'''
  if bool_epsilon :
    result_list += ['@']

  #print "result_list"
  #print result_list

  if current_node in table :
    table[current_node] = list(set(result_list).union(set(table[current_node])))
  else :
    table.update({current_node : list(set(result_list))})



# update first ends here


def dfs(current_node) :
  global table, graph

  '''
  print current_node
  return
  
  '''

  #print "yesyes"

  if current_node in table :
    return

  ''' segementing the production into 
    a list'''



  for prod in graph[current_node] :
    check_available = check_first_available_for_production(prod)

    list_term_non_term = segment_production_into_terminals_and_non_terminals(prod)

    #print prod
    #print list_term_non_term

    if check_available ==  1 :
      #print "kljdalfjkljyes"
      update_first(current_node, list_term_non_term)
    else :

      for elem in list_term_non_term :
        #print elem
        dfs(elem)

      update_first(current_node, list_term_non_term)
    # else block ends here
  # loop ends here

  return
# dfs ends here




def find_first(root) :
  global graph, set_terminal, set_non_terminal

  print root

  dfs(root)
  
  for elem in graph :
    dfs(elem)
  #endd loop

# find_first method ends here



def preprocess_terminals() :
  global table, set_terminal

  for elem in set_terminal :
    table.update({elem : [elem]})
  #loop ends here

  '''for elem in set_non_terminal :
    table.update({elem : []})'''

  # for our program '@' is the epsilon
  table.update({'@' : ['@']})


#method ends here



def first_and_l_recursion_removal(input_file) :
  global graph, root, set_terminal, set_non_terminal, table

  graph, root, set_terminal, set_non_terminal = elem_left_recursion.left_recursion_removed_graph(input_file)

  preprocess_terminals()

  find_first(root)

  return graph, root, set_terminal, set_non_terminal, table


def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, table

  data = read_file(input_file)
  #print data
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  preprocess_terminals()

  #print root

  #print set_terminal

  #print set_non_terminal

  #root = "T"

  find_first(root)

  #print segment_production_into_terminals_and_non_terminals(graph["E'"][0])
  
  result_str = prepare_output()

  print table

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")
#end method




def prepare_output() :
  global table

  result_str = ""

  for non_terminals in table :
    result_str +=  non_terminals + " -> "
    for first in table[non_terminals] :
      result_str += first + " , "
    # end loop

    result_str += "\n"
  # end outer loop

  return result_str
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