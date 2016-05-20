#include "tree.hpp"
#include "list.hpp"

Tree::TreeNode::TreeNode(NodeInt* e): content(e), left(NULL), right(NULL) {}

Tree::TreeNode::~TreeNode()
{
    if(content != NULL)
        delete content;
}

Tree::Tree(): head(NULL) {}

Tree::~Tree()
{
    if(!isEmpty())
        dissolveTree(head);
}

void Tree::dissolveTree(TreeNode* n)
{
    if(n->left != NULL)
        dissolveTree(n->left);
    if(n->right != NULL)
        dissolveTree(n->right);
    delete n;
}

bool Tree::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void Tree::insertItem(NodeInt* e)
{
    TreeNode* n = new TreeNode(e);
    if(isEmpty())
        head = n;
    else
    {
        TreeNode* temp = head;
        TreeNode* target = temp;
        while(temp != NULL)
        {
            target = temp;
            if(e->getId() < temp->content->getId())
                temp = temp->left;
            else
                temp = temp->right;
        }
        if(e->getId() < target->content->getId())
            target->left = n;
        else
            target->right = n;
    }
}

void Tree::printTree()
{
    if(!isEmpty())
    {
        printTreeInternal(head);
    }
    else
        std::cout << "Tree is empty" << std::endl;
}

void Tree::printTreeInternal(TreeNode* n)
{
    n->content->print();
    if(n->left != NULL){std::cout << "My Left Kid: ";
        printTreeInternal(n->left);}
    if(n->right != NULL){std::cout << "My Right Kid: ";
        printTreeInternal(n->right);}
}

void Tree::emptyTree(List* l)
{
    if(!isEmpty())
        emptyTreeInternal(l, head);
    head = NULL;
}

void Tree::emptyTreeInternal(List* l, TreeNode* n)
{
    if(n->left != NULL)
        emptyTreeInternal(l, n->left);
    if(n->right != NULL)
        emptyTreeInternal(l, n->right);
    l->insertItem(n->content);
    n->content = NULL;
    delete n;
}

NodeInt* Tree::lookUpNode(int i)
{
    return lookUpNodeInternal(i, head);
}

NodeInt* Tree::lookUpNodeInternal(int i, TreeNode* n)
{
    if(n == NULL)
        return NULL;
    if(i == n->content->getId())
        return n->content;
    if(i < n->content->getId())
        return lookUpNodeInternal(i, n->left);
    return lookUpNodeInternal(i, n->right);
}

bool Tree::exists(int i)
{
    return existsInternal(i, head);
}

bool Tree::existsInternal(int i, TreeNode* n)
{
    if(n == NULL)
        return false;
    if(i == n->content->getId())
        return true;
    if(i < n->content->getId())
        return existsInternal(i, n->left);
    return existsInternal(i, n->right);
}

void Tree::clearTreeInternal(TreeNode* n)
{
    n->content = NULL;
    if(n->left != NULL)
        clearTreeInternal(n->left);
    if(n->right != NULL)
        clearTreeInternal(n->right);
}

void Tree::clearTree()
{
    if(head != NULL)
        clearTreeInternal(head);
}

void Tree::removeItem(int id)
{
    if(head == NULL)
        return;

    if(head->content->getId() == id)
    {
        head = replaceNode(head);
        return;
    }

    TreeNode* parent = head;
    TreeNode* tbd;
    bool isLeft = false;

    if(id > parent->content->getId())
        tbd = head->right;
    else{
        tbd = head->left;
        isLeft = true;
    }

    while(tbd != NULL && tbd->content->getId() != id)
    {
        parent = tbd;
        if(id > tbd->content->getId())
        {
            tbd = tbd->right;
            isLeft = false;
        }
        else
        {
            isLeft = true;
            tbd = tbd->left;
        }
    }
    if(tbd == NULL)
        return;

    if(tbd->left == NULL)
    {
        if(isLeft)
            parent->left = tbd->right;
        else
            parent->right = tbd->right;
        
        delete tbd;
        return;
    }
    if(tbd->right == NULL)
    {
        if(isLeft)
            parent->left = tbd->left;
        else
            parent->right = tbd->left;
        
        delete tbd;
        return;
    }

    if(isLeft)
        parent->left = replaceNode(tbd);
    else
        parent->right = replaceNode(tbd);

}

Tree::TreeNode* Tree::replaceNode(TreeNode* t)
{
    if(t->left == NULL)
    {
        TreeNode* temp = t->right;
        delete t;
        return temp;
    }
    TreeNode* temp = t->left;
    if(temp->right == NULL)
    {
        temp->right = t->right;
        delete t;
        return temp;
    }
    TreeNode* parent;
    while(temp->right != NULL)
    {
        parent = temp;
        temp = temp->right;
    }
    parent->right = temp->left;
    temp->left = t->left;
    temp->right = t->right;
    delete t;
    return temp;
}




