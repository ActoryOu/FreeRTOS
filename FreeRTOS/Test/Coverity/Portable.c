/*
 * FreeRTOS <DEVELOPMENT BRANCH>
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 */

/* Include standard libraries */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "list.h"

volatile BaseType_t xInsideInterrupt = pdFALSE;
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];

/* Provide a main function for the build to succeed. */
int main()
{
    return 0;
}
/*-----------------------------------------------------------*/

void * pvPortMalloc( size_t xWantedSize )
{
    ( void ) xWantedSize;
    return NULL;
}
/*-----------------------------------------------------------*/

void vPortFree( void * pv )
{
    ( void ) pv;
}
/*-----------------------------------------------------------*/

void vApplicationDaemonTaskStartupHook( void )
{
}
/*-----------------------------------------------------------*/

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    ( void ) ppxTimerTaskTCBBuffer;
    ( void ) ppxTimerTaskStackBuffer;
    ( void ) pulTimerTaskStackSize;
}
/*-----------------------------------------------------------*/

void vPortDeleteThread( void * pvTaskToDelete )
{
    ( void ) pvTaskToDelete;
}

void vApplicationIdleHook( void )
{
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
}
/*-----------------------------------------------------------*/

uint32_t ulGetRunTimeCounterValue( void )
{
    return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
    return pdPASS;
}
/*-----------------------------------------------------------*/

void vPortEnterCritical( void )
{
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void )
{
}
/*-----------------------------------------------------------*/

StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                     TaskFunction_t pxCode,
                                     void * pvParameters )
{
    ( void ) pxTopOfStack;
    ( void ) pxCode;
    ( void ) pvParameters;

    return NULL;
}
/*-----------------------------------------------------------*/

void vPortGenerateSimulatedInterrupt()
{
}
/*-----------------------------------------------------------*/

void vPortCloseRunningThread( void * pvTaskToDelete,
                              volatile BaseType_t * pxPendYield )
{
    ( void ) pvTaskToDelete;
    ( void ) pxPendYield;
}
/*-----------------------------------------------------------*/

void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    ( void ) ppxIdleTaskTCBBuffer;
    ( void ) ppxIdleTaskStackBuffer;
    ( void ) pulIdleTaskStackSize;
}
/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
}
/*-----------------------------------------------------------*/
