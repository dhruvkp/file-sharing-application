/*
 * =====================================================================================
 *
 *       Filename:  hello.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/20/2011 01:09:43 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dr. Fritz Mehner (mn), mehner@fh-swf.de
 *        Company:  FH Südwestfalen, Iserlohn
 *
 * =====================================================================================
 */
#include"unp.h"
#include<errno.h>
#include<dirent.h>
#include<semaphore.h>

int main(void)
{
	FILE *f;
	int pid;
	char s[100];
	char *args[2];
	pid=fork();
	if(pid==0)
	{
	args[0]="./ip2.sh";
	args[1]=NULL;
	execv("./ip2.sh", args);
	}
//	execlp("ip2.sh","ip2.sh",(char *)0);
	/*if(i==-1) printf("error");
	else printf("fuckoff");*/
//	f=fopen("./ipaddress","r");
 //       fscanf(f,"%s",s);
	return 0;
}
