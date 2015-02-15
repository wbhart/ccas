#include "ccas.h"
#include "zz.h"

void zz_fread(zz_ptr a, FILE * file)
{
   /* enough for a sign, single word and null terminator */
   char * str = malloc(22);
   int_t num = 0, chars = 21;
   char c = getc(file);

   /* read sign */
   if (c == '-')
      str[num++] = '-';
   else
      ungetc(c, file);

   /* while we have digits */
   while ((c = getc(file)) && c >= 48 && c <= 57)
   {
      str[num++] = c;

      if (num > chars)
      {
         str = realloc(str, 2*chars + 1);
         if (str == NULL)
         {
            printf("ccas: out of memory\n");
            abort();
         }

         chars *= 2;
      }
   }

   ungetc(c, file);

   str[num] = '\0';

   zz_setstr(a, str);

   free(str);
}

