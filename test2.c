#include"unp.h"
#include<unistd.h>
int main(void)
{
	char s[17];
	FILE *f;
	execl("./ip.sh", "ip.sh", NULL, (char *)0);
	f=fopen("./ipaddress","r");
	fscanf(f,"%s",s);
	printf("%s",s);
	return 0;
}
