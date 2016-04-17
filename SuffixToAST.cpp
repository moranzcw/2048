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

stack<Node*> astStack;
bool syntaxError = false;

Node* constructAST(list<Node*> suffixList)
{
    Node* curNode;
    for(list<Node*>::iterator it = suffixList.begin();it != suffixList.end();it++)
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
        // return NULL;
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
        cout<<node->val<<" ";
    }
}

int main(int argc, char const *argv[])
{
    list<Node*> suffixList;
    suffixList.push_back(new Node('a',Node::CHARACTER));
    suffixList.push_back(new Node('*',Node::OPERATOR));
    suffixList.push_back(new Node('b',Node::CHARACTER));
    suffixList.push_back(new Node('*',Node::OPERATOR));
    suffixList.push_back(new Node('|',Node::OPERATOR));
    suffixList.push_back(new Node('*',Node::OPERATOR));
    Node* astTree = constructAST(suffixList);
    dfsTree(astTree);
    cout<<endl;
    syntaxError ? cout<<"Syntax Error."<<endl:0;
    return 0;
}
 