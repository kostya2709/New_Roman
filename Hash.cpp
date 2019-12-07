#include "Front_Head.h"


int* Make_Hash_Array (char** array, int size)
{
    int* hash = (int*)calloc (sizeof (int), size);
    for (int i = 0; i < size; i++)
        hash[i] = Make_Hash_Str(*(array + i));

    return hash;
}

int Make_Hash_Str (char* str)
{
    int hash = 1;

    int j = 0;
    while (*(str + j))
    {
        hash += 27 * (*(str + j)) + pow (j, 5);
        j++;
    }
    return hash;
}

int Find_Hash (int* hash_ar, int size, int target)
{
    int answer = -1;
    for (int i = 0; i < size; i++)
        if (hash_ar[i] == target)
        {
            answer = i;
            break;
        }
    return answer;
}

Hash::Hash()
{
    this->k_words = Make_Hash_Array ((char**)key_words_str, K_WORDS_NUMBER);
    this->oper = Make_Hash_Array ((char**)oper_str, OPERATOR_NUMBER);
    this->var = (int*)calloc (sizeof (int), MAX_VAR_AMOUNT);
    var_amount = 0;
}

Hash::~Hash()
{
    free (this->k_words);
    free (this->oper);
    free (this->var);
}

int Hash::Add_Var(int var_hash)
{
    if (var_amount < MAX_VAR_AMOUNT)
    {
        this->var [this->var_amount] = var_hash;
        this->var_amount += 1;
        return var_amount;
    }

    return 0;
}
