#include "Front_Head.h"
#include <stdint.h>

int64_t* Make_Hash_Array (char** array, int size)
{
    int64_t* hash = (int64_t*)calloc (sizeof (int64_t), size);
    for (int i = 0; i < size; i++)
        hash[i] = Make_Hash_Str(*(array + i));

    return hash;
}

int64_t Make_Hash_Str (char* str)
{
    int64_t hash = 1;

    int j = 0;
    while (str[j])
    {
        hash <<= 1;
        hash += 27 * str[j];
        j++;
    }
    return hash;
}

int Find_Hash (int64_t* hash_ar, int size, int64_t target)
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

int Find_Hash (int64_t* hash_ar, int min, int max, int64_t target, int last_global)
{
    int answer = -1;

    for (int i = 0; i < last_global; i++)
        if (hash_ar[i] == target)
        {
            answer = i;
            return answer;
        }

    for (int i = min; i < max; i++)
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
    this->var = (int64_t*)calloc (sizeof (int64_t), MAX_VAR_AMOUNT);
    this->funcs = (int64_t*)calloc (sizeof (int64_t), MAX_FUNCS_AMOUNT);
    this->args = (int*)calloc (sizeof (int), MAX_ARGS_AMOUNT);

    var_amount = 0;
    last_global = 0;
    funcs_num = 0;
}

Hash::~Hash()
{
    free (this->k_words);
    free (this->oper);
    free (this->var);
    free (this->funcs);
    free (this->args);
}

int Hash::Add_Var (int64_t var_hash)
{
    if (var_amount < MAX_VAR_AMOUNT)
    {
        this->var [this->var_amount] = var_hash;
        this->var_amount += 1;
        return var_amount;
    }

    return 0;
}

int Hash::Add_Func (int64_t func_hash)
{
    if (funcs_num < MAX_VAR_AMOUNT)
    {
        this->funcs [this->funcs_num] = func_hash;
        this->funcs_num += 1;
        return funcs_num;
    }
    else
    {
        printf ("Too many functions!\n");
        exit (0);
    }

    return 0;
}
