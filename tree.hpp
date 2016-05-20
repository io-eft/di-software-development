#ifndef __TREE_HPP
#define __TREE_HPP

class List;
class NodeInt;

class Tree
{
    struct TreeNode
    {
        NodeInt* content;

        TreeNode* left;
        TreeNode* right;

        TreeNode(NodeInt* e);
        ~TreeNode();
    };
    TreeNode* head;

    void dissolveTree(TreeNode* n);
    void printTreeInternal(TreeNode* n);
    void emptyTreeInternal(List* l, TreeNode* n);
    NodeInt* lookUpNodeInternal(int i, TreeNode* n);
    bool existsInternal(int i, TreeNode* n);
    void clearTreeInternal(TreeNode* n);

    public:
        Tree();
        ~Tree();

        bool isEmpty();
        void insertItem(NodeInt* e);
        void emptyTree(List* l);
        void printTree();

        NodeInt* lookUpNode(int i);
	    bool exists(int i);
	    void clearTree();
	    void removeItem(int id);
	    TreeNode* replaceNode(TreeNode* t);
};

#endif // __TREE_HPP
