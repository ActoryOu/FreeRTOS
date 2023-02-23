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
 * @file only_one_task_enter_suspendall.c
 * @brief Only one task shall be able to enter the section protected by vTaskSuspendAll/xTaskResumeAll.
 *
 * Procedure:
 *   - Create ( num of cores ) tasks.
 *   - All tasks increase the counter for TASK_INCREASE_COUNTER_TIMES times.
 *     - Call vTaskSuspendAll.
 *     - Increase the counter for TASK_INCREASE_COUNTER_TIMES times.
 *     - Call xTaskResumeAll.
 * Expected:
 *   - Counter should be increased by COUNTER_MAX for a task in its loop.
 *   - Counter should be ( num of cores * COUNTER_MAX ) in the end.
 */

/* Kernel includes. */
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */

#include "unity.h"    /* unit testing support functions */
/*-----------------------------------------------------------*/

/**
 * @brief As time of loop for task to increase counter.
 */
#define TASK_INCREASE_COUNTER_TIMES    ( 10000 )

/**
 * @brief Timeout value to stop test.
 */
#define TEST_TIMEOUT_MS                ( 10000 )
/*-----------------------------------------------------------*/

#if ( configNUMBER_OF_CORES < 2 )
    #error This test is for FreeRTOS SMP and therefore, requires at least 2 cores.
#endif /* if ( configNUMBER_OF_CORES < 2 ) */

#if ( configRUN_MULTIPLE_PRIORITIES != 1 )
    #error test_config.h must be included at the end of FreeRTOSConfig.h.
#endif /* if ( configRUN_MULTIPLE_PRIORITIES != 1 ) */

#if ( configMAX_PRIORITIES <= 1 )
    #error configMAX_PRIORITIES must be larger than 1 to avoid scheduling idle tasks unexpectly.
#endif /* if ( configMAX_PRIORITIES <= 2 ) */
/*-----------------------------------------------------------*/

/**
 * @brief Test case "Only One Task Enter SuspendAll".
 */
static void Test_OnlyOneTaskEnterSuspendAll( void );

/**
 * @brief Task function to increase counter then keep delaying.
 */
static void prvTaskIncCounter( void * pvParameters );

/**
 * @brief Function to increase counter in critical section.
 */
static void loopIncCounter( void );
/*-----------------------------------------------------------*/

#if ( configNUMBER_OF_CORES < 2 )
    #error This test is for FreeRTOS SMP and therefore, requires at least 2 cores.
#endif /* if configNUMBER_OF_CORES != 2 */
/*-----------------------------------------------------------*/

/**
 * @brief Handles of the tasks created in this test.
 */
static TaskHandle_t xTaskHanldes[ configNUMBER_OF_CORES ];

/**
 * @brief Counter for all tasks to increase.
 */
static BaseType_t xTaskCounter = 0;
/*-----------------------------------------------------------*/

static void Test_OnlyOneTaskEnterSuspendAll( void )
{
    TickType_t xStartTick = xTaskGetTickCount();

    /* Delay for other cores to run tasks. */
    vTaskDelay( pdMS_TO_TICKS( 10 ) );

    /* Wait other tasks. */
    while( xTaskCounter < configNUMBER_OF_CORES * TASK_INCREASE_COUNTER_TIMES )
    {
        vTaskDelay( pdMS_TO_TICKS( 10 ) );

        if( ( xTaskGetTickCount() - xStartTick ) >= pdMS_TO_TICKS( TEST_TIMEOUT_MS ) )
        {
            break;
        }
    }

    TEST_ASSERT_EQUAL_INT( xTaskCounter, configNUMBER_OF_CORES * TASK_INCREASE_COUNTER_TIMES );
}

/*-----------------------------------------------------------*/

static void loopIncCounter( void )
{
    BaseType_t xTempTaskCounter = 0;
    BaseType_t xIsTestPass = pdTRUE;
    int i;

    vTaskSuspendAll();

    xTempTaskCounter = xTaskCounter;

    for( i = 0; i < TASK_INCREASE_COUNTER_TIMES; i++ )
    {
        xTaskCounter++;
        xTempTaskCounter++;

        if( xTaskCounter != xTempTaskCounter )
        {
            xIsTestPass = pdFALSE;
        }
    }

    xTaskResumeAll();

    TEST_ASSERT_TRUE( xIsTestPass );
}
/*-----------------------------------------------------------*/

static void prvTaskIncCounter( void * pvParameters )
{
    ( void ) pvParameters;

    loopIncCounter();

    while( pdTRUE )
    {
        vTaskDelay( pdMS_TO_TICKS( 100 ) );
    }
}
/*-----------------------------------------------------------*/

/* Runs before every test, put init calls here. */
void setUp( void )
{
    int i;
    BaseType_t xTaskCreationResult;

    /* Create configNUMBER_OF_CORES low priority tasks. */
    for( i = 0; i < configNUMBER_OF_CORES; i++ )
    {
        xTaskCreationResult = xTaskCreate( prvTaskIncCounter,
                                           "IncCounter",
                                           configMINIMAL_STACK_SIZE,
                                           NULL,
                                           configMAX_PRIORITIES - 1,
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
 * @brief A start entry for test runner to run FR10.
 */
void vRunOnlyOneTaskEnterSuspendAll( void )
{
    UNITY_BEGIN();

    RUN_TEST( Test_OnlyOneTaskEnterSuspendAll );

    UNITY_END();
}
