import sys
import operator
import re
import os
import shutil
import atexit
import yaml
import json
import sys
import gc
import math
import Queue

#this recursive function prepares ..
#  recursive tab seperated output
#  FORMAT : tree format
#  keys
#     keys1 : [value / list of value]    
#       keys2
#       keys3
#         keys4 : [value/ list of values] ......

def tab_seperated_output(x, tab) : # do not change anymore
  result = ""
  typer = type(x)

  if typer is list :
    print x
    for i in x :
      if not type(i) == str :
        result += str(i) + '\t'
      else :
        result += i + '\t'

    return result + '\n'

  if typer is int or typer is float or typer is str :
    return str(x) + '\n'

  #print x

  if typer is dict :
    for keys in x :
      #print str(keys) + 'k'
      if type(keys) == str :
        result += '\n' + tab +  keys + ' : ' + str(tab_seperated_output(x[keys], tab + '\t'))
      else :
        result += '\n' + tab +  str(keys) + ' : ' + str(tab_seperated_output(x[keys], tab + '\t'))

    return result



#filename is tile in which file is intended to be witten
#dic is the dictionary that is to be written
#typer is the type of output that is desired in the file that is
# that is either json file 
# or tab seperated output
def write_output(filename, inp, typer) :
  if typer == "json":  # we will later extend this code for tab seperated output
    #i.e. in tab seperated the values will be json output : minus the parenthesis
    fHandle = open(filename, 'w')
    fHandle.write(json.dumps(inp, indent = 1))
    fHandle.close()
  if typer == "tsv" :
    result = tab_seperated_output(inp, "")
    #print  result
    fHandle = open(filename, 'w')
    fHandle.write(result)
    fHandle.close()
  if typer == "str" :
    fHandle = open(filename, 'w')
    fHandle.write(inp)
    fHandle.close()
