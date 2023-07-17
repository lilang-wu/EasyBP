//
//  inline_hook.h
//  makedebugpoint
//
//  Created by lilang_wu on 2019/3/1.
//  Copyright © 2019 lilang_wu. All rights reserved.
//

#ifndef inline_hook_h
#define inline_hook_h


#include <mach/kern_return.h>
#include <sys/types.h>
#include <mach/mach_vm.h>
#include <libkern/libkern.h>

#include "configuration.h"


typedef enum
{
    INLINE_ENUM_IS_IO_CONNECT_METHOD = 0,
    INLINE_ENUM_EXEC_MACH_IMGACT = 1,
    INLINE_ENUM_MAX =2,
} enum_inline_point_t;



typedef struct
{
    char * symbol;
    mach_vm_address_t ori_func_addr;
    mach_vm_address_t trampline_func_addr;
    mach_vm_address_t inlined_func_header_addr;
    char ori_func_bytes[TRAMPOLINE_SIZE+1];
    boolean_t bSet;
    boolean_t bFuzzing;
} _inline_hook_entry, inline_hook_entry_t;


kern_return_t init_inline_hook(void);
kern_return_t install_inline_hook(void);
kern_return_t uninstall_inline_hook(void);
kern_return_t uninit_inline_hook(void);

#endif /* inline_hook_h */
