/***********************
* Yoav Berger
* 01
* ass6
***********************/

#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"

//defining tree roots struct
typedef struct Root {
	int key, value;
	struct Root* left;
	struct Root* right;
} Root;

//defining dictonary struct
struct Dictionary {
	Root* treePtr;
};

/******************
* Function Name:initDictionary
* Input:
* Output:Dictionary*
* Function Operation: initiate new dictionary
******************/
Dictionary* initDictionary()
{
	Dictionary* DictionaryPtr = malloc(sizeof(Dictionary));
	if (DictionaryPtr == NULL)
	{
		return NULL;
	}
	DictionaryPtr->treePtr = NULL;
	return DictionaryPtr;
}

/******************
* Function Name:destroyRoots
* Input:Root* rootptr
* Output:void
* Function Operation: freeing the roots of the tree
******************/
void destroyRoots(Root* rootptr)
{
	if (rootptr == NULL)
	{
		return;
	}
	destroyRoots(rootptr->left);
	destroyRoots(rootptr->right);
	free(rootptr);
}

/******************
* Function Name:destroyDictionary
* Input:Root* Dictionary *
* Output: void
* Function Operation: freeing the roots of the tree
******************/
void destroyDictionary(Dictionary* d)
{
	if (d == NULL)
	{
		printf("there is no dictionary to destroy");
		return;
	}
	Root * rootptr = d->treePtr;
	destroyRoots(rootptr);
	free(d);
}

/******************
* Function Name:CreateLeaf
* Input:key, value 
* Output: Root*
* Function Operation: creating specific node of the tree
******************/
Root* CreateLeaf(key, value)
{
	Root* rootptr = malloc(sizeof(Root));
	if (rootptr == NULL)
	{
		return NULL;
	}

	//setting the frame of the node
	rootptr->key = key;
	rootptr->value = value;
	rootptr->left = NULL;
	rootptr->right = NULL;
	return rootptr;
}

/******************
* Function Name:addToTree
* Input:Root* rootptr, int key, int value
* Output: Result
* Function Operation: adds a node to the tree in the right place according to binary tree
******************/
Result addToTree(Root* rootptr, int key, int value)
{
	//checks if we got the same key and just changes the value without adding
	if (rootptr->key == key)
	{
		rootptr->value = value;
		return SUCCESS;
	}

	//checks binary tree rules according to key
	if (rootptr->key > key)
	{

		//in case we have sons on the left go deeper
		if (rootptr->left != NULL)
		{
			return addToTree(rootptr->left, key, value);
		}

		//insers the node and return result
		rootptr->left = CreateLeaf(key, value);
		if (rootptr->left == NULL)
		{
			return MEM_ERROR;
		}
		else
		{
			return SUCCESS;
		}
	}

	//checks binary tree rules according to key
	if (rootptr->key < key)
	{

		//in case we have sons on the right go deeper
		if (rootptr->right != NULL)
		{
			return addToTree(rootptr->right, key, value);
		}

		//insers the node and return result
		rootptr->right = CreateLeaf(key, value);
		if (rootptr->right == NULL)
		{
			return MEM_ERROR;
		}
		else
		{
			return SUCCESS;
		}
	}
}

/******************
* Function Name:putInDictionary
* Input:Dictionary* d, int key, int value
* Output: Result
* Function Operation: lowering from tree ptr to root ptr in order to add a node to the tree
******************/
Result putInDictionary(Dictionary* d, int key, int value)
{

	//in case we have empty tree
	if (d->treePtr == NULL)
	{
		d->treePtr = malloc(sizeof(Root));
		if (d->treePtr == NULL)
		{
			return MEM_ERROR;
		}
		d->treePtr->key = key;
		d->treePtr->value = value;
		d->treePtr->left = NULL;
		d->treePtr->right = NULL;
		return SUCCESS;
	}

	//getting the root ptr of the tree
	Root* rootptr = d->treePtr;
	return addToTree(rootptr, key, value);
}

/******************
* Function Name:sumRoot
* Input:Root* rootptr
* Output: int
* Function Operation: summing the number of nodes inside of the tree
******************/
int sumRoot(Root* rootptr)
{
	if (rootptr == NULL)
	{
		return 0;
	}
	return 1 + sumRoot(rootptr->left) + sumRoot(rootptr->right);
}

/******************
* Function Name:sizeOfDictionary
* Input:Root* Dictionary*
* Output: int
* Function Operation: lowering from dictionary ptr to root ptr in order to calculate the 
					  size of the tree
******************/
int sizeOfDictionary(Dictionary* d)
{
	if (d->treePtr == NULL)
	{
		return 0;
	}
	Root* rootptr = d->treePtr;
	return sumRoot(rootptr);
}

/******************
* Function Name:fitValue
* Input:Root* rootptr, int key
* Output: int
* Function Operation: iterating threw the nodes of the tree in order to find the key
******************/
int fitValue(Root* rootptr, int key)
{
	if (rootptr == NULL)
	{
		return 0;
	}

	//searching the key according to binary tree rules
	if (rootptr->key > key)
	{
		return fitValue(rootptr->left, key);
	}
	if (rootptr->key < key)
	{
		return fitValue(rootptr->right, key);
	}
	return rootptr->value;
}

