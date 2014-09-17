#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <fcntl.h>

// test pid startaddr endaddr
int main(int argc, char *argv[])
{
	int pid, i, fd;
	unsigned int saddr, eaddr, size;
	char procfname[256];
	unsigned char *buf;
	FILE *fp;
	
	if (argc != 4)
	{
		printf("Usage: %s pid startaddr endaddr\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		pid = atoi(argv[1]);
		saddr = strtoul(argv[2], NULL, 0);
		eaddr = strtoul(argv[3], NULL, 0);
		
	}
	printf("PID: %d\n", pid);
	printf("Start Address: %d\n", saddr);
	printf("End Address: %d\n", eaddr);
	size = (eaddr - saddr);
	printf("Size: %fKB\n", size / 1024.0);
	buf = (char *)malloc(size);
	if (buf == NULL)
	{
		fprintf(stderr, "[!] memory allocate failure.");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("memory allocated.\n");
	}
	
	sprintf(procfname, "/proc/%d/mem", pid); // ゆるして
	
	fd = open(procfname, O_RDONLY);
	if (fd == -1)
	{
		fprintf(stderr, "[!] %s open failure.\n", procfname);
		free(buf);
		exit(EXIT_FAILURE);
	}
	ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	printf("attached. waiting process..\n");
	waitpid(pid, NULL, 0);
	printf("working...\n");
	lseek(fd, saddr, SEEK_SET); // Set to start address
	if (read(fd, buf, size) == -1) // 遅そう
	{
		fprintf(stderr, "[!] memory read failure.\n");
	}
	ptrace(PTRACE_DETACH, pid, NULL, NULL);
	printf("detouched.\ndump complete.\n");
	close(fd);
	printf("write to file...\n");
	fp = fopen("memory.bin", "w");
	if (fwrite(buf, size, 1, fp) < 1)
	{
		fprintf(stderr, "[!] fwrite failure.\n");
	}
	fclose(fp);
	free(buf);
	printf("done.\n");
}
