#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_

typedef enum ObjectType {
	Rectangle,
	Cube,
	Node2D,
} ObjectType;

typedef struct Object {
	char* name;
	ObjectType object_type;
	ObjectType* child_comps;
} Object;



#endif // OBJECT_H_