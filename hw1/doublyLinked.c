/**
 * @file doublyLinked.c 
 * @brief This file implements the functions to access and control the doubly linked list
 * Adding nodes, removing nodes, search and destroy are some operations performed
 * @author Raj Kumar Subramaniam
 * @date September 5,2017
**/

#include <stdlib.h>
#include "doublyLinked.h"

DL_Status DL_Destroy(DL_t **head)
{
	DL_t* temp;
	while(*head != NULL)
	{
		temp = *head;		/*Store the node in temp variable*/	
		*head = (*head)->next;
		free(temp);	/*Deletes all elements*/
	}
	return DL_SUCCESS;
}

DL_Status DL_AddNode(DL_t **head, uint32_t data, size_t index)
{
	size_t size;
	DL_t* temp = *head;
	
	if(DL_SUCCESS != DL_Size(temp,&size))	/*Size of the list*/
		return DL_ERROR;
		
	//printf("ADDNODE::STARTED:%d\n",size);
	
	if(index > size)	/*If index is over the limit*/
	{
		return DL_INDEX_ERROR;
	}
	else
	{
		DL_t* newNode;
		newNode = (DL_t*)malloc(sizeof(DL_t));	/*Allocate New Node*/
		newNode->data = data;
		/*Determine position to add*/
		for(size_t i=1;i<index;i++)	
		{
			temp=temp->next;
		}
		
		if((temp == NULL) && (index == 0))	/*Case for adding the First Element*/
		{
			temp = newNode;
			temp->next = NULL;
			temp->previous = NULL;
			*head = newNode;
		}
		else if((temp != NULL) && (index == 0)) /*Case for adding as first element*/
		{
			newNode->next = temp;
			newNode->previous = NULL;
			temp->previous = newNode;
			*head = newNode;
		}			
		else if(temp->next == NULL)		/*Case for adding as last Element*/
		{
			temp->next = newNode;
			newNode->previous = temp;
			newNode->next = NULL;
		}
		else if(temp != NULL)	/*Case for adding a Element in between*/
		{
			newNode->previous = temp;
			newNode->next = temp->next;
			temp->next->previous = newNode;
			temp->next = newNode;
		}
	}
	return DL_SUCCESS;
}

DL_Status DL_RemoveNode(DL_t **head, uint32_t *data, size_t index)
{
	size_t size;
	DL_t* temp = *head;
	
	if(DL_SUCCESS != DL_Size(temp,&size))	/*Size of the list*/
		return DL_ERROR;
		
	if(index >= size)	/*If index is over the limit*/
	{
		return DL_INDEX_ERROR;
	}
	else
	{
		for(size_t i=0;i<index;i++)	/*Index Parsing*/
		{
			temp = temp->next;
		}
		
		if(index == 0)	/*case for removing first element*/
		{
			if(temp->next != NULL)
			{
				temp->next->previous = NULL;
				*head = temp->next;
			}
		}
		else if(temp->next != NULL) /*case for removing middle element*/
		{
			temp->next->previous = temp->previous;
			temp->previous->next = temp->next;
		}
		else	/*case for removing lasy element*/
		{
			temp->previous->next = NULL;
		}
		free(temp);	/*Deallocate memory*/
	}
	return DL_SUCCESS;
}

DL_Status DL_Search(DL_t *head, uint32_t data, size_t *index)
{
	*index = 0;
	while(head != NULL)		/*Loops until data is found or NULL encountered*/
	{
		if(head->data == data)	
		{
			return DL_SUCCESS;
		}
		else
		{
			head = head->next;
			*index = *index+1;	/*increases the index*/
		}			
	}
	return DL_ERROR;
}

DL_Status DL_Size(DL_t *head, size_t *size)
{
	*size = 0;
	while(head != NULL)	/*Parse until NULL Enountered*/
	{
		head = head->next;
		*size = *size+1;
	}
	return DL_SUCCESS;
}
