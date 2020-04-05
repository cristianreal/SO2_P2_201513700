/**
 * MODULO PARA LOS PROCESOS DEL CPU
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


#define PROCESZ "201513700_cpu" //name of file
#define BUFSIZE 100 // buffer for write file

/**
 * definitions about modules
 */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Modulo informacion de los proceso del cpu.");
MODULE_AUTHOR("201513700_CReal");


void assignuser_proc(struct seq_file *m, int uid){
    switch (uid){
        case 0    :seq_printf(m, "<td> root </td>");break;
        case 1    :seq_printf(m, "<td> daemon </td>");break;
        case 2    :seq_printf(m, "<td> bin </td>");break;
        case 3    :seq_printf(m, "<td> sys </td>");break;
        case 4    :seq_printf(m, "<td> sync </td>");break;
        case 5    :seq_printf(m, "<td> games </td>");break;
        case 6    :seq_printf(m, "<td> man </td>");break;
        case 7    :seq_printf(m, "<td> lp </td>");break;
        case 8    :seq_printf(m, "<td> mail </td>");break;
        case 9    :seq_printf(m, "<td> news </td>");break;
        case 10   :seq_printf(m, "<td> uucp </td>");break;
        case 13   :seq_printf(m, "<td> proxy </td>");break;
        case 33   :seq_printf(m, "<td> www-data </td>");break;
        case 34   :seq_printf(m, "<td> backup </td>");break;
        case 38   :seq_printf(m, "<td> list </td>");break;
        case 39   :seq_printf(m, "<td> irc </td>");break;
        case 41   :seq_printf(m, "<td> gnats </td>");break;
        case 65534:seq_printf(m, "<td> nobody </td>");break;
        case 100  :seq_printf(m, "<td> systemd-network </td>");break;
        case 101  :seq_printf(m, "<td> systemd-resolve </td>");break;
        case 102  :seq_printf(m, "<td> syslog </td>");break;
        case 103  :seq_printf(m, "<td> messagebus </td>");break;
        case 104  :seq_printf(m, "<td> _apt </td>");break;
        case 105  :seq_printf(m, "<td> uuidd </td>");break;
        case 106  :seq_printf(m, "<td> avahi-autoipd </td>");break;
        case 107  :seq_printf(m, "<td> usbmux </td>");break;
        case 108  :seq_printf(m, "<td> dnsmasq </td>");break;
        case 109  :seq_printf(m, "<td> rtkit </td>");break;
        case 110  :seq_printf(m, "<td> speech-dispatcher </td>");break;
        case 111  :seq_printf(m, "<td> whoopsie </td>");break;
        case 112  :seq_printf(m, "<td> kernoops </td>");break;
        case 113  :seq_printf(m, "<td> saned </td>");break;
        case 114  :seq_printf(m, "<td> pulse </td>");break;
        case 115  :seq_printf(m, "<td> avahi </td>");break;
        case 116  :seq_printf(m, "<td> colord </td>");break;
        case 117  :seq_printf(m, "<td> hplip </td>");break;
        case 118  :seq_printf(m, "<td> geoclue </td>");break;
        case 119  :seq_printf(m, "<td> gnome-initial-setup </td>");break;
        case 120  :seq_printf(m, "<td> gdm </td>");break;
        case 1000 :seq_printf(m, "<td> creal </td>");break;
        case 121  :seq_printf(m, "<td> cups-pk-helper </td>");break;
        case 999  :seq_printf(m, "<td> vboxadd </td>");break;
        default   :seq_printf(m, "<td> root </td> "); break;
    }
}

static int wdata_proc(struct seq_file *m, void *v){
    int total_proc = 0;
    int proc_ejecutandose = 0;
    int proc_suspendidos = 0;
    int proc_detenidos = 0;
    int proc_zombies = 0;
    for_each_process(task){
        switch (task->state){
            case 0:proc_ejecutandose++;break;
            case 1:proc_suspendidos++;break;
            case 2:proc_detenidos++;break;
            default:proc_zombies++;break;
        }
        total_proc++;
    }           

    seq_printf(m, "<table class=\"table table-hover\">\n");
    seq_printf(m, "<tbody>\n");
    seq_printf(m, "<tr class=\"table-secondary\">\n");

    seq_printf(m, "<td align=\"center\"> Total de Procesos");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", total_proc);
    seq_printf(m, "</td>\n");

    seq_printf(m, "<td align=\"center\"> En ejecucion");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", proc_ejecutandose);
    seq_printf(m, "</td>\n");

    seq_printf(m, "<td align=\"center\"> Suspendidos");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", proc_suspendidos);
    seq_printf(m, "</td>\n");

    seq_printf(m, "<td align=\"center\"> Detenidos");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", proc_detenidos);
    seq_printf(m, "</td>\n");

    seq_printf(m, "<td align=\"center\"> Zombies");
    seq_printf(m, "<span class=\"badge badge-danger badge-pill\">%d</span>", proc_zombies);
    seq_printf(m, "</td>\n");

    seq_printf(m, "</tr></tbody></table>\n");

    seq_printf(m, "<table class=\"table table-hover\">\n");

    seq_printf(m, "<thead>\n");
    seq_printf(m, "<tr class=\"table-info\">\n");
    seq_printf(m, "<th scope=\"col\">PID</th>\n");
    seq_printf(m, "<th scope=\"col\">Nombre</th>\n");
    seq_printf(m, "<th scope=\"col\">Memoria</th>\n");
    seq_printf(m, "<th scope=\"col\">Estado</th>\n");
    seq_printf(m, "<th scope=\"col\">Usuario</th>\n");
    seq_printf(m, "<th scope=\"col\"> </th>\n");
    seq_printf(m, "</tr>\n");
    seq_printf(m, "</thead>\n");

    seq_printf(m, "<tbody>\n");
    for_each_process(task){
        seq_printf(m, "<tr class=\"table-success\">\n");
        seq_printf(m, "<th scope=\"row\"> %d </th>\n", task->pid);
        seq_printf(m, "<td> %s </td>\n", task->comm);
        if (task->mm){
            down_read(&task->mm->mmap_sem);
            size = (task->mm->mmap->vm_end - task->mm->mmap->vm_start);
            seq_printf(m, "<td> %ld </td>\n", size);
            up_read(&task->mm->mmap_sem);
        }else{
            seq_printf(m, "<td> 0 </td>\n");
        }
        switch (task->state){
            case 0:seq_printf(m, "<td> Ejecucion </td>\n");break;
            case 1:seq_printf(m, "<td> Suspendido </td>\n");break;
            case 2:seq_printf(m, "<td> Detenido </td>\n");break;
            default:seq_printf(m, "<td> Zombie </td>\n");break;
        }
        int uid = __kuid_val(task_uid(task));
        assignuser_proc(m, uid);
        seq_printf(m, "<td><a class=\"btn btn-warning\" onclick=\"KillProcess(%d)\" role=\"button\">Eliminar</a></td>\n", task->pid);
        seq_printf(m, "</tr>\n");
    }
    seq_printf(m, "</tbody>\n");
    seq_printf(m, "</table>\n");
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
    printk(KERN_INFO "[init]cpu\n");
    return 0;
}

static void end_allproc(void){
    remove_proc_entry(PROCESZ, NULL);
    printk(KERN_INFO "[end]cpu\n");
}

module_init(init_allproc);
module_exit(end_allproc);