// System includes

#include "Friendlyname.h"

tDWORD framedata[Framefractions] ={0} ;
tDWORD DataElement[NbSlots];

void f_getfriendlyname(tDWORD framedata[Framefractions],tBYTE friendlyname[Namesize],tBYTE* NotifyFriendlyName)
{// acquire 32Bytes of data from the frame and arrange it in Byte table[32]
     tBYTE i = 0;
     tBYTE j = 0;
     tBYTE k = 0;
     tBYTE endtext = 0;

     if (*NotifyFriendlyName == 0){
        for (i=0;i<Framefractions && !endtext;i++){
                for (j=0;j<ConversionConstant;j++){
                        if (k < Namesize-1){
                            friendlyname[k]= ((framedata[i] >> (j*8)) & 0xFF);
                            if (friendlyname[k]== TextEnd){
                                endtext = 1;
                                break;
                            }
                            else
                            {
                               k++;
                            }
                        }
                        else
                        {
                            friendlyname[Namesize-1]= TextEnd;
                            break;
                        }
                }
        }
        *NotifyFriendlyName = 1;
    }
    else
    { // do nothing
    }
}

void f_compress_friendlyname (tBYTE friendlyname[Namesize],tBYTE Shortfriendlyname[ShortenedNamesize],tBYTE* NotifyFriendlyName)
{ // squeeze the 32Bytes in the table to 12bytes
    tBYTE i = 0;

    if ((*NotifyFriendlyName == 1) && (ShortenedNamesize <= Namesize)){
        for (i=0;i<ShortenedNamesize;i++){
            Shortfriendlyname[i] = friendlyname[i];
        }
        *NotifyFriendlyName = 2;
    }
    else
    {
      // do nothing
    }
}

void f_storefriendlyname(tBYTE Shortfriendlyname[ShortenedNamesize],const tDWORD LastMemorySlot[NbSlots], tDWORD MemorySlot[NbSlots],tBYTE* NotifyFriendlyName )
{ // stored in EEPROM 10 FIFO FriendlyNames
     tBYTE i = 0;
     tBYTE k = 0;
     tBYTE slotIndex = 0;

    if (*NotifyFriendlyName == 2){

        for (k=0;k<ShortenedNamesize/ConversionConstant;k++){
                // Clear the size of a new Name
        MemorySlot[k]=0U;
        }

        for (i=0;i<ShortenedNamesize;i++){
                // fill the memory slots on top with the new name
                slotIndex = i/ConversionConstant;
                MemorySlot[slotIndex] = (((MemorySlot[slotIndex] >> 8)  & 0xFFFFFFFF) \
                                         | ((Shortfriendlyname[i] << ((ConversionConstant-1)*8)) & 0xFFFFFFFF));

        for (k=(ShortenedNamesize/ConversionConstant);k<NbSlots;k++){
                // shift from top to buttom the old names
                MemorySlot[k]= LastMemorySlot[k-(ShortenedNamesize/ConversionConstant)];
        }

        }
        *NotifyFriendlyName = 3;
    }
    else
    {
       // do nothing
    }
}


void f_NameConcat(tDWORD CompleteName[Framefractions], const tBYTE *fragments )
{ // For test only

    tBYTE i = 0;

    for (i=0;i<Framefractions;i++){
    CompleteName[i]= 0U;
 }

    for (i=0;i<ShortenedNamesize;i++){
    CompleteName[i/ConversionConstant]= (CompleteName[i/ConversionConstant] | ((fragments[i] & 0xFF)<< (i*8)));
 }
}

/// @brief Update the CAN frame content for simulation
/// @param Index element of the names table
/// @param framedata CAN frame data with the raw friendlyName
void f_updateframe(tBYTE Index , tDWORD framedata[Framefractions])
{ // For test only


tBYTE i = 0;
const tBYTE *NewName = nameTable[Index];

f_NameConcat(framedata,NewName);

}

void Test_FriendlyName(tDWORD lastDataElement[NbSlots],
                        tBYTE temptab[Namesize],
                        tBYTE Compressedtemptab[ShortenedNamesize],
                        tBYTE *NotifyFriendlyName)
{ // For test only
    tDWORD Textoutput[Framefractions];
     tBYTE i,j,k,n = 0;

    for (k=0;k<12;k++){
    f_updateframe(k,framedata);

        for (j=0;j<NbSlots;j++){
            lastDataElement[j]= DataElement[j];
        }

             j=0;

         printf("***************** Iteration %d *******************\n", k);
    *NotifyFriendlyName = 0;
        printf("Step %d : NewName= %s\n",*NotifyFriendlyName,framedata);
    f_getfriendlyname(framedata,temptab,NotifyFriendlyName);
        //printf("Step %d : temptab= %s\n",*NotifyFriendlyName,temptab);
    f_compress_friendlyname (temptab,Compressedtemptab,NotifyFriendlyName);
        //printf("Step %d : Compressedtemptab = %s\n",*NotifyFriendlyName,Compressedtemptab);
    f_storefriendlyname(Compressedtemptab,lastDataElement,DataElement,NotifyFriendlyName);
        for (n=0;n<10;n++)
        {
            for (i=0;i<Framefractions;i++){
                Textoutput[i]= DataElement[j++];
            }
        printf("FriendlyNameMem[%d]= %s\n",n,Textoutput);
        }
    printf(" \n ");
    }

}


// Main function
void main()
{
    tDWORD lastDataElement[NbSlots];
    tBYTE temptab[Namesize];
    tBYTE Compressedtemptab[ShortenedNamesize];
    tBYTE NotifyFriendlyName = 0 ;

     //main
//    NotifyFriendlyName = 0;
//    f_getfriendlyname(framedata,temptab,&NotifyFriendlyName);
//    f_compress_friendlyname (temptab,Compressedtemptab,&NotifyFriendlyName);
//    f_storefriendlyname(Compressedtemptab,lastDataElement,DataElement,&NotifyFriendlyName);


// for test only
Test_FriendlyName(lastDataElement,temptab,Compressedtemptab,&NotifyFriendlyName);
// for test only
}
