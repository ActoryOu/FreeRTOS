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
 * @file schedule_highest_priority.c
 * @brief The scheduler shall correctly schedule the highest priority ready tasks.
 *
 * Procedure:
 *   - Create ( num of cores ) tasks ( T0~Tn-1 ). Priority T0 > T1 > ... > Tn-2 > Tn-1.
 * Expected:
 *   - When a task runs, all tasks have higher priority are running.
 */

/* Kernel includes. */

#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */

#include "unity.h"    /* unit testing support functions */
/*-----------------------------------------------------------*/

/**
 * @brief Timeout value to stop test.
 */
#define TEST_TIMEOUT_MS    ( 10000 )
/*-----------------------------------------------------------*/

#if ( configNUMBER_OF_CORES < 2 )
    #error This test is for FreeRTOS SMP and therefore, requires at least 2 cores.
#endif /* if configNUMBER_OF_CORES != 2 */

#if ( configMAX_PRIORITIES <= configNUMBER_OF_CORES )
    #error This test creates tasks with different priority, requires configMAX_PRIORITIES to be larger than configNUMBER_OF_CORES.
#endif /* if configNUMBER_OF_CORES != 2 */
/*-----------------------------------------------------------*/

/**
 * @brief Test case "Only One Task Enter Critical".
 */
void Test_ScheduleHighestPirority( void );

/**
 * @brief Function that implements a never blocking FreeRTOS task.
 */
static void prvEverRunningTask( void * pvParameters );

/**
 * @brief Function that returns which index does the xCurrentTaskHandle match.
 *        0 for T0, 1 for T1, -1 for not match.
 */
static int prvFindTaskIdx( TaskHandle_t xCurrentTaskHandle );
/*-----------------------------------------------------------*/

/**
 * @brief Handles of the tasks created in this test.
 */
static TaskHandle_t xTaskHanldes[ configNUMBER_OF_CORES ];

/**
 * @brief A flag to indicate if test case is finished.
 */
static BaseType_t xIsTestFinished = pdFALSE;
/*-----------------------------------------------------------*/

static int prvFindTaskIdx( TaskHandle_t xCurrentTaskHandle )
{
    int i = 0;
    int matchIdx = -1;

    for( i = 0; i < configNUMBER_OF_CORES; i++ )
    {
        if( xCurrentTaskHandle == xTaskHanldes[ i ] )
        {
            matchIdx = i;
            break;
        }
    }

    return matchIdx;
}
/*-----------------------------------------------------------*/

static void prvEverRunningTask( void * pvParameters )
{
    int i = 0;
    int currentTaskIdx = prvFindTaskIdx( xTaskGetCurrentTaskHandle() );
    eTaskState taskState;

    /* Silence warnings about unused parameters. */
    ( void ) pvParameters;

    for( i = 0; i < currentTaskIdx; i++ )
    {
        /* The tasks with higher priority must be created first. On the other hand,
         * If the task is not created yet, the following tasks are not created. */
        if( xTaskHanldes[ i ] == NULL )
        {
            TEST_ASSERT_TRUE( xTaskHanldes[ i ] != NULL );
            break;
        }

        taskState = eTaskGetState( xTaskHanldes[ i ] );

        /* Because priority of T0 > T1 > ... > Tn-1, the tasks, whose index is lower than currentTaskIdx,
         * have higher priority. So they must be in running state. */
        TEST_ASSERT_EQUAL_INT( eRunning, taskState );
    }

    /* If the task is the last task, then we finish the check because all tasks are checked. */
    if( currentTaskIdx == ( configNUMBER_OF_CORES - 1 ) )
    {
        xIsTestFinished = pdTRUE;
    }

    for( ; ; )
    {
        /* Always running, put asm here to avoid optimization by compiler. */
        __asm volatile ( "nop" );
    }
}
/*-----------------------------------------------------------*/

void Test_ScheduleHighestPirority( void )
{
    TickType_t xStartTick = xTaskGetTickCount();

    /* Wait other tasks. */
    while( xIsTestFinished == pdFALSE )
    {
        vTaskDelay( pdMS_TO_TICKS( 100 ) );

        if( ( xTaskGetTickCount() - xStartTick ) >= pdMS_TO_TICKS( TEST_TIMEOUT_MS ) )
        {
            break;
        }
    }

    TEST_ASSERT_TRUE( xIsTestFinished == pdTRUE );
}
/*-----------------------------------------------------------*/

/* Runs before every test, put init calls here. */
void setUp( void )
{
    int i;
    BaseType_t xTaskCreationResult;

    /* Create configNUMBER_OF_CORES - 1 low priority tasks. */
    for( i = 0; i < configNUMBER_OF_CORES; i++ )
    {
        xTaskCreationResult = xTaskCreate( prvEverRunningTask,
                                           "EverRun",
                                           configMINIMAL_STACK_SIZE * 2,
                                           NULL,
                                           configMAX_PRIORITIES - 1 - i,
                                           &( xTaskHanldes[ i ] ) );

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
        }
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief A start entry for test runner to run FR02.
 */
void vRunScheduleHighestPriorityTest( void )
{
    UNITY_BEGIN();

    RUN_TEST( Test_ScheduleHighestPirority );

    UNITY_END();
}
/*-----------------------------------------------------------*/
