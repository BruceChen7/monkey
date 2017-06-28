#include "lex.h"
#include "parser.h"
#include "obj.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

static Object* eval(const string& input) {
    auto p = unique_ptr<Parser>(new Parser(input)); 
    auto program = unique_ptr<Program>(p->parseProgram()); 
    return program->eval();
}

static bool testIntegerObject(Object* i, int64_t val) {
    IntegerObject* pi = dynamic_cast<IntegerObject*>(i);
    assert(pi != nullptr);

    if(pi->val != val) {
        cout <<  "object has wrong value. got = " << pi->val << " want = " << val << endl;
        return false;
    } else {
        return true;
    }
}

static bool testBooleanObject(Object* o, bool t) {
    auto pb = dynamic_cast<BooleanObj*>(o);
    assert(pb != nullptr);

    if(pb->val != t) {
        cout <<  "object has wrong value. got = " << pb->val << " want = " << t << endl;
        return false;
    } else {
        return true;
    }

}

static bool testInfixExpresion(Object* o, int val) {
    
}

static bool testIfElseStatement() {
    auto if_test = unique_ptr<Object>(eval("if ( 2 * 5 > 2) { 2 } ")); 
    assert(if_test->inspect() == "2");
    if_test.reset(eval("if (!true) { 1 }  else { 3 }"));
    assert(if_test->inspect() == "3");
    if_test.reset(eval("if (!true) { 1 }"));
    assert(if_test->inspect() == "null");
            
}

int main() {
    auto val = unique_ptr<Object>(eval("5")); 
    testIntegerObject(val.get(), 5); 
    val.reset(eval("10"));
    testIntegerObject(val.get(), 10);

    // bool test
    auto b = unique_ptr<Object>(eval("true"));
    testBooleanObject(b.get(), true);
    b.reset(eval("false"));
    testBooleanObject(b.get(), false); 
    testIfElseStatement();
    return 0; 
}


