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

root = ""

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
#input file ends\


def execute_external(input_file) : # this file is needed by follow
  global graph, set_terminal, set_non_terminal, first_table, augmented_dic, root

  get_data(input_file)

  augmented_dic = generate_follow_variables()

  create_follow_table = gen_follow_table(root)

#end method


def follow_and_l_recursion_removal(input_file) :
  global graph, root, set_terminal, set_non_terminal, first_table, follow_table, augmented_dic

  graph, root, set_terminal, set_non_terminal, first_table = first.first_and_l_recursion_removal(input_file)

  augmented_dic = generate_follow_variables()

  #print augmented_dic

  create_follow_table = gen_follow_table(root)

  return first_table, follow_table


def get_data_follow(input_file) : # this is by follow to return data to other files
  global graph, set_terminal, set_non_terminal, first_table, root, follow_table, serial_productions
  graph, set_terminal, set_non_terminal, first_table, root, serial_productions = first.get_data(input_file)

  execute_external(input_file)

  return graph, set_terminal, set_non_terminal, first_table, follow_table, root, serial_productions
#definiton ends here




def get_data(input_file) : 
  global graph, set_terminal, set_non_terminal, first_table, root, serial_productions
  graph, set_terminal, set_non_terminal, first_table, root, serial_productions = first.get_data(input_file)
#definiton ends here


def update_aug(lists) :
  return copy.copy(lists)
#end


def generate_follow_variables() :
  global graph, set_terminal, set_non_terminal

  augmented_dic = {}


  for elem in graph :
    for prod in graph[elem] :

      segmented_production = first.segment_production_into_terminals_and_non_terminals(prod)

      len_of_production = len(segmented_production)

      for i in range(len_of_production - 1) :
        if segmented_production[i] in augmented_dic :
          augmented_dic[segmented_production[i]] += [update_aug(segmented_production[i + 1:] + [elem])]
        else :
          augmented_dic[segmented_production[i]] = [update_aug(segmented_production[i + 1:] + [elem])]
      #end loop

      #linking last element to the BOSS
      if segmented_production[len_of_production - 1] in augmented_dic :
        augmented_dic[segmented_production[len_of_production - 1]] += [update_aug([elem])]
      else :
        augmented_dic[segmented_production[len_of_production - 1]] = [update_aug([elem])]

    #end 2nd inner loop
  #end outer loop

  for elem in set_non_terminal :
    #print elem
    if not elem in augmented_dic :
      print "yes"
      augmented_dic[elem] = [[]]
  #covering the loop hole

  #print augmented_dic
  #print "--Augmented dic----"


  return augmented_dic

#end of method



#this compute follow is equivalent to the dfs routine

def compute_follow(queue) :
  global augmented_dic, follow_computed, reverse_relation, follow_table, first_table

  dependency = {}

  #print augmented_dic

  while not queue.empty() :

    elem = queue.get()
    list_dependency = []
    last_values = []

    print elem
    #print len()
    print follow_table


    if elem in follow_table :
      last_values = follow_table[elem]



    for lists in augmented_dic[elem] : 
      # list of augmented structure containing elements per production that are 
      #required for calculation of the follow_table
      #print lists
      len_list = len(lists)

      if len_list == 1:
        gen_elem = lists[len_list - 1]
        if not gen_elem in follow_table :
          queue.put(gen_elem)
        else :
          list_dependency += follow_table[gen_elem]


        # E -> ABDDK... reverse_relation[E] = K#
        ''' so that if E is updated in the future then the FOLLOW of K is again calculated'''
        if gen_elem in reverse_relation :
          if not elem in reverse_relation[gen_elem]:
            reverse_relation[gen_elem] += [elem]
        else :
          reverse_relation[gen_elem] = [elem]

        # E -> ABDDK... dependency[K] = E#
        if elem in dependency :
          if not gen_elem in dependency[elem] :
            dependency[elem] += [gen_elem]
        else :
          dependency[elem] = [gen_elem]
      #endif

  
      for elem_index in range(len_list - 1) :
        list_dependency += first_table[lists[elem_index]]

        ''' checking if the last element contains EPSILON in FIRST SET'''
        if '@' in first_table[lists[elem_index]] :
          if elem_index == (len_list - 2) : 
            gen_elem = lists[len_list - 1]

            if not gen_elem in follow_table :
              queue.put(gen_elem)
            else :
              list_dependency += follow_table[gen_elem]


            # E -> ABDDK... reverse_relation[E] = K#
            ''' so that if E is updated in the future then the FOLLOW of K is again calculated'''
            if gen_elem in reverse_relation :
              if not lists[elem_index] in reverse_relation[gen_elem]:
                reverse_relation[gen_elem] += [lists[elem_index]]
            else :
              reverse_relation[gen_elem] = [lists[elem_index]]

            # E -> ABDDK... dependency[K] = E#
            if lists[elem_index] in dependency :
              if not gen_elem in dependency[lists[elem_index]] :
                dependency[lists[elem_index]] += [gen_elem]
            else :
              dependency[lists[elem_index]] = [gen_elem]

            #recursive call if the code 
            #compute_follow(lists[index])

            #list_dependency += follow_table[gen_elem]
        else :
          break

        ''' this means that the moment a PRODCUTION ELEMENT is found that does not contain the EPSILON '@' it is the end'''
      #end inner loop
    #end outer loop

    #if updated .. then dependant variables have to be updated and hence inserted into the queue
    list_dependency = list(set(list_dependency))
    if '@' in list_dependency :
    	list_dependency.remove('@')

    if not list(set(list_dependency)) == last_values :
      follow_table[elem] = copy.copy(list(set(list_dependency).union(set(last_values)))) # becoz '@' is our epsilon

      if elem in reverse_relation :
        for nodes in reverse_relation[elem] :
          queue.put(nodes)
        #end loop
    #end loop

#end method


def gen_follow_table(root) :
  global graph, set_terminal, augmented_dic, set_terminal,set_non_terminal, first_table, follow_table

  follow_computed = {}
  reverse_relation = {}

  queue = Queue.Queue()
  queue.put(root)

  follow_table[root] = ['$']

  print follow_table

  for elem in set_non_terminal :
    queue.put(elem)
    #print elem + ' l l l '

  compute_follow(queue)
  #  end loop

#end method

def executer(input_file, output_file) :
  global graph, set_terminal, set_non_terminal, first_table, augmented_dic, root

  get_data(input_file)

  augmented_dic = generate_follow_variables()

  #print augmented_dic

  create_follow_table = gen_follow_table(root)

  result_str = prepare_output()

  write_to_file.write_output(filename = output_file, inp = result_str, typer="str")

#end method




def prepare_output() :
  global follow_table

  result_str = ""

  for non_terminals in follow_table :
    result_str +=  non_terminals + " -> "
    for first in follow_table[non_terminals] :
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