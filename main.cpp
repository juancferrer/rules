#include <iostream>
#include <functional>
#include "Rules.h"
using namespace std;

// Sum of the two working numbers >= to 1000
ArithmeticRule<function<int(int,int)>, function<bool(int,int)>> 
    ruleA = ArithmeticRule<function<int(int,int)>,function<bool(int,int)>>(
            "RuleA", plus<int>(), 1000, greater_equal<int>());

// New number-500 >= last number
BooleanRule<function<bool(int,int)>> 
    ruleB = BooleanRule<function<bool(int,int)>>("RuleB", greater_equal<int>(), -500);

// Average of all the numbers processed is >= 500
AvgRule<function<bool(int,int)>> 
    ruleC = AvgRule<function<bool(int,int)>>("RuleC", 500, greater_equal<int>());

int main(int argc, char *argv[]){
   for(int i=1; i<argc; i++){
        int val = atoi(argv[i]);
        ruleA.process(val);
        ruleB.process(val);
        ruleC.process(val);
    }
    cout << ruleA << '\n' << ruleB << '\n' << ruleC << '\n' ;
    if(ruleA && ruleB && ruleC){
        cout << "ALL rules passed" << '\n';
    }
    return 0;
}
