#include <stdlib.h>
#include <stdio.h>

int Get_Value_Roman (char letter);

const int Max_Roman_Size = 30;

int Is_Roman_Num (const char* number)
{
    while (*number)
    {
        if (Get_Value_Roman (*number)== -1)
                return 0;
        number++;
    }
    return 1;
}

int Get_Value_Roman (char letter)
{
    switch (letter)
    {
        case 'I':
            return 1;
        case 'V':
            return 5;
        case 'X':
            return 10;
        case 'L':
            return 50;
        case 'C':
            return 100;
        case 'D':
            return 500;
        case 'M':
            return 1000;
        default:
        {
            if (letter == 0)
                return 0;
            else
               return -1;
        }

    }
}

int Roman_Number (const char* number)
{
    int i = 0;

    int answer = 0;
    int sign = 1;

    if (*number == '-')
    {
        sign = -1;
        number++;
    }

    while (*number)
    {
        int cur_val = Get_Value_Roman(*number);
        int next_val = Get_Value_Roman(*(number + 1));

        if (cur_val == -1)
            return -1;

        if (next_val > cur_val)
        {
            answer += next_val - cur_val;
            number++;
        }
        else
            answer += cur_val;
        number++;
    }

    double temp = answer;

    return sign * answer;
}


char* Get_Roman (int d_number)
{
    char* answer = (char*)calloc (1, Max_Roman_Size);
    char* start = answer;

    if (d_number < 0)
    {
        sprintf (answer, "-");
        answer++;
        d_number *= -1;
    }

    int value[7] = {1, 5, 10, 50, 100, 500, 1000};
    char letter[7] = {'I', 'V', 'X', 'L', 'C', 'D', 'M'};

    int step = 6;
    int i = 0;

    int number = d_number;

    while (number)
    {
        if (number / value[step])
            for (i = 0; i < number / value[step]; i++)
            {
                sprintf (answer, "%c", letter[step]);
                answer++;
            }
        number = number % value[step];
        if ((step == 2 || step == 4 || step == 6) && (number >= value[step] - value[step - 2]))
        {
            sprintf (answer, "%c%c", letter [step - 2], letter [step]);
            answer += 2;
            number %= value[step] - value[step - 2];
        }
        else if ((step == 1 || step == 3 || step == 5) && (number >= value[step] - value [step - 1]))
            {
                sprintf (answer, "%c%c", letter [step - 1], letter [step]);
                answer += 2;
                number %= value[step] - value[step - 1];

            }


        step--;
    }
    return start;
}
