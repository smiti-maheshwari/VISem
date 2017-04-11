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
import LR_0_states

#import matplotlib.pyplot as plt

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

rev_graph = {}

set_terminal = ()

set_non_terminal = ()

follow_table = {}

terminals_mark = []

root =''

unique_next_elem = {}

mini_item_set = {}

action = {} # for terminals the actions that are supposed to be taken

goto = {} # for non-terminals the goto states

# this will contain the list of production and thier serial numbers
serial_productions = {}

# this will contain the state transitions i.e. del(S1,a) -> S2
state_transitions = {}
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


def get_data_LR_0_states(input_file) :  # needed by this function
  global graph, set_terminal, set_non_terminal, root, first_table, mini_item_set, follow_table, serial_productions, state_transitions
  graph, set_terminal, set_non_terminal, root, first_table, mini_item_set, follow_table, serial_productions, state_transitions = LR_0_states.get_data_LR_0_states(input_file)

  #return graph, set_terminal, set_non_terminal, first_table, follow_table, root, mini_item_set, serial_productions
#definiton ends here



def get_data_parser_table(input_file) :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, serial_productions, action, goto

  get_data_LR_0_states(input_file) # get data

  return_val = generate_LR_0_parser_table()

  return graph, set_terminal, set_non_terminal, first_table, follow_table, root, action, goto, serial_productions 

#methos ends here





def update_aug(lists) :
  return copy.copy(lists)
#end


def generate_LR_0_parser_table() :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, mini_item_set, action, state_transitions,serial_productions, goto

  LL1_table = {}

  action = {}
  goto = {}
  error = 0

  print mini_item_set

  set_terminal = set(list(set_terminal) + ['$'])


  '''for state_id in copy.copy(mini_item_set) :
    # print mini_item_set[state_id]

    for tuple_of_3 in mini_item_set[state_id] :
      print tuple_of_3'''

  for state_id in copy.copy(mini_item_set) :
    #print len(mini_item_set[state_id])
    #continue

    for tuple_of_3 in mini_item_set[state_id] :
      #print tuple_of_3
      #tuple_of_3 = mini_item_set[state_id][tuple_of_3]
      next_elem = LR_0_states.extract_next_element(tuple_of_3) 
      #print next_elem + ";;;"
      #print tuple_of_3

      if tuple_of_3[0] == root and tuple_of_3[2] == len(tuple_of_3[1]) : # that means that it is the transition S' -> S.
        action.update({state_id : {'$' : ['A', 'Accept']}})
      elif next_elem in set_terminal :
        if not next_elem in  state_transitions[state_id] :
          error = 1 #no transitoin found.. error in construction
          break

        if state_id in action :
          if not next_elem in action[state_id] :
            action[state_id].update({next_elem : ['S', state_transitions[state_id][next_elem]]})
          elif action[state_id][next_elem][0] == 'R' :
            action[state_id].update({next_elem : ['S', state_transitions[state_id][next_elem]]})
          else :
            if action[state_id][next_elem] == state_transitions[state_id][next_elem] :
              print "yesyes"
              error = 2 #conflict
              break
        else :
          action.update({state_id : {next_elem : ['S', state_transitions[state_id][next_elem]]}})

      elif next_elem == "" :
        #meaning that it is a productio of form                     A -> aBG.
        serial_no_prod = serial_productions[tuple_of_3[0]][tuple_of_3[1]] # this contains the serial number of the production.... tuple_of_3[0]-> tuple_of_3[1]

        #meaning that it is a productio of form                     A -> aBG.
        for term_elem in set_terminal : # tuple_of_3[0] = A for above example

          if state_id in action :
            if not term_elem in action[state_id] :
              action[state_id].update({term_elem : ['R', serial_no_prod]})
            else :
              if not action[state_id][term_elem][0] == 'S' :
                action[state_id].update({term_elem : ['R', serial_no_prod]})
              #error = 2 #conflict
              #break
          else :
            action.update({state_id : {term_elem : ['R', serial_no_prod]}}) 

      elif next_elem in set_non_terminal :
        if not next_elem in  state_transitions[state_id] :
          error = 1 #no transitoin found.. error in construction
          break

        #print tuple_of_3

        if state_id in goto :
          #print next_elem
          if not next_elem in goto[state_id] :
            goto[state_id].update({next_elem : state_transitions[state_id][next_elem]})
          else :
            if not goto[state_id][next_elem] == state_transitions[state_id][next_elem] :
              error = 2 #conflict
              break
        else :
          goto.update({state_id : {next_elem : state_transitions[state_id][next_elem]}})     

    if error == 2 or error == 1 :
      break



  #print action

  print set_terminal

  if error == 1 :
    print "The tree construction failed [ No transition found erro ]!!"
    return 1
  elif error == 2 :
    print "The grammar has conflict!!"
    return 2
  
  return 0

#end LL1 parser

def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, first_table, follow_table, root, serial_productions, action, goto

  get_data_LR_0_states(input_file) # get data

  return_val = generate_LR_0_parser_table()

  print "--action--"
  print action
  print "--goto--"
  print goto
  print "--serial_productions--"
  print serial_productions

  print "<-----------START----------------->"

  if return_val == 0 : # no error
    print "action"
  else :
    print return_val
    print "Terminating due to exception"

  #write_to_file.write_output(filename = output_file, inp = result_str, typer="str")

#end method




def prepare_output() :
  global LL1_table

  result_str = ""

  for non_terminals in LL1_table :
    for terminal in LL1_table[non_terminals] :
      result_str +=  '(' + non_terminals + ", " + terminal + ") -> " + LL1_table[non_terminals][terminal] + '\n'
    
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