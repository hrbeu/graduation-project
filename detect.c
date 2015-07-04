#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>

#define LENGTH 111

// cpu info
struct cpu_info{
	char name[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
};
//mem info
struct mem_info{
     char total[50];     
     char free[50];
     char buffers[50];
     char cached[50];
};
//processor info
struct processor_info{
     char processor[50];     
     char vendor_id[50];
     char cpufamily[50];
     char model[50];
       };

float g_cpu_used;
int cpu_num;
int cpu_num_total;

// read cpu info from /proc/stat
void get_cpu_info(struct cpu_info *p)
{
	FILE *fp;
	int i;
	char buff[1024];
	
	if ((fp = fopen("/proc/stat", "r")) == NULL) {
		fprintf(stderr, "cant open /proc/stat\n");
		fclose(fp);
	}
	fgets(buff, sizeof(buff), fp);
	for (i=0; i<cpu_num; i++) {
		fgets(buff, sizeof(buff), fp);
		sscanf(buff, "%s%u%u%u%u", p[i].name, &(p[i].user), &(p[i].nice), &(p[i].system), &(p[i].idle));
	}
	fclose(fp);
}
// calculate cpu occupation
void cal_occupy(struct cpu_info *p1, struct cpu_info *p2)
{
	double od, nd;
	od = (double)(p1->user + p1->nice + p1->system + p1->idle);
	nd = (double)(p2->user + p2->nice + p2->system + p2->idle);
	g_cpu_used = ((p2->user + p2->system + p2->nice) - (p1->user + p1->system + p1->nice)) / (nd - od) * 100;
}
//read mem info from /proc/meminfo
struct mem_info get_mem_info(){
      FILE* fd;
      int n;int i;
      char buff[1024];
      memset(buff,0,sizeof(char)*1024);
      struct mem_info  m;
      
      if((fd=fopen("/proc/meminfo","r")) == NULL)
         {     
               fprintf(stderr, "cant open /proc/meminfo\n");
		   fclose(fd);
         }   
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*s%s",m.total);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*s%s",m.free);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*s%s",m.buffers);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*s%s",m.cached);
       fclose(fd);//�ر��ļ�fd
       return m;
      }
//read processor info from /proc/cpuinfo
struct processor_info get_pro_info(){
      FILE* fd;
      int n;int i;
      char buff[1024];
      memset(buff,0,sizeof(char)*1024);
      struct processor_info p;
      if((fd=fopen("/proc/cpuinfo","r")) == NULL)
         {     
               fprintf(stderr, "cant open /proc/cpuinfo\n");
		   fclose(fd);
         }   
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*[^:]:%s",p.processor);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*[^:]:%s",p.vendor_id);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*[^:]:%s",p.cpufamily);
       fgets(buff,sizeof(buff),fd);
       sscanf(buff,"%*[^:]:%s",p.model);
       fclose(fd);//�ر��ļ�fd
       return p;
      }
int main(int argc, char **argv)
{
//��ȡ��Ƶ	
       FILE *fp;
       char str[81];
       memset(str,0,sizeof(char)*81);
       fp=popen("cat /proc/cpuinfo|grep cpu\\ MHz|sed -e 's/.*:[^0-9]//'","r");
       if(fp<0){
                printf("�޷���ȡCPU��Ƶ��Ϣ\n");
                exit(1);
          }
      fgets(str,80,fp);
      fclose(fp);
      double spd=atof(str);
      
      struct cpu_info ocpu[10];
      struct cpu_info ncpu[10];
	  int i;	
	  pid_t pid = fork();
	  if (pid == 0)
		    execvp(argv[1], &argv[1]);	//start another process to observe cpu
 	  else {
		    cpu_num = sysconf(_SC_NPROCESSORS_ONLN);//�����ں���Ŀ 
            cpu_num_total=sysconf(_SC_NPROCESSORS_CONF);
  while (1) {
	   get_cpu_info(ocpu);
	   sleep(1);
	   get_cpu_info(ncpu);
	   system("clear");   //���� 
       //ʱ��                 
       time_t nowtime;
       time(&nowtime);       
       struct tm *timeinfo;
       timeinfo=localtime(&nowtime);       
       int year, month, day, hour, min, sec;
       year = timeinfo->tm_year + 1900;   //��1900�꿪ʼ����������+1900
       month = timeinfo->tm_mon + 1;    //��0��ʼ����������+1
       day = timeinfo->tm_mday;
       hour = timeinfo->tm_hour;
       min = timeinfo->tm_min;
       sec = timeinfo->tm_sec;
       printf("%d %d %d %d:%d:%d\n", year, month, day, hour, min, sec);
       printf("\n");          
//��������Ϣ 
       struct processor_info p;
       p=get_pro_info();
       printf("Processor Information:     \n");
       printf("processor :  %s    ",p.processor);
       printf("vendor_id :  %s  \n",p.vendor_id);
       printf("cpu family:  %s    ",p.cpufamily);
       printf("model     :  %s  \n",p.model);       
       printf("\n");        
//cpu��Ϣ 
       for (i=0; i<cpu_num; i++) {				                        
                  printf("total cpu number   : %d\n",cpu_num_total);
       	      printf("running cpu number : %d\n",cpu_num); 
                  printf("\n");
                  printf("name   nice  system   idle     usage       MHz\n");
                  printf("\n");
                  cal_occupy(&ocpu[i], &ncpu[i]);
                  printf("%s   ",ocpu[i].name);
                  printf("%u   ",ocpu[i].nice);
                  printf("%u   ",ocpu[i].system);
                  printf("%u   ",ocpu[i].idle);
                  printf("%.4f%%   ",g_cpu_used);
                  printf("%f  \n",spd);
                  printf("\n");    
                  printf("                (��λ��jiffies .1 jiffies=0.01 second)\n");
                       }
//�ڴ���Ϣ	
    struct mem_info m;
    m=get_mem_info();
    printf("Memory Information:   \n");
    printf("MemTotal:  %s \n",m.total);
    printf("MemFree :  %s \n",m.free);
    printf("Buffers :  %s \n",m.buffers);
    printf("Cached  :  %s \n",m.cached);    
    
    sleep(1);
		}
	}	
	return 0;
}
