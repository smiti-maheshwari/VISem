import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
import write_to_file
import first
import follow
import Queue
import copy

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

set_terminal = ()

set_non_terminal = ()

first_table = {}

follow_table = {}

follow_computed = {}

reverse_relation = {}

LL1_table = {}

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
#input file ends\


def get_data_ll1(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root
  graph, set_terminal, set_non_terminal, first_table, follow_table, root = follow.get_data_follow(input_file)

  generate_LL1_parser_table()

  return graph, set_terminal, set_non_terminal, first_table, follow_table, LL1_table, root

#definiton ends here

def follow_and_l_recursion_removal(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root
  graph, root, set_terminal, set_non_terminal, first_table, follow_table = follow.follow_and_l_recursion_removal(input_file)
#definiton ends here


def ll1_table_follow_and_l_recursion_removal(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, LL1_table
  graph, root, set_terminal, set_non_terminal, first_table, follow_table = follow.follow_and_l_recursion_removal(input_file)

  generate_LL1_parser_table()

  return graph, root, set_terminal, set_non_terminal, first_table, follow_table, LL1_table

#definiton ends here


def get_data_follow(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root
  graph, set_terminal, set_non_terminal, first_table, follow_table, root = follow.get_data_follow(input_file)
#definiton ends here


def update_aug(lists) :
  return copy.copy(lists)
#end


def find_first(prod) :

  list_first_production = []

  bool_epsilon = True

  list_elem_prod = first.segment_production_into_terminals_and_non_terminals(prod)


  for elem in list_elem_prod :
    print elem + ' ---- '
    list_first_production += []

    list_first_production += first_table[elem]

    if not '@' in list_first_production :
      bool_epsilon = False
      break
  # end for


  firsts = list(set(list_first_production))
  #print firsts

  if '@' in firsts : 
    return firsts.remove('@'), bool_epsilon
  else :
    return firsts, bool_epsilon

def generate_LL1_parser_table() :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, LL1_table

  LL1_table = {}

  error = False

  for elem in graph :
    for prod in graph[elem] :

      first_prod, bool_epsilon = find_first(prod)

      #CONDITION 1 of first
      if not first_prod == None :
        #print first_prod 
        for terminal in first_prod :
          if elem in LL1_table :
            if terminal in LL1_table[elem] :
              error = True
              break

            LL1_table[elem][terminal] = copy.copy(prod)
          else :
            LL1_table.update({elem : {terminal : copy.copy(prod)}})
        #end inner loop

      if error :
        break

      #CONDITION 2 of follow
      if bool_epsilon :
        print 'yrs'
        for follows in follow_table[elem] :
          print follows + '---------->'
          if elem in LL1_table :
            if follows in LL1_table[elem] :
              error = True
              break

            LL1_table[elem][follows] = copy.copy(prod)
          else :
            LL1_table.update({elem : {follows : copy.copy(prod)}})
        #end inner loop
      #end if

    #end inner loop 1


    if error :
      break
  #break outer loop 

  print LL1_table

  if error :
    print "This grammar has ambiguity .. error!!"

#end LL1 parser

def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root

  #get_data_follow(input_file)

  follow_and_l_recursion_removal(input_file)

  print "<-----------START----------------->"

  generate_LL1_parser_table()

  result_str = prepare_output()

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")

#end method




def prepare_output() :
  global LL1_table

  result_str = ""

  for non_terminals in LL1_table :
    for terminal in LL1_table[non_terminals] :
      result_str +=  '(' + non_terminals + ", " + terminal + ") -> " + LL1_table[non_terminals][terminal] + '\n'
    
  # end outer loop

  print LL1_table

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