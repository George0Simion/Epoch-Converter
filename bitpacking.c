#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif

#define five_bits 0x1F
#define four_bits 0xF
#define six_bits 0x3F
#define doi 2
#define patru 4
#define cinci 5
#define sase 6
#define unu 1
#define sapte 7
#define opt 8
#define noua 9
#define paispe 14
#define cincispe 15
#define treiunu 31
#define treidoi 32
#define UnixYear 1970
#define ui unsigned int

typedef struct {
    unsigned int day : cinci;
    unsigned int month : patru;
    unsigned int year : sase;
    unsigned int int1;
    unsigned int int2;
}Date;

int compare(const void *a, const void *b) {
    const Date *date1 = (const Date *)a;
    const Date *date2 = (const Date *)b;

    if ((ui)date1->year != (ui)date2->year) {
        return (int)date1->year - (int)date2->year;
    }

    if ((ui)date1->month != (ui)date2->month) {
        return (int)date1->month - (int)date2->month;
    }

    return (int)date1->day - (int)date2->day;
}

const char* getMonthName(unsigned int month) {
    const char* monthNames[] = {
        "ianuarie", "februarie", "martie", "aprilie",
        "mai", "iunie", "iulie", "august",
        "septembrie", "octombrie", "noiembrie", "decembrie"
    };
    return monthNames[month - unu];
}

unsigned int countSetBits(unsigned int n) {
    unsigned int count = 0;
    while (n) {
        count += n & unu;
        n >>= unu;
    }
    return count;
}

int main() {
    int task = 0;
    scanf("%d", &task);
    if (task == sapte) {
        int N = 0;
        scanf("%d", &N);
        Date *dates = (Date *)malloc(N * sizeof(Date));

        for (int i = 0; i < N; i++) {
            unsigned int input = 0;
            scanf("%u", &input);

            dates[i].day = input & five_bits;
            dates[i].month = (input >> cinci) & four_bits;
            dates[i].year = (input >> noua) & six_bits;
        }

        qsort(dates, N, sizeof(Date), compare);
        for (int i = 0; i < N; i++) {
            printf("%u %s %u\n", (int)dates[i].day, getMonthName((ui)dates[i].month), (int)dates[i].year + UnixYear);
        }
        free(dates);

    } else if (task == opt) {
        int N = 0;
        scanf("%d", &N);
        int nr = (N * cincispe + treiunu) / treidoi;

        unsigned int *data = malloc(nr * sizeof(unsigned int));
        unsigned int *controlBits = malloc((nr / treidoi + (nr % treidoi != 0)) * sizeof(unsigned int));
        Date *dates = malloc(N * sizeof(Date));

        for (int i = 0; i < nr; i++) {
            scanf("%u", &data[i]);
        }
        for (int i = 0; i < (nr / treidoi + (nr % treidoi != 0)); i++) {
            scanf("%u", &controlBits[i]);
        }

        int dateCount = 0;
        int i = 0;
        unsigned long long buffer = 0;
        int bufferBits = 0;
        int bitsRead = 0;

        while (bitsRead < treidoi && dateCount < N && i < nr) {
            int bitsToRead = min(cincispe - bufferBits, treidoi - bitsRead);
            unsigned long long newBits = (unsigned long long)(data[i] >> bitsRead);
            newBits &= (1ULL << bitsToRead) - unu;
            buffer |= (newBits << bufferBits);
            bufferBits += bitsToRead;
            bitsRead += bitsToRead;

            if (bufferBits >= cincispe) {
                Date d;
                d.day = buffer & five_bits;
                buffer >>= cinci;
                d.month = buffer & four_bits;
                buffer >>= patru;
                d.year = buffer & six_bits;
                buffer >>= noua;
                bufferBits -= cincispe;

                dates[dateCount].day = d.day;
                dates[dateCount].month = d.month;
                dates[dateCount].year = d.year;

                unsigned int bitStart = i * treidoi + bitsRead - cincispe;
                dates[dateCount].int1 = bitStart / treidoi;
                dates[dateCount].int2 = (bitStart + paispe) / treidoi;
                dateCount++;
            }

            if (bitsRead == treidoi) {
                i++;
                bitsRead = 0;

                if (bufferBits > 0 && i < nr) {
                    int nextBitsToRead = cincispe - bufferBits;
                    unsigned long long nextBits = (unsigned long long)(data[i]);
                    nextBits &= (unu << nextBitsToRead) - unu;
                    buffer |= (nextBits << bufferBits);
                    bufferBits += nextBitsToRead;
                    bitsRead += nextBitsToRead;
                }
            }
        }

        Date *valid_date = malloc(N * sizeof(Date));
        int k = 0;
        for (int i = 0; i < dateCount; i++) {
            unsigned int controlBitIndex = dates[i].int1 / treidoi;
            unsigned int controlBitPosition = dates[i].int1 % treidoi;
            unsigned int controlBit = (controlBits[controlBitIndex] >> controlBitPosition) & unu;
            unsigned int nextControlBitIndex = dates[i].int2 / treidoi;
            unsigned int nextControlBitPosition = dates[i].int2 % treidoi;
            unsigned int nextControlBit = (controlBits[nextControlBitIndex] >> nextControlBitPosition) & unu;

            unsigned int setBitsParity = countSetBits(data[dates[i].int1]) % doi;
            unsigned int nextSetBitsParity = countSetBits(data[dates[i].int2]) % doi;

            if ((controlBit == setBitsParity) && (nextControlBit == nextSetBitsParity)) {
                valid_date[k++] = dates[i];
            }
        }

        qsort (valid_date, k, sizeof(Date), compare);
        for (int i = 0; i < k; i++) {
            int an = (int)valid_date[i].year+UnixYear;
            int zi = (int)valid_date[i].day;
            printf("%u %s %u\n", zi, getMonthName((ui)valid_date[i].month), an);
        }
    }

    return 0;
}
