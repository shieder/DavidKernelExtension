//
//  DavidKernel.c
//  DavidKernel
//
//  Created by shieder on 12. 1. 18..
//  Copyright (c) 2012년 __MyCompanyName__. All rights reserved.
//

#include <mach/mach_types.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/conf.h>
#include <miscfs/devfs/devfs.h>

extern int seltrue(dev_t, int, struct proc*);
static int david_ioctl(dev_t, u_long, caddr_t, int, struct proc *);

#define DEVICENAME "davidkernel"

typedef struct bigstring
{
    char string[1024];
} bigstring;


#define COPYSTRING _IOWR('d', 0, bigstring);
static struct cdevsw david_cdevsw = 
{
    (d_open_t *)&nulldev,           //*d_open
    (d_close_t *)&nulldev,          //*d_close
    (d_read_t *)&nulldev,           //*d_read
    (d_write_t*)&nulldev,           //*d_write
    david_ioctl,                    //*d_ioctl
    (d_stop_t *)&nulldev,           //*d_stop
    (d_reset_t*)&nulldev,           //*d_reset
    0,                              //struct tty **d_ttys
    (select_fcn_t*)seltrue,         //*d_select
    eno_mmap,                       //*d_mmap
    eno_strat,                      //*d_strategy
    eno_getc,                       //*d_getc
    eno_putc,                       //*d_putc
    D_TTY,                          //int d_type    
};


//davidkernel device 로 IOCTL 쿼리시 호출
static int david_ioctl(dev_t dev, u_long cmd, caddr_t data, int flag, struct proc *p)
{
    char string[1024];
    printf("[+] entering david ioctl\n");
    printf("[+] command : 0x%x\n", cmd);
    printf("[+] data address : %p\n", p);
    printf("[+] copy string to kernel...\n");
    
    sprintf(string, "%s\n", data);
    printf("[+] result : %s\n", string);
    
    return 0;
}

void *devnode = NULL;
int devindex = -1;

kern_return_t DavidKernel_start(kmod_info_t * ki, void *d);

kern_return_t DavidKernel_stop(kmod_info_t *ki, void *d);

kern_return_t DavidKernel_start(kmod_info_t * ki, void *d)
{
    //디바이스 전역 엔트리 테이블 등록 
    devindex = cdevsw_add(-1, &david_cdevsw);

    if(devindex == -1)
    {
        printf("[+] cdevsw_add failed\n");
        return KERN_FAILURE;
    }
    
    //캐릭터 디바이스 노드 생성
    devnode = devfs_make_node(makedev(devindex, 0), DEVFS_CHAR, UID_ROOT, GID_WHEEL, 0777, DEVICENAME);
    
    if(devnode == NULL)
    {
        printf("[+] davidkernel devie make_node failed!!\n");
        return KERN_FAILURE;
    }
    
    return KERN_SUCCESS;
}

kern_return_t DavidKernel_stop(kmod_info_t *ki, void *d)
{
    if(devnode != NULL)
    {
        devfs_remove(devnode);
    }
    
    if(devindex != -1)
    {
        cdevsw_remove(devindex, &david_cdevsw);
    }
    
    return KERN_SUCCESS;
}
