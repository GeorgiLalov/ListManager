/**
*
*/

//! Used for malloc.
#include <stdlib.h>
//! Used for memmove
#include <string.h>

#include "array_list.h"

//!
#define CAPACITY_EXTEND_RATE    2

/**
 * @brief Swap the data between two nodes.
 * @param fp_pListInfo          The list info.
 * @param fp_firstNodeIndex     The index of the first node.
 * @param fp_secondNodeIndex    The index of the second node.
 * @return
 */
static int fs_swapTwoNodes(const tsArrayListInfo *fp_pListInfo,
                           const unsigned int    fp_firstNodeIndex,
                           const unsigned int    fp_secondNodeIndex)
{
    void *pFirstNode  = NULL;
    void *pSecondNode = NULL;
    void *pBuffer     = NULL;
    int  retVal       = 0;

    if (fp_pListInfo == NULL)
    {
        retVal = -1;
    }
    else
    {
        //! Allocate memory for the buffer.
        pBuffer = malloc(fp_pListInfo->dataSize);

        //! On memory error.
        if (pBuffer == NULL)
        {
            retVal = -2;
        }
        else
        {
            //! Get the nodes at the specified indexes.
            pFirstNode  = ArrayList_Get(fp_pListInfo, fp_firstNodeIndex);
            pSecondNode = ArrayList_Get(fp_pListInfo, fp_secondNodeIndex);

            //! If memory error or bounds error.
            if (pFirstNode  == NULL ||
                pSecondNode == NULL)
            {
                retVal = -3;
            }
            else
            {
                //! Copy the first node to the buffer.
                memmove(pBuffer, pFirstNode, fp_pListInfo->dataSize);
                //! Copy the second node to the first node.
                memmove(pFirstNode, pSecondNode, fp_pListInfo->dataSize);
                //! Copy the buffer(first node) to the second node.
                memmove(pSecondNode, pBuffer, fp_pListInfo->dataSize);
            }

            //! Free the allocated memory for the buffer.
            free(pBuffer);
        }
    }

    return retVal;
}

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

/**
 * @brief ArrayList_Add
 * @param fp_pListInfo
 * @param fp_pNewData
 * @return
 */
int ArrayList_Add(tsArrayListInfo *fp_pListInfo,
                  const void      *fp_pNewData)
{
    int  retVal       = 0;
    void *pReallocRet = NULL;

    //! Check arguments for null.
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
            //! If capacity is dynamic reallocate the list.
            if (fp_pListInfo->capacityType == eDynamicCapacity)
            {
                //! Extend the capacity, as it is dynamic.
                fp_pListInfo->totalCapacity *= CAPACITY_EXTEND_RATE;

                //! Reallocate the list memory with the new capacity.
                pReallocRet = realloc(fp_pListInfo->pHead,
                                      (fp_pListInfo->dataSize * fp_pListInfo->totalCapacity));

                //! On error with allocating.
                if (pReallocRet == NULL)
                {
                    retVal = -2;
                }
                else
                {
                    //! Get the new allocated memory.
                    fp_pListInfo->pHead = pReallocRet;
                }
            }
            //! If capacity is static return error, no more space in the list.
            else
            {
                retVal = -3;
            }
        }

        //! Copy only if no errors.
        if (retVal == 0)
        {
            //! Copy the data to the list.
            //!              Cast to char to know that compiler thinks pHead for 1 byte.
            //!              dataSize is how many bytes one node is and multiply by
            //!              the current count to get to the position of the free memory.
            memmove((void *) (((char *) fp_pListInfo->pHead) + (fp_pListInfo->count * fp_pListInfo->dataSize)),
                    fp_pNewData, fp_pListInfo->dataSize);

            //! Increase the count.
            fp_pListInfo->count++;
        }
    }

    return retVal;
}

int   ArrayList_AddLast(tsArrayListInfo *fp_pListInfo,
                        const void      *fp_pNewData);

/**
 * @brief Remove node from the list at specific index.
 *        Put this node at the end of list. Slap with every node after the
 *        removed one.
 * @param fp_pListInfo
 * @param fp_index
 * @return
 */
int ArrayList_RemoveAt(tsArrayListInfo    *fp_pListInfo,
                       const unsigned int fp_index)
{
    int          retVal = 0;
    unsigned int index  = 0;

    if (fp_pListInfo        == NULL ||
        //! For (count - 1) if count is 0 (unsigned int) (0 - 1) = max_int
        fp_pListInfo->count == 0 ||
        //! On bounds error.
        fp_index >= fp_pListInfo->count)
    {
        retVal = -1;
    }
    else
    {
        //! From the removed index to the index of the last element.
        for (index = fp_index; index < (fp_pListInfo->count - 1); index++)
        {
            retVal = fs_swapTwoNodes(fp_pListInfo,
                                     index,
                                     (index + 1));

            //! On swap error break the swapping.
            if (retVal != 0)
            {
                break;
            }
        }

        if (retVal == 0)
        {
            //! Decrease the count on successfull remove.
            fp_pListInfo->count--;
        }
    }

    return retVal;
}

