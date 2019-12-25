# ----------------------init
import os
from os import path
import glob
import sys
import shutil
#from subprocess import call

# -----------------------------
# python3 build_win32_migw64.py
# -----------------------------
# -------------------------------functions------------------------------------


def copy_dir(src, dst, folder, just_contents):
    dirs = [folder]
    i = 0
    line_counter = 0
    while True:
        if(i == len(dirs)):
            break
        # print("Dir:"+dirs[i],i,len(dirs),src,dst)
        item = dirs[i]
        #del dirs[i]
        with os.scandir(src+"/"+item) as it:
            for entry in it:
                if entry.is_file():
                    new_dst = dst+"/"+item+"/"+entry.name
                    if item == folder:
                        new_dst = dst+"/"+entry.name
                    # print("Cfile:",src+"/"+item+"/"+entry.name,new_dst);
                    if os.path.exists(new_dst):
                        os.remove(new_dst)
                    ret = shutil.copyfile(src+"/"+item+"/"+entry.name, new_dst)
                    # print("\nCOpy",ret)
                    #if len(ret)<2 : print("Not copy :(");
                elif entry.is_dir():
                    # print("Cdir:",dst+"/"+item+"/"+entry.name);
                    if not os.path.exists(dst+"/"+item+"/"+entry.name):
                        os.makedirs(dst+"/"+item+"/"+entry.name)
                    dirs.append(item+"/"+entry.name)
        i += 1


# -------------------------------functions------------------------------------

os.system("clear")
# os.system("echo -e \"Default \e[34mBlue\"")
os.system("tput setaf 4") #set foreground blue
# ----------------------define vars
# enable warnings :  -Wall -Wextra
cflags = "-I ../include -std=c99  -fmax-errors=2 -c "
build_folder = "../linux32-release"
obj_folder = "../tmp"
scr_folder = "../src"
compiler = "gcc "
is_error = 0
logfile = "build_linux32_gcc_list.txt"
compfiles = []
writefiles = []
# ----------------------open build_linux32_gcc_list.txt
if os.path.exists(logfile):
    f = open(logfile, "r")
    compfiles = f.readlines()
    f.close()
# for j in compfiles:
#	print(compfiles)
# ----------------------
print("\t~~~~~MAHDI Build Tool (BY Python3) V 4.1~~~~~")
print("=== Start Building linux32 release of MAHDI Interpreter using GCC (C99) ....")
# ----------------------init dirs
# -----create docs file
if not os.path.exists(build_folder+"/docs"):
    os.makedirs(build_folder+"/docs")
# copy_dir("../", build_folder, "docs", False)
# -----create modules file
if not os.path.exists(build_folder+"/modules"):
    os.makedirs(build_folder+"/modules")
# -----create libs file
if not os.path.exists(build_folder+"/packs"):
    os.makedirs(build_folder+"/packs")
# -----create samples file
if not os.path.exists(build_folder+"/samples"):
    os.makedirs(build_folder+"/samples")
# -----delete all obj/.*o
# if os.path.exists(obj_folder):
 #   objs=os.listdir(obj_folder);
  #  for ob in objs:
   #     os.remove("obj/"+ob);
# -----create obj file
if not os.path.exists(obj_folder):
    os.makedirs(obj_folder)

# -----delete mahdi executable
if os.path.exists(build_folder+"/bin/mahdi"):
    os.remove(build_folder+"/bin/mahdi ")

