/**
 * MODULO PARA EL ARBOL DE PROCESOS
 */
#include <linux/module.h>   // Needed by all modules
#include <linux/kernel.h>   // KERN_INFO
#include <linux/sched.h>    // for_each_process, pr_info
#include <linux/init.h>     // Needed for the macros
#include <linux/fs.h>       //file operations struct
#include <asm/uaccess.h>    //for put_user or get_user
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/hugetlb.h>
#include <linux/sched/signal.h>

/**
 * Each task_struct data structure describes a process or task in the system.
*/
struct task_struct *task;
struct task_struct *task_child;
/**
 * These macros define and operate on three types of data structures: 
 * lists, tail queues, and circular queues.
 */
struct list_head *list;
/**
 * structure for memory management
 */
struct mm_struct *mm;
unsigned long size;

#define PROCESZ "201513700_arbol" //name of file
#define BUFSIZE 100 // buffer for write file

/**
 * definitions about modules
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo para obtener el arbol de procesos.");
MODULE_AUTHOR("201513700_CReal");


static int wdata_proc(struct seq_file *m, void *v){
    int contadorProcesos = 0;
    for_each_process(task){
        contadorProcesos++;
    }

    seq_printf(m, "<table class=\"table table-hover\">\n");
    seq_printf(m, "<tbody>\n");
    seq_printf(m, "<tr class=\"table-secondary\">\n");
    seq_printf(m, "<td align=\"center\"> PROCESOS");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", contadorProcesos);
    seq_printf(m, "</td>\n");
    seq_printf(m, "</tr></tbody></table>\n");

    seq_printf(m, "<ul id=\"myUL\">\n");
    for_each_process(task){
        int contadorHijos = 0;
        list_for_each(list, &task->children){
            contadorHijos++;
        }
        if (contadorHijos > 0){
            seq_printf(m, "\t<li><span class=\"caret\">[%d] %s</span>\n", task->pid, task->comm);
            seq_printf(m, "\t  <ul class=\"nested\">\n");
            list_for_each(list, &task->children){
                task_child = list_entry(list, struct task_struct, sibling);
                seq_printf(m, "\t<li>[%d] %s</li>\n", task_child->pid, task_child->comm);
            }
            seq_printf(m, "\t  </ul>\n");
            seq_printf(m, "\t</li>\n");
        }else{
            seq_printf(m, "\t<li>[%d] %s</li>\n", task->pid, task->comm);
        }
    }
    seq_printf(m, "</ul>\n");
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
    printk(KERN_INFO "[init]arbol\n");
    return 0;
}

static void end_allproc(void){
    remove_proc_entry(PROCESZ, NULL);
    printk(KERN_INFO "[end]arbol\n");
}
module_init(init_allproc);
module_exit(end_allproc);