#include <iostream>
#include <stack>
#include <list>
using namespace std;

struct Node
{
    enum ValueType
    {
        CHARACTER,
        SINGLE_OPERATOR,
        BINARY_OPERATOR,
        OTHER_OPERATOR
    };
    ValueType valType;
    char val;
    Node(char x,ValueType y) : val(x), valType(y) {}
};

list<Node*> suffixList;
stack<Node*> operatorStack;
bool parenthesesMismatchError = false;

int priorityList[256] = {0};

void initPriority()
{
    priorityList['('] = priorityList[')'] = 4;
    priorityList['*'] = priorityList['+'] = priorityList['?'] = 3;
    priorityList['-'] = priorityList[')'] = 2;
    priorityList['|'] = priorityList[')'] = 1;
}

bool isSingleOperator(char &c)
{
    if(c == '*' || c == '+' || c == '?')
        return true;
    return false;
}

bool isBinaryOperator(char &c)
{
    if(c == '|' || c == '\\')
        return true;
    return false;
}

bool isOtherOperator(char &c)
{
    if(c == '(' || c == ')')
        return true;
    return false;
}

bool isCharactor(char &c)
{
    if(!isSingleOperator(c) && !isBinaryOperator(c) && !isOtherOperator(c))
        return true;
    return false;
}

void releaseUntilLower(int _priority)
{
    while( !operatorStack.empty()
        && (operatorStack.top()->val != '(')
        && priorityList[operatorStack.top()->val] >= _priority)
    {
        suffixList.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

void releaseUntilLeftPar()
{
    while( !operatorStack.empty() 
        && operatorStack.top()->val != '(')
    {
        suffixList.push_back(operatorStack.top());
        operatorStack.pop();
    }
    if(!operatorStack.empty() 
        && operatorStack.top()->val == '(')
        operatorStack.pop();
    else
        parenthesesMismatchError = true;
}

bool needConnector(Node* lastNode, char curChar)
{
    if(lastNode == NULL)
        return false;

    if( lastNode->valType == Node::CHARACTER 
        || lastNode->valType == Node::SINGLE_OPERATOR 
        || (lastNode->valType == Node::OTHER_OPERATOR && lastNode->val == ')') )
    {
        if( isCharactor(curChar) || curChar == '(' )
            return true;
    }
    return false;
}

list<Node*>& infixToSuffix(const string &regex)
{
    initPriority();

    Node* lastNode;
    char curChar;
    for(int i=0;i<regex.size();i++)
    {
        /* Deel with the connect operator. */
        curChar = regex[i];
        if(needConnector(lastNode,curChar))
        {
            curChar = '-';
            releaseUntilLower(priorityList[curChar]);
            operatorStack.push(new Node(curChar,Node::BINARY_OPERATOR));
        }

        /* Deel with the connect input. */
        curChar = regex[i];
        if(isSingleOperator(curChar))
        {
            suffixList.push_back(new Node(curChar,Node::SINGLE_OPERATOR));
            lastNode = suffixList.back();
        }
        else if(isBinaryOperator(curChar))
        {
            releaseUntilLower(priorityList[curChar]);
            operatorStack.push(new Node(curChar,Node::BINARY_OPERATOR));
            lastNode = operatorStack.top();
        }
        else if(isOtherOperator(curChar))
        {
            if(curChar == ')')  /* If current char is an operator ')'. */
                releaseUntilLeftPar();
            else
            {
                operatorStack.push(new Node(curChar,Node::OTHER_OPERATOR));
                lastNode = operatorStack.top();
            }
        }
        else
        {
            suffixList.push_back(new Node(curChar,Node::CHARACTER));
            lastNode = suffixList.back();
        }
    }

    while( !operatorStack.empty() )
    {
        if(operatorStack.top()->val == '(')
            parenthesesMismatchError = true;
        else
            suffixList.push_back(operatorStack.top());
        operatorStack.pop();
    }
    return suffixList;
}

int main(int argc, char const *argv[])
{
    string regex("(a+|b?)*");
    // string regex("(abc+|b?)*");
    cout<<"Infix  :  "<<regex<<endl;
    list<Node*> result(infixToSuffix(regex));
    list<Node*>::iterator it = result.begin();
    cout<<"Suffix :  ";
    while(it != result.end())
    {
        cout<<(*it)->val;
        it++;
    }
    cout<<endl;
    parenthesesMismatchError?cout<<"Parentheses Mismatch Error. "<<endl : 0;
    return 0;
}
