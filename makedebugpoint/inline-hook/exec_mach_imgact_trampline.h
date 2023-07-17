//
//  exec_mach_imgact_trampline.h
//  makedebugpoint
//
//  Created by lilang_wu on 2023/7/17.
//  Copyright © 2023 zuff. All rights reserved.
//

#ifndef exec_mach_imgact_trampline_h
#define exec_mach_imgact_trampline_h


#include <mach/kern_return.h>
#include <sys/types.h>
#include <mach/mach_vm.h>
#include <libkern/libkern.h>
#include <IOKit/IOTypes.h>
#include <device/device_types.h>
#include <sys/param.h>
#include <sys/malloc.h>
#include <sys/dirent.h>
#include <sys/vnode.h>
#include <string.h>
#include <sys/attr.h>
#include <sys/imgact.h>


__attribute__ ((naked)) void inlined_part_exec_mach_imgact(void);


typedef kern_return_t (*fn_exec_mach_imgact)(struct image_params *imgp);
kern_return_t trampline_exec_mach_imgact (struct image_params *imgp);


#endif /* exec_mach_imgact_trampline_h */
