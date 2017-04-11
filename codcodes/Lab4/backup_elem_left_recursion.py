import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
import write_to_file

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

rev_graph = {}

set_terminal = ()

set_non_terminal = ()

terminals_mark = []

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

  return 0
#end of method



def classify_terminals_non_terminals(data) :
  global set_terminal, set_non_terminal

  non_terminal_list = []
  terminal_list = []

  split_data = data.split('\n')

  for line in split_data :
    for elem in line :
      class_elem = upper_case_lower_case(elem)
      if class_elem == 1:
        non_terminal_list += [elem]
      elif class_elem == 2 :
        terminal_list += [elem]

    #end inner loop
  #end outer loop

  set_terminal = set(terminal_list)
  set_non_terminal = set(non_terminal_list)


#classification functions ends


def elr_core(non_terminal, prod, count) :
  global graph

  new_list = []

  new_non_terminal = non_terminal + ("'" * count)

  for elem in graph[non_terminal] :
    if elem == prod :
      continue

    if not elem == '$' :
      new_list += [elem + new_non_terminal]
    else :
      new_list += [new_non_terminal]
  #end loop

  extract_alpha = prod[len(non_terminal):]

  #update to graph
  graph.update({new_non_terminal : [extract_alpha + new_non_terminal, '$']})

  graph[non_terminal] = new_list

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

  non_terminal_count = count_apostrophe(non_terminal_temp_list)

  #non_terminal_count = [0] * len(non_terminal_temp_list)

  while True:
    count = non_terminal_count[i]

    for prod in graph[non_terminal_temp_list[i]] :
      if prod.startswith(non_terminal_temp_list[i]) :
        count += 1
        #next line needs editing
        new_non_terminal = elr_core(non_terminal_temp_list[i], prod, count)
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