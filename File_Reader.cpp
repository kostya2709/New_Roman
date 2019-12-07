#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifdef Debug
#define PRINTF printf
#else
#define PRINTF if (0) printf
#endif // My_Debug

char* File_Reader (char* file_name, long* num_lines, long* num_symb);

char* Create_Str (long* length, const char* file_read);

long ReadFile (const char* name, char* str, int length);

long ReadFileLength (const char* file);

long Symb_Counter (char* str, char c);


/*! This function creates a string from file file_name, returns the
amount of lines in the text in num_lines and the amount of symbols
in num_symb, returns the string with the file.
@param [in] file_name The name of the file to read the text from.
@param [in] num_lines The variable to write the amount of lines in.
@param [in] num_symb The variable to write the amount of symbols in.

\return The pointer to the string with the written file.
*/

char* File_Reader (char* file_name, long* num_lines, long* num_symb)
{
    assert (file_name);
    assert (num_lines);
    assert (num_symb);

    *num_symb = ReadFileLength (file_name);
    PRINTF ("Number of symbols in the text = %ld\n", *num_symb);

    char* my_string = Create_Str (num_symb, file_name);
    PRINTF ("The REAL size is %ld, file_name is %s\n", *num_symb, file_name);

    *num_lines = Symb_Counter (my_string, '\n');
    PRINTF ("There are %ld lines in the file.\n", *num_lines);

    return my_string;
}

/*! This function creates a string with the maximum amount length of symbols from the file.
@param[in] length The maximum amount of symbols.
@param[in] file_read The name of the file to read the string from.
\return The proper string from the file.
*/

char* Create_Str (long* length, const char* file_read)
{
    assert (length);
    assert (file_read);

    char* my_string = (char*) calloc (*length + 1, sizeof(char));

    *length = ReadFile (file_read, my_string, *length);
    realloc (my_string, *length + 2);

    if (* (my_string + *length - 1) != '\n')
        {
            *(my_string + *length) = '\n';
            (*length) = *length + 1;
        }

    return my_string;

}


/*! This function reads a text from a file to a string.
@param[in] name The name of the file.
@param[in] str A pointer to the string where the text writes to.
@param[in] length The maximum length of the file.
\return 1 if everything is alright.
*/

long ReadFile (const char* name, char* str, int length)
{
    assert (name);
    assert (str);
    assert (length >= 0);

    FILE* f = fopen ((char*)name, "r");

    long real_size = fread ( (char*)str, 1, length, f);
    fclose (f);
    return real_size;
}

/*! This function counts the number of symbols in the file.
@param[in] file The pointer to the name of the file
\return The amount of symbols in the file
\warning This function also counts special invisible symbols as:
"\n" and "\r".
*/

long ReadFileLength (const char* file)
{
    assert (file);

    FILE* f = fopen ((char*)file, "r");

    fseek (f, 0, SEEK_END);
    long pointer2 = ftell (f);

    fclose (f);
    return (pointer2);

}

/*! This functions counts the number of char c in the string str.
@param[in] str String in which symbols are counted
@param[in] c The symbol need counting
\return The amount of the symbol in the string
*/

long Symb_Counter (char* str, char c)
{
    assert (str);

    long counter = 0;

    for (; *str; str++)
        if (*str == c) counter++;

    return counter;
}
