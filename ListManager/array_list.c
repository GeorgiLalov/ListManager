/**
*
*/

//! Used for malloc.
#include <stdlib.h>
//! Used for memmove
#include <string.h>

#include "array_list.h"

//! The number of bits in one byte.
#define NUMBER_OF_BITS_IN_BYTE    8


/**
 * @brief Initialize the array list info struct and
 *        allocate all memory for the list.
 *
 * @param fp_pListInfoDest    The array list info that will be set.
 * @param fp_dataSize         The size of one node.
 * @param fp_capacity         The total number of nodes in the list.
 * @param fp_eCapacityType    The type of the capacity, is it fixed or not.
 * @return
 *          #0  - On success.
 *          #-1 - On null parameter.
 *          #-2 - On error allocating memory.
 */
int ArrayList_Init(tsArrayListInfo      *fp_pListInfoDest,
                   const unsigned int   fp_dataSize,
                   const unsigned int   fp_capacity,
                   const teCapacityType fp_eCapacityType)
{
    int retVal = 0;

    //! Check parameter for null.
    if (fp_pListInfoDest == NULL)
    {
        //! If parameter is null, return error.
        retVal = -1;
    }
    else
    {
        //! Init the list info structure properly.
        fp_pListInfoDest->dataSize      = fp_dataSize;
        fp_pListInfoDest->totalCapacity = fp_capacity;
        fp_pListInfoDest->capacityType  = fp_eCapacityType;
        fp_pListInfoDest->count         = 0;

        //! Allocate memory for the list.
        //!                                      capacity is the total count of the nodes.
        //!                                      dataSize is the size of one node.
        fp_pListInfoDest->pHead         = malloc(fp_dataSize * fp_capacity);

        //! If memory error occured.
        if (fp_pListInfoDest->pHead == NULL)
        {
            //! Return memory error.
            retVal = -2;
        }
    }

    return retVal;
}

int ArrayList_Add(tsArrayListInfo *fp_pListInfo,
                  const void      *fp_pNewData)
{
    int retVal = 0;

    if (fp_pListInfo == NULL ||
        fp_pNewData  == NULL)
    {
        retVal = -1;
    }
    else
    {
        //! If array list is full.
        if (fp_pListInfo->count == fp_pListInfo->totalCapacity)
        {

        }
        //! If list is not full, add normally.
        else
        {
            //! Copy the data to the list.
            //!              Cast to char to know that compiler thinks pHead for 1 byte.
            //!              dataSize is bit size, so divide by 8 to get how many bytes one node is and multiply by
            //!              the current count to get to the position of the free memory.
            memmove((void *) (((char *) fp_pListInfo->pHead) + (fp_pListInfo->count * (fp_pListInfo->dataSize / NUMBER_OF_BITS_IN_BYTE))),
                    fp_pNewData, fp_pListInfo->dataSize);

            //! Increase the count.
            fp_pListInfo->count++;
        }
    }

    return retVal;
}

int   ArrayList_AddLast(tsArrayListInfo *fp_pListInfo,
                        const void      *fp_pNewData);
int   ArrayList_RemoveAt(tsArrayListInfo    *fp_pListInfo,
                         const unsigned int fp_index);
void* ArrayList_Get(const tsArrayListInfo *fp_pListInfo,
                    const unsigned int    fp_index);

/**
 * @brief Destroy the array list.
 * @param fp_pListInfo    Pointer to the list info.
 */
void ArrayList_Destroy(tsArrayListInfo *fp_pListInfo)
{
    //! Check parameter for null.
    if (fp_pListInfo != NULL)
    {
        //! Free the allocated space.
        free(fp_pListInfo->pHead);

        //! Reset the head and the count of nodes.
        fp_pListInfo->pHead = NULL;
        fp_pListInfo->count = 0;
    }
}

int   ArrayList_Sort(tsArrayListInfo      *fp_pListInfo,
                     const tCmpNodesFuncP *fp_pCmpFuncP);
void* ArrayList_Search(const tsArrayListInfo *fp_pListInfo,
                       const tCmpNodesFuncP  *fp_pCmpFuncP,
                       const void            *fp_pSearchedData);
int   ArrayList_SortedInsert(tsArrayListInfo      *fp_pListInfo,
                             const tCmpNodesFuncP *fp_pCmpFuncP,
                             const void           *fp_pNewData);
