#include <MAHDI/system.h>
//**************************************************************
/**
 * interate all packages and collect packages that have valid inherit package, and then inherit them!
 * @author madkne
 * @version 1.0
 * @since 2019.12.24
 * @return Boolean : if success to inheritance all packages or failed
 */
Boolean INHERIT_run(){
    //=>init vars

    //=>coolect all packages that iterate the other package
    blst *tmp1 = entry_table.blst_pack_start;
    if (tmp1 == 0) return false;
    for (;;) {
        //=>check for inherit pack
        if(tmp1->inherit!=0){
            //=>search for inherit package
            datas s=_datas_search(tmp1->inherit,0,true);
            //=>if not found inherit package
            if(s.name==0){
                EXP_print_error(tmp1->line,"not_found_inherit",entry_table.sources_list[tmp1->source_id],tmp1->inherit,0,"INHERIT_run");
                tmp1 = tmp1->next;
                if (tmp1 == 0) break;
                continue;
            }
            inpk inpk1={tmp1->id,tmp1->label,s.pack_id,s.name};
            _inpk_append(inpk1);
        }

        tmp1 = tmp1->next;
        if (tmp1 == 0) break;
    }
    //=>if programmer debug is bigger than 1
    if(is_programmer_debug>=2){
        COM_print_struct(PRINT_INHERIT_ST);
    }
    //=>iterate inpk list, to inherit packages
    inpk *tmp2 = entry_table.inpk_start;
    if (tmp2 == 0) return false;
    for (;;) {
        //=>check for inherit pack
        if(INHERIT_apply(*tmp2)){
            printf("DONE\n");
        }

        tmp2 = tmp2->next;
        if (tmp2 == 0) break;
    }
}
//**************************************************************
Boolean INHERIT_apply(inpk s){
    //=>
}
