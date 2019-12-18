#include <Mahdi/system.h>
//******************************************************
/**
 * get a name of var,func,pack and detect if is valid or not!
 * @author madkne
 * @version 1.0
 * @since 2019.12.18
 * @param name : name of var can bundled with index like var,var[0]
 * @param is_array : split name and index or not!
 * @return Boolean : if valid or not valid name
 */ 
Boolean RUNKIT_is_valid_name(String name, Boolean is_array){
    //=>init vars
    String name_v = 0, index = 0, buf = 0, extra = 0;
    Boolean is_string = false, is_end = false;
    int32 bras = 0;
    //=>get length of name    
    uint32 name_len = STR_length(name);
    //=>if name is null, not valid!
    if (name_len < 1) {
        return false;
    }
    //=>check name is mahdi keyword, not valid
    if (STR_search(keywords, name, StrArraySize(keywords))) {
        return false;
    }
    
    
    //=>if must check array and splite name and index
    if (is_array) {
        //=>split index , name like n[2]=>n,2
        for (uint32 i = 0; i < name_len; i++) {
            //=>check is string
            if (name[i] == '\"' && (i == 0 || name[i - 1] != '\\')) {
                is_string = BOOL_switch(is_string);
            }
            //=>count pars
            if (!is_string) {
                if (name[i] == '[') bras++;
                else if (name[i] == ']') bras--;
            }
            //=>set name of var like ar[2]=>name_v='ar'
            if (!is_string && name[i] == '[' && name_v == 0 && bras == 1) {
                name_v = STR_trim_space(buf);
                buf = 0;
                continue;
            }
            //=>set index of var
            if (!is_string && index == 0 && name[i] == ']' && name_v != 0 && bras == 0){
                index = STR_trim_space(buf);
                buf = 0;
                is_end = true;
                continue;
                //break
            }
            //=>append to buf
            buf = CH_append(buf, name[i]);
        }
        //=>set extra like k1[0]+3=>k1,0,+3
        if (!is_string && is_end && buf != 0) {
            STR_init(&extra, buf);
        }
        //=>if no detect name, then set buffer as name
        if (name_v == 0) {
            STR_init(&name_v, buf);
        }

    } 
    //=>if not array
    else {
        STR_init(&name_v, name);
    }
    //  printf("&%%%:%s;%s;%s;%s\n", name, name_v, index, extra);
    //=>check for extra, if not null, then not valid!
    if (extra != 0) {
        return false;
    }
    //=>check for name_v, if has pattern like _var (just in parser level) or 133var, not valid
    if (STR_length(name_v) > 0 &&
        ((name_v[0] >= '0' && name_v[0] <= '9') || (name_v[0] == '_' && STR_equal(interpreter_level, "parse")))) {
        return false;
    }
    //=>iterate name var chars
    for (uint32 i = 0; i < STR_length(name_v); i++) {
        //=>if chars of name is valid, ignore it like _var,var,@var,var34
        if ((name_v[i] >= '0' && name_v[i] <= '9') || (name_v[i] >= 'A' && name_v[i] <= 'Z') ||
            (name_v[i] >= 'a' && name_v[i] <= 'z') || name_v[i] == '_' || (name_v[i] == '@' && i == 0)) {
            continue;
        } else {
            return false;
        }
    }
    //=>check for index, if is_array
    if (is_array && index == 0) {
    //return false
    }
    //=>if name is valid, then return True!
    return true;
}