#include "parser.h" 
#include <memory> 
#include <iostream>
#include <cassert>
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
    {"!-a", "(!(-a))"},
    {"a+b+c", "((a + b) + c)"},
    {"a+b-c", "((a + b) - c)"},
    {"a*b*c", "((a * b) * c)"},
    {"a*b/c", "((a * b) / c)"},
    {"a-b/c", "(a - (b / c))"},
    {"a+b*c+d/e-f", "(((a + (b * c)) + (d / e)) - f)"},
    {"3+4; -5 * 5", "(3 + 4)((-5) * 5)"},
    {"5>4==3<4", "((5 > 4) == (3 < 4))"},
    {"5>4!=3<4", "((5 > 4) != (3 < 4))"},
    {"3+4*5==3*1+4*5", "((3 + (4 * 5)) == ((3 * 1) + (4 * 5)))"},
    {"1+(2+3)+4", "((1 + (2 + 3)) + 4)"},
    {"(2+3)*4", "((2 + 3) * 4)"},
    {"!(5+5)", "(!(5 + 5))"},
    {"!(true == false)", "(!(true == false))"},
};


struct TestPrefixExpression { 
    string input;
    string op;
    string value;
    TestPrefixExpression(const string& in, const string& o, const string& v):input(in), op(o), value(v) {}
};

vector<TestPrefixExpression> testPrefixExpressionSet = { 
    {"!5", "!", "5"},
    {"-51", "-", "51"},
    {"!true", "!", "true"},
    {"!false", "!", "false"},
};

struct TestIfExpression {
    string input;
    string if_token;
    string condtion;
    string consequence;
    string alternative; 
    TestIfExpression(const string& in, const string& if_tok, const string& cond, const string& cons, const string& alter=""):
        input(in), if_token(if_tok), condtion(cond), consequence(cons), alternative(alter) { }

    bool operator==(const TestIfExpression& o) { 
        return (input == o.input) && (if_token == o.if_token) 
            && (condtion == o.condtion) && (alternative == o.alternative);
    }
};

vector<TestIfExpression> testIfExpression {
    {"if(x < y)  { x }", "if", "(x < y)", "x", ""},
    {"if(x < y)  { x } else { y }", "if", "(x < y)", "x", "y"},

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

static void testPrefixExpresion() { 
    cout << "Tests for PrefixExpression parsing " << endl;

    for(const auto& t : testPrefixExpressionSet) { 
        unique_ptr<Parser> p(new Parser(t.input)); 
        auto program = unique_ptr<Program>(p->parseProgram());
        checkParseErros(p.get());
        auto s = program->getStatements(); 
        assert(s->size() == 1); 
        ExpressionStatement* pes = dynamic_cast<ExpressionStatement*>(s->front().get());
        auto expr = pes->getExpression(); 
        PrefixExpression* pre_expr = dynamic_cast<PrefixExpression*>(expr); 
        assert(pre_expr->getOP() == t.op); 
        auto operand = pre_expr->getExpression()->toString();

        if(operand != t.value ) { 
           cout << "expect " << t.value  << ", get " << operand << "\n";
        }
    } 
}

static void testIfStatement() {
    cout << "Tests for If statement " << "\n";

    for(const auto& t : testIfExpression) {
       unique_ptr<Parser> p(new Parser(t.input)); 
       checkParseErros(p.get());
       auto program = unique_ptr<Program>(p->parseProgram()); 
       auto s = program->getStatements(); 
       assert(s->size() == 1);
       auto if_expr_statement = dynamic_cast<ExpressionStatement*>(s->front().get());
       assert(if_expr_statement != nullptr);
       IfExpression* if_expr = dynamic_cast<IfExpression*>(if_expr_statement->getExpression()); 
       
       BlockStatement* alter = if_expr->getAlternative(); 
       string alter_str = "";
       if(alter != nullptr) {
            alter_str = alter->toString();
       }

       TestIfExpression test_if_expr {
           t.input,
           if_expr->getToken().value,
           if_expr->getCondition()->toString(),
           if_expr->getConsequence()->toString(), 
           alter_str 
       };
       assert(test_if_expr == t); 
   }
} 


static void testOperationPrecedence() {
    cout << "Tests for Operator Precedence" << "\n";

    int i = 0; 
    for(const auto& t : testOperationPrecedenceSets) {
       unique_ptr<Parser> p(new Parser(t.expr)); 
       auto program = unique_ptr<Program>(p->parseProgram()); 
       auto res = program->toString();

       if (res != t.res) { 
           cout << "expected = " << t.res << ",  got = " << res << "\n"; 
           i++;
       }
       
    } 

    if(i) {
        cout << i << " Tests are failed " << endl; 
    } else {
        cout << "All Operator Precedence tests are passed " << endl;
    } 
}

int main() { 
    cout << "Tests for statement parsing " << endl;

    for(const auto& t : tests) {
       unique_ptr<Parser> p(new Parser(t)); 
       auto program = unique_ptr<Program>(p->parseProgram());
       checkParseErros(p.get());
    } 
    testPrefixExpresion();
    testOperationPrecedence();
    testIfStatement();
    return 0;
}
