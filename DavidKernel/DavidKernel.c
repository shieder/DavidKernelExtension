//
//  DavidKernel.c
//  DavidKernel
//
//  Created by shieder on 12. 1. 18..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include <mach/mach_types.h>
#include <sys/systm.h>

kern_return_t DavidKernel_start(kmod_info_t * ki, void *d);
kern_return_t DavidKernel_stop(kmod_info_t *ki, void *d);

kern_return_t DavidKernel_start(kmod_info_t * ki, void *d)
{
    printf("I'm David Kernel\n");
    printf("Why Not!!!\n");
    return KERN_SUCCESS;
}

kern_return_t DavidKernel_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
