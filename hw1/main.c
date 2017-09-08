#include <stdio.h>
#include <stdint.h>
#include "doublyLinked.h"
#include "circbuff.h"
int main()
{
	DL_t *head = NULL;
	DL_t* parser = NULL;
	size_t search;
	uint32_t data;
	printf("main::Add node:%d\n",DL_AddNode(&head,1,0));
	printf("main::Add node:%d\n",DL_AddNode(&head,2,1));
	printf("main::Add node:%d\n",DL_AddNode(&head,3,2));
	printf("main::Add node:%d\n",DL_AddNode(&head,4,3));
	printf("main::Add node:%d\n",DL_AddNode(&head,5,0));
	DL_Search(head,4,&search);
	printf("Found at %ld\n",search);	
	DL_RemoveNode(&head,&data,0);
	DL_RemoveNode(&head,&data,3);
	DL_RemoveNode(&head,&data,1);
	parser = head;
	while(parser != NULL)
	{
		printf("%d ",parser->data);
		parser = parser->next;
	}
	printf("\nCircular Buffer\n");
	DL_Destroy(&head);
	
	CB_t *buffer = NULL;
	CirBuffInit(&buffer,10);
	for(int i=0;i<5;i++)
	{
		CirBuffAddItem(buffer,&i);
	}
	CirBuffDump(buffer);
	
	return 0;
}
