import commands
import platform
import os
import random
import numpy as np
from os.path import join
from matplotlib import pyplot as plt
from pylab import *
from matplotlib.ticker import MultipleLocator, FormatStrFormatter

global_results = {}
average_results = {}
folder = ""
folder_windows = ""


""" This script takes 2 arguments
    1 - What to do:
        run_tests: runs the application base.c, and print a graphic with the results
        make_test: make the test to the program
    2 - Folder
        This argument is the folder where the user wants to work. In case of the run_tests arg, this is the folder where the tests are.
    
    Made by:
    Marco Simoes
    Nuno Lourenco
    
"""


def make_graphs(graph_title, files):
    x = range(len(files))
    fig = plt.figure(figsize=(10,7))
    ax1 = fig.add_subplot(111)
    plt.subplots_adjust(left=0.075, right=0.85, top=0.9, bottom=0.20)
    plt.ylabel("Time(s)")
    count = 0
    print global_results
    for key in global_results:
        title(graph_title)
        ax1.axis = [0,max(degree),0,20]
        ax1.plot(x,global_results[key],'o-',label=str(key))
        xticks(x, tuple(files),rotation=45)
        count += 1
        legend(loc=2)
        savefig(folder + "/" + graph_title + ".png")
    show()


def execute_command(cmd):
    output = ""
    if windows == True:
       (child_stdin, child_output) = os.popen2(cmd)
       for line in child_output:
            output += line
    else:
        (exit_status, output) = commands.getstatusoutput(cmd)
    return output
    
 
def make_test(n, num_inputs):

    cmd = "echo "+ str(n) + " " + str(num_inputs) +"| python in-out/generate-inputs.py > " + folder + "/input" + str(n) + "_" + str(num_inputs) + ".txt"
    junk = execute_command(cmd)


def run_evaluation(input_file):
    global windows
    if windows == True:
        cmd = "TP1CAD.exe < "+ folder_windows + "\\" + input_file
    else:
        cmd = "./base < " + folder + "/" + input_file
    return execute_command(cmd)

def save_results(results, inp):
    for key in results:
        if key in global_results:
            aux = global_results[key]
            if len(aux) > inp:
                aux[inp] += results[key]
            else:
                aux.append(results[key])
            global_results[key] = aux
        else:
            global_results[key] = [results[key]]
    
        
def convert_to_time(s):
    aux = s.split(" = ")
    return float(aux[1])
    
def sort_input_files(input_files):
    ordered_vector = []
    file_vector = []
    print input_files
    start = len("input")
    for name in input_files:
        end = name.index("_")
        number = int(name[start:end])
        if ordered_vector == []:
            ordered_vector.append(number)
            file_vector.append(name)
        else:
            inserted = False
            for i in range(len(ordered_vector)):
                if number < ordered_vector[i]:
                    ordered_vector.insert(i,number)
                    file_vector.insert(i,name)
                    inserted = True
                    break
            if inserted == False:
                ordered_vector.append(number)
                file_vector.append(name)
    print file_vector
    return file_vector
                
            
        
        
    

def run_tests():
    global windows
    count = 0
    tests_result = {}
    if windows == True:
        result = os.listdir(folder_windows)
    else:
        cmd = "ls " + folder
        result = execute_command(cmd)
        result = result.split("\n")
    input_files = result
    #for fi in ['generate-inputs.py', 'Professor_Tests']:
     #   input_files.remove(fi)
        
    input_files = filter(lambda x : (x.startswith("input") == True),result)
    input_files = sort_input_files(input_files)
    inp = 0
    for input_file in input_files:
          print input_file
          count +=1
          result = run_evaluation(input_file)
          result = result.split("\n")
          for i in range(0,len(result) - 1,2):
              tests_result[result[i]] = convert_to_time(result[i + 1]) 
          save_results(tests_result, inp)
          inp += 1
          
          #if count == len():
              #break
    return input_files
      
    
if __name__ == "__main__":
    global windows
    degree = [0,1,2,3]
    total_runs = 3
    folder = sys.argv[2]
    print folder
    graph_title = "High Degree"
    if platform.system()[:3].lower() == "win":
        folder_windows = folder
        windows = True
    else:
        windows = False       
    if sys.argv[1] == "make_tests":
        try:
            os.mkdir(folder)
        except:
            print "Folder Already Exists"
        tests = 1
        num_inputs = 50
        n = 0
        for n in degree:
            print n
            num_inputs = 100
            make_test(n,num_inputs)
        tests += 1
    elif sys.argv[1] == "run_tests":
        for i in range(total_runs):
            files = run_tests()
        for key in global_results:
            for i in range(len(global_results[key])):
                global_results[key][i] /= total_runs
        make_graphs(graph_title,files)
    
    
    


