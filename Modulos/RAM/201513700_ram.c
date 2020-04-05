/**
 * MODULO PARA LA MEMORIA RAM
 */
#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // KERN_INFO
#include <linux/init.h>     // Needed for the macros
#include <linux/fs.h>       //file operations struct
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/hugetlb.h>


#define PROCESZ "201513700_ram" //name of file
#define BUFSIZE 100 // buffer for write file

/**
 * definitions about modules
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo informacion de la memoria.");
MODULE_AUTHOR("201513700_CReal");

/**
 * returns certain statistics on memory and swap usage, as 
 * well as the load average.
 */
struct sysinfo i;

static int wdata_proc(struct seq_file *m, void *v){
    #define S(x) ((x) << (PAGE_SHIFT - 10))
    si_meminfo(&i);
       
    long total = (S(i.totalram))/1000;
    long libre = (S(i.freeram))/1000;
    long usado = total-libre;

    seq_printf(m, "Total de memoria MB:%8lu\n", total);
    seq_printf(m, "Memoria cosumida MB:%8lu\n", usado);
    seq_printf(m, "Memoria disponible MB:%8lu\n", libre);
    seq_printf(m, "Porcentaje de consumo:%8lu\n", usado*100/total);
    return 0;
}

static int openfile_proc(struct inode *inode, struct file *file){
    return single_open(file, wdata_proc, NULL);
}

static struct file_operations myops = {
    .owner = THIS_MODULE,
    .open = openfile_proc,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};


static int init_allproc(void){
    proc_create(PROCESZ, 0, NULL, &myops);
    printk(KERN_INFO "[init]ram\n");
    return 0;
}

static void end_allproc(void){
    remove_proc_entry(PROCESZ, NULL);
    printk(KERN_INFO "[end]ram\n");
}
module_init(init_allproc);
module_exit(end_allproc);
