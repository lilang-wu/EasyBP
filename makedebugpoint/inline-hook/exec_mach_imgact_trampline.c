//
//  exec_mach_imgact_trampline.c
//  makedebugpoint
//
//  Created by lilang_wu on 2023/7/17.
//  Copyright © 2023 zuff. All rights reserved.
//

#include "exec_mach_imgact_trampline.h"

#include "inline_hook.h"


extern inline_hook_entry_t g_inline_hook_entry[];
static uint64_t s_exec_mach_imgact_JmpBackAddr = -1;


kern_return_t trampline_exec_mach_imgact (struct image_params *imgp)
{
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    //__asm__ volatile ("int3");
    
    kern_return_t  kr =0;
    s_exec_mach_imgact_JmpBackAddr = g_inline_hook_entry[INLINE_ENUM_EXEC_MACH_IMGACT].ori_func_addr + TRAMPOLINE_SIZE;
    
    printf("---abc--- trampline_exec_mach_imgact called");
    
    kr = ((fn_exec_mach_imgact )inlined_part_exec_mach_imgact)(imgp);
    
    return kr;
}


__attribute__ ((naked)) void inlined_part_exec_mach_imgact()
{
    __asm__ volatile (
                      "  push %rbp\n"
                      "  mov %rsp, %rbp\n"
                      "  push %r15\n"
                      "  push %r14\n"
                      "  push %r13\n"
                      "  push %r12\n"
                      //"  push %rbx\n"
                      //"  sub $0x108, %rsp"
                      );
    __asm__ volatile (
                      "  jmp *%0\n"
                      //"  mov %%rax, %0"
                      :
                      :"m" (s_exec_mach_imgact_JmpBackAddr)
                      //:"%rax"
                      );
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");//10
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");
    __asm__ volatile ("int3");//20
}


