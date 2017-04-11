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

rev_graph = {}

set_terminal = ()

set_non_terminal = ()

follow_table = {}

terminals_mark = []

root =''

unique_next_elem = {}

mini_item_set = {}

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
#input file ends

def get_data(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, root, follow_table, serial_productions
  graph, set_terminal, set_non_terminal, first_table, follow_table, root, serial_productions = follow.get_data_follow(input_file)
#definiton ends here


def get_data_LR_0_states(input_file) :
  global  graph, set_terminal, set_non_terminal, root, first_table, mini_item_set, follow_table, serial_productions, state_transitions

  data = read_file(input_file)
  #print data
  
  get_data(input_file)

  LR0_table_construct()

  return  graph, set_terminal, set_non_terminal, root, first_table, mini_item_set, follow_table, serial_productions, state_transitions

#definiton ends here


def closure(production, count_of_states) :
  global set_terminal, set_non_terminal, graph

  #q = Queue.queue()

  mini_graph = []

  q2 = Queue.Queue()

  '''adding the elements of the queue into 'q2'.. productions whose closure is to be found'''

  print production

  for prod_elem in production :
    q2.put(prod_elem)
    #print prod_elem


  non_terminal_processed = []

  while not q2.empty() :

    prod_elem = q2.get()

    print prod_elem

    key = prod_elem[0] # there is only one key
    prod = prod_elem[1]
    loc = prod_elem[2]

    next_elem = extract_next_element(prod_elem)

    # we are making a list of the 3-tuple production for each element that was found in the queue top
    # the mini_graph hence produced will become the closure set of the 'count_of_state'
    print prod
    if len(prod) == loc or next_elem in set_terminal :
      print prod + 'll'
      print prod_elem 
      if not prod_elem in mini_graph :
        print "yes"
        mini_graph += [copy.copy(prod_elem)]
      continue

    if not prod_elem in mini_graph :
      print prod_elem
      print "----prod_elem-----"
      mini_graph += [prod_elem]

    #IF it is a non terminal then the corresponfing closure is computed and added to MINI_GRAPH
    closure_set_of_non_terminal = []
    ''' example
      A -> k.LJ
      adding 
      L -> .PKJ
      L -> .PKq
    '''

    #next_elem has already been computed above
    next_elem = extract_next_element(prod_elem) 

    #print next_elem
    #print set_non_terminal

    if next_elem in set_non_terminal and not next_elem in non_terminal_processed:
      non_terminal_processed += [next_elem] 
      print next_elem
      for elem in graph[next_elem] :
        temp_list = [next_elem, elem, 0]

        closure_set_of_non_terminal += [copy.copy(temp_list)]

        q2.put(copy.copy(temp_list))
      #end

      mini_graph += closure_set_of_non_terminal


    #---------------------------------------
  #mini_graph = copy.copy(production)
    print mini_graph
    print "------mini------"

  ''' this method is defined in the section below'''
  find_unique_next_element(mini_graph, count_of_states)

  print mini_graph
  print "------mini------"
  return mini_graph

#end of closure function


def extract_next_element(prod) :

  init = prod[2]

  if init == len(prod[1]) :
    return ""

  elem = prod[1][prod[2]]

  while init < len(prod[1]) :
    if init < len(prod[1]) - 1 and prod[1][init + 1] == "'" :
      elem += "'"
      init += 1
    else :
      break

  return elem

def find_unique_next_element(mini_graph, count_of_states) :
  global unique_next_elem 

  ''' this is the method that is used to compute those terminals or non-termianls that are common
  ex - > 
    A -> N.Lk
    G -> J.Lk

    the L is common to both and there will be transition for L to common states for these two productions
  '''
  i = 0

  unique_next_elem[count_of_states] = {}

  for prod in mini_graph :
    if prod[2] == len(prod[1]) :
      continue

    desired_elem = extract_next_element(prod)

    if desired_elem in unique_next_elem[count_of_states] :
      unique_next_elem[count_of_states][desired_elem] += [i]
    else :
      unique_next_elem[count_of_states][desired_elem] = [i]
    #endif

    i += 1

  #end for loop

#end method





def generate_seed(root) :
  global graph

  '''mini_graph = {}
  mini_graph[root] = ['.' + graph[root]]'''

  print graph
  #this contains the first production that will enter the Queue
  list_of_3 = [root, graph[root][0], 0]

  return list_of_3

#end definition




def generate_LR0_states_transition() :
  global graph, LR0_state_count, transitions, set_terminal, set_non_terminal, root, seed, mini_item_set, unique_next_elem, state_transitions

  queue = Queue.Queue()

  seed = generate_seed(root)

  queue.put([seed])

  seen_production = []

  added_to_queue = []

  #dfa_count_to_state = []

  count_of_states = 0
  present = 0
  queue_count = 0 #counts the number of states to be generated
  #update_mini_graph(seed)

  while not queue.empty() :

    #print queue

    queue_top = queue.get()

    print queue_top

    seen_production += [queue_top]

    mini_graph = closure(queue_top, count_of_states)

    print mini_graph

    mini_item_set[count_of_states] = copy.copy(mini_graph)

    transition_elements_done = []

    for prod_elem in mini_graph :
      desired_elem = extract_next_element(prod_elem)

      if desired_elem in transition_elements_done or desired_elem == "" :
        continue

      transition_elements_done += [desired_elem]
      temp_list = []

      for position in unique_next_elem[count_of_states][desired_elem] :
        temp_3_tuple = mini_graph[position]

        if temp_3_tuple[2] == len(temp_3_tuple[1]) :
          continue

        temp_list += [[temp_3_tuple[0], temp_3_tuple[1], temp_3_tuple[2] + len(desired_elem)]]
      #done for this particular desired elem

      #we add the temp_list of transitions produced only if "temp_list" is has not been seen before

      found =  contained(temp_list ,added_to_queue) 

      print found
      print "----found----"
      if found == -1 :
        queue_count += 1

        '''the following if condition is to update transition from current to a state that 
         will be expanded in the future '''
        if count_of_states in state_transitions :
          state_transitions[count_of_states].update({desired_elem : queue_count})
        else :
          state_transitions.update({count_of_states : {desired_elem : queue_count}})
        #conidition ends

        added_to_queue += [temp_list]
        queue.put(temp_list)
        print temp_list
      else :
        if count_of_states in state_transitions :
          state_transitions[count_of_states].update({desired_elem : found})
        else :
          state_transitions.update({count_of_states : {desired_elem : found}})
        #conidition ends

    #end prod elemenent

    count_of_states += 1
#end definition


def contained(temp_list, seen_production) :
  global mini_item_set

  default = False

  print temp_list
  print seen_production
  print "------seen_production--------"

  print mini_item_set
  print "-------mini_item_set--------"

  if temp_list in seen_production :
    default = True
    return seen_production.index(temp_list) + 1
  '''else :
    default = True

    for elem in seen_production :
      default = True
      for elem2 in temp_list :
        if not elem2 in elem :
          default = False
          break
      #done

      if default :
        break
    #end loop
  #end else'''



  '''if default : # it has been has been found in existing states
    for states in mini_item_set :
      default = True

      for elem2 in temp_list :
        if not elem2 in mini_item_set[states] :
          default = False
          break
      #dine
      if default :
        break
    #end loop

    return states
  else :
    return -1 #that it has not been found any existing state'''

  return -1
#end

def initialize() :
  global graph, LR0_table, transitions, set_terminal, set_non_terminal, root

  list_non_terminal = list(set_non_terminal)

  list_terminal = list(set_terminal)

  list_terminal += ["S'"]

  graph["S'"] = [root]

  root = "S'"

  set_non_terminal = set(list_non_terminal)

  set_terminal = set(list_terminal)

#end initialize


def LR0_table_construct() :
  global graph, LR0_state_count, transitions, set_terminal, set_non_terminal, root

  initialize()

  generate_LR0_states_transition()

  #print extract_next_element(['A', "Bl''p", 1]) + "ll"



def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, root, mini_item_set, follow_table, serial_productions, state_transitions

  data = read_file(input_file)
  #print data
  
  get_data(input_file)

  LR0_table_construct()

  #print graph
  #print first_table
  #print follow_table
  print mini_item_set

  print serial_productions

  print state_transitions

  #write_to_file.write_output(filename = output_file, inp = result_str, typer="str")
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