
/**
 * Name: Oest, Riley

 * Email: roest1@lsu.edu
  
 * Project: PA-2 (Page-Replacement)
  
 * Instructor: Feng Chen
 
 * Class: cs4103-sp23
 
 * Login ID: cs410355
 */

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

class Page {
    private int pageNum; // page number
    private boolean referenceBit; // recently used?
    private boolean modifiedBit; // dirty?

    
    // Accessors //
    public int getPageNum() {
        return pageNum;
    }

    public boolean getReferenceBit() {
        return referenceBit;
    }

    public boolean getModifiedBit() {
        return modifiedBit;
    }
    // Accessors //

    // Modifiers //
    public void setReferenceBit(boolean bit) {
        this.referenceBit = bit;
    }

    public void setModifiedBit(boolean bit) {
        this.modifiedBit = bit;
    }

    public void setPageNum(int pageNum) {
        this.pageNum = pageNum;
    }
    // Modifiers //
}

public class Main {
    public static void main(String[] args) throws IOException {
        if (args.length != 5) {
            System.err.println(
                    "Compile program by running:\njavac Main.java\nThen run the program by passing in the number of page frames available, the swap in time cost, the swap out time cost, and the input and output files. ie:\njava Main 100 10 20 input.txt trace.txt\n");
            return;
        }
        int numFramesAvailable = Integer.parseInt(args[0]); // total number of page frames available
        int swapInTimeCost = Integer.parseInt(args[1]); // swap in time cost
        int swapOutTimeCost = Integer.parseInt(args[2]); // swap out time cost

        BufferedReader reader = new BufferedReader(new FileReader(args[3])); // read file
        BufferedWriter writer = new BufferedWriter(new FileWriter(args[4])); // write file

        Page[] pageTable = new Page[numFramesAvailable];
        for (int i = 0; i < numFramesAvailable; i++) {
            pageTable[i] = new Page();
        }

        int clockIndex = 0; // index of the frame the clock hand it pointing to
        int leastRecentlyUsed = 0; // index of least recently used frame
        int index = 1; // index for printing
        int pageID = 0; // page number of current page
        char readOrWrite = 0; // read or write character
        boolean pageHit = false; // determine page hit

        int numReadFaults = 0; // number of page faults on read operation
        int numWriteFaults = 0; // number of page faults on write operation
        int totalSwapInTime = 0; // total time spent swapping in
        int totalSwapOutTime = 0; // total time spent swapping out

        String line = "";
        while ((line = reader.readLine()) != null) {
            String[] parts = line.split(" ");
            readOrWrite = parts[0].charAt(0);
            pageID = Integer.parseInt(parts[1]);
            pageHit = false;

            for (int i = 0; i < leastRecentlyUsed; i++) {
                if (pageTable[i].getPageNum() == pageID) {
                    writer.write(index++ + ": " + readOrWrite + " " + pageID + "H -1 0 0");
                    writer.newLine();
                    pageTable[i].setReferenceBit(false);
                    if (readOrWrite == 'W' || readOrWrite == 'w') {
                        pageTable[i].setModifiedBit(true);
                    }
                    pageHit = true;
                    break;
                }
            }
            if (pageHit) {
                continue;
            }
            if (leastRecentlyUsed != numFramesAvailable) {
                // swap in page at least recently used
                writer.write(index++ + ": " + readOrWrite + " " + pageID + " F -1 " + swapInTimeCost + " 0");
                writer.newLine();
                pageTable[leastRecentlyUsed].setPageNum(pageID);
                if (readOrWrite == 'W' || readOrWrite == 'w') {
                    pageTable[leastRecentlyUsed].setModifiedBit(true);
                    numWriteFaults++;
                } else {
                    numReadFaults++;
                }
                totalSwapInTime += swapInTimeCost;
                leastRecentlyUsed++;
            } else {
                // Need to evict a page
                while (true) {
                    if (pageTable[clockIndex].getReferenceBit()) {
                        // swap in new page
                        int modBit = pageTable[clockIndex].getModifiedBit() ? 1 : 0;
                        int swapOutOperationCost = modBit * swapOutTimeCost;
                        writer.write(
                                index++ + ": " + readOrWrite + " " + pageID + " F " + pageTable[clockIndex].getPageNum()
                                        + " " + swapInTimeCost + " " + swapOutOperationCost);
                        writer.newLine();
                        pageTable[clockIndex].setPageNum(pageID);
                        pageTable[clockIndex].setReferenceBit(false);
                        if (readOrWrite == 'W' || readOrWrite == 'w') {
                            pageTable[clockIndex].setModifiedBit(true);
                            numWriteFaults++;
                        } else {
                            pageTable[clockIndex].setModifiedBit(false);
                            numReadFaults++;
                        }
                        totalSwapInTime += swapInTimeCost;
                        totalSwapOutTime += swapOutOperationCost;
                        break;
                    }
                    pageTable[clockIndex].setReferenceBit(true);
                    clockIndex = (clockIndex + 1) % numFramesAvailable;
                }
            }
        }
        writer.close();
        System.out.printf("Total number of page references = %d\n", --index);
        System.out.printf("Total number of page faults on read references = %d\n", numReadFaults);
        System.out.printf("Total number of page faults on write references = %d\n", numWriteFaults);
        System.out.printf("Total number of time units for swapping in pages from swap space = %d\n",
                totalSwapInTime);
        System.out.printf("Total number of time units for swapping out pages to swap space = %d\n",
                totalSwapOutTime);
    }
}
