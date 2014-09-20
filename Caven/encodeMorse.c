#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include<linux/sched.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#define CHAR_SIZE 256
#define MORSE_BIN 16

typedef __u16 morse_t;
int len,temp;
char *msg;
morse_t charToBin[CHAR_SIZE];

void initialize(){
  int i=0;
  for(; i<CHAR_SIZE;i++){
    charToBin[i] = 0b11;
  }
  charToBin['a'] = 0b110;
  charToBin['b'] = 0b10111;
  charToBin['c'] = 0b10101;
  charToBin['d'] = 0b1011;
  charToBin['e'] = 0b11;
}

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
  if(count>temp)
    {
      count=temp;
    }
  temp=temp-count;
  copy_to_user(buf,msg, count);
  if(count==0)
    temp=len;
   
  return count;
}

void print(char* head, int len){
  int i=0; 
  for(; i<len; i++){
    printk(KERN_DEBUG "DBG\t%c ", *(head+i));
  }
}

int write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
  // note: cannot use malloc here.
  size_t COPY_SIZE = count*sizeof(char)*20;
  // unsigned long copy_from_user (void * to, const void __user * from, unsigned long n)
  // to is in the kernel space, from is in the user space, n means the number of bytes to copy
  copy_from_user(msg,buf,count);
  //char* newmsg = kmalloc(COPY_SIZE, GFP_KERNEL);
  char* newmsg = vmalloc(COPY_SIZE);
  char* p = newmsg;
  int i=0;
  for(; i<count; i++){
    char ch = *(msg+i);
    // note: I cannot put the rest of the loop in a function. It is wrong.
    int shift = MORSE_BIN-1;
    morse_t bin = charToBin[ch];
    for( ; shift>=0; shift--){
      morse_t mask = 1<<shift;
      if((mask & bin) != 0)
	break;
    }
    shift--;
    for(; shift>=0; shift--){
      morse_t mask = 1<<shift;
      if((mask & bin) != 0){
	*(p++) = 'd';
	*(p++) = 'i';
	*(p++) = 't';
      }else{
	*(p++) = 'd';
	*(p++) = 'a';
	*(p++) = 'h';
      }
      *(p++) = (shift==0?' ':'-');
    }
  }
  *(p++) = '\0';
  msg = newmsg;
  len=COPY_SIZE;
  temp=len;
  return COPY_SIZE;
}

struct file_operations proc_fops = {
 read: read_proc,
 write: write_proc
};
void create_new_proc_entry() 
{
  proc_create("encodeBuffer",0,NULL,&proc_fops);
  msg=kmalloc(GFP_KERNEL,10*sizeof(char));
}


int proc_init (void) {
  initialize();
  create_new_proc_entry();
  return 0;
}

void proc_cleanup(void) {
  remove_proc_entry("encodeBuffer",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
