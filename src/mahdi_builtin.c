#include <Mahdi/system.h>

//***********************************
void BUILT_init()
{
    _bifs_append(0, 0, 0, 0,0);
    /**
   * a=true|'sample'|45.4|var[0]|{'a':0,'b':1}|[4,7,5] : value
   * aa=bool[?,..]|str[?,..]|num[?,..]|pack[?,..] : var
   */
    //----------------------------------mahdi functions(builtins/mahdi)
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_LEN, "len", "aa", "num"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_TYPEOF, "typeof", "aa|a", "str;str"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_EXEC, "exec", "str", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_ERRHAND, "errhand", "num;str;str","bool"); //[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_DEL, "del", "aa", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_NIL, "nil", "aa", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_STR, "str", "aa|a", "str"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_NUM, "num", "aa|a", "num"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_INUM, "inum", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_IMAP, "imap", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_GLOBAL,_BUILT_ILIST, "ilist", "aa|a", "bool"); //=>[..]
    //----------------------------------list functions(builtins/list)
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_PUSH, "push", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_POP, "pop", 0, "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_TOP, "top", 0, "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_POPFIRST, "popfirst", 0, "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_TOPFIRST, "topfirst", 0, "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_PUSHFIRST, "pushfirst", "aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_FILL, "fill", "num;aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_LIST,_BUILT_FIND, "find", "aa|a", "num"); //=>[..]
    //----------------------------------map functions(builtins/map)
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_GET, "get", "str", "a"); //=>[..]
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_SET, "set", "str;aa|a", "bool"); //=>[..]
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_KEYS, "keys", 0, "aa"); //=>[..]
    _bifs_append(_BUILT_PARENT_MAP,_BUILT_MDEL, "del", "str", "bool"); //=>[..]
}

//*************************************************************
//****************built_in_funcs functions*********************
//*************************************************************
void _bifs_append(uint32 parent,uint32 id, String func_name, String params, String returns){
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
//*************************************************************
bifs _bifs_get_by_name(String fname){
    bifs null = {0, 0, 0,0,0,0,0,0};
    uint32 ii = 0;
    bifs *tmp1=entry_table.bifs_start;
    if(tmp1==0) return null;
    for (;;) {
        if (STR_equal(tmp1->func_name,fname)) {
            return (*tmp1);
        }
        tmp1 = tmp1->next;
        if (tmp1 == 0) break;
    }

    return null;
}
