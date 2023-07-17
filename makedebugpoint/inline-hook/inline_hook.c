//
//  inline_hook.c
//  makedebugpoint
//
//  Created by lilang_wu on 2019/3/1.
//  Copyright © 2019 zuff. All rights reserved.
//

#include "inline_hook.h"

#include "trampline_manager.h"
#include "is_io_connect_method_trampline.h"
#include "exec_mach_imgact_trampline.h"
#include "kernel_info.h"

inline_hook_entry_t g_inline_hook_entry[INLINE_ENUM_MAX] = {0};

extern struct kernel_info g_kernel_info;
static mach_vm_address_t _execsw = 0;

void init_inline_item(enum_inline_point_t index,
                      char * symName,
                      mach_vm_address_t trampFuncAddr,
                      mach_vm_address_t  inlinedFuncAddr)
{
    mach_vm_address_t fnAddr = 0;
    if (index < INLINE_ENUM_MAX && index != INLINE_ENUM_EXEC_MACH_IMGACT)
    {
        fnAddr = solve_kernel_symbol(&g_kernel_info, symName);
        g_inline_hook_entry[index].symbol = symName;
        g_inline_hook_entry[index].ori_func_addr = fnAddr;//original
        g_inline_hook_entry[index].trampline_func_addr = trampFuncAddr;//trampline
        g_inline_hook_entry[index].inlined_func_header_addr = inlinedFuncAddr;//inlined header
    }
    
    if (index < INLINE_ENUM_MAX && index == INLINE_ENUM_EXEC_MACH_IMGACT) {
        _execsw = solve_kernel_symbol(&g_kernel_info, "_execsw");
        if (_execsw == 0) {
            return;
        }
        fnAddr = *(int64_t*)_execsw;
        printf("execsw addr = %x", _execsw);
        printf("execsw func addr = %x", fnAddr);
        g_inline_hook_entry[index].symbol = symName;
        g_inline_hook_entry[index].ori_func_addr = fnAddr;//original
        g_inline_hook_entry[index].trampline_func_addr = trampFuncAddr;//trampline
        g_inline_hook_entry[index].inlined_func_header_addr = inlinedFuncAddr;//inlined header
    }
}


kern_return_t init_inline_hook()
{
    //__asm__ volatile ("int3");
    kern_return_t kr = KERN_SUCCESS;
    memset((char *)g_inline_hook_entry,0, sizeof(g_inline_hook_entry));
    
    
    //Set API_SYMBOL_IS_IO_CONNECT_METHOD
    init_inline_item(INLINE_ENUM_IS_IO_CONNECT_METHOD,
                     API_SYMBOL_IS_IO_CONNECT_METHOD,
                     trampline_is_io_connect_method,
                     inlined_part_is_io_connect_method);
    
    init_inline_item(INLINE_ENUM_EXEC_MACH_IMGACT,
                     API_SYMBOL_EXEC_MACH_IMGACT,
                     trampline_exec_mach_imgact,
                     inlined_part_exec_mach_imgact);
    
    kr = init_mutext_for_fuzz_sample();
    return kr;
}


kern_return_t install_inline_hook()
{
    kern_return_t kr = 0;
    char * symbol = 0 ;
    char origBytes[TRAMPOLINE_SIZE+0x100] = {0};
    mach_vm_address_t tramplineAddr = 0;
    mach_vm_address_t origiAddr = 0;
    mach_vm_address_t inlinedPartAddr = 0;
    inline_hook_entry_t entry = {0};
    for(int i = 0; i< INLINE_ENUM_MAX; i++)
    {
        //kr = find_inline_info(symbol, &entry);
        entry = g_inline_hook_entry[i];
        symbol = entry.symbol;
        tramplineAddr = entry.trampline_func_addr;
        origiAddr = entry.ori_func_addr;
        inlinedPartAddr = entry.inlined_func_header_addr;
        if (symbol)
        {
            kr = install_trampoline_any(origiAddr, tramplineAddr, origBytes);
            if (!kr)
            {
                memcpy(g_inline_hook_entry[i].ori_func_bytes, origBytes, TRAMPOLINE_SIZE);
                g_inline_hook_entry[i].bSet = true;
            }
        }
        
    }
    return kr;
}


kern_return_t uninstall_inline_hook()
{
    kern_return_t kr = 0;
    char * symbol = 0 ;
    //char origBytes[TRAMPOLINE_SIZE+0x100] = {0};
    mach_vm_address_t tramplineAddr = 0;
    mach_vm_address_t origiAddr = 0;
    mach_vm_address_t inlinedPartAddr = 0;
    inline_hook_entry_t entry = {0};
    for(int i = 0; i< INLINE_ENUM_MAX; i++)
    {
        //kr = find_inline_info(symbol, &entry);
        entry = g_inline_hook_entry[i];
        symbol = entry.symbol;
        tramplineAddr = entry.trampline_func_addr;
        origiAddr = entry.ori_func_addr;
        inlinedPartAddr = entry.inlined_func_header_addr;
        char * origBytes = 0;
        if (g_inline_hook_entry[i].bSet && symbol)
        {
            kr = remove_trampoline_any(origiAddr, (origBytes=g_inline_hook_entry[i].ori_func_bytes));
            if (!kr)
            {
                g_inline_hook_entry[i].bSet = false;
            }
        }
    }
    return kr;
    
}

kern_return_t uninit_inline_hook()
{
    un_init_mutext_for_fuzz_sample();
    return 0;
}



