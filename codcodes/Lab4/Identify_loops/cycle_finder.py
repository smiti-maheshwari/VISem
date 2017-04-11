import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
sys.path.insert(0, '/usr/local/lib/python2.7/dist-packages/')
import Dominator
import write_to_file

#import matplotlib.pyplot as plt

result_str = ""

visited = []

graph = {}

rev_graph = {}

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


def create_graph_parse_DOT_file(data) :
  dic = {}
  rev_dic = {}

  for line in data.split('\n') :
    line = line.strip()

    if len(line) <= 1 or not line[-1] == ';':
      continue
    
    if line[-1] == ';' and line[-2] == ']' :
      continue

    #print line[:-1]

    a, b = line[:-1].split('->')

    #print line
    a = a.strip()
    b = b.strip()

    #print a
    #print b

    if a in dic :
      dic[a] += [b]
    else :
      dic.update({a : [b]})

    if b in rev_dic :
      rev_dic[b] += [a]
    else :
      rev_dic.update({b : [a]})

  #loop ends

  update_keys = list(set(dic.keys()).union(rev_dic.keys()))

  for keys in update_keys :
    if not keys in dic :
      dic.update({keys : []})

    if not keys in rev_dic :
      root = keys
      rev_dic.update({keys : []})
  #end loop


  return dic, rev_dic, root
# end of funciton generate cycles




def update_result(list_trav) :
  global result_str

  for elem in list_trav :
    result_str += elem + "\t"

  result_str += "\n\n"

#end method




def dfs_st(elem, back_edge_parent, traversed) :
  global rev_graph

  for pred in rev_graph[elem] :
    if pred == back_edge_parent :
      update_result(traversed + [pred])
  
    if pred in traversed :
      continue

    dfs_st(pred, back_edge_parent, traversed + [pred])
  #end loop

#end method


def cycle_finder(dom_graph) :
  global result_str, graph, rev_graph

  for elem in dom_graph :
    print elem
    print dom_graph[elem]
    print graph[elem]
    back_edge_parent = (set(dom_graph[elem]).intersection(graph[elem]))

    print back_edge_parent

    if len(back_edge_parent) == 0 :
      continue

    for pred in list(back_edge_parent) :
      dfs_st(elem, pred, [elem])
    # end inner loop

  #end outer loop


def executer(input_file, output_file) :
  global graph, rev_graph

  data = read_file(input_file)

  graph, rev_graph, root = create_graph_parse_DOT_file(data)

  dom_graph = Dominator.dominator(graph, rev_graph, root)

  print dom_graph

  cycle_finder(dom_graph)

  write_to_file.write_output(filename = output_file, inp = result_str, typer = "str")

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