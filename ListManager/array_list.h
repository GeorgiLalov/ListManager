/**
*   Define API for using Array type of list.
*
*   Created 27.06.2017 by G.Lalov
*/

#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

//! Define the capacity type of the list.
typedef enum
{
    //! If set the total capacity of the list will be fixed.
    eStaticCapacity  = 0,
    //! If set the total capacity of the list will be flexible.
    eDynamicCapacity = 1
} teCapacityType;

//! Define the compare function return type.
typedef enum
{
    //! Return when the first arg is smaller than the second.
    eCmpFuncRet_Smaller = -1,
    //! Return when the first and the second args are equal.
    eCmpFuncRet_Equal   = 0,
    //! Return when the first arg is bigger than the second.
    eCmpFuncRet_Bigger  = 1
} teCmpFuncRetType;

//! Define the struct holding information for the list.
typedef struct arrayListInfo
{
    unsigned int   count;
    unsigned int   totalCapacity;
    teCapacityType capacityType;
    unsigned int   dataSize;
    //! Pointing to the begining of the list.
    void           *pHead;
} tsArrayListInfo;

//! Define the function type of the function which will compare the nodes.
typedef teCmpFuncRetType (*tCmpNodesFuncP)(const void *fp_pFirstNode,
                                           const void *fp_pSecondNode);

int   ArrayList_Init(tsArrayListInfo      *fp_pListInfoDest,
                     const unsigned int   fp_dataSize,
                     const unsigned int   fp_capacity,
                     const teCapacityType fp_eCapacityType);
int   ArrayList_Add(tsArrayListInfo *fp_pListInfo,
                    const void      *fp_pNewData);
int   ArrayList_AddLast(tsArrayListInfo *fp_pListInfo,
                        const void      *fp_pNewData);
int   ArrayList_RemoveAt(tsArrayListInfo    *fp_pListInfo,
                         const unsigned int fp_index);
void* ArrayList_Get(const tsArrayListInfo *fp_pListInfo,
                    const unsigned int    fp_index);
void  ArrayList_Destroy(tsArrayListInfo *fp_pListInfo);
int   ArrayList_Sort(tsArrayListInfo      *fp_pListInfo,
                     const tCmpNodesFuncP *fp_pCmpFuncP);
void* ArrayList_Search(const tsArrayListInfo *fp_pListInfo,
                       const tCmpNodesFuncP  *fp_pCmpFuncP,
                       const void            *fp_pSearchedData);
int   ArrayList_SortedInsert(tsArrayListInfo      *fp_pListInfo,
                             const tCmpNodesFuncP *fp_pCmpFuncP,
                             const void           *fp_pNewData);

#endif // ARRAY_LIST_H
