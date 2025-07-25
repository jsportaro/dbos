#include <drivers/ata/ata.h>
#include <mmio.h>
#include <status.h>

#include <stddef.h>

static Disk primaryDisk = {0};

static int DiskReadSector(int lba, int total, void *buffer) 
{
    outb(0x1f6, (lba >> 24) | 0xe0);
    outb(0x1f2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1f7, 0x20);

    unsigned short *ptr = (unsigned short *)buffer;

    for (int i = 0; i < total; i++)
    {
        // Wait to read (polling)
        char c = insb(0x1f7);
        while (!(c & 0x08))
        {
            c = insb(0x1f7);
        }

        // Copy from disk to memory
        for (int j = 0; j < 256; j++)
        {
            *ptr = insw(0x1f0);
            ptr++;
        }
    }

    return SUCCESS;
}

void DiskSearchAndInit(void)
{
    primaryDisk.diskType = DISK_TYPE_REAL;
    primaryDisk.sectorSize = DISK_SECTOR_SIZE;
}

Disk *DiskGet(int index)
{
    if (index != 0)
    {
        return NULL;
    }

    return &primaryDisk;
}

int DiskReadBlock(Disk *disk, unsigned int lba, int total, void *buffer)
{
    if (&primaryDisk != disk)
    {
        return -EIO;
    }

    return DiskReadSector(lba, total, buffer);
}
