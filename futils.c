#include"futils.h"

#ifdef __linux__
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#endif

int create_dir(const char *name)
{
#ifdef __linux__
	struct stat chk = {0};
	if(stat(name, &chk) == -1){
		return mkdir(name, 0700);
	}else{
		return 1;
	}
#endif
	return 1;
}
