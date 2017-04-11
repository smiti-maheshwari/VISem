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

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

correspondance_dic = {}


follow_pos = {}

root = ""

seed = []

nodes = {}


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


def extract_first_pos(line) :

  str_data = (line.split('::')[3]).strip()

  first_p_list = []

  #print str_data

  for components in str_data.split(',') :

    if components.strip() == '' :
      continue

    first_p_list += [int(components.strip())]


  return first_p_list

#end of method


def create_data_structures(input_follow1, input_misc2) :
  global graph, follow_pos, correspondance_dic, root, seed

  data = read_file(input_follow1)
  follow_list = []

  for line in data.split('\n') :
    print line

    if line.strip() == '':
      continue

    a, b = line.split('::')

    a = a.strip()
    b = b.strip()

    follow_list = []

    if b == '' :
      continue


    for elem in b.split(',') :
      if elem == '' :
        continue

      follow_list += [int(elem.strip())]
    #end loop

    follow_pos[int(a)] = copy.copy(follow_list) # in the input file the follow_lists the duplicates have been removbed
  #end loop





  # crating correspondance lists ...........................................

  data = read_file(input_misc2)
  count = 0

  seed = []

  # extracting seed and updating correspomdance list
  for line in data.split('\n') :

    if line.strip() == '' :
      continue

    temp_list = line.split('::')

    if count == 0 : #required to update seed
      count = 1
      root = temp_list[0].strip()
      seed = extract_first_pos(line)
    #end if

    val = temp_list[0].strip()

    if not (val == '.' or val == '|' or val == '*') :
      if val in correspondance_dic :
        correspondance_dic[val] += [int(temp_list[1].strip())] # updating position corresponding to the variables
      else :
        correspondance_dic[val] = [int(temp_list[1].strip())]
    #endif

    #follow_pos[a] = copy.copy(follow_lists) # in the input file the follow_lists the duplicates have been removbed
  #end loop


  print seed
  print correspondance_dic

#end definition

def node_number(dfa_state) :
  global nodes

  for counts in nodes :
    if nodes[counts] == dfa_state :
      return counts


  return -1
#end method


def generate_DFA_graph() :
  global graph, correspondance_dic, seed, root, follow_pos, nodes

  print follow_pos

  queue = Queue.Queue()

  queue.put(set(seed))

  temp_list = []

  count = 1

  while not queue.empty() :

    print count

    dfa_state = queue.get()
    temp_list += [dfa_state] # to check if this node has been proccessed


    if node_number(dfa_state) == -1 :
      nodes[count] = dfa_state # to give a numberring to a dfa state
      count += 1

    present = node_number(dfa_state)

    graph[present] = {}

    new_state = []

    for val in correspondance_dic :
      new_state = []
      print val,
      print '----------->'

      for entry in correspondance_dic[val] :
        if entry in dfa_state :
          new_state += follow_pos[entry]
        #end if 
      #end inner loop

      if new_state == [] :
        continue

      set_new_state = set(new_state)


      # if a new state produced that has not been numbered
      dummy = node_number(set_new_state)

      if dummy == -1 :
        print present
        print count,
        print set_new_state
        nodes[count] = set_new_state
        graph[present][val] = count
        count += 1
      else :
        graph[present][val] = dummy
      # the ndoe is numbered

      if not set_new_state in temp_list :
        queue.put(set_new_state)
      #this will be expanded later

    #checking for each variable possible

    #count += 1
  #ending while loop



# end definition

def executer(input_follow1, input_misc2, output_file) :
  global graph,correspondance_dic, follow_pos, root, seed, nodes

  create_data_structures(input_follow1, input_misc2)

  generate_DFA_graph()

  print graph

  print nodes

  result_str = prepare_output()

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")

#end method




def prepare_output() :
  global nodes, graph

  result_str = ""

  for entry in graph :
    for elem in graph[entry] :
      result_str +=  str(entry) + ' , ' + elem + ' -> ' + str(graph[entry][elem]) + '\n'
    
  # end outer loop

  result_str += '\n\n\n'

  for entry in nodes :
    result_str += '(' + ','.join([str(elem) for elem in nodes[entry]]) + ') -> ' + str(entry) + '\n'

  return result_str
#end method





def main() :
  args = sys.argv[1:]

  if not args:
    print 'usage1: [--input_follow1 <follow_file>] [--input_misc2 <val_pos_nullable_firstpos_lastpos>] [--output_file <dfa_graph>]'
    #print 'usage2: [old_filename]'
    sys.exit(1)

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_follow1" :
    input_follow1 = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_follow1 <follow_file>] [--input_misc2 <val_pos_nullable_firstpos_lastpos>] [--output_file <dfa_graph>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--input_misc2" :
    input_misc2 = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_follow1 <follow_file>] [--input_misc2 <val_pos_nullable_firstpos_lastpos>] [--output_file <dfa_graph>]'
    sys.exit(1) 

  #-----------------------------------------------------------------------------------------------------------------------------------

  #INPUT filename
  if args[0] == "--output_file" :
    output_file = args[1]
    del args[0:2]
  else :
    print 'usage1: [--input_follow1 <follow_file>] [--input_misc2 <val_pos_nullable_firstpos_lastpos>] [--output_file <dfa_graph>]'
    sys.exit(1) 

  #-------------------------------------------------------------------------------------------------------------------------------------
  executer(input_follow1 = input_follow1,  input_misc2 = input_misc2, output_file = output_file) 

if __name__ == '__main__' :
  main()