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

int main() {
    auto val = unique_ptr<Object>(eval("5")); 
    testIntegerObject(val.get(), 5); 

    auto b = unique_ptr<Object>(eval("true"));
    testBooleanObject(b.get(), true);
    b.reset(eval("false"));
    testBooleanObject(b.get(), false);
    return 0;

}


