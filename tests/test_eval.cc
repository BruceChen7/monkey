#include "lex.h"
#include "parser.h"
#include "obj.h"
#include "env.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

static shared_ptr<Object> eval(const string& input) {
    auto p = unique_ptr<Parser>(new Parser(input)); 
    auto program = unique_ptr<Program>(p->parseProgram()); 
    auto env = unique_ptr<Env>(new Env()); 
    return program->eval(env.get());
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

static bool testIfElseStatement() {
    auto if_test = eval("if ( 2 * 5 > 2) { 2 } ");
    assert(if_test->inspect() == "2");
    if_test = eval("if (!true) { 1 }  else { 3 }");
    assert(if_test->inspect() == "3");
    if_test = eval("if (!true) { 1 }");
    assert(if_test->inspect() == "null");
            
}

static void testReturnValue() {
    auto return_test = eval("1; return 2;"); 
    assert(return_test->inspect() == "2");
    return_test = eval("return 1; 2");
    assert(return_test->inspect() == "1");
    return_test = eval("return 2 * 5 * 10; 2");
    assert(return_test->inspect() == "100"); 
}

static bool testErrorHandling() {
    auto error_test = eval("5 + true"); 

    assert(error_test->inspect() == "type mismatch: INTEGER + BOOLEAN");
    error_test = eval("5 + true; 5;");
    assert(error_test->inspect() == "type mismatch: INTEGER + BOOLEAN");
    
    error_test = eval("-true");
    assert(error_test->inspect() == "unknown operator: -BOOLEAN"); 

    error_test = (eval("true + false;"));
    assert(error_test->inspect() == "unknown operator: BOOLEAN + BOOLEAN"); 

    error_test = eval("5; true + false; 5");
    assert(error_test->inspect() == "unknown operator: BOOLEAN + BOOLEAN"); 

    error_test = eval("if ( 10 > 1) {true + false; }");
    assert(error_test->inspect() == "unknown operator: BOOLEAN + BOOLEAN"); 
}

static bool testEvalLetstatement() {
    auto error_test = eval("let a = 1; let b = a;");
}


static void testFunctionObj() {
    auto func = eval("fn(x){ x + 2; };");
    auto pfunc = dynamic_cast<FunctionObj*>(func.get());
    assert(pfunc != nullptr);
    assert((pfunc->params).size() == 1); 
    assert(pfunc->body->toString() == "(x + 2)");
    assert((pfunc->params[0]->toString() == "x"));
}

static void testFuncApplication() {
    auto fn = eval("let identity = fn(x) {x;} identity(5);");
    testIntegerObject(fn.get(), 5);
    fn = eval("let identity = fn(x) { return x; }; identity(5)");
    testIntegerObject(fn.get(), 5);
}


int main() {
    auto val = eval("5"); 
    testIntegerObject(val.get(), 5); 
    
    val = eval("10");
    testIntegerObject(val.get(), 10);

    // bool test
    auto b = eval("true");
    testBooleanObject(b.get(), true);
    b = eval("false");
    testBooleanObject(b.get(), false); 
    testIfElseStatement();

    testReturnValue();
    testErrorHandling();
    testEvalLetstatement();
    testFunctionObj();
    // testFuncApplication();
    return 0; 
}


