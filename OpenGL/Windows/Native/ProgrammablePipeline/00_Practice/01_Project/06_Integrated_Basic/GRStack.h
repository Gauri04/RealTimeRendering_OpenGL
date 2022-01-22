#pragma once
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include"vmath.h"
using namespace vmath;

// struct declaration
typedef struct Node
{
	mat4 matrix;
	struct Node* ptr;
}
Node;

// function declaration
extern "C" mat4 PushToStack(mat4);
extern "C" mat4 PopFromStack();
extern "C" mat4 PeekFromStack();
extern "C" Node* CreateNode();
extern "C" int isEmpty();
extern "C" bool initLog();
extern "C" void destroyLog();

