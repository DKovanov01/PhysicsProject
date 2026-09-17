#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H
#include <iostream>
#include <memory>
#include <fstream>

std::size_t usedMemoryTotal = 0;
std::size_t totalAllocations = 0;
std::size_t totalFrees = 0;
std::size_t peakMemory = 0;

void usedMemoryAdd(std::size_t numBytes)
{
    usedMemoryTotal += numBytes;
    totalAllocations++;

    if (usedMemoryTotal > peakMemory)
        peakMemory = usedMemoryTotal;
}

void usedMemorySubtract(std::size_t numBytes)
{
    usedMemoryTotal -= numBytes;
    totalFrees++;
}

//void usedMemory() {
//    std::cout << "memory used: " << usedMemoryTotal << "\n";
//}

void writeMemoryLog()
{
    std::ofstream file("memory_log.txt");

    if (!file.is_open())
        return;

    file << "===== MEMORY MONITOR =====\n";
    file << "Total allocations: " << totalAllocations << "\n";
    file << "Total frees: " << totalFrees << "\n";
    file << "Current allocations: " << totalAllocations - totalFrees << "\n";
    file << "Current memory: " << usedMemoryTotal << " bytes\n";
    file << "Peak memory: " << peakMemory << " bytes\n";

    if (usedMemoryTotal == 0)
        file << "\nNo outstanding memory allocations.\n";
    else
        file << "\nPotential memory leak detected.\n";
}

void* operator new(std::size_t numBytes) {
    //std::cout << "allocating " << numBytes << " bytes of memory\n";
    usedMemoryAdd(numBytes);
    //usedMemory();
    return std::malloc(numBytes);
}

void operator delete(void* memoryLocation, std::size_t numBytes) {
    //std::cout << "freeing " << numBytes << " bytes of memory\n";
    usedMemorySubtract(numBytes);
    //usedMemory();
    std::free(memoryLocation);
}
/*
/// This is a trick to get access to numBytes 
/// w/o wild typecasts. 
struct ArraySize {
    std::size_t numBytes;
};

/// I did a little hack to hide the total number of bytes allocated in the array itself. 
void* operator new[](std::size_t numBytes) {
    std::cout << "allocating an array of " << numBytes << " bytes of memory\n";
    ArraySize* array = reinterpret_cast<ArraySize*>(std::malloc(numBytes + sizeof(ArraySize)));
    if (array) array->numBytes = numBytes;
    usedMemoryAdd(numBytes);
    return array + 1;
}

/// This overload doesn't work as advertised in VS22
void operator delete[](void* memoryLocation) {
    ArraySize* array = reinterpret_cast<ArraySize*>(memoryLocation) - 1;
    std::cout << "freeing array " << array->numBytes << " bytes of memory\n";
    usedMemorySubtract(array->numBytes);
    free(array);
}*/

#endif
