/*
Please note that it's Function problem i.e.
you need to write your solution in the form of Function(s) only.
Driver Code to call/invoke your function would be added by GfG's Online Judge.*/


/* The structure of the Node is
struct Node
{
    int data;
    Node *left;
    Node *right;
    int height;
};
*/

Node* right_rotate(Node* parent)
{
    Node* child = parent->left;
    parent->left = child->right;
    child->right = parent;
    return child;
}

Node* left_rotate(Node* parent)
{
    Node* child = parent->right;
    parent->right = child->left;
    child->left = parent;
    return child;
}

int get_height(Node* node)
{
    if(node==NULL) return 0;
    else return node->height;
}

void update_height(Node* node)
{
    if(node->left) update_height(node->left);
    if(node->right) update_height(node->right);
    node->height = max(get_height(node->left), get_height(node->right))+1;
}

int get_balance_factor(Node* node)
{
    // > 0: righ heavy, ==0 balance; <0 right heavy
    return get_height(node->left) - get_height(node->right);
}

Node* create_leaf(int data)
{
    auto leaf = new Node();
    leaf->data = data;
    leaf->left = NULL;
    leaf->right = NULL;
    leaf->height = 1;
    return leaf;
}

void inorder_travel(Node * node)
{
    stack<Node*> s;
    Node* last = NULL;
    while(!s.empty() || node)
    {
        if(node)
        {
            s.push(node);
            node = node->left;
        }else
        {
            node = s.top();
            if(last && last->data > node->data)
                cout << last->data <<  ">" << node->data << " ";
            //cout << node->data << " ";
            s.pop();
            node = node->right;
        }
    }
    //cout << endl;
    return;
}

/*You are required to complete this method */
Node* insertToAVL( Node* node, int data)
{
    if(node==NULL)  return create_leaf(data);
    
    // BST insert
    if(data <= node->data)
    {
        node->left = insertToAVL(node->left, data);
        
    }else
    {
        node->right = insertToAVL(node->right, data);
    }
    
    // re-balance
    int balance = get_balance_factor(node);
    if(balance > 1)
    {
        if(get_balance_factor(node->left) > 0)
        {
            //LH+LH => right rotate
            node = right_rotate(node);
        }else
        {
            //LH+RH => left rotate + right rorate
            node->left = left_rotate(node->left);
            node = right_rotate(node);
        }
    }else if(balance < -1)
    {
        if(get_balance_factor(node->right) < 0)
        {
            //RH+RH => left_rotate
            node = left_rotate(node);
        }else
        {
            //RH+LH => right_rotate + left rotate
            node->right = right_rotate(node->right);
            node = left_rotate(node);
        }
    }
    
    // update height for node
    update_height(node);
    inorder_travel(node);
    return node;
}

