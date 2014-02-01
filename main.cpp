#include <iostream>
#include <functional>
#include "Rules.h"
using namespace std;

ArithmeticRule<function<int(int,int)>, function<bool(int,int)>> 
    ruleA = ArithmeticRule<function<int(int,int)>,function<bool(int,int)>>("RuleA", plus<int>(), 1000, greater_equal<int>());
BooleanRule<function<bool(int,int)>> 
    ruleB = BooleanRule<function<bool(int,int)>>("RuleB", greater<int>(), -500);
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
        cout << "ALL rules passed." << '\n';
    }
    return 0;
}
