#pragma once
#include <iostream>

#define printValue(var) std::cout<<#var<<": "<<var<<std::endl;


#define printCase(c) \
	case c: \
		std::cout << #c << std::endl; \
	break;

#define printObjectType(a) \
		switch (a) {\
			printCase(ObjectType::Box);\
			printCase(ObjectType::Block);\
			printCase(ObjectType::Player);\
		}

