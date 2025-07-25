#ifndef __OS_ATA_H__
#define __OS_ATA_H__

#include <stdint.h>

typedef uint32_t DiskType;

#define DISK_TYPE_REAL 0
#define DISK_SECTOR_SIZE 512

typedef struct Disk
{
    DiskType diskType;
    int sectorSize;
} Disk;

void DiskSearchAndInit(void);
Disk *DiskGet(int index);
int DiskReadBlock(Disk *disk, unsigned int lba, int total, void *buffer);

#endif
