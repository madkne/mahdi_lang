#include <Mahdi/system.h>

//***********************************
void BUILT_init()
{
    _bifs_append(0, 0, 0, 0,0);
    /**
   * a=true|'sample'|45.4|var[0]|{'a':0,'b':1}|[4,7,5] : value
   * aa=bool[?,..]|str[?,..]|num[?,..]|pack[?,..] : var
   */
    //   len(var),typeof(var),exec(str),errhand(num,str,str),del(var),nil(var),str(var),num(var),inum(str)
    //----------------------------------system functions(sys_builtin)
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_LEN, "len", "aa", "num"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_TYPEOF, "typeof", "aa|a", "str;str"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_EXEC, "exec", "str", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_ERRHAND, "errhand", "num;str;str","bool"); //[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_DEL, "del", "aa", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_NIL, "nil", "aa", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_STR, "str", "aa|a", "str"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_NUM, "num", "aa|a", "num"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_INUM, "inum", "str", "bool"); //=>[..]
    //----------------------------------list functions(list_builtin)
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_PUSH, "push", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_POP, "pop", "", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_TOP, "top", "", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_POPFIRST, "popfirst", "", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_TOPFIRST, "topfirst", "", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_PUSHFIRST, "pushfirst", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_FILL, "fill", "num;aa|a", "bool"); //=>[..]
    //----------------------------------map functions(map_builtin)
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_GET, "get", "str", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_SET, "set", "str;aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_KEYS, "keys", "", "aa"); //=>[..]
}

//*************************************************************
//****************built_in_funcs functions*********************
//*************************************************************
void _bifs_append(uint32 parent,uint32 id, String func_name, String params, String returns)
{
    //=>init vars
    bifs *q;
    uint8 par_len = 0, ret_len = 0;
    //=>get malloc
    q = (bifs *)malloc(sizeof(bifs));
    if (q == 0)
        return;
    q->id = id;
    q->parent_type=parent;
    //=>get params and returns count
    par_len = CH_search_count(params, ';',STR_length(params));
    ret_len = CH_search_count(returns, ';',STR_length(returns));
    q->params_len = par_len;
    q->returns_len = ret_len;
    STR_init(&q->func_name, func_name);
    STR_init(&q->params, params);
    STR_init(&q->returns, returns);
    q->next = 0;
    //entry_table.bifs_len++;
    if (entry_table.bifs_start == 0)
    {
        entry_table.bifs_start = entry_table.bifs_end = q;
    }
    else
    {
        entry_table.bifs_end->next = q;
        entry_table.bifs_end = q;
    }
}
