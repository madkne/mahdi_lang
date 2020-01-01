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
            datas parent=_datas_search(tmp1->inherit,0,true);
            //=>if not found inherit package
            if(parent.name==0){
                EXP_print_error(tmp1->line,"not_found_inherit",entry_table.sources_list[tmp1->source_id],tmp1->inherit,0,"INHERIT_run");
                tmp1 = tmp1->next;
                if (tmp1 == 0) break;
                continue;
            }
            _inpk_append(parent.pack_id,parent.name,tmp1->id,tmp1->label);
        }

        tmp1 = tmp1->next;
        if (tmp1 == 0) break;
    }
    //=>if programmer debug is bigger than 1
    if(is_programmer_debug>=2){
        COM_print_struct(PRINT_INHERIT_ST);
    }
    //=>iterate inpk list, to get not done inherit packages
    while(true){
        inpk s=_inpk_get_by_done(false);
        //=>if null, then break
        if(s.inherit_name==0){
            break;
        }
        //=>check for inherit pack
        if(INHERIT_apply(s)){
            printf("DONE:%s\n",s.inherit_name);
        }
        //=>if inherit is failed
        else{
            //TODO:fetal
            printf("ERR24560\n");
            return false;
        }
    }
    return true;
}
//**************************************************************
/**
 * get an inpk node and apply inheritace for child by parent package
 * @author madkne
 * @version 1.0
 * @since 2019.12.30
 * @return Boolean : if success to inheritance child or failed
 */
