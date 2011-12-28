#include<sys/shm.h>
#include"unp.h"
//int cnt;
//int k;
int *downloadstatus;
int *cnt;
struct client *clients;
struct client
{
                int id;
		int fileno;
		char ipaddress[20];
                char *name;
                char list[1000][30];
};
//struct client clients[1000];
void sendlist(void *fd)
{
	int confd=*(int *)fd;
	int err;
	char filename[30];
	char handshake[15];
	struct client *list;
	list=clients;
	strcpy(handshake,"CNTSTART");
	err=write(confd,(const void *)handshake,15);
	err=write(confd,(const void *)cnt,sizeof(int));
	if(err==-1) printf("Error in sending cnt.\n");
	err=write(confd,(const void *)list,*cnt*sizeof(struct client));
	if(err==-1) printf("error in sending list.\n");
	printf("List sent.\n");
}
void idcheck(void *kuchbhi)
{
	int id=((int *)kuchbhi)[1];
	int confd=((int *)kuchbhi)[0];
	char request='d';
	//printf("Download checking started for client %d.\n",id);
	while(*downloadstatus!=id);
	*downloadstatus=-1;
	printf("Client %d accepted the download request.\n\n",id);
	write(confd,(const void *)&request,sizeof(request));
}

void download(int id,int did)
{
	
	*downloadstatus=did;
}

int first(void* fd, int id)
{
        int confd=*(int *)fd;
	char *c;
	int ret=0;
	int did;
	int array[2];
	pthread_t tid;
	array[0]=confd;
	array[1]=id;
	pthread_create(&tid,NULL,idcheck,(void *)array);
	while(1)
	{
	c=(char *)malloc(2*sizeof(char));
a:	ret=read(confd,(void *)c,2);
	if(ret==-1) goto a;
	if(c[0]=='l')
	{
		printf("\nGot List Request from Client %d\n",id);
		sendlist(fd);
	}
	else if(c[0]=='d')
	{
		sleep(1);
		read(confd,(void *)&did,sizeof(int));
		printf("\nGot Download Request from Client %d\n",id);
		write(confd,(void *)clients[did].ipaddress,20*sizeof(char));
		//printf("IP %s sent.\n",clients[did].ipaddress);
		//printf("%d read.\n",did);
		download(id,did);
	}
	else
	{
		printf("\nClient %d is exiting.\n",id);
		clients[id-1].fileno=0;
		pthread_exit();
		exit(0);
	}
	free(c);
	}
}

int main(void)
{
        int sockfd,j;
	int cntaddress,listaddress,daddress;
	socklen_t len;
	pid_t pid[1000];
        struct sockaddr_in servaddr,cliaddr;
	cntaddress=shmget(800,sizeof(int),IPC_CREAT | 0755);
	listaddress=shmget(900,15*sizeof(struct client),IPC_CREAT | 0755);
	daddress=shmget(1000,sizeof(int),IPC_CREAT | 0755);
	cnt=(int *)shmat(cntaddress,NULL,0);
        clients=(struct client *)shmat(listaddress,NULL,0);
	downloadstatus=(int *)shmat(daddress,NULL,0);
	*downloadstatus=-1;
	sockfd=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family=AF_INET;
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_port=htons(13000);
        bind(sockfd,(SA *) &servaddr,sizeof(servaddr));
	*cnt=0;
        listen(sockfd,LISTENQ);
                len=sizeof(cliaddr);
                printf("\nServer Ready\n");
                while(1)
                {
			int confd,i;
			int ret=0;
			char ip[17];
			char filename[30];
			printf("Waiting for clients ..\n\n");
acc:	                confd=accept(sockfd,(SA *) &cliaddr,&len);
			printf("Connection Established.\n");
			ret=read(confd,(void *)ip,17);
			if(ret==-1) 
				printf("read error.\n");
			strncpy(clients[*cnt].ipaddress,ip,17);
			printf("IP Address of Client %d connected is %s\n",(*cnt),clients[*cnt].ipaddress);
			strcpy(filename,"");
			clients[*cnt].id=(*cnt);
			clients[*cnt].fileno=0;
			sleep(500000);
			do
			{			
a:				ret=read(confd,(void *)filename,30);
				printf("Listing the files of Client %d.\n",(*cnt));
				if(ret==-1) goto a;
			}while(strcmp(filename,"STARTOFFILE")!=0);
			do
			{
				clients[*cnt].fileno++;
b:				ret=read(confd,(void *)filename,30);
				printf("%s\n",filename);
				if(ret==-1) goto b;
				if(strcmp(filename,"ENDOFFILE")==0) break;
				
				strcpy(clients[*cnt].list[clients[*cnt].fileno],filename);
			}while(1);
			pid[*cnt]=fork();
			if(pid[*cnt]==0)
			{
				close(sockfd);
				first((void *)&confd, (*cnt));
			}
			sleep(1);
			//wait(&pid[*cnt]);
			//close(confd);
			(*cnt)++;
			printf("No. of Clients Connected: %d\n\n",*cnt);
                }
        return 0;
}
