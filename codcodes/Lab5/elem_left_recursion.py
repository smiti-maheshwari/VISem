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

serial_productions = {}

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


def graph_root_terminal_non_terminal(input_file) : # return details without left recursion removal
  global graph, set_terminal, set_non_terminal, serial_productions

  data = read_file(input_file)
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  return graph, root, set_terminal, set_non_terminal, serial_productions
#end of method



def left_recursion_removed_graph(input_file) :
  global graph, set_terminal, set_non_terminal

  data = read_file(input_file)
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  elem_left_recursion(graph)

  return graph, root, set_terminal, set_non_terminal
#end of method


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



def partition_prod_alpha_beta(non_terminal) :
  global graph

  alpha_list = []
  beta_list = []

  len_non_terminal = len(non_terminal)

  for prod in graph[non_terminal] :
    if prod.startswith(non_terminal) :
      alpha_list += [prod[len_non_terminal:]]
    else :
      beta_list += [prod]
  #end loop

  return alpha_list, beta_list


# end definition


#'@' is considered to be epsilon here

def elr_core(non_terminal) :
  global graph

  new_list = []

  new_non_terminal = non_terminal + "'"

  #new method added while modification
  alpha, beta = partition_prod_alpha_beta(non_terminal)

  alpha_production_list = []
  beta_production_list = []

  for elem in alpha :
    alpha_production_list += [elem + new_non_terminal]


  for elem in beta :
    beta_production_list += [elem + new_non_terminal]


  #update graph
  graph[non_terminal] = copy.copy(beta_production_list)
  graph[new_non_terminal] = copy.copy(alpha_production_list + ['@'])

  return new_non_terminal


# elr_core

def count_apostrophe(non_terminal_temp_list) :
  non_terminal_count = [0] * len(non_terminal_temp_list)

  for index in range(len(non_terminal_temp_list)) :
    striped_elem = non_terminal_temp_list[index].strip()

    non_terminal_count[index] = len(striped_elem[1:])
  #end loop

  return non_terminal_count

#end definition


def elem_left_recursion(root) :
  global graph, set_terminal, set_non_terminal



  i = 0
  non_terminal_temp_list = list(set_non_terminal)

  print non_terminal_temp_list
  
  non_terminal_count = count_apostrophe(non_terminal_temp_list)

  #non_terminal_count = [0] * len(non_terminal_temp_list)

  while True:
    count = non_terminal_count[i]

    for prod in graph[non_terminal_temp_list[i]] :
      if prod.startswith(non_terminal_temp_list[i]) :
        count += 1
        #next line needs editing
        new_non_terminal = elr_core(non_terminal_temp_list[i])
        non_terminal_temp_list += [new_non_terminal]
        non_terminal_count[i] = count
        non_terminal_count += [0]
        i -= 1 #this has been done, so that the CURRENT NON-TERMINAL IS ANALYZED AGAIN for LEFT RECURSION
        break # for REVALUATION of NON TERMINAL
      # end if
    #end inner loop

    i += 1

    if i == len(set_non_terminal) :
      break
  # end loop

# end left recursion





def prepare_output(graph) :
  
  result_str = ""

  for non_terminals in graph :
    result_str +=  non_terminals + " -> "
    for prod in graph[non_terminals] :
      result_str += prod + " | "
    # end loop

    result_str += "\n"
  # end outer loop

  return result_str
#end method




def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal

  data = read_file(input_file)
  
  classify_terminals_non_terminals(data)

  graph, root = construct_graph(data)

  elem_left_recursion(graph)

  result_str = prepare_output(graph)

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")

#end method






def main() :
  args = sys.argv[1:]

  if not args:
    print 'usage1: [--input_file <producitons>] [--output_file <enhanced productions>]'
    #print 'usage2: [old_filename]'
    sys.exit(1)

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_file" :
    input_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <producitons>] [--output_file <enhanced productions>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--output_file" :
    output_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <producitons>] [--output_file <enhanced productions>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------
  executer(input_file = input_file, output_file = output_file) 

if __name__ == '__main__' :
  main()