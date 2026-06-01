#include "unity.h"

#define TEST_CASE(...)

#include "fsm.h"
#include "mock_test_fsm.h"

#include <stdlib.h>

/**
 * @file test_fsm_legacy.c
 * @author 
 * @author 
 * @brief Tests for existing fsm module
 * @version 0.1
 * @date 2024-04-09
 * 
 */

/**
 * @brief Stub or Callback for fsm_malloc that calls real malloc. 
 * 
 * @param[in] s Amount of bytes to allocate
 * @param[in] n Amount of calls to this function
 * 
 * @return pointer to allocated memory if success; NULL if fails
 * 
 */
static void* cb_malloc(size_t s, int n) {
    return malloc(s);
}

/**
 * @brief Stub or Callback for fsm_free that calls real free. 
 * 
 * @param[in] p Pointer to allocated memory to free
 * @param[in] n Amount of calls to this function
 * 
 */
static void cb_free(void* p, int n) {
    return free(p);
}

void setUp(void)
{
}

void tearDown(void)
{
}

/**
 * @brief Comprueba que la funcion de fsm_new devuelve NULL 
 *        y no llama a fsm_malloc si la tabla de transiciones es NULL 
 */
void test_fsm_new_nullWhenNullTransition(void)
{
    fsm_t *f = (fsm_t*)1;

    f = fsm_new(NULL, "fsm_1");

    TEST_ASSERT_EQUAL (NULL, f);
}

/**
 * @brief La máquina de estados devuelve NULL
 *        y no llama a fsm_malloc si el estado de origen
 *        de la primera transición es -1 (fin de la tabla)
 */
void test_fsm_nullWhenFirstOrigStateIsMinusOne(void) {
    fsm_trans_t tt[] = {{-1, check_func, 1, output_func}};
    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt, "fsm_1");
 
    TEST_ASSERT_EQUAL (f, NULL);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si el estado de destino de la primera transición es -1 (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstDstStateIsMinusOne(void) {
    fsm_trans_t tt[] = {{1, check_func, -1, output_func}};
    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt, "fsm_1");
   
    TEST_ASSERT_EQUAL (f, NULL);
}

/**
 * @brief La máquina de estados devuelve NULL y no llama a fsm_malloc si la función de comprobación de la primera transición es NULL (fin de la tabla)
 * 
 */
void test_fsm_nullWhenFirstCheckFunctionIsNull(void) {
    fsm_trans_t tt[] = {{1, NULL, 1, output_func}};
    fsm_t *f = (fsm_t*)1;
    f = fsm_new(tt, "fsm_1");
   
    TEST_ASSERT_EQUAL (f, NULL);
}

/**
 * @brief Devuelve puntero no NULL y llama a fsm_malloc (usa Callback a cb_mallock y fsm_malloc_ExpectXXX) al crear la maquina de estados con una transición válida
 *        con función de actualización (salida) NULL o no NULL.
 *        Hay que liberar la memoria al final llamando a free
 * 
 */
TEST_CASE(NULL)
TEST_CASE(output_func)
void test_fsm_new_nonNullWhenOneValidTransitionCondition(fsm_output_func_t out)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, out},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_AddCallback(cb_malloc);
    fsm_malloc_ExpectAnyArgsAndReturn(NULL);
    fsm_t *f = fsm_new(tt, "fsm_1"); 
    TEST_ASSERT_NOT_NULL(f);
     
    free(f);
}


/**
 * @brief Estado inicial corresponde al estado de entrada de la primera transición de la lista al crear una maquina de estados y es valido.
 *        Usa Stub para fsm_malloc y luego libera la memoria con free
 */