/******************
* Function Name:getFromDictionary
* Input:Dictionary* d, int key
* Output: int
* Function Operation: lowering from dictionary ptr to root ptr in order to check if key exist
******************/
int getFromDictionary(Dictionary* d, int key)
{
	if (d->treePtr == NULL)
	{
		return 0;
	}
	Root* rootptr = d->treePtr;
	return fitValue(rootptr, key);
}

/******************
* Function Name:printTreeByRoot
* Input:Root* rootptr
* Output: void
* Function Operation: printing the roots of the tree in order
******************/
void printTreeByRoot(Root* rootptr)
{
	if (rootptr == NULL)
	{
		return;
	}
	printTreeByRoot(rootptr->left);
	printf("[%d:%d]", rootptr->key, rootptr->value);
	printTreeByRoot(rootptr->right);
}

/******************
* Function Name:printDictionary
* Input:Dictionary* d
* Output: void
* Function Operation: lowering the tree ptr to root ptr, and prints the {}
******************/
void printDictionary(Dictionary* d)
{
	printf("{");
	if (sizeOfDictionary(d) != 0)
	{
		printTreeByRoot(d->treePtr);
	}
	printf("}");
}

/******************
* Function Name:createDictionaryFromArrays
* Input:int keys[], int values[], int size
* Output: Dictionary*
* Function Operation: takes to arrays and creating new tree
******************/
Dictionary* createDictionaryFromArrays(int keys[], int values[], int size)
{
	Dictionary * newDic = initDictionary();
	Result addStatus;

	//adding the nodes to the new dictionary
	for (int i = 0; i < size; i++)
	{
		addStatus = putInDictionary(newDic, keys[i], values[i]);
		if (addStatus != SUCCESS)
		{
			printf("something went wrong in the adding of the arrays");
			return NULL;
		}
	}
	return newDic;
}

/******************
* Function Name:dismantleDic
* Input:Root * rootptr, int keyArr[], int  valArr[], int key, int lastIndex
* Output: int
* Function Operation: iterating threw tree nodes and insert the data into arrays
					  and ignoring the node we want to delete
******************/
int dismantleDic(Root * rootptr, int keyArr[], int  valArr[], int key, int lastIndex)
{
	if (rootptr == NULL)
	{
		return lastIndex;
	}

	//increasing i in case we are not at null node
	int i = lastIndex + 1;

	//in case we arrived to the key we want to delete decreasing i
	if (rootptr->key == key)
	{
		i = i - 1;
	}
	else
	{
		keyArr[i] = rootptr->key;
		valArr[i] = rootptr->value;
	}

	//recursive calls according to tree rules and in order to keep the iterating value
	lastIndex = dismantleDic(rootptr->left, keyArr, valArr, key, i);
	return dismantleDic(rootptr->right, keyArr, valArr, key, lastIndex);
}

/******************
* Function Name:removeFromDictionary
* Input:Dictionary* d, int key
* Output: Result
* Function Operation: removing node from dictionary according to key,
					  and deals with special cases
******************/
Result removeFromDictionary(Dictionary* d, int key)
{

	int isExist, sizeOfNewDic;
	Root* rootptr = d->treePtr;

	//checking if ptr is not initiate
	if (rootptr == NULL)
	{
		return FAILURE;
	}
	
	//checks if size of dictionary is 0
	if (sizeOfDictionary(d) == 0)
	{
		return FAILURE;
	}

	//checks if there is what to delete
	isExist = fitValue(rootptr, key);
	if (isExist == 0)
	{
		return FAILURE;
	}

	//sizeof the new dic that will be created
	sizeOfNewDic = sumRoot(rootptr) - 1;

	//in case we are deleting tree which have only one node
	if (isExist != 0 && sizeOfDictionary(d) == 1)
	{
		destroyRoots(d->treePtr);
		d->treePtr = NULL;
		return SUCCESS;
	}

	if (sizeOfNewDic > 0)
	{
		//initiate arrays to save the key and the values from the tree
		int* keyArr = (int*)malloc(sizeOfNewDic * sizeof(int));
		if (keyArr == NULL)
		{
			return MEM_ERROR;
		}
		int* valArr = (int*)malloc(sizeOfNewDic * sizeof(int));
		if (valArr == NULL)
		{
			return MEM_ERROR;
		}

		//iterating threw the tree and saving keys and values
		dismantleDic(rootptr, keyArr, valArr, key, -1);

		//destroing the tree
		destroyRoots(rootptr);
		d->treePtr = NULL;

		//creating the new tree without the key we want to remove
		Result newResult;
		for (int i = 0; i < sizeOfNewDic; i++)
		{
			newResult = putInDictionary(d, keyArr[i], valArr[i]);
		}

		//need to free both arrays
		free(keyArr);
		free(valArr);
		return SUCCESS;
	}
	
}