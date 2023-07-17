//
//  execsw_hook.c
//  makedebugpoint
//
//  Created by lilang_wu on 2023/7/14.
//  Copyright © 2023 zuff. All rights reserved.
//
#include <kern/lock_rw.h> //for lck_spin_t

#include "execsw_hook.h"
#include "kernel_info.h"

#include "cpu_protection.h"
#include "configuration.h"

extern struct kernel_info g_kernel_info;
lck_rw_t *exec_lock = NULL;
static mach_vm_address_t _execsw = 0;
static int (*_exec_mach_imgact_orignal)(struct image_params *imgp);



static int activator_trampoline(struct image_params *imgp) {
    kern_return_t kr;
    printf("---activator_trampoline---");
    kr = _exec_mach_imgact_orignal(imgp);
    return kr;
}

/*
 exec_mach_imgact function call style had changed, so this method would not work anymore
 */

kern_return_t execsw_hook_init(void) {
    // prepare kernel symbols
    _execsw = solve_kernel_symbol(&g_kernel_info, "_execsw");
    if (_execsw == 0) {
        return KERN_FAILURE;
    }
    
    printf("_execsw addr = %x", _execsw);
    printf("activator_trampoline = %x", activator_trampoline);
    disable_interrupts();
    disable_wp();
    _exec_mach_imgact_orignal = *(void**)_execsw;
    *(int64_t*)_execsw = (void*)activator_trampoline;
    *(int64_t*)(_execsw + 0x10) = (void*)activator_trampoline;
    enable_wp();
    enable_interrupts();
    
    printf("_execsw addr1 = %x", *(int64_t*)_execsw);
    printf("activator_trampoline1 = %x", _exec_mach_imgact_orignal);
    return KERN_SUCCESS;
}

kern_return_t execsw_hook_uninit(void) {
    disable_interrupts();
    disable_wp();
    *(int64_t*)_execsw = _exec_mach_imgact_orignal;
    *(int64_t*)(_execsw + 0x10) = (void*)_exec_mach_imgact_orignal;
    enable_wp();
    enable_interrupts();
    return KERN_SUCCESS;
}
