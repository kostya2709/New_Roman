#include "Front_Head.h"

Node::Node(): left (NULL), right (NULL), parent (NULL), data (-1), node_type (0)
{
    sym = (char*)calloc (1, operator_size);
}

Node::~Node()
{
    free (sym);
}

Tree::Tree(): elem_num (0)
{
    Node zero_node;
    this->first_elem = &zero_node;
}

Tree::~Tree()
{

}

int Tree::Insert_Node (Node* node1, Node* node_new, int pos)
{
    assert (node_new);
    assert (node1);

    if (pos == 0)
        node1->left = node_new;
    else if (pos == 1)
        node1->right = node_new;
    else
    {
        printf ("Error! Wrong position in Insert_Node\n\
                 Expected 0 for left or 1 for right. Received %d.", pos);
        return -1;
    }
    this->elem_num += 1;

    node_new->parent = node1;

    return 0;
}

int Tree::Insert_Node (Node* node_new)
{
    assert (node_new);

    this->first_elem = node_new;
    this->elem_num = 1;
    return 0;
}

int Tree::Dump (void)
{
    char* file_name = (char*)calloc (1, FILE_NAME_SIZE);
    char* file_png = (char*)calloc (1, FILE_NAME_SIZE);


    file_name = "equation_print.gv";
    file_png = "equation_print_pic.png";

    FILE* f = fopen (file_name, "w");

    fprintf (f, "digraph First{\n");
    fprintf (f, "node [shape=\"Mrecord\", style=\"filled\", fillcolor=\"lightblue\"];\n");

    Tree_Print (this->first_elem, f);

    fprintf (f, "}");

    fclose (f);

    system ("dot -Tpng equation_print.gv -o equation_print_pic.png");
    system ("xdg-open equation_print_pic.png");


    file_name = "equation_dump.gv";
    file_png = "equation_dump_pic.png";

    f = fopen (file_name, "w");

    fprintf (f, "digraph First{\n");
    fprintf (f, "node [shape=\"Mrecord\", style=\"filled\", fillcolor=\"lightblue\"];\n");

    Tree_Info_Dump(this->first_elem, f);

    fprintf (f, "}");

    fclose (f);

    system ("dot -Tpng equation_dump.gv -o equation_dump_pic.png");


    return 0;
}

void Tree::Tree_Print (const Node* node1, FILE* f)
{
    assert (node1);

        char* color = (char*)calloc (1, 10);

#define DEF_COLOR(node)             \
if (node->node_type == OPERATOR)   \
    sprintf (color, "yellow");      \
else if (node->node_type == K_WORD)   \
    sprintf (color, "orange");      \
else if (node->node_type == VAR)   \
    sprintf (color, "green");       \

    DEF_COLOR (node1);

    if (node1 == this->first_elem)
    {
        if (node1->node_type == NUMBER)
            fprintf (f, "\"box%0x\" [label=\"" ACCUR "\"];\n", node1, node1->data);
        else
            fprintf (f, "\"box%0x\" [fillcolor=\"%s\",label=\"" "%s" "\"];\n", node1, color, node1->sym);
    }

    if (node1->left)
        {
            DEF_COLOR (node1->left);

            if (node1->left->node_type == NUMBER)
                fprintf (f, "\"box%0x\" [label=\"" ACCUR "\"];\n", node1->left, (node1->left)->data);
            else
                fprintf (f, "\"box%0x\" [fillcolor=\"%s\",label=\"" "%s" "\"];\n", node1->left, color, node1->left->sym);
            fprintf (f, "\"box%0x\" -> \"box%0x\"[color=\"red\"];\n", node1, node1->left);

            Tree_Print (node1->left, f);
        }


    if (node1->right)
    {
        DEF_COLOR (node1->right);

        if (node1->right->node_type == NUMBER)
            fprintf (f, "\"box%0x\" [label=\"" ACCUR "\"];\n", node1->right, (node1->right)->data);
        else
            fprintf (f, "\"box%0x\" [fillcolor=\"%s\",label=\"" "%s" "\"];\n", node1->right, color, node1->right->sym);
        fprintf (f, "\"box%0x\" -> \"box%0x\"[color=\"green\"];\n", node1, node1->right);

        Tree_Print (node1->right, f);
    }


    #undef DEF_COLOR
    free (color);

}