void test_fsm_new_fsmGetStateReturnsOrigStateOfFirstTransitionAfterInit(void)
{
    int STATE0 = 123;
    int STATE1 = 321;
    fsm_trans_t tt[] = {
        {STATE0, check_func, STATE1, output_func},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_Stub(cb_malloc);
    fsm_t *f = fsm_new(tt, "fsm_1");
    int state = fsm_get_state(f);
    TEST_ASSERT_EQUAL(STATE0, state);

    free(f);
}

/**
 * @brief La maquina de estado no transiciona si la funcion devuelve 0
 * 
 */
void test_fsm_fire_isTrueReturnsFalseMeansDoNothingIsNotCalledAndStateKeepsTheSame(void)
{
    int STATE0 = 123;
    int STATE1 = 321;
    fsm_trans_t tt[] = {
        {STATE0, check_func, STATE1, output_func},
        {-1, NULL, -1, NULL}
    };
    check_func_IgnoreAndReturn(0);
    fsm_malloc_Stub(cb_malloc);
    fsm_t *f = fsm_new(tt, "fsm_1");
    int state = fsm_get_state(f);
    TEST_ASSERT_EQUAL(STATE0, state);

    free(f);
}

/**
 * @brief Comprueba que el puntero pasado a fsm_fire es pasado a la función de guarda cuando se comprueba una transición
 * 
 */
void test_fsm_fire_checkFunctionCalledWithFsmPointerFromFsmFire(void)
{

    fsm_trans_t tt[] = {
        {0, check_func, 1, NULL},
        {-1, NULL, -1, NULL}
    };

    fsm_malloc_Stub(cb_malloc);

    fsm_t *f = fsm_new(tt, "fsm_1");
    check_func_ExpectAndReturn(f, 1);
    fsm_fire(f);

    free(f);
}

/** 
 * @brief Comprueba que el fsm_fire funciona y tiene el estado correcto cuando la transición devuelve true (cambia) y cuando devuelve false (mantiene)
 * 
 */
TEST_CASE(false, 0)
TEST_CASE(true, 1)
void test_fsm_fire_checkFunctionIsCalledAndResultIsImportantForTransition(bool returnValue, int expectedState)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    fsm_init(&f, tt,"fsm_1");
    check_func_IgnoreAndReturn(returnValue);
    fsm_fire(&f);
    int state = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(expectedState, state);
}


/**
 * @brief La creación de una máquina de estados devuelve NULL si la reserva de memoria falla.
 *        Usa el Mock llamando a fsm_malloc_ExpectXXX sin Stub ni Callback.
 *
 */
void test_fsm_new_nullWhenFsmMallocReturnsNull(void)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, NULL},
        {1, check_func, 0, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_ExpectAnyArgsAndReturn(NULL);
    fsm_t *f = fsm_new(tt, "fsm_2");
    TEST_ASSERT_EQUAL(NULL, f);

    free(f);
}

/**
 * @brief Llamar a fsm_destroy provoca una llamada a fsm_free
 *        Usa el Mock llamando a fsm_free_Expect sin Stub ni Callback
 *
 */
void test_fsm_destroy_callsFsmFree(void)
{
    fsm_t *f = (fsm_t*)1;
    fsm_free_ExpectAnyArgs();
    fsm_destroy(f);
}

/**
 * @brief Comprueba que solo se llame a la función de guarda que toca según el estado actual
 * 
 */
void test_fsm_fire_callsFirstIsTrueFromState0AndThenIsTrue2FromState1(void)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, NULL},
        {1, check_func2, 0, NULL},  
        {-1, NULL, -1, NULL}
    };
    fsm_t f;
    int res;
    fsm_init(&f, tt,"fsm_1");
    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(0, res);
    check_func_ExpectAndReturn(&f, 1);
    fsm_fire(&f);
    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(1, res);
    check_func2_ExpectAndReturn(&f, 1);
    fsm_fire(&f);
    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(0, res);
}

/**
 * @brief Comprueba que se pueden crear dos instancias de máquinas de estados simultánteas y son punteros distintos.
 *        Usa Stub para fsm_malloc y luego libera la memoria con free
 *
 */
void test_fsm_new_calledTwiceWithSameValidDataCreatesDifferentInstancePointer(void)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, NULL},
        {-1, NULL, -1, NULL}
    };
    fsm_malloc_Stub(cb_malloc);
    fsm_t *f1 = fsm_new(tt, "fsm_1");
    fsm_t *f2 = fsm_new(tt, "fsm_2");

    TEST_ASSERT_NOT_EQUAL(f1, f2);

    free(f1);
    free(f2);
}

/**
 * @brief Comprueba que si la tabla de transiciones es valida y tiene función de salida, esta se ejecuta.
 *
 */
