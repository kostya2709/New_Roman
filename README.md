# New_Roman language

## Brief description

New_Roman - is a high level programming language. 
Its syntax is very C-like although the key-words are definitely others: they are the Roman Empire style. On the other hand,
this language has some features that C doesn't have (look further to see more details). This project was improved by adding new BackEnd that translates 
this language to `x86-64 assembly`.

## Syntax and key-words

Here you can see New_Roman key-words and they C/C++ equivalents:

|New_Roman  | English translation | C/C++ |
|-----------|---------------------|-------|
|AVE_MARIA! | Hail Mary! (a crusades motto)| { |
|DEUS_VULT! | God wills it! (also a crusades motto)| } |
|имеет_силу | has power | = |
|выступает_в_сенате| speak in the senate | printf ( "%lg", _var_); |
|познает_волю_богов | knows gods will | scanf ( "%lg", &_var_); |
|В_Рим_возвращается | returns to Rome | return |
|Объявить_плебеям | declare plebeians | puts ( "*...*"); |

It should be noticed that condition constructions are quite different from the ones in C/C++. 
This is an example of condition statement and its C/C++ equivalent.

```
Вдруг Ганнибал нападает_на Цезарь
AVE_MARIA!
	
  Равные_силы:
  AVE_MARIA!
  ...
  DEUS_VULT!
  
  Неравные_силы:
  AVE_MARIA!
  ...
  DEUS_VULT!
  
  Победа:
  AVE_MARIA!
  ...
  DEUS_VULT!
  
  Бой_до_последней_капли_крови
  Славная_победа:
  AVE_MARIA!
  ...
  DEUS_VULT!
  
  Поражение: ...
  
  Горькое_поражение: ...
  
  Если_нет: ...
  
DEUS_VULT!  
  
```

The same code in C/C++ looks like this:

```C++
if ( Hannibal == Caesar)
{
  ...
}
else if ( Hannibal != Caesar)
{
  ...
}
else if ( Hannibal >= Caesar)
{
  ...
}
while ( Hannibal > Caesar)
{
  ...
}
if ( Hannibal <= Caesar)
  ...;
else if ( Hannibal < Caesar)
  ...;
else
  ...;
```

Some comments:
* At first, there are variables after `Вдруг` which a programmer wants to compare. After that there is a block with conditions.
* It is not necessary to write all the conditions every time.
A programmer can write only the ones he needs.
* The order of the conditions is not important.
* A condition can have either its own block of commands or just one command.
The syntax in this case is similar to the one in C/C++. If a condition has only one command it should be written on the
line with the condition.
* It is possible to add `Бой_до_последней_капли_крови` before a condition to make a cycle.
* This feature allows a programmer to check different relations between variables without mentioning the variables every time.

## Example

Thus, all the main aspects of the language are considered. Below you can see an example of a factorial function in New_Roman
programming language.

```
Галльская_Война (Цезарь)
AVE_MARIA!

Галлы имеет_силу 0
Вдруг Цезарь нападает_на Галлы
AVE_MARIA!
	Поражение: В_Рим_возвращается I
DEUS_VULT!

В_Рим_возвращается Цезарь * Галльская_Война (Цезарь - I)
DEUS_VULT!

main()
AVE_MARIA!

Цезарь познает_волю_богов

Гонец имеет_силу Галльская_Война (Цезарь)

Объявить_плебеям "Результат войны таков:\n"

Гонец выступает_в_сенате

В_Рим_возвращается NIHIL

DEUS_VULT!
```

You can compare the code with its C/C++ equivalent

```C
#include <stdio.h>

int Gallician_War (int Caesar)
{
  int Gauls = 0;
  
  if (Caesar <= Gauls)
    return 1;
  
  return Caesar * Gallician_War (Caesar - 1);
}

int main()
{
  int Caesar = 0;
  scanf ("%d", &Caesar);
  int messenger = Gallician_War (Caesar);
  printf ("The result of the war:\n");
  printf ("%d", messenger);
  
  return 0;
}

```

Some comments:
* It is possible to write numbers using both roman numbers, and arabic ones (but roman numbers are preferable of course)
* It is possible to name identificators both in English, and in Russian.




