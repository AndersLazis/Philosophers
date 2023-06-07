
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>


int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах

    printf("Текущее время : %lld\n", milliseconds/1000);
    sleep(1);
    gettimeofday(&tv, NULL);
    long long milliseconds2 = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // Время в миллисекундах
    printf("Текущее время : %lld\n", milliseconds2/1000);


    return 0;
}