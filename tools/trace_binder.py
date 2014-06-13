#--*-- coding:utf-8 --*--
#
#     Author: Feng,Li(lifeng1519@gmail.com)

"""
获取Binder调用的Client端和Service端
==============================
分析ftrace的输出，可以得出基本的调用关系。 分析基于以下原理：

- Client是同步请求
- Client在内核中是先调用binder_thread_write发送数据，然后再调用binder_thread_read阻塞住，等待service返回数据。此时会设置service进程（线程）为running状态，调度器选择service进程（线程）运行。
- service的进程（线程）在空闲时是阻塞binder_thread_read上。被唤醒后，从binder_thread_read继续运行。
- 当一个进程A调用完binder_thread_read之后，被切换走，运行另外一个进程B从binder_thread_read继续调用，可以得出结论，binder的调用是从A--->B的。

工具见： https://gist.github.com/vonnyfly/40c3299b917306c20498

import sys
"""

def ps_parser(psfile):
  '''
  eg:
  system    2582  2571  461960 30156 c054dadc 40016110 S Binder Thread #
  '''
  pid_list = {}
  with open(psfile,"r") as f:
    lines = f.readlines()
  for line in lines[1:]:
    fields = line.split()
    if len(fields) < 9:
      continue
    pid_list[fields[1]] = [fields[2]," ".join(fields[8:])]
  return pid_list

"""
argv[1]: ftrace function output
argv[2]: ps -t -P -x output
"""
def main():
  with open(sys.argv[1],"r") as f:
    data = f.read()
  pid_list = ps_parser(sys.argv[2])
  lines = [line for line in data.split("\n") if len(line.strip()) != 0]
  tmp_data = []
  for line in lines:
    tmp_data.append([line[0:21],line[30:41],line[43:]]) if not line.startswith("#") else None
  print "{0:^15} {1:^25}    {2:^25}".format("Time","caller","callee")
  for i,t in enumerate(tmp_data[:-1]):
    if "binder_thread_read <-" in tmp_data[i][2] and "<-binder_thread_read" in tmp_data[i+1][2] and tmp_data[i][0] != tmp_data[i+1][0]:

      task_name = tmp_data[i][0]
      if "Binder Thread #" in task_name:
        pid = task_name.split('-')[1]
        if pid in pid_list.keys():
          ppid = pid_list[pid][0]
          tmp_data[i+1][0] = pid_list[ppid][1]

      task_name = tmp_data[i+1][0]
      if "Binder Thread #" in task_name:
        pid = task_name.split('-')[1]
        if pid in pid_list.keys():
          ppid = pid_list[pid][0]
          tmp_data[i+1][0] = pid_list[ppid][1]
      else:
        pid = task_name.split('-')[1]
        if pid in pid_list.keys():
          tmp_data[i+1][0] = pid_list[pid][1]


      print "{0:15} {1:25} => {2:25}".format(tmp_data[i][1],tmp_data[i][0],tmp_data[i+1][0])

if __name__ == '__main__':
  main()
