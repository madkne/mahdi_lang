
import sys
import os.path
import os
#----------------------------------------------------lambda for check a file in binary
textchars = bytearray({7,8,9,10,12,13,27} | set(range(0x20, 0x100)) - {0x7f})
is_binary_string = lambda bytes: bool(bytes.translate(None, textchars))
#----------------------------------------------------
print("\t Files Line Counter(V 1.6)")
filename=os.path.dirname(__file__);
if len(sys.argv)>1: filename=sys.argv[1];
path = os.path.abspath(os.path.join(filename))
print ("Searched path is: \""+path+"\"")
#----------------------------------------------------
dirs=[path];
i=0;
line_counter=0
while True:
    if(i==len(dirs)):break;
    #print("Dir:"+dirs[i],i,len(dirs))
    item=dirs[i]
    #del dirs[i]
    with os.scandir(item) as it:
        for entry in it:
            if entry.name.startswith('.'): continue;
            if entry.is_file():
                if is_binary_string(open(item+"/"+entry.name, 'rb').read(1024)) : continue
                f = open(item+"/"+entry.name,"r")
                co=0;
                #print("###:"+item+"/"+entry.name)
                while True:
                    line = f.readline().strip(' ')
                    if(len(line)>1):co+=1
                    if not line: break
                print("- File:"+item+"/"+entry.name+"\t .....=> "+str(co))
                line_counter+=co
            elif entry.is_dir():
                dirs.append(item+"/"+entry.name)
    i+=1
#----------------------------------------------------
#print(str(dirs))
print("==========\n\t Total Counter:"+str(line_counter)+"\n==========")
        