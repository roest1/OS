#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct page
{
    int num;
    char time;
    char changed;
} Page;

/*
 * Page schema:
 * num - identifier of page
 * time - "reference bit", counts up each cycle until eviction
 * changed - "modify bit", tracks whether the page has been modified
 */

int main(int argc, char **argv)
{

    if (argc != 6)
    {
        printf("Incorrect amount of params\n");
        printf("Usage:\n");
        printf("./clock {#pageFrames} {swapInCost} {swapOutCost} {inputFilePath} {outputFilePath}\n");
        return 0;
    }

    int tableSize = atoi(argv[1]);
    Page *pages = (Page *)(calloc(tableSize, sizeof(Page)));
    int swapIn = atoi(argv[2]);
    int swapOut = atoi(argv[3]);
    FILE *in = fopen(argv[4], "r");
    if (in == NULL)
    {
        printf("File %s not found", argv[4]);
        return 0;
    }
    FILE *out = fopen(argv[5], "w+");
    int current = 0;
    int vacancy = 0;
    int count = 1;
    int n;
    char c;
    char done;

    int readFaults = 0;
    int writeFaults = 0;
    int swapInTime = 0;
    int swapOutTime = 0;
    /*
     * Variable list:
     * pages -> array of struct page, holds the primary info
     * tableSize -> total number of page frames, length of pages
     * swapIn -> cost of swapping in page
     * swapOut -> cost of swapping out page
     * in -> file pointer to input file
     * out -> file pointer to output file
     * current -> frame the clock hand is pointing at (array index)
     * vacancy -> index of lowest vacant frame, once vacancy == tableSize no more vacancies
     * count -> number of current operation for printing
     * n -> ID of page for current operation
     * c -> type of current operation, 'R' for read and 'W' for write
     * done -> boolean flag for determining hit
     *
     * readFaults -> stores amount of page faults on reads
     * writeFaults -> stores amount of page faults on writes
     * swapInTime -> stores total amount of time for swapping in
     * swapOutTime -> stores total amount of time for swapping out
     */
    while (fscanf(in, "%c %d\n", &c, &n) != EOF)
    {
        done = 0;
        for (int i = 0; i < vacancy; i++)
        {
            if (pages[i].num == n)
            {
                // hit
                fprintf(out, "%d: %c %d H -1 0 0\n", count++, c, n);
                pages[i].time = 0;
                if (c == 'W')
                {
                    pages[i].changed = 1;
                }
                done = 1;
                break;
            }
        }
        if (done)
        {
            continue;
        }
        if (vacancy != tableSize)
        {
            // swap in page at vacancy, increment vacancy
            fprintf(out, "%d: %c %d F -1 %d 0\n", count++, c, n, swapIn);
            pages[vacancy].num = n;
            // default time value is 0
            if (c == 'W')
            {
                pages[vacancy].changed = 1;
                writeFaults++;
            }
            else
            {
                // default changed value is 0
                readFaults++;
            }
            swapInTime += swapIn;
            vacancy++;
        }
        else
        {
            // evict something
            while (1)
            {
                if (pages[current].time == 1)
                {
                    // vacate page at current, swap in new page, break
                    int swapOutOp = pages[current].changed * swapOut; // swapOutOperation
                    fprintf(out, "%d: %c %d F %d %d %d\n", count++, c, n, pages[current].num, swapIn, swapOutOp);
                    pages[current].num = n;
                    pages[current].time = 0;
                    if (c == 'W')
                    {
                        pages[current].changed = 1;
                        writeFaults++;
                    }
                    else
                    {
                        pages[current].changed = 0;
                        readFaults++;
                    }
                    swapInTime += swapIn;
                    swapOutTime += swapOutOp;
                    break;
                }
                pages[current].time++;
                current = (current + 1) % tableSize;
            }
        }
    }
    printf("# of references: %d\n", --count);
    printf("# of faults on read: %d\n", readFaults);
    printf("# of faults on write: %d\n", writeFaults);
    printf("total swap in time: %d\n", swapInTime);
    printf("total swap out time: %d\n", swapOutTime);
    return 0;
}
