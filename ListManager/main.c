#include <stdio.h>
#include "array_list.h"

#define COUNT 10

typedef struct
{
    int i;
    float f;
} shitStruct;

teCmpFuncRetType cmpFunction(const void *fp_pFirstNode,
                             const void *fp_pSecondNode)
{
    shitStruct *pFirstNode  = (shitStruct *) fp_pFirstNode;
    shitStruct *pSecondNode = (shitStruct *) fp_pSecondNode;
    teCmpFuncRetType eCmpRet =  eCmpFuncRet_Equal;
    
    if (pFirstNode != NULL && pSecondNode != NULL)
    {
        if ((((float) pFirstNode->i) + pFirstNode->f) > (((float) pSecondNode->i) + pSecondNode->f))
        {
            eCmpRet = eCmpFuncRet_Bigger;
        }
        else if ((((float) pFirstNode->i) + pFirstNode->f) < (((float) pSecondNode->i) + pSecondNode->f))
        {
            eCmpRet = eCmpFuncRet_Smaller;
        }
    }
    
    return eCmpRet;
}

int main()
{
    tsArrayListInfo info = { 0 };
    int i = 0;
    shitStruct shit = { 0 };
    shitStruct *v = NULL;
    int ret = 0;
    tCmpNodesFuncP pCmpFunk = cmpFunction;

    ret = ArrayList_Init(&info, sizeof(shitStruct), COUNT, eDynamicCapacity);

    if (ret != 0)
    {
        printf("Error Init\n");
    }
    else
    {
        for (i = 0; i < COUNT; i++)
        {
            shit.i = i;
            shit.f = (float) (i + 0.1);
            ret = ArrayList_Add(&info, (void*) &shit);

            if (ret != 0)
            {
                printf("Error adding %d\n", i);
            }
        }


        for (i = 0; i < COUNT; i++)
        {
            v = (shitStruct *) ArrayList_Get(&info, i);

            if (v == NULL)
            {
                printf("v is NULL\n");
            }
            else
            {
                printf("Read %d %f\n", v->i, v->f);
            }
        }

        /*for (i = 0; i < COUNT; i++)
        {
            ret = ArrayList_RemoveAt(&info, 0);
            if (ret != 0)
            {
                printf("Error remove at %d\n", i);
            }
        }*/

        //ArrayList_RemoveAll(&info);

        printf("\n");

        /*for (i = COUNT; i < (COUNT * 2); i++)
        {
            shit.i = i;
            shit.f = (float) (i + 0.1);
            ret = ArrayList_Add(&info, (void*) &shit);

            if (ret != 0)
            {
                printf("Error adding %d\n", i);
            }
        }*/
        
        ArrayList_Sort(&info, pCmpFunk, eDescendingOrder);

        for (i = 0; i < (COUNT); i++)
        {
            v = (shitStruct *) ArrayList_Get(&info, i);

            if (v == NULL)
            {
                printf("v is NULL, %d\n", i);
            }
            else
            {
                printf("Read %d %f\n", v->i, v->f);
            }
        }
    }

    ArrayList_Destroy(&info);


    return 0;
}
