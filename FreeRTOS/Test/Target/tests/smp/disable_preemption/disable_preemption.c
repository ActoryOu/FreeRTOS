/*
 * FreeRTOS V202212.00
 * Copyright (C) 2022 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/**
 * @file disable_preemption.c
 * @brief The scheduler shall not preempt a task for which preemption is disabled.
 *
 * Procedure:
 *   - Create ( num of cores + 1 ) tasks ( T0~Tn ), each with equal priority.
 *   - Disable preemption for task T0. Task T0 will then decrease
 *     its own priority and busy loop for TEST_T0_BUSY_TIME_MS with it still disabled.
 *   - Task T1~Tn in busy loop to check if T0 is still looping with preemption disabled.
 * Expected:
 *   - Task T0 will not be interrupted for the TEST_T0_BUSY_TIME_MS that it
 *     has preemption disabled.
 */

/* Standard includes. */
#include <stdint.h>

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */

#include "unity.h"    /* unit testing support functions */
/*-----------------------------------------------------------*/

/**
 * @brief Timeout value to stop test.
 */
#define TEST_TIMEOUT_MS         ( 1000 )
/*-----------------------------------------------------------*/

#if ( configNUMBER_OF_CORES < 2 )
    #error This test is for FreeRTOS SMP and therefore, requires at least 2 cores.
#endif /* if configNUMBER_OF_CORES != 2 */

#if ( configUSE_TASK_PREEMPTION_DISABLE != 1 )
    #error configUSE_TASK_PREEMPTION_DISABLE must be enabled by including test_config.h in FreeRTOSConfig.h.
#endif /* if configUSE_TASK_PREEMPTION_DISABLE != 1 */

#if ( configMAX_PRIORITIES <= ( configNUMBER_OF_CORES + 2 ) )
    #error configMAX_PRIORITIES must be larger than ( configNUMBER_OF_CORES + 2 ) to avoid scheduling idle tasks unexpectly.
#endif /* if ( configMAX_PRIORITIES <= ( configNUMBER_OF_CORES + 2 ) ) */
/*-----------------------------------------------------------*/

/**
 * @brief Test case "Disable Preemption".
 */
void Test_DisablePreemption( void );

/**
 * @brief Disable preemption test task.
 */
static void prvTestPreemptionDisableTask( void * pvParameters );
/*-----------------------------------------------------------*/

/**
 * @brief Handles of the tasks created in this test.
 */
static TaskHandle_t xTaskHanldes[ configNUMBER_OF_CORES + 1 ];

/**
 * @brief Indexes of the tasks created in this test.
 */
static uint32_t xTaskIndexes[ configNUMBER_OF_CORES + 1 ];

/**
 * @brief Flags to indicate if test task result.
 */
static BaseType_t xTaskTestResult = pdFAIL;
/*-----------------------------------------------------------*/

static void prvTestPreemptionDisableTask( void * pvParameters )
{
    uint32_t currentTaskIdx = *( ( int * ) pvParameters );
    uint32_t taskIndex;
    eTaskState taskState;
    BaseType_t highPriorityTasksSuspended = pdFALSE;

    if( currentTaskIdx == configNUMBER_OF_CORES )
    {
        /* Wait for all other higher priority tasks suspend themselves. */
        while( highPriorityTasksSuspended == pdFALSE )
        {
            highPriorityTasksSuspended = pdTRUE;
            for( taskIndex = 0; taskIndex < configNUMBER_OF_CORES; taskIndex++ )
            {
                taskState = eTaskGetState( xTaskHanldes[ taskIndex ] );
                if( taskState != eSuspended )
                {
                    highPriorityTasksSuspended = pdFALSE;
                    break;
                }
            }
        }

        /* Disable preemption and wake up all the other higher priority tasks.
         * There are core number plus one tasks. If preemption is not disabled,
         * the scheduler will choose higher priority task to run. */
        vTaskDisablePreemption( NULL );

        for( taskIndex = 0; taskIndex < configNUMBER_OF_CORES; taskIndex++ )
        {
            vTaskResume( xTaskHanldes[ taskIndex ] );
        }

        /* If preemption is not disabled, this task will be switched out due to
         * lowest priority. The following line won't be run. */
        xTaskTestResult = pdPASS;
    }
    else
    {
        vTaskSuspend( NULL );
    }

    /* Busy looping here to occupy this core. */
    for( ; ; )
    {
        /* Always running, put asm here to avoid optimization by compiler. */
        __asm volatile ( "nop" );
    }
}
/*-----------------------------------------------------------*/

void Test_DisablePreemption( void )
{
    eTaskState taskState;

    /* TEST_TIMEOUT_MS is long enough to run this task. */
    vTaskDelay( pdMS_TO_TICKS( TEST_TIMEOUT_MS ) );

    /* Verify the lowest priority task runs after resuming all test tasks. */
    TEST_ASSERT_EQUAL( pdPASS, xTaskTestResult );

    /* Enable preemption of the lowest priority task. */
    vTaskDisablePreemption( xTaskHanldes[ configNUMBER_OF_CORES ] );

    /* Verify the task is of ready state now. */
    taskState = eTaskGetState( xTaskHanldes[ configNUMBER_OF_CORES ] );
    TEST_ASSERT_EQUAL( eReady, taskState );
}
/*-----------------------------------------------------------*/

/* Runs before every test, put init calls here. */
void setUp( void )
{
    uint32_t i;
    BaseType_t xTaskCreationResult;

    /* Create configNUMBER_OF_CORES - 1 low priority tasks. */
    for( i = 0; i < ( configNUMBER_OF_CORES + 1 ); i++ )
    {
        xTaskIndexes[ i ] = i;
        xTaskCreationResult = xTaskCreate( prvTestPreemptionDisableTask,
                                           "TestPreemptionDisable",
                                           configMINIMAL_STACK_SIZE * 2,
                                           &xTaskIndexes[ i ],
                                           configMAX_PRIORITIES - 2 - i,
                                           &xTaskHanldes[ i ] );

        TEST_ASSERT_EQUAL_MESSAGE( pdPASS, xTaskCreationResult, "Task creation failed." );
    }
}
/*-----------------------------------------------------------*/

/* Runs after every test, put clean-up calls here. */
void tearDown( void )
{
    int i;

    /* Delete all the tasks. */
    for( i = 0; i < configNUMBER_OF_CORES; i++ )
    {
        if( xTaskHanldes[ i ] != NULL )
        {
            vTaskDelete( xTaskHanldes[ i ] );
            xTaskHanldes[ i ] = 0;
        }
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief A start entry for test runner to run disable preemption test.
 */
void vRunDisablePreemptionTest( void )
{
    UNITY_BEGIN();

    RUN_TEST( Test_DisablePreemption );

    UNITY_END();
}
/*-----------------------------------------------------------*/
