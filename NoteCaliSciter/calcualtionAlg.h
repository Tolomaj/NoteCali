#include <vector>
#include <string>
#include <cstdio>

// this undefinition is becouse sciter is defining this and exprtk.h dont like that
#undef min
#undef max
#include "exprtk.h"

void trig_function(){
    typedef double T; // numeric type (float, double, mpfr etc...)

    typedef exprtk::symbol_table<T> symbol_table_t;
    typedef exprtk::expression<T>   expression_t;
    typedef exprtk::parser<T>       parser_t;

    std::string expression_string = "x+3 = z - y" ;

    T x = T(10);
    T y = T(2);
    T z = T(5);

    symbol_table_t symbol_table;
    symbol_table.add_variable("x", x);
    symbol_table.add_variable("y", y);
    symbol_table.add_variable("z", z);

    expression_t expression;
    expression.register_symbol_table(symbol_table);
    

    parser_t parser;


    if (!parser.compile(expression_string, expression))
    {
       
        debugLOG(L"Compilation error...");
        return;
    }
   
    T result = expression.value();
    //printf(" %19.15f\t %19.15f\t %19.15f\n", x, y, z);
    debugLOG("papre");
    debugLOG(x);
    debugLOG(y);
    debugLOG(z);
    debugLOG(result);
    return;
}


class MatematicalSolver {
   

public:
    

	void solve(vector<mline>* lines) { // TODO
		trig_function();

	}

    

};
