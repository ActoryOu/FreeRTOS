/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/***
 * See https://www.FreeRTOS.org/coremqtt for configuration and usage instructions.
 ***/

/* Standard includes. */
#include <stdio.h>
#include <time.h>

/* Visual studio intrinsics used so the __debugbreak() function is available
 * should an assert get hit. */
#include <intrin.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"

/* TCP/IP stack includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

/* Demo logging includes. */
#include "logging.h"

/* Demo Specific configs. */
#include "demo_config.h"

#include "transport_interface.h"
#include "qualification_test.h"
#include "transport_interface_test.h"
#include "network_connection.h"
#include "platform_function.h"
#include "semphr.h"
#include "test_param_config.h"
/* Transport interface implementation include header for TLS. */
#include "using_mbedtls.h"

/**
 * @brief Each compilation unit that consumes the NetworkContext must define it.
 * It should contain a single pointer to the type of your desired transport.
 * When using multiple transports in the same compilation unit, define this pointer as void *.
 *
 * @note Transport stacks are defined in FreeRTOS-Plus/Source/Application-Protocols/network_transport.
 */
struct NetworkContext
{
    TlsTransportParams_t* pParams;
};

typedef struct TaskParam
{
    StaticSemaphore_t joinMutexBuffer;
    SemaphoreHandle_t joinMutexHandle;
    FRTestThreadFunction_t threadFunc;
    void* pParam;
    TaskHandle_t taskHandle;
} TaskParam_t;

EventGroupHandle_t xSystemEvents = NULL;
NetworkContext_t xNetworkContext;
TlsTransportParams_t xTlsTransportParams = { 0 };
NetworkContext_t xSecondNetworkContext;
TlsTransportParams_t xSecondTlsTransportParams = { 0 };
NetworkCredentials_t xNetworkCredentials = { 0 };
TransportInterface_t xTransport;

static void ThreadWrapper(void* pParam)
{
    TaskParam_t* pTaskParam = pParam;

    if ((pTaskParam != NULL) && (pTaskParam->threadFunc != NULL) && (pTaskParam->joinMutexHandle != NULL))
    {
        pTaskParam->threadFunc(pTaskParam->pParam);

        /* Give the mutex. */
        xSemaphoreGive(pTaskParam->joinMutexHandle);
    }
    free(pParam);
    vTaskDelete(NULL);
}

/*-----------------------------------------------------------*/

FRTestThreadHandle_t FRTest_ThreadCreate(FRTestThreadFunction_t threadFunc,
    void* pParam)
{
    TaskParam_t* pTaskParam = NULL;
    FRTestThreadHandle_t threadHandle = NULL;
    BaseType_t xReturned;

    pTaskParam = malloc(sizeof(TaskParam_t));
    configASSERT(pTaskParam != NULL);

    pTaskParam->joinMutexHandle = xSemaphoreCreateBinaryStatic(&pTaskParam->joinMutexBuffer);
    configASSERT(pTaskParam->joinMutexHandle != NULL);

    pTaskParam->threadFunc = threadFunc;
    pTaskParam->pParam = pParam;

    xReturned = xTaskCreate(ThreadWrapper,          /* Task code. */
        "ThreadWrapper",        /* All tasks have same name. */
        4096,                   /* Task stack size. */
        pTaskParam,             /* Where the task writes its result. */
        tskIDLE_PRIORITY,       /* Task priority. */
        &pTaskParam->taskHandle);
    configASSERT(xReturned == pdPASS);

    threadHandle = pTaskParam;

    return threadHandle;
}

/*-----------------------------------------------------------*/

int FRTest_ThreadTimedJoin(FRTestThreadHandle_t threadHandle,
    uint32_t timeoutMs)
{
    TaskParam_t* pTaskParam = threadHandle;
    BaseType_t xReturned;
    int retValue = 0;

    /* Check the parameters. */
    configASSERT(pTaskParam != NULL);
    configASSERT(pTaskParam->joinMutexHandle != NULL);

    /* Wait for the thread. */
    xReturned = xSemaphoreTake(pTaskParam->joinMutexHandle, pdMS_TO_TICKS(timeoutMs));
    if (xReturned != pdTRUE)
    {
        LogError(("Waiting thread exist failed after %u %d. Task abort.", timeoutMs, xReturned));

        /* Return negative value to indicate error. */
        retValue = -1;

        /* There may be used after free. Assert here to indicate error. */
        configASSERT(false);
    }

    free(pTaskParam);

    return retValue;
}

