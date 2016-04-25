#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <stdint.h>

#define BUF_SIZE 2
#define LOCK_MAX 1
#define SLEEP_MS 100

double gettime()
{
    struct timeval adios_timer_tp;
    gettimeofday(&adios_timer_tp, NULL); \
        return  ((double)adios_timer_tp.tv_sec + ((double)adios_timer_tp.tv_usec)/1000000.0);
}

void sleep_with_interval (double timeout_sec, int interval_ms)
{
    int stay_in_poll_loop = 1;
    double t1 = gettime();
    double elapsed_time;

    struct timespec delay;
    delay.tv_sec = interval_ms/1000;
    delay.tv_nsec = (((uint64_t)interval_ms * 1000000L)%1000000000L);

    while (stay_in_poll_loop)
    {
        elapsed_time = gettime() - t1;

        if (elapsed_time > timeout_sec)
            stay_in_poll_loop = 0;
        else
            nanosleep(&delay, NULL);
    }
    //printf("Spent: %g (sec)\n", elapsed_time);
}

int lockcreate(const char* fname)
{
    int fd;
    if ((fd = open(fname, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) < 0)
    {
        perror("open");
        exit(1);
    }
    return fd;
}

void wlock(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;    /* F_RDLCK, F_WRLCK, F_UNLCK */
    fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
    fl.l_start = 0;         /* byte offset relative to l_whence */
    fl.l_len = 0;           /* #bytes (0 means to EOF) */

    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
}

void wunlock(int fd)
{
    struct flock fl;
    fl.l_type = F_UNLCK;    /* F_RDLCK, F_WRLCK, F_UNLCK */
    fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
    fl.l_start = 0;         /* byte offset relative to l_whence */
    fl.l_len = 0;           /* #bytes (0 means to EOF) */

    if (fcntl(fd, F_SETLKW, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }
}

pid_t wlockcheck(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;    /* F_RDLCK, F_WRLCK, F_UNLCK */
    fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
    fl.l_start = 0;         /* byte offset relative to l_whence */
    fl.l_len = 0;           /* #bytes (0 means to EOF) */

    if (fcntl(fd, F_GETLK, &fl) == -1)
    {
        perror("fcntl");
        exit(1);
    }

    if (fl.l_type == F_UNLCK)
        return (-1);             /* false, region is not locked by another
                                 * process */
    return (fl.l_pid);        /* true, return pid of lock owner */
}

void lockup(const char* fname)
{
    char buffer[BUF_SIZE+1];
    int fd;

TRY:
    fd = lockcreate(fname);
    wlock(fd);
    if (lseek(fd, 0, SEEK_SET) == -1) goto END;
    if (read(fd, buffer, BUF_SIZE) == -1) goto END;
    if (atoi(buffer) < LOCK_MAX)
    {
        lseek(fd, 0, SEEK_SET);
        snprintf(buffer, BUF_SIZE+1, "%d\n", atoi(buffer)+1);
        write(fd, buffer, BUF_SIZE);
    }
    else
    {
        wunlock(fd);
        close(fd);
        printf("Waiting down-lock: %s\n", fname);
        sleep_with_interval(0.1, 100);
        goto TRY;
    }
END:
    wunlock(fd);
    close(fd);
}

void lockdown(const char* fname)
{
    char buffer[BUF_SIZE+1];
    int fd;

TRY:
    fd = lockcreate(fname);
    wlock(fd);
    if (lseek(fd, 0, SEEK_SET) == -1) goto END;
    if (read(fd, buffer, BUF_SIZE) == -1) goto END;
    if (atoi(buffer) > 0)
    {
        lseek(fd, 0, SEEK_SET);
        snprintf(buffer, BUF_SIZE+1, "%d\n", atoi(buffer)-1);
        write(fd, buffer, BUF_SIZE);
    }
    else
    {
        wunlock(fd);
        close(fd);
        printf("Waiting up-lock : %s\n", fname);
        sleep_with_interval(0.1, 100);
        goto TRY;
    }
END:
    wunlock(fd);
    close(fd);
}
