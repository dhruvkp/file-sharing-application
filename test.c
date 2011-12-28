#include<sys/shm.h>
#include"unp.h"
//int cnt;
//int k;
int *cnt;
struct client *clients;
struct client
{
                int id;
		int fileno;
                char *name;
                char list[1000][30];
};
//struct client clients[1000];
void sendlist(void *fd)
{
	int confd=*(int *)fd;
	int err;
	char filename[30];
	struct client *list;
	list=clients;
	printf("Sending cnt= %d\n",*cnt);
	err=write(confd,(const void *)cnt,sizeof(int));
	if(err==-1) printf("Error in sending cnt.\n");
	printf("cnt sent..\n");
	err=write(confd,(const void *)list,*cnt*sizeof(struct client));
	if(err==-1) printf("error in sending list.\n");
	printf("list sent..\n");
}
int first(void* fd)
{
        int confd=*(int *)fd;
	char *c;
	int ret=0;
//	pthread_detach(pthread_self());
	//printf("Hello, I'm detached thread.\n");
	c=(char *)malloc(2);
	//printf("Thread for this client is created.\n");
	while(1)
	{
//	sleep(2);
a:	ret=read(confd,(void *)c,2);
	if(ret==-1) goto a;
	if(c[0]=='l')
	{
		printf("Got list request..\n");
		sendlist(fd);
		//flush(fd);
	}
	else if(c[0]=='d')
	{
	}
	else
	{
		printf("Sub-process is exiting.\n");
		pthread_exit();
		//break;
		exit(0);
	}
	}
}
int download(void* confd)
{
                void *c;
                FILE *f;
                char *path;
                int n=1000;
                pthread_detach(pthread_self());
                path=(char*)malloc(101);
                c=(void*)malloc(1000);
                read(*(int *)confd,(void *)path,100);
                printf("Requested %s\n",path);
                f=fopen(path,"r");
                if(f==NULL)
                {
                        printf("File couldn't be opened");
                        return 0;
                }
                printf("Uploading %s\n",path);
     while(1)
                {
                        n=fread(c,1,1000,f);
                        //printf("reached printf..\n");
                        write(*(int *)confd,(const void *)c,1000);
                        printf("%d",n);
                        if(n!=1000) break;
                }
                free(c);
                fclose(f);
                close(*(int *)confd);
                return 1;
}
/*void signalhandling(void)
{
	printf("Got SIGSEGV..\n");
	k=-1;
}*/
int main(void)
{
        int sockfd,j,i;
	int cntaddress,listaddress;
	socklen_t len;
	pid_t pid[1000];
        struct sockaddr_in servaddr,cliaddr;
	cntaddress=shmget(800,sizeof(int),0755);
	listaddress=shmget(900,15*sizeof(struct client),0755);
	//printf("Enough memory.\n");
	cnt=(int *)shmat(cntaddress,NULL,0);
        clients=(struct client *)shmat(listaddress,NULL,0);
	if(*cnt==0) printf("No data yet.\n");
	for(i=0;i<*cnt;i++)
	{
		printf("File of client %d\n",i+1);
		for(j=0;j<clients[i].fileno;j++)
		{
			printf("%s\n",clients[i].list[j]);
		}
	}
        //printf("Attached.\n");
	/*sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_port=htons(13000);
        bind(sockfd,(SA *) &servaddr,sizeof(servaddr));
	*cnt=0;
        listen(sockfd,LISTENQ);
                len=sizeof(cliaddr);
                printf("Reached accept.");
		printf("\n");
                while(1)
                {
			int confd,i;
			int ret=0;
			char filename[30];
			printf("Waiting for client..\n");
			//confd=sockfd;
			//k=0;
			//signal(SIGSEGV,signalhandling);
acc:	                confd=accept(sockfd,(SA *) &cliaddr,&len);
			printf("Connected.\n");
			//if(k==-1) goto acc;
			printf("%d",*cnt);
			strcpy(filename,"");
			clients[*cnt].id=*cnt+1;
			clients[*cnt].fileno=0;
			//printf("Reached first read funtion\n");
			do
			{
			printf("Once..\n");
a:			ret=read(confd,(void *)filename,30);
			printf("Read.\n");
			if(ret==-1) goto a;
			}
			while(strcmp(filename,"STARTOFFILE")!=0);
			printf("%s\n",filename);
			do
			{
				clients[*cnt].fileno++;
				printf(filename);
b:				ret=read(confd,(void *)filename,30);
				if(ret==-1) goto b;
				if(strcmp(filename,"ENDOFFILE")==0) break;
				strcpy(clients[*cnt].list[clients[*cnt].fileno],filename);
			}
                        while(1);
			pid[*cnt]=fork();
			if(pid[*cnt]==0)
			{
				close(sockfd);
				first((void *)&confd);
			}
	//		pthread_create(&tid[cnt],NULL,first,(void *)&confd);
			(*cnt)++;
			for(j=0;j<clients[*cnt-1].fileno;j++)
			{
				printf("%s\n",clients[*cnt-1].list[j]);
			}
//			printf("\nNow List:\n");
			printf("Now, cnt value: %d\n",*cnt);
                }*/
/*	for(j=0;j<*cnt;j++)
	{
		char *result;
		pthread_join(pid[j],(void *)result);
	}*/
/*	void signalhandling(void)
	{0
		goto extern acc;
	}*/
        //if(bind(sockfd,)==-1) printf("Socket bind error.");

        return 0;
}
