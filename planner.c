#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

#define NumSecOra 3600
#define UNSPE 11

typedef struct {
    unsigned int time;
    unsigned char tip;
}sorting;

typedef struct {
    unsigned int startTimestamp;
    unsigned int endTimestamp;
}AvailabilityInterval;

typedef struct {
    char nume[UNSPE];
    int K;
    TDateTimeTZ *data;
    AvailabilityInterval *interval;
    int timezoneIndex;
}idk;

int compareIntervals(const void *a, const void *b) {
    const sorting *interval1 = (const sorting *)a;
    const sorting *interval2 = (const sorting *)b;

    if (interval1->time < interval2->time) {
        return -1;
    } else if (interval1->time > interval2->time) {
        return 1;
    } else {
        return 0;
    }
}

int compareIntervals2(const void *a, const void *b) {
    const unsigned int *start1 = (const unsigned int *)a;
    const unsigned int *start2 = (const unsigned int *)b;

    if (*start1 < *start2) {
        return -1;
    } else if (*start1 > *start2) {
        return 1;
    } else {
        return 0;
    }
}

int compareByName(const void *a, const void *b) {
    const idk *ia = (const idk *)a;
    const idk *ib = (const idk *)b;
    return strcmp(ia->nume, ib->nume);
}

int main() {
    int T = 0, P = 0, F = 0, durata_ev = 0;

    scanf("%d", &T);
    TTimezone *zone = malloc(T * sizeof(TTimezone));
    int ceva = 0;
    for (int i = 0; i < T; i++) {
        scanf("%s %d", zone[i].name, &ceva);
        zone[i].utc_hour_difference = (signed char)ceva;
    }

    scanf("%d", &P);
    idk *om = malloc(P * sizeof(idk));

    for (int i = 0; i < P; i++) {
        char tmz[UNSPE];
        scanf("%s %s %d", om[i].nume, tmz, &om[i].K);
        if (om[i].K == 0) {
            om[i].data = malloc(sizeof(TDateTimeTZ));
            om[i].interval = malloc(sizeof(AvailabilityInterval));
        } else {
            om[i].data = malloc(om[i].K * sizeof(TDateTimeTZ));
            om[i].interval = malloc(om[i].K * sizeof(AvailabilityInterval));
        }

        int tzIndex = -1;
        for (int tz = 0; tz < T; tz++) {
            if (strcmp(tmz, zone[tz].name) == 0) {
                tzIndex = tz;
                break;
            }
        }
        om[i].timezoneIndex = tzIndex;

        for (int j = 0; j < om[i].K; j++) {
            om[i].data[j].tz = &zone[tzIndex];

            int year = 0, month = 0, day = 0, hour = 0, ore = 0;
            scanf("%d %d %d %d %d", &year, &month, &day, &hour, &ore);
            om[i].data[j].date.year = (unsigned int)year;
            om[i].data[j].date.month = (unsigned char)month;
            om[i].data[j].date.day = (unsigned char)day;
            om[i].data[j].time.hour = (unsigned char)hour;
            om[i].data[j].time.min = 0;
            om[i].data[j].time.sec = 0;

            unsigned int bau = convertDateTimeTZToUnixTimestamp(om[i].data[j]);
            om[i].interval[j].startTimestamp = bau;
            om[i].interval[j].endTimestamp = bau + ore * NumSecOra;
        }
    }
    scanf("%d%d", &F, &durata_ev);

    int nr = 0;
    for (int i = 0; i < P; i++) {
        int j = 0;
        while (j < om[i].K - 1) {
            if (om[i].interval[j].endTimestamp >= om[i].interval[j + 1].startTimestamp) {
                if (om[i].interval[j].endTimestamp < om[i].interval[j + 1].endTimestamp) {
                    om[i].interval[j].endTimestamp = om[i].interval[j + 1].endTimestamp;
                }

                for (int k = j + 1; k < om[i].K - 1; k++) {
                    om[i].interval[k] = om[i].interval[k + 1];
                }
                om[i].K--;
            } else {
                j++;
            }
        }
        nr += om[i].K;
    }

    int k = 0;
    unsigned int *v = malloc(nr * sizeof(unsigned int));
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < om[i].K; j++) {
            v[k++] = om[i].interval[j].startTimestamp;
        }
    }
    qsort(v, k, sizeof(unsigned int), compareIntervals2);

    unsigned int *uniqueV = malloc(nr * sizeof(unsigned int));
    int uniqueCount = 0;
    for (int i = 0; i < k; i++) {
        if (i == 0 || v[i] != v[i - 1]) {
            uniqueV[uniqueCount] = v[i];
            uniqueCount++;
        }
    }

    int sem = 0;
    unsigned int suitableTime = 0;
    for (int i = 0; i < uniqueCount; i++) {
        int count = 0;
        for (int j = 0; j < P; j++) {
            for (int x = 0; x < om[j].K; x++) {
                unsigned int start = om[j].interval[x].startTimestamp;
                unsigned int end = om[j].interval[x].endTimestamp;
                if (uniqueV[i] >= start && uniqueV[i] + NumSecOra * durata_ev <= end) {
                    count++;
                    break;
                }
            }
        }
        if (count >= F) {
            sem = 1;
            suitableTime = uniqueV[i];
            break;
        }
    }
    qsort(om, P, sizeof(idk), compareByName);

    if (sem) {
        for (int i = 0; i < P; i++) {
            int isAvailable = 0;
            for (int j = 0; j < om[i].K; j++) {
                if (suitableTime >= om[i].interval[j].startTimestamp && suitableTime < om[i].interval[j].endTimestamp) {
                    isAvailable = 1;
                    break;
                }
            }

            if (isAvailable) {
                TDateTimeTZ localTime = convertUnixTimestampToDateTimeTZ(suitableTime, zone, om[i].timezoneIndex);
                printf("%s: ", om[i].nume);
                printDateTimeTZ(localTime);
                printf("\n");
            } else {
                printf("%s: invalid\n", om[i].nume);
            }
        }
    } else {
        printf("imposibil\n");
    }

    for (int i = 0; i < P; i++) {
        free(om[i].data);
        free(om[i].interval);
    }
    free(om);
    free(zone);
    free(v);
    free(uniqueV);

    return 0;
}
