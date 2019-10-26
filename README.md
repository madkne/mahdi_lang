**mAh_:** MAHDI Programming Language - MAHDI (BETA-0.0)
============================================

_This is a small,fast interpreter also Assemble all the good features of programming languages_


TODO
--------
 
 * get first build **[OK]** (1)


### ((:publish MAHDI-BETA version of MAHDI (for linux):))

* init exceptions **[OK]**
* define mpl arguments **[OK]**
* defnie mpl help **[OK]**
* init basic interpreter variables **[OK]**
* init data defined **[OK]**
* init virtual memory(vm) **[OK]**
* init built-in functions **[OK]**
* init built-in defines **[OK]**
* create memory structure **[OK]**
* reading source file as utf8 **[OK]**
* remove comments and convert utf8 to ascii **[OK]**
* extract tokens **[OK]**
* parsing tokens **[OK]**
* parsing imports **[OK]**
* parsing functions **[OK]**
* parsing structs **[OK]**
* parsing instructions **[OK]**
* parsing structures **[OK]**
* start runtime **[OK]**
* init global vars **[OK]**
* start APP_CONTROLLER **[OK]**
* start INSTRUCTION_EXECUTOR **[OK]**
* labeled instructions **[OK]**
* implement define vars **[OK]**
* set new var in memory **[OK]**
* delete var in memory **[OK]**
* manage exceptions **[OK]**
* create other memory functions **[OK]**
* determine type of data **[OK]**
* calculate math expressions **[OK]**
* calculate radix numbers **[OK]**
* calculate integer,float numbers **[OK]**
* calculate string expressions **[OK]**
* calculate utf8 string expressions **[OK]**
* calculate boolean expressions **[OK]**
* define struct expressions **[OK]**
* define complex struct expressions **[OK]**
* set built-in $def variables **[OK]**
* set built-in $con variables **[OK]**
* using magic macros **[OK]**
* set new $def **[OK]**
* define global vars **[OK]**
* edit $con **[OK]**
* calculate sum huge numbers **[OK]**
* calculate multiplication huge numbers **[OK]**
* calculate division huge numbers **[OK]**
* calculate power huge numbers **[OK]**
* implement function call **[OK]**
* determine type of function parameter values **[OK]**
* validation called function parameters **[OK]**
* set arrays in functions parameter **[OK]**
* init function parameters **[OK]**
* support for vars values parameters **[OK]**
* set structs in functions parameter **[OK]**
* switch to another function **[OK]**
* implement return keyword **[OK]**
* return values from function **[OK]**
* return value by index from function **[OK]**
* call built-in functions **[OK]**
* implement control characters **[OK]**
* implement print built-in function **[OK]**
* implement some built-in functions **[OK]**
* return values from built-in functions **[OK]**
* complete built-in function call **[OK]**
* implement garbage collector for functions **[OK]**
* implement alloc_vars **[OK]**
* alloc values expressions **[OK]**
* alloc array expressions **[OK]**
* alloc struct expressions **[OK]**
* replace values of vars by ':=' **[OK]**
* define an array by '?' index **[OK]**
* support alloc struct has array by '?' **[OK]**
* define an empty array **[OK]**
* implement short alloc by '++','--' pre **[OK]**
* implement short alloc by '++','--' post **[OK]**
* implement structures **[OK]**
* define manage **[OK]**
* start exception_handler **[OK]**
* implement push built-in functions **[OK]**
* push errors,warnings for manage var exception **[OK]**
* call gc for deleting structure vars **[OK]**
* define if **[OK]**
* define elif **[OK]**
* define else **[OK]**
* follow order of if,elif else **[OK]**
* implement condition level for managing if,elif,else **[OK]**
* recursive functions **[OK]**
* can use multiple 'if' one after another **[OK]**
* define loop **[OK]**
* define header defined vars **[OK]**
* labeled loop instructions **[OK]**
* check loop conditions every time **[OK]**
* do loop part3 every time **[OK]**
* delete local vars in end of loop **[OK]**
* implement loop level for managing loop **[OK]**
* implement next **[OK]**
* implement break **[OK]**
* using break instruction **[OK]**
* using next instruction **[OK]**
* delete header loop vars in finish **[OK]**
* access overhead vars **[OK]**

* **_complete main structure of mpl_** **[OK]**

