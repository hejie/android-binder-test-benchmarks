#--*-- coding:utf-8 --*--
#
#     Author: Feng,Li
#     Mail  :   lifeng1519@gmail.com
#
#
#
#
#
#


import sys
import os

def ps_parser(psfile):
  '''
  eg:
  system    2582  2571  461960 30156 c054dadc 40016110 S Binder Thread #
  return [pid] = [ppid,proc_name]
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
argv[1]: transaction output
argv[2]: "ps -t -P -x" output
"""
def main():
  with open(sys.argv[1],"r") as f:
    data = f.read()
  pid_list = ps_parser(sys.argv[2])
  lines = [line for line in data.split("\n") if len(line.strip()) != 0]
  print "{0:^15} {1:^25}    {2:^25}".format("type","caller","callee")
  tmp_data = []
  for line in lines:
    fields = line.split()
    calltype = fields[1]
    from_pid = fields[3]
    to_pid = fields[5]

    pid = from_pid.split(':')[0]
    from_name = pid
    if pid in pid_list:
      from_name = os.path.basename(pid_list[pid][1])

    pid = to_pid.split(':')[0]
    to_name = pid
    if pid in pid_list:
      to_name = os.path.basename(pid_list[pid][1])
    print "{0:15} {1:25} => {2:25}".format(calltype,from_name,to_name)


if __name__ == '__main__':
  main()
