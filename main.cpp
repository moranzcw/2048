#include <iostream>
#include <stack>
#include <list>
using namespace std;

struct Node
{
    enum ValueType
    {
        CHARACTER,
        OPERATOR
    };
    ValueType valType;
    char val;
    Node *left;
    Node *right;
    Node(char x,ValueType y) : val(x), valType(y), left(NULL), right(NULL) {}
};

stack<Node*> operatorStack;
stack<Node*> astStack;

bool parenthesesMismatchError = false;
bool syntaxError = false;

int priority(const char &c)
{
    switch(c)
    {
        case '(':
        case ')':       
            return 4;
        case '*':
            return 3;
        case '-':
            return 2;
        case '|':
            return 1;
    }
    return 0;
}

/* Using in determining the input regex string */
bool isCharactor(char &c)
{
    if(c != '(' && c != ')' && c != '|' && c != '*')
        return true;
    return false;
}

bool isOperator(char &c)
{
    /* The '*' is a unary operator , so treat it as a charactor. */
    if(c == '(' || c == ')' || c == '|' || c == '*')
        return true;
    return false;
}

void releaseUntilLower(list<Node*> &suffixList, stack<Node*> &operatorStack, int _priority)
{
    while( !operatorStack.empty()
        && (operatorStack.top()->val != '(')
        && priority(operatorStack.top()->val) >= _priority)
    {
        suffixList.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

void releaseUntilLeftPar(list<Node*> &suffixList, stack<Node*> &operatorStack)
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

bool needConnector(char lastChar, char curChar)
{
    if(isCharactor(curChar) || curChar == '(')
    { 
        if(isCharactor(lastChar) || lastChar == ')' || lastChar == '*')
            return true;
    }
    return false;
}

void infixToSuffix(const string &inffix, list<Node*> &suffixList)
{
    char lastChar = '|';
    char curChar;
    for(int i=0;i<inffix.size();i++)
    {
        /* Deel with the connect operator. */
        curChar = inffix[i];
        if(needConnector(lastChar,curChar))
        {
            curChar = '-';
            releaseUntilLower(suffixList,operatorStack,priority(curChar));
            operatorStack.push(new Node(curChar,Node::OPERATOR));
        }

        /* Deel with the input. */
        curChar = inffix[i];
        if(isOperator(curChar)) /* Operator. */
        {
            if(curChar == ')')  /* Operator ')'. */
                releaseUntilLeftPar(suffixList,operatorStack);
            else if(curChar == '*')  /* operator '*' is a single operator. */
                suffixList.push_back(new Node(curChar,Node::OPERATOR));
            else  /* If current char is an operator. */
            {
                releaseUntilLower(suffixList,operatorStack,priority(curChar));
                operatorStack.push(new Node(curChar,Node::OPERATOR));
            }
        }
        else /* If current char is a charactor, put the current char into the list.*/
            suffixList.push_back(new Node(curChar,Node::CHARACTER));

        lastChar = curChar;
    }

    while( !operatorStack.empty() )
    {
        if(operatorStack.top()->val == '(')
            parenthesesMismatchError = true;
        else
            suffixList.push_back(operatorStack.top());
        operatorStack.pop();
    }
}

Node* constructAST(const list<Node*> &suffixList)
{
    Node* curNode;
    for(list<Node*>::const_iterator it = suffixList.begin();it != suffixList.end();it++)
    {
        curNode = (*it);
        if( curNode->valType == Node::CHARACTER )
            astStack.push(curNode);
        else
        {
            /* Binary operator.*/
            if(curNode->val == '|' || curNode->val == '-')
            {
                if(!astStack.empty())
                {
                    curNode->right = astStack.top();
                    astStack.pop();
                }
                else
                    syntaxError = true;

                if(!astStack.empty())
                {
                    curNode->left = astStack.top();
                    astStack.pop();
                }
                else
                    syntaxError = true;

                astStack.push(curNode);
            }
            /* Single operator.*/
            else
            {
                if(!astStack.empty())
                {
                    curNode->left = astStack.top();
                    astStack.pop();
                }
                else
                    syntaxError = true;

                astStack.push(curNode);
            }
        }
    }

    if(astStack.size() != 1)
    {
        syntaxError = true;
        return NULL;
    }

    return astStack.top();
}


void dfsTree(Node* node)
{
    if(node == NULL)
        return;
    else
    {
        dfsTree(node->left);
        dfsTree(node->right);
        cout<<node->val;
    }
}

int main(int argc, char const *argv[])
{
    string inffix("(a*|b*)*");
    list<Node*> suffixList;
    infixToSuffix(inffix,suffixList);
    Node* astTree = constructAST(suffixList);

    /*Print Suffix*/
    cout<<"Infix  :  "<<inffix<<endl;
    list<Node*>::iterator it = suffixList.begin();
    cout<<"Suffix :  ";
    while(it != suffixList.end())
    {
        cout<<(*it)->val;
        it++;
    }
    cout<<endl;
    parenthesesMismatchError?cout<<"Parentheses Mismatch Error. "<<endl : 0;

    /*Print AST*/
    cout<<"AST    :  ";
    dfsTree(astTree);
    cout<<endl;
    syntaxError ? cout<<"Syntax Error."<<endl:0;

    return 0;
}