* start compiling fs module **[OK]**
* init import modules to main program **[OK]**
* call module functions **[OK]**
* support for module libraries **[OK]**
* start fs module **[OK]**
* define fs functions header **[OK]**
* search for fs function and call it **[OK]**
* start for developing 'mprog' program **[OK]**
* init pre magic config,define entries **[OK]**
* analyze $con values after parsing **[OK]**
* implement mpl debugger (mdebug) **[OK]**
* create new breakpoint in mdebug **[OK]**
* complete print commands in mdebug **[OK]**
* complete next,run commands in mdebug **[OK]**
* complete symbols for commands in mdebug **[OK]**
* validate struct entries as a variable **[OK]**
* support struct entries as a data **[OK]**
* participate struct entries in math calculation **[OK]**
* participate struct entries in str calculation **[OK]**
* participate struct entries in bool calculation **[OK]**
* support struct entries as a struct **[OK]**
* alloc struct entries **[OK]**
* change virtual memory as hash for optimizing speed **[OK]**
* reform memory functions **[OK]**
* start offline documentation of mpl **[OK]**
* complete structure of mpl-docs **[OK]**
* start OS built-in functions **[OK]**
* implement error_handle function **[OK]**
* implement shell,argvs functions **[OK]**
* start data built-in functions **[OK]**
* implement all bit-based functions **[OK]**
* comparison huge numbers **[OK]**
* convert '' to "" as a string **[OK]**
* development $def,$ses to accept all of data **[OK]**
* accept array values for magic macros **[OK]**
* accept struct values for magic macros **[OK]**
* add -i argument as info to mpl **[OK]**
* set $con as standard **[OK]**
* set ErrorsState, WarningsState, MaxHugeDivideSteps, MaxHugeDecimalNumbers, TabSize in $con **[OK]**
* set SessionMode,HelpArgumentMode,OverwriteBuiltinMode,DebugMode in $con **[OK]**
* set ExportByteCode,ExportLogFile,SessionDatabasePath in $con **[OK]**
* implement MPLV1 encoding algorithm **[OK]**
* set new $ses **[OK]**
* store session entries in database **[OK]**
* support array of utf8 strings **[OK]**
* support utf8 strings for magic macros **[OK]**
* store utf8 entries in database **[OK]**
* load and decoding session database **[OK]**
* implement MPLV1 decoding algorithm **[OK]**
* compatible with control chars like \n,\t **[OK]**
* load session database in program **[OK]**
* decode values and import entries to $ses magic macro **[OK]**
* set logo for mpl interpreter execute **[OK]**
* implement sqlite interface **[OK]**
* get sqlite lib version **[OK]**
* send utf8 strings to functions **[OK]**
* implement open,close functions of sqlite **[OK]**
* implement query function of sqlite **[OK]**
* support sqlite as a module **[OK]**
* set ExportBuildFile,AppIcon,BuildMode in $con **[OK]**
* start implement BuildMode config **[OK]**
* check exist compiler,linker,builder.dll **[OK]**
* translate utst struct in builder **[OK]**
* translate datas struct in builder **[OK]**
* translate blst funcs struct in builder **[OK]**
* translate blst stru struct in builder **[OK]**
* translate instru struct in builder **[OK]**
* remove external include files for builder **[OK]**
* complete main codes of program in builder **[OK]**
* using modules in output executable program **[OK]**
* compile and link with builder.dll and export execute file **[OK]**
* implement rand,argvs built-in functions **[OK]**
* implement console color **[OK]**
* building MPL website **[OK]**
* implement printf built-in function **[OK]**
* implement at,into built-in functions **[OK]**
* implement ConAll,DefAll,DefIsset built-in functions **[OK]**
* implement len built-in function **[OK]**
* implement SesAll,SesIsset built-in functions **[OK]**
* implement tnum,tbool,tstr built-in functions **[OK]**
* implement inum,ibool built-in functions **[OK]**
* implement tarray built-in function **[OK]**
* implement log file for program **[OK]**
* implement exec built-in function **[OK]**
* implement crop built-in function **[OK]**
* implement pop built-in function **[..]**
* implement del built-in function **[OK]**
* implement AbsPath,mkdir built-in functions  **[OK]**
* implement dbslah built-in function **[OK]**
* implement fopen,fclose built-in functions  **[OK]**
* implement fwrite built-in function **[OK]**
* implement version command of mprog **[OK]**
* implement init command of mprog **[..]**
* implement date built-in function **[OK]**
* implement test command of mprog 
* implement start command of mprog 
* implement version,locate command of mprog 
* implement root,help command of mprog
* implement list,remove command of mprog 
* implement search command of mprog 
* implement recent command of mprog 
* implement DefUnset,SesUnset built-in functions 
* implement OverwriteBuiltinMode for program
* start review_array_loop **[OK]**
* alloc review array every time


* **_complete full structure of mpl_** **[..]**

