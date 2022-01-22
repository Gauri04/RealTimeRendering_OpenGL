#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include"vmath.h"
#include"GRStack.h"

using namespace vmath;

// global variable declaration
Node* top = NULL;
mat4 copyMatrix, mulMatrix, topMatrix;
FILE* fp = NULL;



extern "C" bool initLog()
{
	fopen_s(&fp, "GRLogs_Stack.txt", "w");
	if (fp == NULL)
	{
		//printf("\n Error : Logs file cannot be opened. Exiting Now..");
		return(false);
	}

	//fprintf(fp, "**** Logs ****\n");
	return(true);
}

extern "C" void destroyLog()
{
	if (fp)
	{
		//fprintf(fp, "\n ************* Exiting log file ***************");
		fclose(fp);
		fp = NULL;
	}
}

extern "C" Node* CreateNode()
{
	Node* temp = (Node*)malloc(sizeof(Node));
	return(temp);
}

extern "C" int isEmpty()
{
	if (top == NULL)
		return(0);

	return(1);
}

extern "C" mat4 PushToStack(mat4 pushMatrix)
{
	// variable declaration
	Node* temp;
	int i,j;
	mulMatrix = mat4::identity();
	topMatrix = mat4::identity();
	
	temp = CreateNode();

	if (top != NULL)
	{
		topMatrix = top->matrix;
		mulMatrix = topMatrix * pushMatrix;
	}
	else
	{
		mulMatrix = pushMatrix;
	}
	temp->matrix = mulMatrix;
	temp->ptr = top;
	top = temp;

	//fprintf(fp, "\n In push to stack, top : %p, top->matrix[15] : %f", top, top->matrix[0][4]);	

	// return the current matrix which is at top of the stack
	return(mulMatrix);

}

mat4 PopFromStack()
{
	Node* temp;
	mat4 tempMatrix;

	
	if (isEmpty() == 1)
	{
		temp = top;
		tempMatrix = top->matrix;
		top = top->ptr;
		temp->ptr = NULL;
		free(temp);
		//fprintf(fp, "\n\n In pop from stack, popped matrix matrix[0][4] : %f", tempMatrix[0][4]);

		if (isEmpty() == 1)
		{
			return(top->matrix);
		}
		else
		{
			tempMatrix = mat4::identity();
			return(tempMatrix);
		}
		
	}
	else
	{
		//printf("\n Stack empty");
		tempMatrix = mat4::identity();
		return(tempMatrix);
	}

}

mat4 PeekFromStack()
{
	
	if (isEmpty() == 1)
	{
		copyMatrix = top->matrix;
		return(copyMatrix);
	}
	else
	{
		return(mat4::identity());
	}
		
}












