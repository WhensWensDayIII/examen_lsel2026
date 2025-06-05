#include "snake_random.h"

#include <stdlib.h>
#include <time.h>

int
snake_random_init(__attribute__((unused)) snake_game_t* p_game)
{
  /* Inicializa semilla */
  srand (time(NULL));
  return 1;
}

int
snake_random_get(int limit)
{
  /* Devuelve entero entre 0 (incluido) y limit (no incluido) */
  return rand() % limit;
}

void
snake_random_close(__attribute__((unused)) snake_game_t* p_game)
{
  /* No hace nada */
}
