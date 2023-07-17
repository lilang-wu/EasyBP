//
//  execsw_hook.h
//  makedebugpoint
//
//  Created by lilang_wu on 2023/7/14.
//  Copyright © 2023 zuff. All rights reserved.
//

#ifndef execsw_hook_h
#define execsw_hook_h


#include <mach/kern_return.h>
kern_return_t execsw_hook_init(void);
kern_return_t execsw_hook_uninit(void);

#endif /* execsw_hook_h */
