#include <string.h>
#include <stdio.h>
#include "ccas.h"
#include "Poly.h"

void safe_copy(char ** str, int_t * len, int_t * alloc, const char * str2)
{
   int_t len2 = strlen(str2);

   /* need more space in target string, realloc */
   if (len2 >= (*alloc) - (*len))
   {
      (*alloc) = CCAS_MAX(2*(*alloc), (*len) + len2 + 1);
      (*str) = realloc((*str), (*alloc));
   }

   /* copy string */
   char * ptr = (*str) + (*len);
      
   for (int_t i = 0; i < len2; i++)
      ptr[i] = str2[i];

   /* adjust length */
   (*len) += len2;
}

int needs_parens(const char * str)
{
   return strchr(str, ' ') != NULL;
}

int is_one(const char * str)
{
   return strlen(str) == 1 && str[0] == '1';
}

int is_zero(const char * str)
{
   return strlen(str) == 1 && str[0] == '0';
}

char * poly_getstr(Poly_t pol)
{
   char buff[21];
   char * str = malloc(10);
   int_t alloc = 10;
   int_t i, len = 0;

   char * (* getstr)(void *) = pol->parent->base->getstr;
   int_t size = pol->parent->base->size;
   char * var = (char *) pol->parent->aux;

   if (pol->length == 0)
   {
      str[0] = '0';
      str[1] = '\0';

      return str;
   }

   for (i = pol->length - 1; i >= 1; i--)
   {
      char * coeff = getstr(pol->coeffs + i*size);
      char * coeff1 = coeff + (coeff[0] == '-');

      if (!is_zero(coeff))
      {
         if (i != pol->length - 1)
         {
            if (coeff[0] == '-')
               safe_copy(&str, &len, &alloc, " - ");
            else
               safe_copy(&str, &len, &alloc, " + ");
         } else
         {
            if (coeff[0] == '-')
               safe_copy(&str, &len, &alloc, "-");
         }
            
         if (!is_one(coeff1))
         {
            if (needs_parens(coeff1))
               safe_copy(&str, &len, &alloc, "(");

            safe_copy(&str, &len, &alloc, coeff1);

            if (needs_parens(coeff1))
                safe_copy(&str, &len, &alloc, ")");

            safe_copy(&str, &len, &alloc, "*");
         }

         safe_copy(&str, &len, &alloc, var);

         if (i != 1)
         {
            safe_copy(&str, &len, &alloc, "^");
            sprintf(buff, "%ld", i);
            safe_copy(&str, &len, &alloc, buff);
         }
      }

      free(coeff);
   }

   char * coeff = getstr(pol->coeffs);
   char * coeff1 = coeff + (coeff[0] == '-');

   if (!is_zero(coeff))
   {
      if (i != pol->length - 1)
      {
         if (coeff[0] == '-')
            safe_copy(&str, &len, &alloc, " - ");
         else
            safe_copy(&str, &len, &alloc, " + ");
      } else
      {
         if (coeff[0] == '-')
            safe_copy(&str, &len, &alloc, "-");
      }

      safe_copy(&str, &len, &alloc, coeff1);
   }

   free(coeff);

   str[len] = '\0';

   return str;
}

