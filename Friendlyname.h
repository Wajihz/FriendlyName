#ifndef FRIENDLYNAME_H
#define FRIENDLYNAME_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define Namesize 32  // FriendlyName size is 32 Bytes
#define ShortenedNamesize 32  // Compressed friendlyName size is 32 Bytes
#define ConversionConstant 4 // 4 bytes in uint32
#define Framefractions (Namesize / ConversionConstant)  // need 32 bits fractions to contain the rawFriendlyName in the framedata
#define TextEnd 0x00// End of the text symbol \0
#define NbSlots (ShortenedNamesize * 8 * 10 / 32) // Number of memory slots to store the last 10 friendlyNames


typedef uint32_t tDWORD;
typedef uint8_t tBYTE;

// for test only
   const tBYTE *nameTable[] = {
    "John Smith Jane Doe",
    "Michael\0 Johnson",
    "Emily Davis",
    "William Brown",
    "Olivia Wilson",
    "James Lee",
    "Sophia Martin",
    "Robert Anderson",
    "Ava Taylor",
    "Jean Dubois Marie Lefevre",
    "Pierre Martin",
    "Claire Bernard",
    "Luc Thomas",
    "Sophie Robert",
    "Richard Antoine",
    "Camille Petit",
    "Louis Durand Amélie Leroy"
};
// for test only

void f_getfriendlyname(tDWORD framedata[Framefractions],tBYTE friendlyname[Namesize],tBYTE* NotifyFriendlyName);
void f_compress_friendlyname (tBYTE friendlyname[Namesize],tBYTE Shortfriendlyname[ShortenedNamesize],tBYTE* NotifyFriendlyName);
void f_storefriendlyname(tBYTE Shortfriendlyname[ShortenedNamesize],const tDWORD LastMemorySlot[NbSlots], tDWORD MemorySlot[NbSlots],tBYTE* NotifyFriendlyName);

#endif // FRIENDLYNAME_H
