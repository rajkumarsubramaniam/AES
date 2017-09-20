/**
 * @file circbuff.h 
 * @brief This file implements the function to access and control the Circular buffer
 * Circular Buffer allocation, adding item, reading item, status of the buffer are handled.
 * @author Raj Kumar Subramaniam
 * @date September 5,2017
 **/

#ifndef _CIRCBUFF_H
#define _CIRCBUFF_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
 /*Struct to control the circular buffer*/
typedef struct
{
	uint32_t *buff;
	uint32_t *head;
	uint32_t *tail;
	size_t size;
	size_t count;
}CB_t;

 /*Enum for the status of the buffer*/
typedef enum 
{
	CB_FULL,
	CB_EMPTY,
	CB_SUCCESS,
	CB_NULLPTR,
	CB_SIZE_ERROR,
	CB_ALLOC_FAIL
}CB_Status;

/**
 * @brief Adds an item to the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure, data - Pointer to add data
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffAddItem(CB_t *cirBuff, uint32_t *data);

/**
 * @brief Removes/Reads an item to the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure, data - Pointer to store data read
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffRemoveItem(CB_t *cirBuff, uint32_t *data);

/**
 * @brief Checks the circular buffer status for FULL
 * 
 * @param CirBuff - Pointer to the buffer structure
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffIsFull(CB_t *cirBuff);

/**
 * @brief Prints the contents of the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffIsEmpty(CB_t *cirBuff);

/**
 * @brief Peeks for an item in the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffDump(CB_t *cirBuff);

/**
 * @brief Creates and Initializes the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure, length - size to be alloted
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffInit(CB_t **cirBuff, size_t length);

/**
 * @brief Deallocates and removes the circular buffer
 * 
 * @param CirBuff - Pointer to the buffer structure
 * @return Success/Failure status of the operation
 */
CB_Status CirBuffDestroy(CB_t **cirBuff);

/**
 * @brief Finding the current number of elements
 * 
 * @param CirBuff - Pointer to the buffer structure
 * @return Size of the current number of elements.
 */
size_t CirBuffSize(CB_t *cirBuff);
#endif