* complete $con entries usage **[..]**
* complete mdebug commands
* complete import error_handling
* complete mpl built-in functions **[..]**
* complete built-in constants **[OK]**
* complete data built-in functions **[OK]**
* complete parse error_handling
* complete os built-in functions **[OK]**
* complete mean_run error_handling
* complete built-in error_handling
* complete support utf8
* complete offline docs
* complete 'mprog' program
* complete sample codes
* remove unusable functions from main source
* implement 'Vcache'(10000),'Pcache'(50000) hashing for optimizing speed
* calculate order of all core functions
* calculate order of all tools functions
* calculate order of all built-in functions
* complete all functions describe
* optimizing huge numbers
* optimizing RAM,CPU usage
* find (30/100) bug and debug it!

MPL BUG be reported
--------
* [B1] vars_allocation function messy [OK]
* [B2] check_post_short_alloc bug [OK]
* [B3] not read last line in read_lines_from_file function [OK]
* [B4] bug for complex structs in return_first_array_item [OK]
* [B4] bug for complex structs in simplification_struct_value [OK]
* [B4] bug for complex structs in simplification_array_value [OK]
* [B5] bug in huge_num functions(like:3h/2) [..]
* [B6] bug for identify between ascii and utf8 strings [OK]
* [B6] bug for support utf8 in calculate_string_expression function [OK]
* [B6] bug for support utf8 in determine_value_type function [OK]
* [B7] bug in call_built_in_funcs function [OK]
* [B8] bug in function_return function [OK]
* [b9] bug for null value for [?,?](like num j[?,?]=null) in set_memory_var function [OK]
* [B10] bug for str mm[1,1,1]={{{"SSSSSSS"}}} in set_memory_var function [..]
* [B11] bug in __syscall_read_file function to reading files [OK]
* [B12] bug in alloc_magic_macros function [OK]
* [B13] bug in init_global_vars function [OK]
* [B14] bug for handle utf8 strings in convert_built_in_module_vars_to_values function [OK]
* [B15] bug for var dimensions in determine_type_name_func_parameters function [OK]
* [B16] bug for complex function calls in function_call function [OK]
* [B17} bug in flush_session_entries function when exit from program [OK]
* [B18] bug in return_file_name_extension_path function [OK]
* [B19] bug in is_exact_function function for vars data type [OK]
* [B20] bug in calculate_math_expression function for unknown one letter var [OK]
* [B21] bug in printf function for string and num values [OK]
* [B22] bug for "print(tstr(jj[0])" error in parser [..]
* [B23] bug in call_built_in_funcs function to determine value as 'a' [OK]
* [B24] bug in is_exact_function function for matching array dimensions [OK]
* [B25] bug in set_function_parameters for assign array var index value to var [OK]
* [B26] bug in printing path with '\' [OK]
* [B27] bug in making a directory by utf8 name [..] 

### ((:publish MPL-RC version of MPL (for windows):))

* publish RC version for linux
* implement DnsMapping function of mhl function
* support for embedded files
* support for package libraries
* set PackageMode,AccessVariablesMode,NameSpace in $con
* implement ExportByteCode for source code
* implement NameSpace,AccessVariablesMode for packages
* implement SafeMode config
* using garbage collector for remove unusable utf8 strings
* implement OptimizeMode config
* implement switch keyword
* implement callback keyword
* add math module for advanced calculation
* add net module
* start mgl module
* comparison struct expressions
* support minimal gui for windows
* add regex module
* development 'vars' type usage for struct and array
* support mpl repository
* support mpl forum

### ((:publish MPL-HELLO version of MPL (for windows,linux):))

* support for multi threading
* support minimal gui for gtk
* serve web clients
* support as a web service
* support as a web scripting language under Apache
* To be continued ...

### ((:publish MPL-HI version of MPL (for windows,linux):))

### To be continued ...

MPL Sample Codes
--------

* files lines counter
* implement a linked-list
* animation console snack
* advanced console calculator
* simple console editor

MPL Modules
--------

* math module
* sqlite module **[OK]**
* mgl module
* net module
* fs module **[..]**
* os module
* strs module
* mhl module

MPL Language Features
--------

> support utf8 strings **[OK]** <br>
> support radix numbers **[OK]** <br>
> has boolean XOR operand **[OK]** <br>
> support data structures **[OK]** <br>
> support magic macros **[OK]** <br>
> support huge numbers **[OK]**<br>
> support override functions **[OK]** <br>
> has built-in functions **[OK]** <br>
> manage exceptions **[OK]** <br>
> support multi dimensions arrays **[OK]** <br>
> has built-in garbage collector(gc) **[OK]** <br>
> expandability by c libraries **[OK]** <br>
> support built-in debugger **[OK]** <br>
> support unlimited function parameters **[OK]** <br>
> support multi return values **[OK]** <br>
> support call by reference vars **[OK]** <br>
> support short allocations for vars **[OK]** <br>
> support built-in encoder **[OK]** <br>
> has bit functions **[OK]** <br>
> support session storage **[OK]** <br>
> support sqlite3 module **[OK]** <br>
> support package libraries **[..]** <br>
> support browse arrays in loop **[..]** <br>
> support switch condition **[..]** <br>
> support module os syscalls **[..]** <br>
> support create executable file from program **[..]** <br>

