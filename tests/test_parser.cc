#include "parser.h" 
#include <memory> 
#include <iostream>
using namespace std;

vector<string> tests = { 
    "let x = 1;",
    "let y = 1;",
    "return 1;" 
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
}
