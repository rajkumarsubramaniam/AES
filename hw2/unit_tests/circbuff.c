/**
 * @file circbuff.c
 * @brief This file implements the function to access and control the Circular buffer
 * Circular Buffer allocation, adding item, reading item, status of the buffer are handled.
 * @author Raj Kumar Subramaniam
 * @date Sep 5,2017
 *
 */
#include "stdio.h"
#include "circbuff.h"

CB_Status CirBuffAddItem(CB_t *circBuff, uint32_t *data)
{
	CB_Status status = CirBuffIsFull(circBuff); /*Check full before adding item*/

	if(status != CB_SUCCESS)
		return status;

	*(circBuff->head) = *data;
	circBuff->count++;

	if(circBuff->head == circBuff->buff + (circBuff->size-1)) /*If head is at last element points to first location again*/
	{
		circBuff->head = circBuff->buff;
	}
	else
	{
		circBuff->head++;	/*Increments Head*/
	}
	return CB_SUCCESS;
}

CB_Status CirBuffRemoveItem(CB_t *circBuff, uint32_t *data)
{
	CB_Status status = CirBuffIsEmpty(circBuff); /*Check empty before removing item*/

	if(status != CB_SUCCESS)
		return status;

	*(data) = *(circBuff->tail);
	circBuff->count--;
	if(circBuff->tail == circBuff->buff + (circBuff->size-1)) /*If tail is at last element points to first location again*/
	{
		circBuff->tail = circBuff->buff;
	}
	else
	{
		circBuff->tail++;	/*Increments Head*/
	}
	return CB_SUCCESS;
}

CB_Status CirBuffIsFull(CB_t *circBuff)
{
	if(circBuff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	if(circBuff->buff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}

	if(circBuff->count == circBuff->size)
	{
		return CB_FULL;	/*Checks for Buffer Full*/
	}
	else
	{
		return CB_SUCCESS; /*Buffer not FULL*/
	}
}

CB_Status CirBuffIsEmpty(CB_t *circBuff)
{
	if(circBuff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	if(circBuff->buff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	if(circBuff->count == 0)
	{
		return CB_EMPTY;	/*Checks for Buffer Empty*/
	}
	else
	{
		return CB_SUCCESS; /*Buffer not empty*/
	}
}

CB_Status CirBuffDump(CB_t *circBuff)
{
	uint32_t data;
	CB_Status status = CirBuffIsEmpty(circBuff); /*Check empty before removing item*/

	if(status != CB_SUCCESS)
		return status;

	while(CirBuffRemoveItem(circBuff,&data) == CB_SUCCESS)
	{
		printf("%d ",data);
	}
	return CB_SUCCESS;
}

CB_Status CirBuffInit(CB_t **circBuff, size_t length)
{
	*circBuff = (CB_t*) malloc(sizeof(CB_t));
	if(*circBuff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	/*Memory Allocation*/
	(*circBuff)->buff = (uint32_t*)malloc(length*(sizeof(uint32_t)));
	if((*circBuff)->buff == NULL)
	{
		return CB_ALLOC_FAIL;
	}
	else
	{
		(*circBuff)->head = (*circBuff)->buff;
		(*circBuff)->tail = (*circBuff)->buff;
		(*circBuff)->size = length;
		(*circBuff)->count = 0;
		return CB_SUCCESS;
	}
}

CB_Status CirBuffDestroy(CB_t **circBuff)
{
	if(*circBuff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	if((*circBuff)->buff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	else
	{
		free((void*)(*circBuff)->buff);
		free((void*)(*circBuff));
		return CB_SUCCESS;
	}
}


size_t CirBuffSize(CB_t *circBuff)
{
	if(circBuff == NULL)
	{
		return CB_NULLPTR;	/*Checks for NULL pointer*/
	}
	return circBuff->count;	/*Current elements in the Circular Buffer*/
}
