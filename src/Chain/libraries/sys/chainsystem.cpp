
#include <sys/chainsystem.h>
#include <stdio.h>


int checkexit(const char* pfile)
{
    if (!pfile)
    {
	printf("pfile is null \n");
        return -1;
    }

    int lockfd = open(pfile,O_WRONLY|O_CREAT);
    if (lockfd == -1)
    {  
	printf("pfile open fail:%s\n",pfile);
        return -2;
    }
  
    int iret = flock(lockfd,LOCK_EX|LOCK_NB);
    if (iret == -1)
    {  
	printf("%s,flock fail\n",pfile);
        return -3;
    }  
 
    return 0;
}
