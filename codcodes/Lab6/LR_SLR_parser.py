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
import LL1
import Queue
import copy
import SLR_1_parser_table
import LR_0_parser_table

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

serial_productions = {}

serial_productions_2 = {}

action_table = {}

goto = {}

root = ""

log = ''

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


def get_data_parser_table(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, action_table, goto, serial_productions, serial_productions_2

  graph, set_terminal, set_non_terminal, first_table, follow_table, root, action_table, goto, serial_productions = LR_0_parser_table.get_data_parser_table(input_file)

  print "******************"
  print first_table
  print "*****first*****" 
  print follow_table
  print "*****follow*****"
  print action_table
  print "*****action******"
  print goto
  print "********goto*****"
  print serial_productions
  print "******serial_productions*****"

  preprocess_serial_production()

  print serial_productions_2

#definiton ends here


def update_aug(lists) :
  return copy.copy(lists)
#end


def update_log(stack_top, stack, inp_top, inp, action) :
  global log

  inp_top = ""
  stack_top = "" # this is done as we are using a legacy system and modified it to get our current work done

  stack_cpy = copy.copy(stack)
  #stack_cpy.reverse()

  log += stack_top + "".join(map(str, stack_cpy)) + '\t\t\t\t' + inp_top + inp + '\t\t\t\t' + action + '\n'

#end update log



def maintain_stack(inp_top, stack) :
  global action_table, goto, serial_productions_2, set_terminal, set_non_terminal

  stack_top = stack[-1]

  prod_no = action_table[stack_top][inp_top][1] # contains the production

  prod_elem_2_tuple = serial_productions_2[prod_no]

  for i in range(len(prod_elem_2_tuple[1]) * 2) :
    if stack == [] :
      return -1 # that is the stack has become prematurely empty
    stack.pop()
  #end loop

  if stack == [] :
    return -1

  stack_top = stack[-1]

  if not type(stack_top) == int :
    return -2

  non_terminal_of_prod = prod_elem_2_tuple[0]

  if not non_terminal_of_prod in goto[stack_top] :
    return -3

  goto_var = goto[stack_top][non_terminal_of_prod]

  stack.append(non_terminal_of_prod)

  stack.append(goto_var)

  return 1
#end method





def preprocess_serial_production():
  global serial_productions, serial_productions_2

  for elem in serial_productions : # this is the non terminal
    for elem2 in serial_productions[elem] : # this is the production
      serial_productions_2.update({serial_productions[elem][elem2] : [elem, elem2]})
    # end loop
  # end outer loop

# end methid


def parse_input(data) :
  global LL1_table, log, set_terminal, set_non_terminal, root, log, action_table, goto, serial_productions_2, serial_productions

  action = ""

  log = ""

  preprocess_serial_production()

  stack = [0]
  stack_top = 0

  inp = data + '$'

  #stack.append(root)

  print stack
  print inp

  accept = False

  while True :
    inp_top = inp[0]
    stack_top = stack[-1]

    print inp_top
    print stack_top
    print action_table[stack_top][inp_top][0]
    
    if not stack_top in action_table : # that is parsing error encounterd
      accept = False
      action = 'error111'
      update_log(stack_top, stack, inp_top, inp, action)
      break
    elif not inp_top in action_table[stack_top] : # that is parsing error encounterd
      accept = False
      action = 'error222'
      update_log(stack_top, stack, inp_top, inp, action)
      break
    elif inp_top in set_terminal and action_table[stack_top][inp_top][0] == 'A':
      action = 'Accepted'
      update_log(stack_top, stack, inp_top, inp, action)
      accept = True
      break
    elif inp_top in set_terminal and action_table[stack_top][inp_top][0] == 'S'  :
      action = 'Shift'
      update_log(stack_top, stack, inp_top, inp, action)
      stack.append(inp_top)
      stack.append(action_table[stack_top][inp_top][1])
      inp = inp[1:]
      inp_top = inp[0]
    elif inp_top in set_terminal and action_table[stack_top][inp_top][0] == 'R' :
      action = 'Reduce'
      prod_no = action_table[stack_top][inp_top][1]
      prod_elem_2_tuple = serial_productions_2[prod_no]
      action += " " + prod_elem_2_tuple[0] + " -> " + prod_elem_2_tuple[1]

      update_log(stack_top, stack, inp_top, inp, action)

      ret_val = maintain_stack(inp_top, stack)

      if not ret_val == 1 :
        print ret_val
        action = 'error'
        update_log(stack_top, stack, inp_top, inp, action)
        break

  return accept
#end method




def executer(input_file, input_string, output_file) :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, log, serial_productions, action_table, goto

  get_data_parser_table(input_file)

  data = read_file(input_string)

  data = data.strip()

  print data

  parse_result = parse_input(data)

  print log
  #print ok
  #result_str = prepare_output()

  if parse_result == True :
    print "Accepted"
  else :
    print "Error in parsing!!"
  

  write_to_file.write_output(filename = output_file, inp = log, typer="str")

#end method






def main() :
  args = sys.argv[1:]

  if not args:
    print 'usage1: [--input_file <DOT_file>] [--input_string <DOT_file>] [--output_file <cycles>]'
    #print 'usage2: [old_filename]'
    sys.exit(1)

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_file" :
    input_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--input_string <DOT_file>] [--output_file <cycles>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  if args[0] == "--input_string" :
    input_string = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--input_string <DOT_file>] [--output_file <cycles>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--output_file" :
    output_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_file <DOT_file>] [--input_string <DOT_file>] [--output_file <cycles>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------
  executer(input_file = input_file, input_string = input_string, output_file = output_file) 

if __name__ == '__main__' :
  main()