# ----------------------compile mahdi sources
print("=== Start compiling source files [mahdi]...")
sources = [
    [scr_folder+"/main.c", scr_folder+"/main.c -o "+obj_folder+"/main.o"],
    # [scr_folder+"/builder.c",scr_folder+"/builder.c -o "+obj_folder+"/builder.o"],
    [scr_folder+"/mahdi_help.c",scr_folder+"/mahdi_help.c -o "+obj_folder+"/mahdi_help.o"],
    # [scr_folder+"/module.c",scr_folder+"/module.c -o "+obj_folder+"/module.o"],
    [scr_folder+"/data_defined.c", scr_folder + "/data_defined.c -o "+obj_folder+"/data_defined.o"],
    [scr_folder+"/exceptions.c",scr_folder+"/exceptions.c -o "+obj_folder+"/exceptions.o"],
    # [scr_folder+"/mahdi_debugger.c",scr_folder+"/mahdi_debugger.c -o "+obj_folder+"/mahdi_debugger.o"],
    [scr_folder+"/mahdi_builtin.c",scr_folder+"/mahdi_builtin.c -o "+obj_folder+"/mahdi_builtin.o"],
    [scr_folder+"/tools/common_funcs.c", scr_folder +"/tools/common_funcs.c -o "+obj_folder+"/common_funcs.o"],
    [scr_folder+"/tools/console.c", scr_folder + "/tools/console.c -o "+obj_folder+"/console.o"],
    [scr_folder+"/tools/strings.c", scr_folder + "/tools/strings.c -o "+obj_folder+"/strings.o"],
    [scr_folder+"/tools/chars.c", scr_folder + "/tools/chars.c -o "+obj_folder+"/chars.o"],
    [scr_folder+"/tools/lists.c", scr_folder + "/tools/lists.c -o "+obj_folder+"/lists.o"],
    # [scr_folder+"/tools/encoder.c", scr_folder +  "/tools/encoder.c -o "+obj_folder+"/encoder.o"],
    [scr_folder+"/tools/utf8.c", scr_folder +"/tools/utf8.c -o "+obj_folder+"/utf8.o"],
    [scr_folder+"/tools/syscalls.c", scr_folder + "/tools/syscalls.c -o "+obj_folder+"/syscalls.o"],
    [scr_folder+"/core/runkit.c",scr_folder+"/core/runkit.c -o "+obj_folder+"/runkit.o"],
    [scr_folder+"/core/parser.c",scr_folder+"/core/parser.c -o "+obj_folder+"/parser.o"],
    [scr_folder+"/core/importer.c",scr_folder+"/core/importer.c -o "+obj_folder+"/importer.o"],
    [scr_folder+"/core/inheritance.c",scr_folder+"/core/inheritance.c -o "+obj_folder+"/inheritance.o"],    [scr_folder+"/core/starter.c",scr_folder+"/core/starter.c -o "+obj_folder+"/starter.o"],
    # [scr_folder+"/core/run_mgr.c",scr_folder+"/core/run_mgr.c -o "+obj_folder+"/run_mgr.o"],
    [scr_folder+"/core/memory.c",scr_folder+"/core/memory.c -o "+obj_folder+"/memory.o"],
    # [scr_folder+"/built_in/mpl_builtin.c",scr_folder+"/built_in/mpl_builtin.c -o "+obj_folder+"/mpl_builtin.o"],
    # [scr_folder+"/built_in/os_builtin.c",scr_folder+"/built_in/os_builtin.c -o "+obj_folder+"/os_builtin.o"],
    # [scr_folder+"/built_in/data_builtin.c",scr_folder+"/built_in/data_builtin.c -o "+obj_folder+"/data_builtin.o"],
    # [scr_folder+"/built_in/fs_builtin.c",scr_folder+"/built_in/fs_builtin.c -o "+obj_folder+"/fs_builtin.o"],

    # [scr_folder+"/modules/sqlite_interface.c",scr_folder+"/modules/sqlite_interface.c -o "+obj_folder+"/sqlite_interface.o"]
]
for i in range(0, len(sources), 1):
    ind = sources[i]
    mtime = os.path.getmtime(ind[0])
    if len(compfiles)==len(sources) and float(compfiles[i]) == mtime:
        print("=> Before Compiled: "+ind[0])
    else:
        # print(mtime,compfiles[i]);
        is_error = os.system(compiler+cflags+ind[1])
        os.system("tput setaf 4") #set foreground blue
        print("=> Compiled: "+ind[0])
        # print('compile:',is_error)
        if is_error != 0:
            os.system("tput setaf 1") #set foreground red
            print("*** Failed Compiling! ***")
            exit(1)

    writefiles.append(mtime)
# ----------------------save modified date of source files
fi = open(logfile, "w+")
for kl in writefiles:
    # print(kl)
    fi.write(str(kl)+"\n")
fi.close()

# ----------------------link object files
print("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_")
print("=== Start linking object files [mahdi]...")

# if os.path.exists("win32rc.res"):
#    os.remove("win32rc.res")
# is_error=os.system("windres win32_resources.rc -O coff -o win32rc.res")
# if is_error==1:os.system("color C0"); exit(1);
obj_files = glob.glob(obj_folder+"/*.o")
all_files = ' '.join(obj_files)
#print("gcc .\win32rc.res "+all_files+" -o "+build_folder+"/mpl.exe")
# is_error=os.system("gcc win32rc.res "+all_files+" -o "+build_folder+"/mpl.exe")
is_error = os.system("gcc -fPIC "+all_files+" -o "+build_folder+"/mahdi")
# print('link:',is_error)
# ----------------------finish
if is_error != 0:
    os.system("tput setaf 1") #set foreground red
    print("*** Failed Linking! ***")
    # ----------------------pause
    # os.system("pause");
else:
    os.system("tput setaf 2") #set foreground green
    print("=== Finish Building! All Done in "+build_folder+" folder")
    # ----------------------run gdb
    #print("=== Running GDB ...");
    #os.system("gdb "+build_folder+"/bin/mpl.exe main.mpl");
    # ----------------------run mahdi
    print("=== Running mahdi executable ...")
    print("_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_")
    os.system("tput sgr0") #reset colors
    os.system("../linux32-release/mahdi ../devdocs/test.mah")
    #os.system("../win32-release/mpl.exe ../mprog.mpl init my_project");
    #os.system("../win32-release/mpl.exe -h keywords null");
    # os.system("dir");
    # os.system("pause");
