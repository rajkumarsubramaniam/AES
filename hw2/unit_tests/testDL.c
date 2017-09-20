/**
 * @file testDL.c
 * @brief cmocka unit testing of the doubly linked list function
 * @author Raj Kumar Subramaniam
 * @date September 18,2017
 **/
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdint.h>
#include "doublyLinked.h"


void test_DL_Destroy(void **state)
{
    DL_t  * cPtr = (DL_t *)malloc(sizeof(DL_t));    /*Allocating and destroying to check the pointer*/
	DL_Status status;
	status = DL_Destroy(&cPtr);
	assert_int_equal(status,DL_SUCCESS);
	assert_null ((void*)cPtr);
}

void test_DL_AddNodes(void **state)
{
    DL_t* n1 = NULL;
    DL_Status status;
    status = DL_AddNode(&n1, 3, 2);    /*Index not in range*/
    assert_int_equal(status,DL_INDEX_ERROR);   
    status = DL_AddNode(&n1, 3, 0);     /*Index addition*/
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(n1->data, 3);     /*Checking the Data added*/
    status = DL_Destroy(&n1);
    assert_int_equal(status,DL_SUCCESS);
}

void test_DL_AddNodes_middle(void **state)
{
    DL_t* n1 = NULL;
    DL_Status status;  
    status = DL_AddNode(&n1, 3, 0);     /*Index addition*/
    status = DL_AddNode(&n1, 5, 1);  
    status = DL_AddNode(&n1, 7, 2);  
    status = DL_AddNode(&n1, 9, 3);  
    assert_int_equal(status,DL_SUCCESS);
    status = DL_AddNode(&n1, 10, 2);    /*add node in the middle*/
    assert_int_equal(status,DL_SUCCESS);
    for(int i=0;i<2;i++)
    {
        n1 = n1->next;
    }
    assert_int_equal(n1->data,10);
    status = DL_Destroy(&n1);
    assert_int_equal(status,DL_SUCCESS);
}
void test_DL_RemoveNodes(void **state)
{
     DL_t* n1 = NULL;
     DL_Status status;  
     uint32_t data;
     status = DL_AddNode(&n1, 3, 0);     /*Index addition*/
     assert_int_equal(status,DL_SUCCESS);
     status = DL_RemoveNode(&n1,&data,4);    
     assert_int_equal(n1->data, 3);     /*Checking the Data added*/
     assert_int_equal(status,DL_INDEX_ERROR); /*Wrong error*/
     status = DL_RemoveNode(&n1,&data,0); 
     assert_int_equal(status,DL_SUCCESS);
     assert_int_equal(data, 3);     /*Checking the Data removed*/
}

void test_DL_RemoveNodes_empty(void **state)
{
     DL_t* n1 = NULL;
     DL_Status status;  
     uint32_t data;
     status = DL_RemoveNode(&n1,&data,0); /*removing from Empty List*/
     assert_int_equal(status,DL_INDEX_ERROR);
}

void test_DL_RemoveNodes_middle_end(void **state)
{
    DL_t* n1 = NULL;
    DL_Status status;  
    uint32_t data;
    status = DL_AddNode(&n1, 3, 0);     /*Index addition*/
    status = DL_AddNode(&n1, 5, 1);  
    status = DL_AddNode(&n1, 7, 2);  
    status = DL_AddNode(&n1, 9, 3);  
    assert_int_equal(status,DL_SUCCESS); 
    status = DL_RemoveNode(&n1,&data,2);    /*Removing from the middle*/
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(data, 7);     /*Checking the Data removed*/ 
    status = DL_RemoveNode(&n1,&data,2);    /*Removing from the end*/
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(data, 9);     /*Checking the Data removed*/
    status = DL_Destroy(&n1);
    assert_int_equal(status,DL_SUCCESS);
}

void test_DL_Search(void **state)
{
    DL_t* n1 = NULL;
    DL_Status status;  
    size_t index;
    status = DL_AddNode(&n1, 3, 0);     /*Index addition*/
    status = DL_AddNode(&n1, 5, 1);  
    status = DL_AddNode(&n1, 7, 2);  
    status = DL_AddNode(&n1, 9, 3);  
    assert_int_equal(status,DL_SUCCESS);
    /*Search first element*/
    status = DL_Search(n1, 3, &index);
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(index,0);
    /*Search last element*/
    status = DL_Search(n1, 9, &index);
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(index,3);
    /*Search middle element*/
    status = DL_Search(n1, 7, &index);
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(index,2);
    /*Destroy*/
    status = DL_Destroy(&n1);
	assert_int_equal(status,DL_SUCCESS);
}

void test_DL_Size(void **state)
{
    DL_t* n1 = NULL;
    DL_Status status;  
    size_t size;
    status = DL_Size(n1, &size);    /*Size when 0*/
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(size,0);
    status = DL_AddNode(&n1, 3, 0);     /*Element addition*/
    status = DL_AddNode(&n1, 5, 1);  
    status = DL_AddNode(&n1, 7, 2);  
    status = DL_AddNode(&n1, 9, 3);  
    assert_int_equal(status,DL_SUCCESS);
    status = DL_Size(n1, &size);    /*Size when 4*/
    assert_int_equal(status,DL_SUCCESS);
    assert_int_equal(size,4);
    status = DL_Destroy(&n1);
	assert_int_equal(status,DL_SUCCESS);
}

int main(int argc, char **argv)	  /*this is main()*/
{
  const struct CMUnitTest tests[] = {
    /*doublyLinked.c Tests*/
    cmocka_unit_test(test_DL_Destroy),
    cmocka_unit_test(test_DL_AddNodes),
    cmocka_unit_test(test_DL_AddNodes_middle),
    cmocka_unit_test(test_DL_RemoveNodes),
    cmocka_unit_test(test_DL_RemoveNodes_empty),
    cmocka_unit_test(test_DL_RemoveNodes_middle_end),
    cmocka_unit_test(test_DL_Search),
    cmocka_unit_test(test_DL_Size),
  };
  return cmocka_run_group_tests(tests, NULL, NULL);
}
