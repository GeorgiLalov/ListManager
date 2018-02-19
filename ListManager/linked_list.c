#include <stdlib.h>
#include <string.h>

#include "linked_list.h"

/**
 * @brief Initialize the list info structure that will hold the
 *        list information.
 * @param fp_dataSize        What is the data size.
 * @param fp_FreeDataFunc    What is the data destructor.
 * @return    Return the reset list info struct.
 */
tsListInfo ListManager_Init(const short       fp_dataSize,
                            const tFreeDataCb fp_FreeDataFunc)
{
    tsListInfo sListInfo =
    {
        NULL,                        /*head of the list*/
        NULL,                        /*tail of the list*/
        (short) 0,                   /*count of the list*/
        (short) fp_dataSize,         /*size of data type*/
        fp_FreeDataFunc,             /*free data function pointer*/
    };

    return sListInfo;
}

/**
 * @brief Adding the node to the head of the list.
 * @param fp_pListInfo    Where to add it.
 * @param fp_pNewData     What to add.
 * @return #0    Node is added successfully.
 *         #1    There is a null parameter.
 *         #4    Malloc have trouble allocating memory.
 */
int ListManager_AddFirst(tsListInfo *fp_pListInfo,
                         const void *fp_pNewData)
{
    tsListNode *pNewNode    = NULL;
    int        returnStatus = -1;
    short      dataSize     = (short) 0;
    void       *pNewData    = NULL;

    if (fp_pListInfo == NULL)
    {
        returnStatus = 1;
    }
    else if (fp_pNewData == NULL)
    {
        returnStatus = 1;
    }
    else
    {
        //! Get the size of the new node.
        dataSize = fp_pListInfo->sizeOfData;
        //! Allocate memory for the new data.
        pNewData = malloc(dataSize);
        //! Allocate memory for the new node.
        pNewNode = (tsListNode*) malloc(sizeof(tsListNode));

        //! If memory error.
        if (pNewData == NULL ||
            pNewNode == NULL)
        {
            returnStatus = 4;
        }
        else
        {
            //! Copy the data to the new allocated memory.
            memmove(pNewData,
                    fp_pNewData,
                    dataSize);

            //! Init the new node.
            pNewNode->data = pNewData;
            pNewNode->next = NULL;
            pNewNode->prev = NULL;

            //! If no elements are in the list.
            if (fp_pListInfo->head == NULL &&
                fp_pListInfo->count == (short) 0)
            {
                fp_pListInfo->head = pNewNode;
                fp_pListInfo->tail = pNewNode;
            }
            else
            {
                pNewNode->next           = fp_pListInfo->head;
                fp_pListInfo->head->prev = pNewNode;
                fp_pListInfo->head       = pNewNode;
            }

            //! Increment the count.
            fp_pListInfo->count++;

            returnStatus = 0;
        }
    }

    return returnStatus;
}

/**
 * @brief Adding the node to the tail of the list.
 * @param fp_pListInfo    Where to add it.
 * @param fp_pNewData     What to add.
 * @return #0    Node is added successfully.
 *         #1    There is a null parameter.
 *         #4    Malloc have trouble allocating memory.
 */
int ListManager_AddLast(tsListInfo *fp_pListInfo,
                        const void *fp_pNewData)
{
    tsListNode *pNewNode    = NULL;
    int        returnStatus = -1;
    short      dataSize     = (short) 0;
    void       *pNewData    = NULL;

    if (fp_pListInfo == NULL)
    {
        returnStatus = 1;
    }
    else if (fp_pNewData == NULL)
    {
        returnStatus = 1;
    }
    else
    {
        //! Get the size of the new node.
        dataSize = fp_pListInfo->sizeOfData;
        //! Allocate memory for the new data.
        pNewData = malloc(dataSize);
        //! Allocate memory for the new node.
        pNewNode = (tsListNode*) malloc(sizeof(tsListNode));

        //! If memory error.
        if (pNewData == NULL ||
            pNewNode == NULL)
        {
            returnStatus = 4;
        }
        else
        {
            //! Copy the data to the new allocated memory.
            memmove(pNewData,
                    fp_pNewData,
                    dataSize);

            //! Init the new node.
            pNewNode->data = pNewData;
            pNewNode->next = NULL;
            pNewNode->prev = NULL;

            //! If no elements are in the list.
            if (fp_pListInfo->tail == NULL &&
                fp_pListInfo->count == (short) 0)
            {
                fp_pListInfo->head = pNewNode;
                fp_pListInfo->tail = pNewNode;
            }
            else
            {
                pNewNode->prev           = fp_pListInfo->tail;
                fp_pListInfo->tail->next = pNewNode;
                fp_pListInfo->tail       = pNewNode;
            }

            //! Increment the count.
            fp_pListInfo->count++;

            returnStatus = 0;
        }
    }

    return returnStatus;
}

