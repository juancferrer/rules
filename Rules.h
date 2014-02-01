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
        Rule(const string &name){
            this->valid = false;
            this->name = name;
        };
        void process(int value){};
        // Shouldn't have to override anything below here
        string getName(){return this->name;};
        bool isValid(){return this->valid;};
        operator bool() const{return this->valid;};
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
         BooleanRule(string name, T op, int offset=0):Rule(name){
            this->op = op;
            this->offset = offset;
        };

        void process(int value){
            if(this->valid) return;
            if(!this->started){
                // Setup the initial value
                this->val1 = value;
                this->started = true;
                return;
            }
            this->val2 = value;
            //Alwas the new value first (  NEW < OLD, NEW > OLD, etc..)
            if(this->op(this->val2+offset, this->val1)){
                this->valid = true;
            }
            swap(this->val1, this->val2);
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
        ArithmeticRule(string name, T1 op, int target, T2 optional=NULL):Rule(name){
            this->op = op;
            this->optional = optional;
            this->target = target;
        };

        void process(int value){
            if(this->valid) return;
            if(!this->started){
                // Setup the initial value
                this->val1 = value;
                this->started = true;
                return;
            }
            this->val2 = value;
            this->result = this->op(this->val2, this->val1);
            //If we have the optional bool operator use that against target
            //otherwise, just do an equality check
            if(this->optional != NULL){
                this->valid = this->optional(this->result, this->target);
            }
            else if(this->result == this->target){
                this->valid = true;
            }
            swap(this->val1, this->val2);
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
        AvgRule(string name, int target, T optional=NULL):Rule(name){
            this->optional = optional;
            this->target = target;
            this->count = 0;
            this->total = 0;
        };
        void process(int value){
            count++;
            this->total += value;
            this->avg = this->total/count;
            //If we have the optional bool operator use that against target
            //otherwise, just do an equality check
            if(this->optional != NULL){
                this->valid = this->optional(this->avg, this->target);
            }
            else{
                this->valid = (this->avg == this->target);
            }
        };
        bool isValid(){return this->valid;};
        operator bool() const{return this->valid;};

    private:
        int target, avg, count, total;
        T optional;
};