MPL Hello World Program
--------

    func main(){
        print("Hello World!\n")
    }

Get Starterd
--------

### [Windows] : Build Mpl.exe

> First clone the repository and then install python3 for ruuning build tool and mingw64 for compiling mpl. Then run 'build_win32_migw64.py' in 'tools' directory. Finally created a folder by name 'win32-release'. <br>
> Now  you can run  a file by name 'main.mpl' through command 'mpl.exe main.mpl' and you can see the result! <br>
> [Mpl is portable!] you can move 'mpl.exe' to any where. it's a static executable file and not need to any files.(It's not need any modules or packages or docs or anything... Their are just for easier programming by **mpl:))** ) <br>

### [Linux] : Build Mpl

> TODO...

MPL Sample Program
--------
     //=>import an external file
    import "file:$/data/sam.mpl"
    //=>edit $con values
    $con["AppName"]="MyProgram"
    //=>edit $ses values. it is store in a built-in database
    $ses["AppName"]="MyProgram"
    //------------
    //=>define main function,start point of any program
    func main(){
        print("Hello World!\n");
        //=>print an utf8 string
        print("سلام بر دنیا!\n"); 
        str uu[2,2]={{"x","v"},{"h","i"}}
        //=> browse an array
        loop(str u,b,num i=0;i<len(uu);i++){
			u=uu[i][0]
			b=uu[i][1]
            //=>print a calculated string
            print("u is: %u%,and b is: %b%\n");
        }
        //=>define some vars by one value
        num sum1,sum2=0;
        //=>call a function by reference var
        add(&sum1,5,7.7,-2,0.7) //=>sum1=3
        sum2=add(sum2,5,7.7,-2,0.7) //=>sum2=3
        struct st1{bool b,str s,num n}
        struct st2{st1 v,bool b}
        //=>create a var by a customized data type
        st1 s1=struct(true,"ty",78.7)
        //=>using multiple struct into other
        st2 s2[2]={struct(struct(false,"ui",-9.99h),true),null}
    }
    //------------
    //=>create a documentation for a function
    /**
    @params:
        ref(num) : can set sum of value by reference var
        nums(vars) : get unlimited numbers
    @output:
        return sum of integers
    @des:
        add up just integer numbers and return it
    @ver:
        1.0.0
    */
    //=>define a function by unlimited data type
    func add(num ref,vars nums){
        loop(num i=0;i<len(nums);i++){
            //=>check type of var by 'typeof' function
            if(typeof(nums[i])=="i") ref+=nums[i]
        }
        return ref
    }

Programmers & Designers
--------

1. Mohammad Amin Delavar Khalafi [main developer,designer],[Iran],[amindelavar@hotmail.com]

MPL VERSIONS
--------
* MPL-BETA(0)	:	0.0.0 - 0.9.9
* MPL-RC(1)		:	1.0.0 - 1.9.9
* MPL-HELLO(2)	:	2.0.0 - 2.9.9
* MPL-HI(3)		:	3.0.0 - 3.9.9
* To Be Continue...

Events
--------

* 0.1.0-0.9.0 : Publish 'BETA' version of MPL for windows (2018-2019)  **[DONE]**
* 0.3.0-1.0.0 : Complete 'www.mpl-lang.ir' website and MPL documentation (2018-2019)
* 0.9.0-1.0.0 : 10 daily rolling publish and complete standard modules (2018-2019)
* 1.0.0-2.0.0 : Publish 'MPL-RC' version of MPL (2019-2020)
* 2.0.0-3.0.0 : Publish 'MPL-HELLO' version of MPL (2019-2021)
* 2.2.0-4.0.0 : Complete GUI programming on windows and linux (2019-2022)
* 2.5.0-4.5.0 : Enter mpl to servers and web applications (2019-2023)
* 3.0.0-4.0.0 : Publish 'MPL-HI' version of MPL (2020-2022)

MPL is best choice FOR
--------

1. build tools of personal projects or big projects 	**[DONE]**
2. start to learning the programming world				**[DONE]**
3. create small and common tools for programmers		**[....]**
4. personal programming for spend time-end time			**[DONE]**
5. create and test new algorithms						**[DONE]**
6. network testing and hacking							**[....]**
7. familiarity and learning SQL							**[DONE]**

MAHDI appreciates for your suggestions and the other :))
--------

> Contact with amindelavar[at]hotmail.com by **__mAh_:__** title


<hr>
<center>
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-<br>
|   MAHDI Programming Language  |<br>
-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-<br>
</center>