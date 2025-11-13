#include <stdio.h>

int main() {
    int bucketSize, outRate, n;

    printf("Enter bucket size (in packets): ");
    scanf("%d", &bucketSize);

    printf("Enter output rate (packets per second): ");
    scanf("%d", &outRate);

    printf("Enter number of time slots: ");
    scanf("%d", &n);

    int arrivals[100];   // assume max 100 time slots
    printf("Enter packets arriving at each second:\n");
    for (int i = 0; i < n; i++) {
        printf("t = %d : ", i);
        scanf("%d", &arrivals[i]);
    }

    int time = 0;
    int bucket = 0;

    printf("\nTime\tArrived\tSent\tDropped\tIn Bucket\n");
    printf("-------------------------------------------------\n");

    // Run until all arrivals processed and bucket becomes empty
    while (time < n || bucket > 0) {
        int incoming = 0;
        if (time < n) {
            incoming = arrivals[time];
        }

        int dropped = 0;

        // Add incoming packets into the bucket
        if (bucket + incoming > bucketSize) {
            dropped = bucket + incoming - bucketSize;
            bucket = bucketSize;          // bucket is full
        } else {
            bucket += incoming;
        }

        // Leak / send packets at constant rate
        int sent = (bucket >= outRate) ? outRate : bucket;
        bucket -= sent;

        printf("%2d\t%7d\t%4d\t%7d\t%9d\n",
               time, incoming, sent, dropped, bucket);

        time++;
    }

    return 0;
}
