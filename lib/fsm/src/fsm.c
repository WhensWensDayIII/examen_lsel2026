/**
 * @file fsm.c
 * @brief Library to create Finite State Machines using composition.
 *
 * This library is expected to be used using composition
 * @author Teachers from the Departamento de Ingeniería Electrónica. Original authors: José M. Moya and Pedro J. Malagón. Latest contributor: Román Cárdenas.
 * @date 2023-09-20
 */

/* Includes ------------------------------------------------------------------*/
/* Standard C includes */
#include <stdlib.h>

/* Other includes */
#include "fsm.h"

//GCOVR_EXCL_START
void* __attribute__((weak)) fsm_malloc(size_t s)
{
    return malloc(s);
}
void __attribute__((weak)) fsm_free(void* p)
{
    free(p);
}
//GCOVR_EXCL_STOP

fsm_t *fsm_new(fsm_trans_t *p_tt, const char *name)
{

    //Si el puntero es nulo o el nombre es nulo, el fsm es inválido
    if (p_tt == NULL || name == NULL)
    {
        return NULL;
    }

    // Validación: Contar caracteres distintos de 0 (máximo 16)
    int len = 0;
    while (name[len] != '\0') {
        len++;
        if (len > 16) {
            return NULL; // Más de 16 caracteres -> Inválido
        }
    }

    if ((p_tt->orig_state == -1) || (p_tt->in == NULL) || (p_tt->dest_state == -1))
    {
        return NULL;
    }
    fsm_t *p_fsm = (fsm_t *)fsm_malloc(sizeof(fsm_t));
    if (p_fsm != NULL)
    {
        fsm_init(p_fsm, p_tt, name);
    }
    return p_fsm;
}

void fsm_destroy(fsm_t *p_fsm)
{
    fsm_free(p_fsm);
}

void fsm_init(fsm_t *p_fsm, fsm_trans_t *p_tt, const char *name)
{
    //Si el puntero es nulo o el nombre es nulo, el fsm es inválido
    if (p_tt == NULL || name == NULL)
    {
        return;
    }
    // Validación: Contar caracteres distintos de 0 (máximo 16)
    int len = 0;
    while (name[len] != '\0') {
        len++;
        if (len > 16) {
            return; // Más de 16 caracteres -> Inválido
        }
    }

        p_fsm->p_tt = p_tt;
        p_fsm->current_state = p_tt->orig_state;
        p_fsm->name = name;

}

int fsm_get_state(fsm_t *p_fsm)
{
    return p_fsm->current_state;
}

void fsm_set_state(fsm_t *p_fsm, int state)
{
    p_fsm->current_state = state;
}

void fsm_fire(fsm_t *p_fsm)
{
    fsm_trans_t *p_t;
    for (p_t = p_fsm->p_tt; p_t->orig_state >= 0; ++p_t)
    {
        if ((p_fsm->current_state == p_t->orig_state) && p_t->in(p_fsm))
        {
            p_fsm->current_state = p_t->dest_state;
            if (p_t->out)
            {
                p_t->out(p_fsm);
            }
            break;
        }
    }
}


