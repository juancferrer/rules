#include<functional>
#include<iostream>
using namespace std;
/* Rules interface */
class IRule{
    public:
        virtual ~IRule(){};
        virtual void process(int value){};
        virtual bool isValid(){};
        virtual string getName(){};
        virtual operator bool() const{};
};

/* Concrete base class for all the rules */
class Rule: public IRule{
    public:
        ~Rule(){};
        Rule(const string &name):valid(valid), name(name){};
        void process(int value){};
        // Shouldn't have to override anything below here
        string getName(){return name;};
        bool isValid(){return valid;};
        operator bool() const{return valid;};
        friend ostream& operator<<(ostream &out, Rule &rule){
            string result = rule.isValid() ? "passed":"failed";
            out << "Rule " << rule.getName() << ' ' << result;
            return out;
        }

    protected:
        string name;
        bool valid;
};


/* Applies a boolean operation to the 2 working values
 * with an optional offset added to the incoming value
 * before performing the boolean operation
 */
template<class T> 
class BooleanRule : public Rule{
    public:
         ~BooleanRule(){};
         BooleanRule(string name, T op, int offset=0):
             Rule(name), op(op), offset(offset){};

        void process(int value){
            if(valid) return;
            if(started){
                // Setup the initial value
                val1 = value;
                started = true;
                return;
            }
            val2 = value;
            //Alwas the new value first (  NEW < OLD, NEW > OLD, etc..)
            if(op(val2+offset, val1)){
                valid = true;
            }
            swap(val1, val2);
        };

    private:
        int val1, val2, offset;
        T op;
        bool started;
};

/* Applies an arithmetic operation to the 2 working values
 * and checks for equality of the operation result with the target value.
 * If the optional boolean operator is given, it will use that operator 
 * instead of the equality operator
 */
template<class T1, class T2 > 
class ArithmeticRule : public Rule{
    public:
        ~ArithmeticRule(){};
        ArithmeticRule(string name, T1 op, int target, T2 optional=NULL):
            Rule(name), op(op), optional(optional), target(target){};

        void process(int value){
            if(valid) return;
            if(!started){
                // Setup the initial value
                val1 = value;
                started = true;
                return;
            }
            val2 = value;
            result = op(val2, val1);
            //If we have the optional bool operator use that against target
            //otherwise, just do an equality check
            if(optional != NULL){
                valid = optional(result, target);
            }
            else if(result == target){
                valid = true;
            }
            swap(val1, val2);
        };

    private:
        string name;
        int val1, val2, target, result;
        T1 op;
        T2 optional;
        bool started;
};

/* Computes the running average and compares for equality to the target value.
 * If the optional boolean operator is given, it will use that operator
 * instead of the equality operator
 */
template<class T> 
class AvgRule : public Rule{
    public:
        ~AvgRule(){};
        AvgRule(string name, int target, T optional=NULL):
            Rule(name), optional(optional), target(target), count(count),
            total(total){};

        void process(int value){
            count++;
            total += value;
            avg = total/count;
            //If we have the optional bool operator use that against target
            //otherwise, just do an equality check
            if(optional != NULL){
                valid = optional(avg, target);
            }
            else{
                valid = (avg == target);
            }
        };
        bool isValid(){return valid;};
        operator bool() const{return valid;};

    private:
        int target, avg, count, total;
        T optional;
};
