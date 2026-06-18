#include <fcntl.h>  // open
#include <unistd.h> // read write close
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h> // metadata info

int main()
{
    int fd = open("test.txt", O_RDONLY);

    if (fd == -1)
    {
        // printf("open filed %d\n", errno);
        // When compiling, the path is relative to the current CWD dir
        perror("open"); // open: No such file or directory
        return 1;
    }

    char buf[1024];

    /**
     *
     * read();
     *
     *
     * >0 read the bytes size of file
     *
     * = 0 EOF
     *
     * = -1 read error
     */
    ssize_t n = read(fd, buf, sizeof(buf));

    if (n == -1)
    {
        perror("read");
    }

    if (n > 0)
    {
        buf[n] = '\0';
    }

    for (ssize_t i = 0; i < n; i++)
    {
        printf("%c", buf[i]);
    }

    printf("\n");

    struct stat sb;

    if (stat("test.txt", &sb) == -1)
    {
        perror("stat");
        return 1;
    }

    printf("permissions: %x\n", (unsigned)sb.st_mode & 0777);

    // close()
    close(fd);
}