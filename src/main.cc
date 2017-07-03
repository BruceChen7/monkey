#include "lex.h" 
#include "parser.h"
#include "ast.h"
#include "obj.h"
#include <algorithm>
#include <memory>
#include <iostream>
using namespace std;

const static string sPrompt = ">>> ";
static void printErros(const vector<string>& e) {
    std::for_each(e.begin(), e.end(), [](const string& s) { 
            cout << s;
    });
}

int main() { 
    string s;
    // FixMe momory leak;
    auto env = unique_ptr<Env>(new Env());
    while (true) {
        cout << sPrompt;
        getline(cin, s);
        auto p = unique_ptr<Parser>(new Parser(s)); 
        auto program = unique_ptr<Program>(p->parseProgram()); 

        if(!p->getErrors().empty()) { 
            printErros(p->getErrors());
        } else { 
            auto val = program->eval(env.get());
            cout << val->inspect() << "\n";
        }
    } 
    return 0;

}
