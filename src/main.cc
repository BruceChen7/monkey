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
    cout << "something error" << endl;
    std::for_each(e.begin(), e.end(), [](const string& s) { 
            cout << s;
    });
}

int main() { 
    string s;
    auto env = shared_ptr<Env>(new Env());
    while (true) {
        cout << sPrompt;
        getline(cin, s);
        auto p = unique_ptr<Parser>(new Parser(s)); 
        auto program = unique_ptr<Program>(p->parseProgram()); 

        if(!p->getErrors().empty()) { 
            printErros(p->getErrors());
        } else { 
            auto val = program->eval(env.get());
            
            if( val->type() == "INTEGER" || val->type() == "BOOLEAN") { 
                cout << val->inspect() << "\n";
            }
            // Support Ctrl-D
            if( val->type() == "NULL") {
                break;
            }
        }
    } 
    return 0;
}