/**
 * @brief Remove all nodes from the list.
 * @param fp_pListInfo    The list info.
 */
void  ArrayList_RemoveAll(tsArrayListInfo *fp_pListInfo)
{
    if (fp_pListInfo != NULL)
    {
        //! Reseting the count will remove all nodes from the list.
        fp_pListInfo->count = 0;
    }
}

/**
 * @brief Get node from the list.
 * @param fp_pListInfo    From which list.
 * @param fp_index        At what index.
 * @return
 *          #NULL          - On error, null parameter or out of bounds access.
 *          #Valid pointer - On success.
 */
void* ArrayList_Get(const tsArrayListInfo *fp_pListInfo,
                    const unsigned int    fp_index)
{
    void *pRetNode = NULL;

     if (fp_pListInfo != NULL &&
         fp_index     <  fp_pListInfo->count)
     {
         pRetNode = (void *) (((char *) fp_pListInfo->pHead) + (fp_index * fp_pListInfo->dataSize));
     }

     return pRetNode;
}

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

/**
 * @brief Sort the whole list.
 * @param fp_pListInfo    From which list.
 * @param fp_pCmpFuncP    
 * @return
 *          #0  - On success.
 *          #-1 - On null parameter.
 *          #-2 - On error allocating memory.
 *          #-3 - ArrayList_Get return error.
 */
int ArrayList_Sort(tsArrayListInfo         *fp_pListInfo,
                   const tCmpNodesFuncP    fp_pCmpFuncP,
                   const teCmpSortingOrder eSortingOrder)
{
    int retVal = 0;
    int endIndex = 0;
    int cmpIndex = 0;
    teCmpFuncRetType eCmpRetVal = eCmpFuncRet_Equal;
    void *pCurrentElement = NULL;
    void *pNextElement = NULL;
    
    if (fp_pListInfo == NULL || fp_pCmpFuncP == NULL)
    {
        retVal = -1;
    }
    else
    {
        //! Iterate through the whole list except the last element and slap.
        for (endIndex = (fp_pListInfo->count - 1); endIndex > 0 ; endIndex--)
        {
            for (cmpIndex = 0; cmpIndex < endIndex; cmpIndex++)
            {
                //! Get the current element form the list.
                pCurrentElement = ArrayList_Get(fp_pListInfo, cmpIndex);
                //! Get the next element form the list.
                pNextElement    = ArrayList_Get(fp_pListInfo, cmpIndex + 1);
                
                //! On error getting the elements from the list.
                if (pCurrentElement == NULL || pNextElement == NULL)
                {
                    retVal = -3;
                    
                    break;
                }
                
                //! Compare two elements next to each other.
                eCmpRetVal = fp_pCmpFuncP(pCurrentElement,
                                          pNextElement);
                                          
                switch (eSortingOrder)
                {
                    //! 
                    case eAscendingOrder:
                        //! If the left element is bigger than the right one, slap is needed.
                        if (eCmpRetVal == eCmpFuncRet_Bigger)
                        {
                            retVal = fs_swapTwoNodes(fp_pListInfo,
                                                     cmpIndex,
                                                     cmpIndex + 1);
                        }
                        
                        break;
                        
                    //! 
                    case eDescendingOrder:
                        //! If the left element is smaller than the right one, slap is needed.
                        if (eCmpRetVal == eCmpFuncRet_Smaller)
                        {
                            retVal = fs_swapTwoNodes(fp_pListInfo,
                                                     cmpIndex,
                                                     cmpIndex + 1);
                        }
                        
                        break;
                        
                }
                
                //! On error with slapping the nodes.
                if (retVal != 0)
                {
                    break;
                }
            }
            
            //! On error.
            if (retVal != 0)
            {
                break;
            }
        }
    }
    
    return retVal;
}


void* ArrayList_Search(const tsArrayListInfo *fp_pListInfo,
                       const tCmpNodesFuncP  fp_pCmpFuncP,
                       const void            *fp_pSearchedData);
int   ArrayList_SortedInsert(tsArrayListInfo      *fp_pListInfo,
                             const tCmpNodesFuncP fp_pCmpFuncP,
                             const void           *fp_pNewData);
