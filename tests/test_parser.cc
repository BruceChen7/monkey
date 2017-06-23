#include "parser.h" 
#include <memory> 
#include <iostream>
using namespace std;

vector<string> tests = { 
    "let x = 1;",
    "let y = 1;",
    "return 1;" 
};

struct TestExpresion {
    string expr;
    string res;
    TestExpresion(const string& e, const string& r): expr(e), res(r) {}
}; 

vector<TestExpresion> testOperationPrecedenceSets  = { 
    { "-a * b", "((-a) * b)"},
    {"!-a", "(!(-a))"} 
};

static void checkParseErros(Parser* p) { 
    static int i = 0;
    auto errors =  p->getErrors();

    i++;
    
    if (errors.empty()) { 
        cout << "test " << i << " are success" << endl;
        return;
    } else { 
        cout << "parser has " << errors.size() << " errors " << endl;

        for(const auto& e : errors) { 
            cout << e;
        }
    }

}

int main() { 

    for(const auto& t : tests) {
       unique_ptr<Parser> p(new Parser(t)); 
       auto program = unique_ptr<Program>(p->parseProgram());
       checkParseErros(p.get());
    }

    cout << "Tests for Operator Precedence" << "\n";

    for(const auto& t : testOperationPrecedenceSets) {
       unique_ptr<Parser> p(new Parser(t.expr)); 
       auto program = unique_ptr<Program>(p->parseProgram()); 
       auto res = program->toString();

       if (res != t.expr) { 
           cout << "expected = " << t.res << " got = " << res << "\n";
       }
    }
}
