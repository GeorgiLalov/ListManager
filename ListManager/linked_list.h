#ifndef LINKED_LIST_H
#define LINKED_LIST_H

//! typedef function pointer for freeing the data.
typedef void (*tFreeDataCb)(void *fp_pData);

//! Double pointing list node.
typedef struct listNode
{
    struct listNode *next;
    struct listNode *prev;
    void            *data;
} tsListNode;

typedef struct
{
    tsListNode  *head;
    tsListNode  *tail;
    short       count;
    // The size of the data type.
    short       sizeOfData;
    tFreeDataCb pFreeDataCb;
} tsListInfo;


tsListInfo  ListManager_Init(const short fp_dataSize,
                             const tFreeDataCb fp_FreeDataFunc);
int         ListManager_AddFirst(tsListInfo *fp_pListInfo,
                                 const void *fp_pNewData);
int         ListManager_AddLast(tsListInfo *fp_pListInfo,
                                const void *fp_pNewData);
int         ListManager_Detach(tsListInfo *fp_pListInfo,
                               tsListNode *fp_pDetachNode);
tsListNode* ListManager_GetHead(const tsListInfo *fp_pListInfo);
tsListNode* ListManager_GetTail(const tsListInfo *fp_pListInfo);
void        ListManager_DestroyList(tsListInfo *fp_pListInfo);

#endif // LINKED_LIST_H
