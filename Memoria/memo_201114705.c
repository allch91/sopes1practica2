#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/quicklist.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <asm/page.h>
#include <asm/pgtable.h>

#define DRIVER_AUTHOR "Abner Lopez 2011 14705"
#define DRIVER_DESC "Memoria ram libre, ocupada, etc."
#define PROCFS_NAME "memo_201114705"

struct sysinfo i;
unsigned long committed;
unsigned long allowed;

unsigned long mt, mf, mu;

long cached;
unsigned long pages[NR_LRU_LISTS];
int lru;

static int OS2_show(struct seq_file *m, void *v){
seq_printf(m, "Carne: 201114705\n");
seq_printf(m, "Nombre: Abner Lopez\n");
seq_printf(m, "SO: Linux Mint\n");

#define K(x) ((x) << (PAGE_SHIFT - 10))
si_meminfo(&i);
for (lru = LRU_BASE; lru < NR_LRU_LISTS; lru++)
pages[lru] = global_numa_state(NR_LRU_BASE + lru);
mt = K(i.totalram);
mf = K(i.freeram);
mu = 100*(mt - mf)/mt;
seq_printf(m,"Memoria Total: %8lu Mb\n",K(i.totalram)/1024);
seq_printf(m,"Memoria Libre: %8lu Mb\n",K(i.freeram)/1024);
seq_printf(m,"Porcentaje Utilizado: %8lu \n",mu);

#undef K
return 0;
}

static int OS2_open(struct inode *inode, struct file *file){
return single_open(file, OS2_show, NULL);
}

static const struct file_operations OS2_fops = {
.owner = THIS_MODULE,
.open = OS2_open,
.read = seq_read,
.llseek = seq_lseek,
.release = single_release,
};

static int __init OS2_init(void){
printk(KERN_INFO "Cargando modulo.\r\n");
printk(KERN_INFO "Carne: 2011-14705\r\n");
proc_create(PROCFS_NAME, 0, NULL, &OS2_fops);
printk(KERN_INFO "Completado. Procceso: /proc/%s.\r\n", PROCFS_NAME);
return 0;
}

static void __exit OS2_exit(void){
remove_proc_entry(PROCFS_NAME, NULL);
printk(KERN_INFO "Modulo deshabilitado.\r\n");
printk(KERN_INFO "Sistemas Operativos 1.\r\n");
}

module_init(OS2_init);
module_exit(OS2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
