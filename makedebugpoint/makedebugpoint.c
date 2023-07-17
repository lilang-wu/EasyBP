//
//  makedebugpoint.c
//  makedebugpoint
//
//  Created by zuff on 2019/3/1.
//  Copyright © 2019 zuff. All rights reserved.
//

#include <mach/mach_types.h>
#include "globle_data_definitions.h"
#include "inline-hook/inline_hook.h"
#include "solve_kernel_symbols/kernel_info.h"
//#include "inline-hook/execsw_hook.h"
//#include "inline-hook/exec_mach_imgact_trampline.h"



kern_return_t makedebugpoint_start(kmod_info_t * ki, void *d);
kern_return_t makedebugpoint_stop(kmod_info_t *ki, void *d);

// global structures
struct kernel_info g_kernel_info;


kern_return_t makedebugpoint_start(kmod_info_t * ki, void *d)
{
    kern_return_t kr= KERN_SUCCESS;
    printf("---abc--- makedebugpoint_start");
    //__asm__ volatile ("int3");
    kr = init_kernel_info(&g_kernel_info);
    printf("---abc--- init_kernel_info succeed");
    kr |= init_inline_hook();
    printf("---abc--- init_inline_hook succeed");
    kr |= install_inline_hook();
    printf("---abc--- install_inline_hook succeed");
    
    if(kr) return KERN_FAILURE;
    return KERN_SUCCESS;
}

kern_return_t makedebugpoint_stop(kmod_info_t *ki, void *d)
{
    uninstall_inline_hook();
    uninit_inline_hook();
    
    cleanup_kernel_info(&g_kernel_info);
    return KERN_SUCCESS;
}