/*-----------------------------------------------------------*/

void FRTest_TimeDelay(uint32_t delayMs)
{
    vTaskDelay(pdMS_TO_TICKS(delayMs));
}

/*-----------------------------------------------------------*/

void* FRTest_MemoryAlloc(size_t size)
{
    return pvPortMalloc(size);
}

/*-----------------------------------------------------------*/

void FRTest_MemoryFree(void* ptr)
{
    return vPortFree(ptr);
}

/*-----------------------------------------------------------*/

NetworkConnectStatus_t prvTransportNetworkConnect(void* pNetworkContext,
    TestHostInfo_t* pHostInfo,
    void* pNetworkCredentials)
{
    /* Connect the transport network. */
    NetworkConnectStatus_t xNetStatus = NETWORK_CONNECT_FAILURE;
    NetworkCredentials_t *pxNetworkCredentials = (NetworkCredentials_t*)pNetworkCredentials;

    pxNetworkCredentials->disableSni = false;
    /* Set the credentials for establishing a TLS connection. */
    pxNetworkCredentials->pRootCa = (const unsigned char*)ECHO_SERVER_ROOT_CA;
    pxNetworkCredentials->rootCaSize = sizeof(ECHO_SERVER_ROOT_CA);
    pxNetworkCredentials->pClientCert = (const unsigned char*)TRANSPORT_CLIENT_CERTIFICATE;
    pxNetworkCredentials->clientCertSize = sizeof(TRANSPORT_CLIENT_CERTIFICATE);
    pxNetworkCredentials->pPrivateKey = (const unsigned char*)TRANSPORT_CLIENT_PRIVATE_KEY;
    pxNetworkCredentials->privateKeySize = sizeof(TRANSPORT_CLIENT_PRIVATE_KEY);

    /* Attempt to create a mutually authenticated TLS connection. */
    xNetStatus = TLS_FreeRTOS_Connect(pNetworkContext,
        pHostInfo->pHostName,
        pHostInfo->port,
        pNetworkCredentials,
        5000,
        5000);

    return xNetStatus;
}

/*-----------------------------------------------------------*/

static void prvTransportNetworkDisconnect(void* pNetworkContext)
{
    /* Disconnect the transport network. */
    TLS_FreeRTOS_Disconnect(pNetworkContext);
}

/*-----------------------------------------------------------*/

static void prvTransportTestDelay(uint32_t delayMs)
{
    /* Delay function to wait for the response from network. */
    const TickType_t xDelay = delayMs / portTICK_PERIOD_MS;
    vTaskDelay(xDelay);
}

/*-----------------------------------------------------------*/

void SetupTransportTestParam(TransportTestParam_t* pTestParam)
{
    //PkiObject_t xPrivateKey = xPkiObjectFromLabel(TLS_KEY_PRV_LABEL);
    //PkiObject_t xClientCertificate = xPkiObjectFromLabel(TLS_CERT_LABEL);
    //PkiObject_t pxRootCaChain[1] = { xPkiObjectFromLabel(TLS_ROOT_CA_CERT_LABEL) };
    TlsTransportStatus_t xTlsStatus = TLS_TRANSPORT_CONNECT_FAILURE;

    //Transport test initialization
    xNetworkContext.pParams = &xTlsTransportParams;
    xSecondNetworkContext.pParams = &xSecondTlsTransportParams;

    xTransport.pNetworkContext = &xNetworkContext;
    xTransport.send = TLS_FreeRTOS_send;
    xTransport.recv = TLS_FreeRTOS_recv;

    /* Setup pTestParam */
    pTestParam->pTransport = &xTransport;
    pTestParam->pNetworkContext = &xNetworkContext;
    pTestParam->pSecondNetworkContext = &xSecondNetworkContext;

    pTestParam->pNetworkConnect = prvTransportNetworkConnect;
    pTestParam->pNetworkDisconnect = prvTransportNetworkDisconnect;
    pTestParam->pNetworkCredentials = &xNetworkCredentials;
}

/*-----------------------------------------------------------*/