/**
 * @brief Detach a node from the list.
 * @param fp_pListInfo      The list from where to detach.
 * @param fp_pDetachNode    The node to detach.
 * @return #0   Node is detached successfully.
 *         #1   There is a null parameter.
 */
int ListManager_Detach(tsListInfo *fp_pListInfo,
                       tsListNode *fp_pDetachNode)
{
    int returnStatus = -1;

    if (fp_pListInfo == NULL)
    {
        returnStatus = 1;
    }
    else if (fp_pDetachNode == NULL)
    {
        returnStatus = 1;
    }
    else if (fp_pListInfo->count == (short) 0)
    {
        // TODO: Maybe something else???
        returnStatus = 1;
    }
    else
    {
        //! If this is the last node.
        if (fp_pListInfo->count == (short) 1)
        {
            fp_pListInfo->head = NULL;
            fp_pListInfo->tail = NULL;
        }
        //! If the node is in the tail of the list.
        else if (fp_pDetachNode->next == NULL)
        {
            fp_pListInfo->tail       = fp_pDetachNode->prev;
            fp_pListInfo->tail->next = NULL;
        }
        //! If the node is in the head of the list.
        else if (fp_pDetachNode->prev == NULL)
        {
            fp_pListInfo->head       = fp_pDetachNode->next;
            fp_pListInfo->head->prev = NULL;
        }
        //! If the node is in the middle of the list.
        else
        {
            fp_pDetachNode->prev->next = fp_pDetachNode->next;
            fp_pDetachNode->next->prev = fp_pDetachNode->prev;
        }

        //! If there is a free data function pointer.
        if (fp_pListInfo->pFreeDataCb != NULL)
        {
            //! Call it to free the data.
            fp_pListInfo->pFreeDataCb(fp_pDetachNode->data);
        }

        //! Free the node.
        free(fp_pDetachNode->data);
        free(fp_pDetachNode);

        //! Decrease the count.
        fp_pListInfo->count--;

        returnStatus = 0;
    }

    return returnStatus;
}

/**
 * @brief Get the head of the list.
 * @param fp_pListInfo    The list info.
 * @return The head of the list.
 */
tsListNode* ListManager_GetHead(const tsListInfo *fp_pListInfo)
{
    tsListNode *pReturnNode = NULL;

    if (fp_pListInfo != NULL)
    {
        pReturnNode = fp_pListInfo->head;
    }

    return pReturnNode;
}

/**
 * @brief Get the tail of the list.
 * @param fp_pListInfo    The list info.
 * @return The tail of the list.
 */
tsListNode* ListManager_GetTail(const tsListInfo *fp_pListInfo)
{
    tsListNode *pReturnNode = NULL;

    if (fp_pListInfo != NULL)
    {
        pReturnNode = fp_pListInfo->tail;
    }

    return pReturnNode;
}

/**
 * @brief Free all the nodes in the list
 * @param fp_pListInfo    The list info that needs to be destroyed.
 */
void ListManager_DestroyList(tsListInfo *fp_pListInfo)
{
    tsListNode *pListIter     = NULL;
    tsListNode *pListIterNext = NULL;

    // Get the head of the pool.
    pListIter = ListManager_GetHead(fp_pListInfo);

    while(pListIter != NULL)
    {
        // Get the next node.
        pListIterNext = pListIter->next;

        // Detach this node.
        ListManager_Detach(fp_pListInfo,
                           pListIter);

        // Go to the next node.
        pListIter = pListIterNext;
    }
}

