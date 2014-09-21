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

int delen, detemp;
char* demsg;

int read_proc_decode(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
  if(count>detemp)
    {
      count=detemp;
    }
  detemp=detemp-count;
  copy_to_user(buf,demsg, count);
  if(count==0)
    detemp=delen;
   
  return count;
}

int write_proc_decode(struct file *filp,const char *buf,size_t count,loff_t *offp)
{
  copy_from_user(demsg,buf,count);
  delen=count;
  detemp=delen;
  return count;
}

struct file_operations proc_fops_decode = {
 read: read_proc_decode,
 write: write_proc_decode
};

void create_new_proc_entry_decode() 
{
  proc_create("decodeBuffer",0,NULL,&proc_fops_decode);
  demsg=kmalloc(GFP_KERNEL,10*sizeof(char));
}



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
  charToBin['f'] = 0b11101;
  charToBin['g'] = 0b1001;
  charToBin['h'] = 0b11111;
  charToBin['i'] = 0b111;
  charToBin['j'] = 0b11000;
  charToBin['k'] = 0b1010;
  charToBin['l'] = 0b11011;
  charToBin['m'] = 0b100;
  charToBin['n'] = 0b101;
  charToBin['o'] = 0b1000;
  charToBin['p'] = 0b11001;
  charToBin['q'] = 0b10010;
  charToBin['r'] = 0b1101;
  charToBin['s'] = 0b1111;
  charToBin['t'] = 0b10;
  charToBin['u'] = 0b1110;
  charToBin['v'] = 0b11110;
  charToBin['w'] = 0b1100;
  charToBin['x'] = 0b10110;
  charToBin['y'] = 0b10100;
  charToBin['z'] = 0b10011;
  charToBin['1'] = 0b110000;
  charToBin['2'] = 0b111000;
  charToBin['3'] = 0b111100;
  charToBin['4'] = 0b111110;
  charToBin['5'] = 0b111111;
  charToBin['6'] = 0b101111;
  charToBin['7'] = 0b100111;
  charToBin['8'] = 0b100011;
  charToBin['9'] = 0b100001;
  charToBin['0'] = 0b100000;
  charToBin['.'] = 0b1101010;
  charToBin['?'] = 0b1110011;
  charToBin['!'] = 0b1010100;
  charToBin['('] = 0b101001;
  charToBin[')'] = 0b1010011;
  charToBin[':'] = 0b1000111;
  charToBin['='] = 0b101110;
  charToBin['-'] = 0b1011110;
  charToBin['"'] = 0b1101101;
  charToBin[','] = 0b1001100;
  charToBin['\'']= 0b1100001;
  charToBin['/'] = 0b101101;
  charToBin[';'] = 0b1010101;
  charToBin['_'] = 0b1110010;
  charToBin['@'] = 0b1100101;
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
  vfree(msg);
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
  create_new_proc_entry_decode();
  return 0;
}

void proc_cleanup(void) {
  remove_proc_entry("decodeBuffer",NULL);
  remove_proc_entry("encodeBuffer",NULL);
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
