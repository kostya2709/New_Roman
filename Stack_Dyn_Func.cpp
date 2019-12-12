#include "Front_Head.h"
#define empty -666

enum Error_codes {OK, Stack_OverFlow, Stack_Empty};

int Stack_Construct (Stack_t* stk)
{
    assert (stk);

    int i = 0;
    for (i = 0; i < MAX_STACK_SIZE; i++)
        stk->data[i] = empty;
    stk->size = 0;
    stk->max_size = MAX_STACK_SIZE;
    stk->error = 0;

    return 0;
}

int Stack_Destruct (Stack_t* stk)
{
    assert (stk);

    int i = 0;
    for (i = 0; i < MAX_STACK_SIZE; i++)
        stk->data[i] = empty;
    stk->size = 0;
    stk->max_size = MAX_STACK_SIZE;
    stk->error = 0;

    return 0;
}

int Stack_Push (Stack_t* stk, elem_t value)
{
    assert (stk);

    if (stk->size >= MAX_STACK_SIZE)
    {
        stk->error = Stack_OverFlow;
        return Stack_OverFlow;
    }

    stk->data [stk->size++] = value;

    return OK;
}

elem_t Stack_Pop (Stack_t* stk)
{
    assert (stk);

    if (stk->size <= 0)
    {
        stk->error = Stack_Empty;
        return Stack_Empty;
    }

    stk->size--;
    elem_t answer = stk->data [stk->size];
    stk->data [stk->size] = empty;

    return answer;
}

elem_t Show_First (Stack_t* stk)
{
    assert (stk);

    elem_t answer = Stack_Pop (stk);
    Stack_Push (stk, answer);

    return answer;
}

int Dump (Stack_t* stk, int line)
{
    printf ("\n\n\nJust looking from %s, line %d\n", __FILE__, line);
    printf ("Stack_t \"%s\" [0x%x]", stk->name, stk);
    if (stk->error == 0)
        printf (" (OK)\n");
    else printf (" (Error)\n");
    printf ("{\n    size = %d\n", stk->size);
    printf ("    data[%d] = [0x%x]\n", stk->max_size, &(stk->data));
    printf ("     {\n");
    int i = 0;
    for (i = 0; i < stk->max_size; i++)
    {
        if (i < stk->size)
            printf ("          *");
        else printf ("           ");

        printf ("[%d] = %d", i, stk->data[i]);
        if (stk->data[i] == empty)
            printf (" [It might be empty]");
        printf ("\n");
    }
    printf ("     }\n");
    printf ("Error = %d\n", stk->error);
    printf ("}\n\n\n");
}

int Stack_OK (Stack_t* stk)
{
    if (stk->size < 0)
    {
        stk->error = Stack_Empty;
        return Stack_Empty;
    }

    if (stk->size > MAX_STACK_SIZE)
    {
        stk->error = Stack_OverFlow;
        return Stack_OverFlow;
    }


}
