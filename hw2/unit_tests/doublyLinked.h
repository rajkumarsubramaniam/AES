/**
 * @file doublyLinked.h 
 * @brief This file implements the functions to access and control the doubly linked list
 * Adding nodes, removing nodes, search and destroy are some operations performed
 * @author Raj Kumar Subramaniam
 * @date September 5,2017
**/

#ifndef _DOUBLYLINKED_H
#define _DOUBLYLINKED_H

#include <stdint.h>


 /*Struct for the Node*/
typedef struct DL_t
{
	uint32_t data;
	struct DL_t *next;
	struct DL_t *previous;
}DL_t;

 /*Enum for the Error of Linked List*/
typedef enum 
{
	DL_ERROR,
	DL_SUCCESS,
	DL_NULLPTR,
	DL_INDEX_ERROR
}DL_Status;

/**
 * @brief Destroys the elements alloted in the Linked List
 * @param head - Pointer to the head node
 * @return Success/Failure status of the operation
**/
DL_Status DL_Destroy(DL_t **head);


/**
 * @brief Add a node to the Linked List
 * @param head - Pointer to the head node, data to be added, index-location to add node
 * @return Success/Failure status of the operation
**/
DL_Status DL_AddNode(DL_t **head, uint32_t data, size_t index);

/**
 * @brief Remove a node to the Linked List
 * @param head - Pointer to the head node, data to be read, index-location to remove node
 * @return Success/Failure status of the operation
**/
DL_Status DL_RemoveNode(DL_t **head, uint32_t *data, size_t index);

/**
 * @brief Search an element in the Linked List
 * @param head - Pointer to the head node, data to be searched, index-location of node found
 * @return Success/Failure status of the operation
**/
DL_Status DL_Search(DL_t *head, uint32_t data, size_t *index);

/**
 * @brief Finds the size of a Linked List
 * @param head - Pointer to the head node, data to be searched, index-location of node found
 * @return Success/Failure status of the operation
**/
DL_Status DL_Size(DL_t *head, size_t *size);

 #endif