void vTransportTest(void* pvParameters)
{
    RunQualificationTest();
}

/* FreeRTOS Cellular Library init and setup cellular network registration. */
extern bool setupCellular( void );

/* The MQTT demo entry function. */
extern void vStartSimpleMQTTDemo( void );

/* The task function to setup cellular with thread ready environment. */
static void CellularDemoTask( void * pvParameters );

/*
 * Just seeds the simple pseudo random number generator.
 *
 * !!! NOTE !!!
 * This is not a secure method of generating random numbers and production
 * devices should use a true random number generator (TRNG).
 */
static void prvSRand( UBaseType_t ulSeed );

/*
 * Miscellaneous initialization including preparing the logging and seeding the
 * random number generator.
 */
static void prvMiscInitialisation( void );

/* Set the following constant to pdTRUE to log using the method indicated by the
 * name of the constant, or pdFALSE to not log using the method indicated by the
 * name of the constant.  Options include to standard out (xLogToStdout), to a disk
 * file (xLogToFile), and to a UDP port (xLogToUDP).  If xLogToUDP is set to pdTRUE
 * then UDP messages are sent to the IP address configured as the UDP logging server
 * address (see the configUDP_LOGGING_ADDR0 definitions in FreeRTOSConfig.h) and
 * the port number set by configPRINT_PORT in FreeRTOSConfig.h. */
const BaseType_t xLogToStdout = pdTRUE, xLogToFile = pdFALSE, xLogToUDP = pdFALSE;

/* Used by the pseudo random number generator. */
static UBaseType_t ulNextRand;
/*-----------------------------------------------------------*/

int main( void )
{
    /***
     * See https://www.FreeRTOS.org/iot-device-shadow for configuration and usage instructions.
     ***/

    /* Miscellaneous initialization including preparing the logging and seeding
     * the random number generator. */
    prvMiscInitialisation();

    /* Start the RTOS scheduler. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks to be created.  See the memory management section on the
     * FreeRTOS web site for more details (this is standard text that is not
     * really applicable to the Win32 simulator port). */
    for( ; ; )
    {
        __debugbreak();
    }
}
/*-----------------------------------------------------------*/

/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
 * events are only received if implemented in the MAC driver. */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
    ( void ) eNetworkEvent;
}
/*-----------------------------------------------------------*/

void vAssertCalled( const char * pcFile,
                    uint32_t ulLine )
{
    volatile uint32_t ulBlockVariable = 0UL;
    volatile char * pcFileName = ( volatile char * ) pcFile;
    volatile uint32_t ulLineNumber = ulLine;

    ( void ) pcFileName;
    ( void ) ulLineNumber;

    printf( "vAssertCalled( %s, %u\n", pcFile, ulLine );

    /* Setting ulBlockVariable to a non-zero value in the debugger will allow
     * this function to be exited. */
    taskDISABLE_INTERRUPTS();
    {
        while( ulBlockVariable == 0UL )
        {
            __debugbreak();
        }
    }
    taskENABLE_INTERRUPTS();
}
/*-----------------------------------------------------------*/

UBaseType_t uxRand( void )
{
    const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;

    /*
     * Utility function to generate a pseudo random number.
     *
     * !!!NOTE!!!
     * This is not a secure method of generating a random number.  Production
     * devices should use a True Random Number Generator (TRNG).
     */
    ulNextRand = ( ulMultiplier * ulNextRand ) + ulIncrement;
    return( ( int ) ( ulNextRand >> 16UL ) & 0x7fffUL );
}
/*-----------------------------------------------------------*/

static void prvSRand( UBaseType_t ulSeed )
{
    /* Utility function to seed the pseudo random number generator. */
    ulNextRand = ulSeed;
}

/*-----------------------------------------------------------*/

static void CellularDemoTask( void * pvParameters )
{
    bool retCellular = true;

    ( void ) pvParameters;
    /* Setup cellular. */
    retCellular = setupCellular();

    if( retCellular == false )
    {
        configPRINTF( ( "Cellular failed to initialize.\r\n" ) );
    }

    /* Stop here if we fail to initialize cellular. */
    configASSERT( retCellular == true );

    /* Run the MQTT demo. */
    /* Demos that use the network are created after the network is
     * up. */
    LogInfo( ( "---------STARTING DEMO---------\r\n" ) );
    vStartSimpleMQTTDemo();

    vTaskDelete( NULL );
}

