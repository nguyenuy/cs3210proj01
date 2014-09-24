#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>

#define CHAR_SIZE 256
#define MORSE_BIN 16

typedef __u16 morse_t;

int len,temp;
char *msg;
char * newmsg;
char binToChar[CHAR_SIZE];

void initialize_morse_map() {
    //dot=1, dash=0
    int i = 0;
    for(; i<CHAR_SIZE; i++) {
        binToChar[i] = 0;
    }
    binToChar[0b110]    = 'a';
    binToChar[0b10111]  = 'b';
    binToChar[0b10101]  = 'c';
    binToChar[0b1011]   = 'd';
    binToChar[0b11]     = 'e';
    binToChar[0b11101]  = 'f';
    binToChar[0b1001]   = 'g';
    binToChar[0b11111]  = 'h';
    binToChar[0b111]    = 'i';
    binToChar[0b11000]  = 'j';
    binToChar[0b1010]   = 'k';
    binToChar[0b11011]  = 'l';
    binToChar[0b100]    = 'm';
    binToChar[0b101]    = 'n';
    binToChar[0b1000]   = 'o';
    binToChar[0b11001]  = 'p';
    binToChar[0b10010]  = 'q';
    binToChar[0b1101]   = 'r';
    binToChar[0b1111]   = 's';
    binToChar[0b10]     = 't';
    binToChar[0b1110]   = 'u';
    binToChar[0b11110]  = 'v';
    binToChar[0b1100]   = 'w';
    binToChar[0b10110]  = 'x';
    binToChar[0b10100]  = 'y';
    binToChar[0b10011]  = 'z';
    binToChar[0b110000] = '1';
    binToChar[0b111000] = '2';
    binToChar[0b111100] = '3';
    binToChar[0b111110] = '4';
    binToChar[0b111111] = '5';
    binToChar[0b101111] = '6';
    binToChar[0b100111] = '7';
    binToChar[0b100011] = '8';
    binToChar[0b100001] = '9';
    binToChar[0b100000] = '0';
    binToChar[0b1101010] = '.';
    binToChar[0b1110011] = '?';
    binToChar[0b1010100] = '!';
    binToChar[0b101001]  = '(';
    binToChar[0b1010011] = ')';
    binToChar[0b1000111] = ':';
    binToChar[0b101110]  = '=';
    binToChar[0b1011110] = '-';
    binToChar[0b1101101] = '"';
    binToChar[0b1001100] = ',';
    binToChar[0b1100001] = '\'';
    binToChar[0b101101]  = '/';
    binToChar[0b1010101] = ';';
    binToChar[0b1110010] = '_';
    binToChar[0b1100101] = '@';
    binToChar[0b11111111]= ' ';
}

int read_proc(struct file *filp,char *buf,size_t count,loff_t *offp ) 
{
    if(count>temp) {
        count=temp;
    }
    temp=temp-count;
    copy_to_user(buf,msg, count);
    if(count==0)
        temp=len;
   
    return count;
}

int write_proc(struct file *filp,const char *buf,size_t count,loff_t *offp)
{

    size_t COPY_SIZE = count * sizeof(char);
    copy_from_user(msg,buf,count);
    
    int i=0;
    
    char* p = newmsg;
    morse_t tempMorse = 1;
    
    for(i=0; i < count; i++) {
        char ch = *(msg+i);
        if(ch == 'i') { //dit
            tempMorse = tempMorse << 1;
            tempMorse += 1;
        } else if (ch == 'a') { //dah
            tempMorse = tempMorse << 1;
            //tempMorse += 0
        }
        
        if(ch == ' ' || i == (count-1)) { //reset for new character or EoS
            *(p++) = binToChar[tempMorse]; //write the character to the new string
            tempMorse = 1; //reset tempMorse
        }
        
    }
    *(p++) = '\0';
    
    memcpy(msg, newmsg, COPY_SIZE);
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
    proc_create("morseDecode",0,NULL,&proc_fops);
    msg = vmalloc(PAGE_SIZE);
    if(msg == NULL) {
        return -ENOMEM;
    }
    newmsg = vmalloc(PAGE_SIZE);
    if(newmsg == NULL) {
        return -ENOMEM;
    }
}


int proc_init (void) {
    initialize_morse_map();
    create_new_proc_entry();
    printk(KERN_INFO "Morse code module initialized.");
    return 0;
}

void proc_cleanup(void) {
    remove_proc_entry("morseDecode",NULL);
    vfree(msg);
    vfree(newmsg);
    printk(KERN_INFO "Morse code module cleaned up.");
}

MODULE_LICENSE("GPL"); 
module_init(proc_init);
module_exit(proc_cleanup);