void Tree::Tree_Info_Dump (const Node* node1, FILE* f)
{

            char* color = (char*)calloc (1, 10);

#define DEF_COLOR(node)             \
if (node->node_type == OPERATOR)   \
    sprintf (color, "yellow");      \
else if (node->node_type == K_WORD)   \
    sprintf (color, "orange");      \
else if (node->node_type == VAR)   \
    sprintf (color, "green");       \

    DEF_COLOR(node1);

    if (node1 == this->first_elem)
        if (node1->node_type == NUMBER)
            fprintf (f, "\"box%0x\" [label=\"{" ACCUR "|adress=%0x|left=%0x|right=%0x}\"];\n", node1, node1->data, node1, node1->left, node1->right);
        else
            fprintf (f, "\"box%0x\" [fillcolor = \"%s\",label=\"{%s|adress=%0x|left=%0x|right=%0x}\"];\n", node1, color, node1->sym, node1, node1->left, node1->right);

    if (node1->left)
        {
            DEF_COLOR (node1->left);

            if (node1->left->node_type == NUMBER)
                fprintf (f, "\"box%0x\" [label=\"{" ACCUR "|adress %0x|left=%0x|right=%0x|parent=%0x}\"];\n",
                            node1->left, (node1->left)->data, node1->left,
                (node1->left)->left, (node1->left)->right, node1->left->parent);
            else fprintf (f, "\"box%0x\" [fillcolor=\"%s\",label=\"{%s|adress %0x|left=%0x|right=%0x|parent=%0x}\"];\n",
                            node1->left, color, (node1->left)->sym, node1->left,
                (node1->left)->left, (node1->left)->right, node1->left->parent);
            fprintf (f, "\"box%0x\" -> \"box%0x\"[color=\"red\"];\n", node1, node1->left);

            Tree_Info_Dump (node1->left, f);
        }


    if (node1->right)
    {
        DEF_COLOR (node1->right);

        if (node1->right->node_type == NUMBER)
            fprintf (f, "\"box%0x\" [label=\"{" ACCUR "|adress=%0x|left=%0x|right=%0x|parent=%0x}\"];\n", node1->right, (node1->right)->data, node1->right, (node1->right)->left, (node1->right)->right, node1->right->parent);
        else
            fprintf (f, "\"box%0x\" [fillcolor=\"%s\",label=\"{%s|adress=%0x|left=%0x|right=%0x|parent=%0x}\"];\n", node1->right, color, (node1->right)->sym, node1->right, (node1->right)->left, (node1->right)->right, node1->right->parent);
        fprintf (f, "\"box%0x\" -> \"box%0x\"[color=\"green\"];\n", node1, node1->right);

        Tree_Info_Dump (node1->right, f);
    }


    #undef DEF_COLOR
    free (color);

}

Node* Create_Node (Node* left, Node* right, Node* parent, elem_t data, char* sym, int node_type)
{
    Node* new_node = new Node();

    new_node->left = left;
    new_node->right = right;
    new_node->parent = parent;

    if (left)
        left->parent = new_node;
    if (right)
        right->parent = new_node;

    new_node->data = data;
    sprintf (new_node->sym, "%s", sym);
    //printf ("IN SUM %s\n");
    new_node->node_type = node_type;

    return new_node;
}


int Tree_Copy_Cycle (Node* prev_node, Node* new_node);

Node* Tree_Copy (Node* prev_node)
{
    Node* new_node = new Node();

    Tree_Copy_Cycle (prev_node, new_node);

    return new_node;

}

int Tree_Copy_Cycle (Node* prev_node, Node* new_node)
{

    new_node->data = prev_node->data;
    sprintf (new_node->sym, "%s", prev_node->sym);

    new_node->node_type = prev_node->node_type;

    if (prev_node->left)
    {
        Node* node_l = new Node();
        new_node->left = node_l;
        node_l->parent = new_node;
        Tree_Copy_Cycle (prev_node->left, node_l);
    }

    if (prev_node->right)
    {
        Node* node_r = new Node();
        new_node->right = node_r;
        node_r->parent = new_node;
        Tree_Copy_Cycle (prev_node->right, node_r);
    }
}
