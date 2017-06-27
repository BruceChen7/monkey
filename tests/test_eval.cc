#include "lex.h"
#include "parser.h"
#include "obj.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

static Object* eval(const string& input) {
    auto p = unique_ptr<Parser>(new Parser(input)); 
    auto program = p->parseProgram(); 
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

int main() {
    Object* val = eval("5"); 
    testIntegerObject(val, 5);
    return 0;
}


