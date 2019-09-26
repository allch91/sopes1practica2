#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h> 

#define DRIVER_AUTHOR "Abner Lopez 2011 14705"
#define DRIVER_DESC "Memoria ram libre, ocupada, etc."
#define PROCFS_NAME "cpu_201114705"    

static int meminfo_proc_show(struct seq_file *m, void *v)
{
    //STRUCT PROCESO
    struct task_struct *task;
    
    //ESTADO DEL PROCESO
    char Estado[16];

    //LISTA JERARQUICA HTML
    seq_printf(m,"PID | NOMBRE | ESTADO");
    //ITERAMOS PARA SACAR TODOS LOS PROCESOS PADRE
    for_each_process(task)
    {

        //ESTADO DEL PROCESO
        if(task->state == TASK_RUNNING){
        	strcpy(Estado, "Running");
        }else if(task->state == TASK_STOPPED){
        	strcpy(Estado, "Stopped");
        }else if(task->state == TASK_INTERRUPTIBLE){
        	strcpy(Estado, "Interruptile");
        }else if(task->state == TASK_UNINTERRUPTIBLE){
        	strcpy(Estado, "Uninterruptible");
        }else if(task->state == EXIT_ZOMBIE){
        	strcpy(Estado, "Zombie");		
        }else{
        	strcpy(Estado, "X");
        }
        
        //PROCESOS PADRE           
        seq_printf(m," %d | %s | %s | %d\n",
        task->pid,
        task->comm,
        Estado,
        (int)task->state);
    }

    return 0;
}

static void __exit final(void) //Salida de modulo
{   
    remove_proc_entry(PROCFS_NAME, NULL);
    printk(KERN_INFO "Modulo deshabilitado.\r\n");
    printk(KERN_INFO "Sistemas Operativos 1.\r\n");
}

static int meminfo_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, meminfo_proc_show, NULL);
}

static const struct file_operations meminfo_proc_fops = {
    .open       = meminfo_proc_open,
    .read       = seq_read,
    .llseek     = seq_lseek,
    .release    = single_release,
};

static int __init inicio(void) //Escribe archivo en /proc
{
    printk(KERN_INFO "Cargando modulo.\r\n");
    printk(KERN_INFO "Carne: 2011-14705\r\n");
    proc_create(PROCFS_NAME, 0, NULL, &meminfo_proc_fops);
    printk(KERN_INFO "Completado. Procceso: /proc/%s.\r\n", PROCFS_NAME);
    return 0;
}


module_init(inicio);
module_exit(final);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
