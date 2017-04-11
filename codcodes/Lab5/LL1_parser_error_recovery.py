import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
import write_to_file
import Queue
import copy
import first
import follow
import LL1_error_recovery


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


def get_data_ll1(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, LL1_table

  graph, set_terminal, set_non_terminal, first_table, follow_table, LL1_table, root = LL1.get_data_ll1(input_file)
#definiton ends here



def ll1_table_follow_and_l_recursion_removal(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, LL1_table
  graph, root, set_terminal, set_non_terminal, first_table, follow_table, LL1_table = LL1_error_recovery.ll1_table_follow_and_l_recursion_removal(input_file)

#definiton ends here




def update_aug(lists) :
  return copy.copy(lists)
#end


def update_log(stack_top, stack, inp_top, inp, action) :
  global log

  #inp_top = ""

  stack_cpy = copy.copy(stack)
  #stack_cpy.reverse()

  log += "".join(stack_cpy) + stack_top + '\t\t\t\t' + inp_top + inp + '\t\t\t\t' + action + '\n'

#end update log

def update_stack(stack, action) :

  if action == '@' :
    return stack

  len_action = len(action)

  '''
  for index in range(len_action -1, -1, -1) :
    stack.append(action[index])'''

  while not action == "" :
    k, action = next_elem_from_back(action)
    stack.append(k)

  #end loop

  return stack
#end stack


#end update stack

#method to segment production into two parts
# ex -> A'eB'F'' to A'eB' and F''
def next_elem_from_back(input_string) :

  result = ""

  i = len(input_string) - 1

  while True :
    k = input_string[i]

    result = k + result

    if not k == "'" or input_string == "" :
      break

    i -= 1

  return result, input_string[:i]

#end method 


# method to extract top of stack
def next_elem(stack) :

  result = ""

  while True :
    k = stack.pop()

    result = k + result

    if not k == "'" or stack == [] :
      break


  return result
#end method 


def parse_input(data) :
  global LL1_table, log, set_terminal, set_non_terminal, root, log

  action = ""

  log = ""

  stack = ['$']

  inp = data + '$'

  stack.append(root)

  accept = False

  while True :

    print inp

    if stack == [] :
      error = 'error / Empty stack'
      stack_top = ''
      update_log(stack_top, stack, inp_top, inp, action)
      break

    stack_top = next_elem(stack)
    
    if (inp == '$' and stack_top == '$') :
      accept = True
      break


    if stack_top in set_terminal :
      inp_top = inp[0]
      inp = inp[1:]

      #print stack_top + 'k'
      #print inp_top + 'k'

      if stack_top == inp_top :
        action = 'match'
      else :
        ''' changes made here'''
        action = 'error, pop'
        #stack_top = stack.pop()
        update_log(stack_top, stack, inp_top, inp, action)
        #break

      update_log(stack_top, stack, inp_top, inp, action)

    else :
      inp_top = inp[0]

      if stack_top in LL1_table :
        if inp_top in LL1_table[stack_top] and LL1_table[stack_top][inp_top] == 'sync' : # this if has been added new
          action = 'error / sync plplp' + str(stack_top)
          #stack_top = stack.pop()
          if len(stack) == 1 :
            print 'pplplpl'
            inp = inp[1:]
            stack.append(stack_top)
            update_log("", stack, inp_top, inp[1:], action)
          else :
            update_log(stack_top, stack, inp_top, inp[1:], action)
        elif inp_top in LL1_table[stack_top] :
          action = LL1_table[stack_top][inp_top]
          update_log(stack_top, stack, inp_top, inp[1:], action)

          stack = update_stack(stack, action)
        else :
          action = 'error / null field - skip ' + str(inp_top) 
          update_log(stack_top, stack, inp_top, inp[1:], action)
          stack.append(stack_top)
          inp = inp[1:] # removing top of input
          #break
      else :
        action = 'error'
        print 'OKOK'
        print set_terminal
        update_log(stack_top, stack, inp_top, inp[1:], action)
        break
    # end else
  #end while

  if inp == '$' and stack == [] :
    log += '$\t\t\t\t$\t\t\t\tAccepted\n'
    accept = True

  return accept
#end method




def executer(input_file, input_string, output_file) :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, log

  ll1_table_follow_and_l_recursion_removal(input_file)

  data = read_file(input_string)

  data = data.strip()

  parse_result = parse_input(data)

  #result_str = prepare_output()

  if parse_result == True :
    print "Accepted"
  else :
    print "Error in parsing!!"

  write_to_file.write_output(filename = output_file, inp = log, typer="str")

  '''a = "T'EE''"

  k, a = next_elem_from_back(a)

  print k
  print a'''


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