void test_fsm_fire_outFunctionCalled(void)
{
    fsm_trans_t tt[] = {
        {0, check_func, 1, output_func},
        {-1, NULL, -1, NULL}
    };
    
    fsm_t f;
    fsm_init(&f, tt, "fsm_1");
    output_func_ExpectAnyArgs();
    check_func_IgnoreAndReturn(1);
    fsm_fire(&f);
}

/**
 * @brief La máquina de estados devuelve NULL si la tabla de transiciones es NULL
 */
void test_fsm_NotModifiedWhenTTnull(void) {
    fsm_t f;
    int STATE0 = 123;
    fsm_set_state(&f, STATE0);
    int res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(STATE0, res);

    fsm_init(&f, NULL, "fsm_1");
    res = fsm_get_state(&f);
    TEST_ASSERT_EQUAL(STATE0, res);
}



//Nuevos tests para comprobar funcionalidad

// Test 1: Comprobar que un nombre válido (<= 16 caracteres) se guarda correctamente
void test_fsm_new_should_accept_valid_name(void){
    fsm_trans_t tt[] = { {0, check_func, 1, output_func}, {-1, NULL, -1, NULL} };
    
    static fsm_t fsm_comparacion;
    fsm_init(&fsm_comparacion, tt, "fsm_corta");

    fsm_malloc_ExpectAndReturn(sizeof(fsm_t), &fsm_comparacion);
    
    fsm_t *fsm = fsm_new(tt, "fsm_corta"); // Ejecuta el código real
    
    // Verificar que fsm_new nos ha devuelto el puntero simulado y tiene el nombre
    TEST_ASSERT_EQUAL_PTR(&fsm_comparacion, fsm);
    TEST_ASSERT_EQUAL_STRING("fsm_corta", fsm->name); 
    
    fsm_free_Expect(&fsm_comparacion);
    
    fsm_destroy(fsm);
}

// Test 2: Comprobar que un nombre inválido (> 16 caracteres) no se acepta
void test_fsm_new_should_not_accept_invalid_name(void){
    fsm_trans_t tt[] = { {0, check_func, 1, output_func}, {-1, NULL, -1, NULL} };
    fsm_t *fsm = fsm_new(tt, "fsm_muy_largo_nombre"); // 20 caracteres

    TEST_ASSERT_NULL(fsm);
}

// Test 3: Comprobar que si el nombre es NULL también es INVÁLIDO
void test_fsm_new_should_fail_if_name_is_null(void)
{
    fsm_trans_t tt[] = { {0, check_func, 1, output_func}, {-1, NULL, -1, NULL} };
    
    // Tampoco debe llamar a fsm_malloc porque sale al principio por el IF
    fsm_t *fsm = fsm_new(tt, NULL); 
    
    TEST_ASSERT_NULL(fsm);
}


//...........................Checkname tests...........................
// Test: Deben coincidir si las cadenas son idénticas
void test_fsm_check_name_should_return_true_if_names_match(void)
{
    fsm_t fsm;
    fsm.name = "fsm_legacy"; // Simulamos que la FSM ya tiene este nombre
    
    int resultado = fsm_check_name(&fsm, "fsm_legacy");
    
    TEST_ASSERT_EQUAL_INT(1, resultado); // 1 significa TRUE
}

// Test: Debe devolver FALSE si el nombre es diferente
void test_fsm_check_name_should_return_false_if_names_differ(void)
{
    fsm_t fsm;
    fsm.name = "fsm_legacy";
    
    int resultado = fsm_check_name(&fsm, "fsm_distinta");
    
    TEST_ASSERT_EQUAL_INT(0, resultado); // 0 significa FALSE
}

// Test: Protección frente a punteros NULL
void test_fsm_check_name_should_return_false_if_pointer_is_null(void)
{
    fsm_t fsm;
    fsm.name = "fsm_legacy";
    
    // Si la FSM es NULL o la cadena a comparar es NULL, debe ser FALSE de forma segura
    TEST_ASSERT_EQUAL_INT(0, fsm_check_name(NULL, "fsm_legacy"));
    TEST_ASSERT_EQUAL_INT(0, fsm_check_name(&fsm, NULL));
}