Boolean INHERIT_apply(inpk s){
    printf("apply inherit:%s->%s\n",s.parent_name,s.inherit_name);
    //=>enable set processing of this node
    _inpk_set_processing(s.inherit_id);
    //=>search for parent pack that its inherit is done
    inpk exist=_inpk_search(s.parent_id,0,false);
    //=>if parent inherit not done
    if(exist.inherit_name!=0 && !exist.is_done){
        if(INHERIT_apply(exist)){
            printf("DONE2:%s\n",s.parent_name);
        }
        //=>if inherit is failed
        else{
            //TODO:fetal
            printf("ERR24567\n");
            return false;
        }
    }
    //=>---------------------------------------
    //=>Level1 : inherit package attributes
    //=>collect all attributes of parent and child package
    fpp *parent_attrs=_fpp_filter(PACK_BLOCK_ID,s.parent_id);
    fpp *child_attrs=_fpp_filter(PACK_BLOCK_ID,s.inherit_id);
    //=>if programmer debug is enable show parent and child attributes list
    if(is_programmer_debug>3){
        _fpp_print(s.parent_name,parent_attrs);
        _fpp_print(s.inherit_name,child_attrs);
    }
    //=>Level 1.1 : if child package has attributes (verify attributes)
    if (child_attrs != 0){
        fpp *child=child_attrs;
        //=>iterate child package attributes
        for (;;) {
            //=>compare child attribute with all parent attributes, if exist any parent attributes
            if (parent_attrs != 0){
                fpp *parent=parent_attrs;
                for (;;) {
                    //=>if attribute name is same
                    if(STR_equal(parent->pname,child->pname)){
                        //=>if attribute private or static or type is different
                        if(child->is_private!=parent->is_private || child->is_static!=parent->is_static || !STR_equal(parent->ptype,child->ptype)){
                            //TODO:fetal
                            printf("ERR09090:%s\n",parent->pname);
                            return false;
                        }
                        //=>if attribute value is different
                        if(!STR_equal(parent->pvalue,child->pvalue)){
                            //=>if attribute is const, then error
                            if(STR_is_const(child->pname)){
                                //TODO:fetal
                                printf("ERR090901\n");
                                return false;
                            }
                            //=>if attribute not const, then warning and change child value to parent value
                            else{
                                //TODO:warning
                                if(!_fpp_change_by_id(child->id,"pvalue",parent->pvalue)) return false;
                            }
                        }
                        //=>remove this attribute from parent_attrs
                        Boolean is_success=false;
                        fpp *parent_tmp=_fpp_delete_by_id(parent->id,parent_attrs,&is_success);
                        // printf("remove:%i=>%i\n",parent->id,is_success);
                        if(is_success){
                            parent_attrs=parent_tmp;
                        }
                        break;
                    }
                    parent = parent->next;
                    if (parent == 0) break;
                }
            }
            child = child->next;
            if (child == 0) break;
        }
    }
    //=>Level 1.2 : insert parent attributes that not exist on child package (append attributes)
    if(parent_attrs!=0){
        fpp *parent=parent_attrs;
        for (;;) {
            //=>append to fpp
            fpp tmp = {0,s.inherit_id,PACK_BLOCK_ID,0, parent->pname,parent->ptype,parent->pvalue,parent->is_private,parent->is_static,parent->is_reference,parent->line,parent->source_id };
            _fpp_append(tmp);

            parent = parent->next;
            if (parent == 0) break;
        }
    }
    //=>---------------------------------------
    //=>Level2 : inherit package methods
    //=>collect all methods of parent package
    blst *parent_met=_blst_filter(entry_table.blst_func_start,FUNC_BLOCK_ID,s.parent_id,0);
    //=>if programmer debug is enable show parent methods list
    if(is_programmer_debug>3){
        _blst_print(s.parent_name,parent_met);
    }
    //=>Level 2.1 : iterate child package methods (verify methods)
    blst *child=entry_table.blst_func_start;
    for (;;) {
        //=>get methods that in child package
        if(child->pack_id==s.inherit_id){
            //=>compare child method with all parent methods, if exist any parent methods
            if (parent_met != 0){
                blst *parent=parent_met;
                for (;;) {
                    //=>if method name is same
                    if(STR_equal(parent->label,child->label)){
                        //=>collect all paremeters of same parent and child methods and check if is same or not
                        fpp *parent_params=_fpp_filter(FUNC_BLOCK_ID,parent->id);
                        fpp *child_params=_fpp_filter(FUNC_BLOCK_ID,child->id);
                        //=>check if parent and child method parameters is same
                        if(INHERIT_check_method_parameters(parent_params,child_params)){
                            //=>check if parent method has 'override' attribute
                            Boolean exist=false;
                            for (uint32 i = 0; i < MAX_FUNCTION_ATTRIBUTES; i++){
                                if(parent->func_attrs[i]==OVERRIDE_METHOD_FATTR){
                                    exist=true;
                                    break;
                                }
                            }
                            if(exist){
                                //TODO:fetal
                                printf("ERR112111\n");
                                return false;
                            }
                            //=>remove this method from parent_met
                            Boolean is_success=false;
                            blst *parent_tmp=_blst_delete_by_id(parent->id,parent_met,&is_success);
                            // printf("remove:%i=>%i\n",parent->id,is_success);
                            if(is_success){
                                parent_met=parent_tmp;
                            }
                            break;
                        }   
                    }
                    parent = parent->next;
                    if (parent == 0) break;
                }
            }
        }
        child = child->next;
        if (child == 0) break;
    }
    // _blst_print(s.parent_name,parent_met);
    //=>Level 2.2 : insert parent methods that not exist on child package (append methods)
    if(parent_met!=0){
        blst *parent=parent_met;
        for (;;) {
            //=>append parent method header to blst func
            blst tmp={0, s.inherit_id, 0,0, FUNC_BLOCK_ID, parent->label,0, parent->params, parent->params_len,false,parent->func_attrs, parent->line, parent->source_id, 0};
            _blst_append(tmp);
            //=>append parent method parameters to fpp
            fpp *parent_params=_fpp_filter(FUNC_BLOCK_ID,parent->id);
            if(parent_params!=0){
                for (;;) {
                    fpp tmp = {0,entry_table.func_id,FUNC_BLOCK_ID,parent_params->porder , parent_params->pname,parent_params->ptype,parent_params->pvalue,false,false,parent_params->is_reference,parent->line,parent->source_id, 0 };
                    _fpp_append(tmp);
                    //=>append parent method instructions to instru
                    Longint cur_stru_id=0;
                    instru *st1 = entry_table.instru_start;
                    if (st1 != 0){
                        for (;;) {
                            if(st1->pack_id==s.parent_id && st1->func_id==parent->id){
                                // printf("DDDD:%s=>%s\n",parent->label,st1->code);
                                //=>reset cur_stru_id
                                if(st1->stru_id==0)cur_stru_id=0;
                                //=>if instruction is a structure
                                if(STR_equal_indexof(st1->code,STRUCTURES_LABEL,0)){
                                    //=>get parent structure
                                    Longint stru_id=STR_to_Longint(STR_substring(st1->code,STR_length(STRUCTURES_LABEL)-1,0));
                                    blst stru=_blst_get_by_id(entry_table.blst_stru_start,stru_id);
                                    //=>append parent stru to blst stru
                                    String inst = STR_multi_append(STRUCTURES_LABEL, STR_from_Longint(entry_table.stru_id + 1), 0, 0, 0, 0);
                                    blst tmp1 = {0,s.inherit_id, entry_table.func_id, cur_stru_id, stru.type, inst,0, stru.params, stru.params_len,false,0, stru.line, stru.source_id, 0};
                                    _blst_append(tmp1);
                                    instru tmp = {0, s.inherit_id,entry_table.func_id, cur_stru_id, st1->order, inst, st1->type, st1->line, st1->source_id, 0};
                                    _instru_append(tmp);
                                    cur_stru_id=entry_table.stru_id;
                                }
                                //=>if the other instructions
                                else{
                                    instru tmp = {0, s.inherit_id,entry_table.func_id, cur_stru_id, st1->order, st1->code, st1->type, st1->line, st1->source_id, 0};
                                    _instru_append(tmp);
                                }
                            }
                            st1= st1->next;
                            if (st1 == 0) break;
                        }
                    }
                    parent_params = parent_params->next;
                    if (parent_params == 0) break;
                }
            }
            parent = parent->next;
            if (parent == 0) break;
        }
    }
    
    //=>finally done inherit package
    return _inpk_set_done(s.inherit_id);
}
//**************************************************************
/**
 * get list of child method parameters and list of parent method parameters and compare them
 * @author madkne
 * @version 1.0
 * @since 2020.1.1
 * @param parent : pointer to start of parent method parameters list
 * @param child : pointer to start of child method parameters list
 * @return Boolean : if same all parameters or not
 */
Boolean INHERIT_check_method_parameters(fpp *parent,fpp *child){
    //=>init vars
    uint32 parent_count=0;
    uint32 child_count=0;
    //=>if child not null
    if (child != 0){
        //=>iterate child method parameters
        for (;;) {
            child_count++;
            //=>compare child parameter with all parent parameters, if exist any parent parameters
            Boolean exist=false;
            parent_count=0;
            if (parent != 0){
                fpp *par=parent;
                for (;;) {
                    parent_count++;
                    //=>if parameter name is same
                    if(STR_equal(par->pname,child->pname)){
                        //=>if parameter types is same
                        if(STR_equal(child->ptype,par->ptype)){
                            exist=true;
                            break;
                        }
                    }
                    par = par->next;
                    if (par == 0) break;
                }
            }
            //=>if child parameter not in parent parameters
            if(!exist){
                return false;
            }
            //=>if child paremeters count bigger than parent
            if(child_count>parent_count){
                return false;
            }
            child = child->next;
            if (child == 0) break;
        }
        //=>if child paremeters count not equal with parent paremeters count
        if(child_count!=parent_count){
            return false;
        }
    }
    //=>if child parameters is null and parent not
    else if(child==0 && parent!=0){
        return false;
    }
    return true;
}