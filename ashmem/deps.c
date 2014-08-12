#include <linux/sched.h>
#include <linux/file.h>
#include <linux/fdtable.h>
//#include <linux/atomic.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#include "deps.h"

static int (*do_fallocate_ptr)(struct file *file, int mode, loff_t offset, loff_t len) = DO_FALLOCATE;
static int (*shmem_zero_setup_ptr)(struct vm_area_struct *vma) = SHMEM_ZERO_SETUP;

int do_fallocate(struct file *file, int mode, loff_t offset, loff_t len)
{
	return do_fallocate_ptr(file,mode,offset,len);
}
int shmem_zero_setup(struct vm_area_struct *vma)
{
	return shmem_zero_setup_ptr(vma);

}
