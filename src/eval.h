#ifndef __EVAL_H__
#define __EVAL_H__
#include "obj.h"

const Object* evalPrefixExpression(const string& op, Object* right);
const Object* evalBangOperatorExpression(Object* right);
const Object* evalMinusPrefixExpression(Object* right);
const Object* evalInprefixExpression(Object* left, Object* right, const string& op);


#endif
