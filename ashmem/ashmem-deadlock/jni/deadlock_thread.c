#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <pthread.h>

#define ASHMEM_SET_SIZE 1074034435
//#define ASHMEM_SET_SIZE 1074296579 //debian x64
//#include <utils/ashmem.h>
//#include "ashmem.h"

#define PAGE_SIZE 0x1000
#undef NDEBUG
#ifndef NDEBUG
#define ONLY_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define DBG(...) do{fprintf(stderr, "*** %s, %s(), %d ***: ", ONLY_FILE, __FUNCTION__, __LINE__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");}while(0);
#else
#define DBG(...)
#endif

static int n = 0,iters = 0,pages=0;

typedef struct thread_info {
    pthread_t pthreadId;
    int id;
} thread_info;

void* worker(void *arg)
{
    int fd;
    int ret;
    void *addr;
    thread_info* thread = (thread_info*)arg;
    int it = 0;
	int mapsize = pages * PAGE_SIZE;
    while(it++ < iters) {
        fd = open("/dev/ashmem",O_RDWR);

        if( fd < 0) {
            perror("error,fd < 0");
            return NULL;
        }
        //fprintf(stderr,"fd=%d\n",fd);
        ret = ioctl(fd,ASHMEM_SET_SIZE,mapsize);
        if( ret < 0) {
            perror("ioctl error");
            return NULL;
        }
        //addr = mmap(NULL,mapsize,PROT_READ | PROT_WRITE,MAP_SHARED, fd,0);
        addr = mmap(NULL,mapsize,PROT_READ | PROT_WRITE,MAP_PRIVATE, fd,0);
		if(addr == MAP_FAILED)
        	fprintf(stderr,"[%d:%d] mmap failed",thread->id,it);
		memset(addr,0,mapsize);	
        //fprintf(stderr,"[%d:%d] addr = 0x%p\n",thread->id,it,addr);
    }
	return NULL;
}
void help()
{
    fprintf(stderr, "-----------------------------------------------------------------------\n");
    fprintf(stderr, "Usage: \n" \
            " [ -t | --threads <num_of_threads>] default is 100\n" \
            " [-i | --iterators <iterators_of_thread>]:default is 100\n" \
            " [-p | --pages <pages>]:defalut is 1page(4k)\n" \
            " [-m | --mallocs <pages>]:defalut is 0,unit is M\n" \
            " [-h | --help ]........: display this help\n" \
            " remember to set ulimit -n to 1000000\n");
    fprintf(stderr, "-----------------------------------------------------------------------\n");
}

int main(int argc,char**argv)
{
    int i;
	int uses = 0;
    //int pid;
    char *addr;
    //int len = 100000*0x1000;
    //addr = malloc(len);
    //memset(addr,0,len);
    while(1) {
        int option_index = 0, c = 0;
        static struct option long_options[] = {
            {"h", no_argument, 0, 0 },
            {"help", no_argument, 0, 0},
            {"t", required_argument, 0, 0},
            {"threads", required_argument, 0, 0},
            {"i", required_argument, 0, 0},
            {"iterators", required_argument, 0, 0},
            {"p", required_argument, 0, 0},
            {"pages", required_argument, 0, 0},
            {"m", required_argument, 0, 0},
            {"mallocs", required_argument, 0, 0},
            {0, 0, 0, 0}
        };

        c = getopt_long_only(argc, argv, "", long_options, &option_index);

        /* no more options to parse */
        if(c == -1) break;

        /* unrecognized option */
        if(c == '?') {
            help();
            return 0;
        }

        switch(option_index) {
            /* h, help */
        case 0:
        case 1:
            help();
            return 0;
            break;

            /* t, threads */
        case 2:
        case 3:
            n = atoi(optarg);
            break;

            /* i, iterators */
        case 4:
        case 5:
            iters = atoi(optarg);
            break;

        case 6:
        case 7:
            pages = atoi(optarg);
            break;
		case 8:
        case 9:
            uses = atoi(optarg);
            break;
default:
			help();
			break;
        }
    }

    if(n<=0)
        n = 100;
    if(iters<=0)
        iters = 100;
    if(pages <= 0)
        pages = 1;
	if(uses <=0)
		uses = 0;

	DBG("n=%d,iters=%d,pages=%d,uses=%d",n,iters,pages,uses);
	DBG("using uses:%dM,ram:%dM",uses,n*iters*pages*4/1000);
	if(uses > 0){
		addr = malloc(uses*1000*1000);
		if(addr){
			memset(addr,0,uses*1000*1000);
		}else{
			fprintf(stderr,"malloc error\n");
		}
	}

    thread_info *threads;
    threads = malloc(sizeof(thread_info) * n);
    for(i=0; i<n; i++) {
        threads[i].id = i;
        pthread_create(&threads[i].pthreadId,NULL,worker,&threads[i]);
    }
    for(i=0; i<n; i++) {
        pthread_join(threads[i].pthreadId,NULL);
    }
    //fprintf(stderr,"len=%d\n",len);
    fprintf(stderr,"exit\n");
    return 0;
}