/*-----------------------------------------------------------*/

void prvQualificationTask( void* pvParameters )
{
    bool retCellular = true;

    (void)pvParameters;

    /* Setup cellular. */
    retCellular = setupCellular();

    if (retCellular == false)
    {
        configPRINTF(("Cellular failed to initialize.\r\n"));
    }

    /* Stop here if we fail to initialize cellular. */
    configASSERT(retCellular == true);

    /* Run the MQTT demo. */
    /* Demos that use the network are created after the network is
     * up. */
    LogInfo(("---------STARTING DEMO---------\r\n"));

    RunQualificationTest();

    vTaskDelete(NULL);
}

static void prvMiscInitialisation( void )
{
    vLoggingInit( xLogToStdout, xLogToFile, xLogToUDP, 0U, configPRINT_PORT );

    /* FreeRTOS Cellular Library init needs thread ready environment.
     * CellularDemoTask invoke setupCellular to init FreeRTOS Cellular Library and register network.
     * Then it runs the MQTT demo. */
    xTaskCreate( prvQualificationTask,         /* Function that implements the task. */
                 "Qualification",              /* Text name for the task - only used for debugging. */
                 democonfigDEMO_STACKSIZE,     /* Size of stack (in words, not bytes) to allocate for the task. */
                 NULL,                         /* Task parameter - not used in this case. */
                 democonfigDEMO_PRIORITY,      /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
                 NULL );                       /* Used to pass out a handle to the created task - not used in this case. */
}
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) || ( ipconfigDHCP_REGISTER_HOSTNAME == 1 )

    const char * pcApplicationHostnameHook( void )
    {
        /* Assign the name "FreeRTOS" to this network node.  This function will
         * be called during the DHCP: the machine will be registered with an IP
         * address plus this name. */
        return mainHOST_NAME;
    }

#endif
/*-----------------------------------------------------------*/

#if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 )

    BaseType_t xApplicationDNSQueryHook( const char * pcName )
    {
        BaseType_t xReturn;

        /* Determine if a name lookup is for this node.  Two names are given
         * to this node: that returned by pcApplicationHostnameHook() and that set
         * by mainDEVICE_NICK_NAME. */
        if( _stricmp( pcName, pcApplicationHostnameHook() ) == 0 )
        {
            xReturn = pdPASS;
        }
        else if( _stricmp( pcName, mainDEVICE_NICK_NAME ) == 0 )
        {
            xReturn = pdPASS;
        }
        else
        {
            xReturn = pdFAIL;
        }

        return xReturn;
    }

#endif /* if ( ipconfigUSE_LLMNR != 0 ) || ( ipconfigUSE_NBNS != 0 ) */

/*-----------------------------------------------------------*/

/*
 * Callback that provides the inputs necessary to generate a randomized TCP
 * Initial Sequence Number per RFC 6528.  THIS IS ONLY A DUMMY IMPLEMENTATION
 * THAT RETURNS A PSEUDO RANDOM NUMBER SO IS NOT INTENDED FOR USE IN PRODUCTION
 * SYSTEMS.
 */
extern uint32_t ulApplicationGetNextSequenceNumber( uint32_t ulSourceAddress,
                                                    uint16_t usSourcePort,
                                                    uint32_t ulDestinationAddress,
                                                    uint16_t usDestinationPort )
{
    ( void ) ulSourceAddress;
    ( void ) usSourcePort;
    ( void ) ulDestinationAddress;
    ( void ) usDestinationPort;

    return uxRand();
}
/*-----------------------------------------------------------*/

/*
 * Set *pulNumber to a random number, and return pdTRUE. When the random number
 * generator is broken, it shall return pdFALSE.
 *
 * THIS IS ONLY A DUMMY IMPLEMENTATION THAT RETURNS A PSEUDO RANDOM NUMBER SO IS
 * NOT INTENDED FOR USE IN PRODUCTION SYSTEMS.
 */
BaseType_t xApplicationGetRandomNumber( uint32_t * pulNumber )
{
    *pulNumber = uxRand();
    return pdTRUE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
/*-----------------------------------------------------------*/
