import sys
import re
import os
import shutil
import atexit
import json
import sys
import gc
sys.path.insert(0, '/usr/local/lib/python2.7/dist-packages/')
#import matplotlib.pyplot as plt
import Queue
import copy

'''
  Objective of the code :

  1. This CODE takes input of the DOT graph ..

  2. Then it calls creates calls DOMINATOR GRAPH ..

  3. Then it calls the CYCLE finding routine 

'''


def dominator(graph, rev_graph, root):
  dom = {}

  q_update = Queue.Queue()

  dom.update({root : [root]})
  q_update.put(root)

  keys_not_root = graph.keys()
  keys_not_root.remove(root)

  '''print graph

  print keys_not_root'''
  
  for node in keys_not_root :
    dom.update({node : copy.copy(graph.keys())})
    #q_update.put(node)

  #print dom
  
  count = 0

  while not q_update.empty() :
    elem = q_update.get()

    count += 1

    #print elem
    for child in graph[elem] :
      list_int = []
      #print child + "-->>"

      child_content = copy.copy(dom[child])

      for parent in rev_graph[child] :
        #print parent + "===========>>>>"
        if list_int == [] :
          list_int = dom[parent]
        else :
          list_int = list(set(list_int).intersection(dom[parent]))

      if child in dom :
        dom[child] = list(set([child]).union(list_int))
      else :
        dom.update({child : list(set([child]).union(list_int))})

      if not set(child_content) == set(dom[child]) :
        #print child
        q_update.put(child)
      '''else :
        print list_int
        print child_content
        print graph[child]'''

      '''print child_content
      print dom[child]'''


    # end loop
    '''if count == 10 :
      break'''

  #end while loop

  return dom
#end of dominator function