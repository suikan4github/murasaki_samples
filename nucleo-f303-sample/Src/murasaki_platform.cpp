/**
 * @file murasaki_platform.cpp
 *
 * @date 2018/05/20
 * @author takemasa
 * @brief A glue file between the user application and HAL/RTOS.
 */

// Include the definition created by CubeMX.
#include <murasaki_platform.hpp>
#include "main.h"

// Include the murasaki class library.
#include "murasaki.hpp"

// Include the prototype  of functions of this file.

/* -------------------- PLATFORM Type and classes -------------------------- */

/* -------------------- PLATFORM VARIABLES-------------------------- */

// Essential definition.
// Do not delete

murasaki::Platform murasaki::platform;
murasaki::Debugger * murasaki::debugger;

/* ------------------------ PERIPHERAL ----------------------------- */

/*
 * Platform dependent peripheral declaration.
 *
 * The variables here are defined at the top of the main.c.
 * Only the variable needed by the InitPlatform() are declared here
 * as external symbols.
 *
 * The declaration here is user project dependent.
 */
extern UART_HandleTypeDef huart2;

// Following block is just sample.
#if 0
extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi4;
extern I2C_HandleTypeDef hi2c2;
extern UART_HandleTypeDef huart3;
extern I2C_HandleTypeDef hi2c1;
#endif

/* -------------------- PLATFORM ALGORITHM ------------------------- */


void InitPlatform()
{
    // UART device setting for console interface.
    // On Nucleo, the port connected to the USB port of ST-Link is
    // referred here.
    murasaki::platform.uart_console = new murasaki::DebuggerUart(&huart2);
    // UART is used for logging port.
    // At least one logger is needed to run the debugger class.
    murasaki::platform.logger = new murasaki::UartLogger(murasaki::platform.uart_console);
    // Setting the debugger
    murasaki::debugger = new murasaki::Debugger(murasaki::platform.logger);
    // Set the debugger as AutoRePrint mode, for the easy operation.
    murasaki::debugger->AutoRePrint();  // type any key to show history.


    // For demonstration, one GPIO LED port is reserved.
    // The port and pin names are fined by CubeMX.
    murasaki::platform.led = new murasaki::BitOut(LD3_GPIO_Port, LD3_Pin);


    // Following block is just for sample.
#if 0
    // For demonstration of the serial communication.
    murasaki::platform.uart = new murasaki::Uart(&huart2);

    murasaki::platform.i2cSlave = new murasaki::I2cSlave(&hi2c2);
    // For demonstration of master and slave SPI
    murasaki::platform.spiMaster = new murasaki::SpiMaster(&hspi1);
    murasaki::platform.spiSlave = new murasaki::SpiSlave(&hspi4);
#endif

}

void ExecPlatform()
{


    murasaki::platform.count = 0;

    // Loop forever
    while (true) {

        // Blinking LED
        murasaki::platform.led->Toggle();

        // Message Out
        murasaki::debugger->Printf("Hello %d \r\n", murasaki::platform.count);

        murasaki::platform.count++;
        // Hold on
        murasaki::Sleep(static_cast<murasaki::WaitMilliSeconds>(700));

    }
}

/* ------------------------- UART ---------------------------------- */

/**
 * @brief Essential to sync up with UART.
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param huart
 * @details
 * This is called from inside of HAL when an UART transmission done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default TX interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::Uart::TransmissionCompleteCallback() function.
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef * huart)
{
    // Poll all uart tx related interrupt receivers.
    // If hit, return. If not hit,check next.
    if (murasaki::platform.uart_console->TransmitCompleteCallback(huart))
        return;

}

/**
 * @brief Essential to sync up with UART.
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param huart
 * @details
 * This is called from inside of HAL when an UART receive done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default RX interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::Uart::ReceiveCompleteCallback() function.
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart)
{
    // Poll all uart rx related interrupt receivers.
    // If hit, return. If not hit,check next.
    if (murasaki::platform.uart_console->ReceiveCompleteCallback(huart))
        return;

}

/**
 * @brief Optional error handling of UART
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param huart
 * @details
 * This is called from inside of HAL when an UART error interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default error interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::Uart::HandleError() function.
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    // Poll all uart error related interrupt receivers.
    // If hit, return. If not hit,check next.
    if (murasaki::platform.uart_console->HandleError(huart))
        return;

}

/* -------------------------- SPI ---------------------------------- */


#ifdef HAL_SPI_MODULE_ENABLED

/**
 * @brief Essential to sync up with SPI.
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param hspi
 * @details
 * This is called from inside of HAL when an SPI transfer done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default TX/RX interrupt call back.
 *
 * In this call back, the SPI device handle have to be passed to the
 * murasaki::Spi::TransmitAndReceiveCompleteCallback () function.
 */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
    // Poll all SPI TX RX related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
     if ( murasaki::platform.spi1->TransmitAndReceiveCompleteCallback(hspi) )
     return;
#endif
}

/**
 * @brief Optional error handling of SPI
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param hspi
 * @details
 * This is called from inside of HAL when an SPI error interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default error interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::Uart::HandleError() function.
 */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef * hspi) {
    // Poll all SPI error interrupt related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
     if ( murasaki::platform.spi1->HandleError(hspi) )
     return;
#endif
}

#endif

/* -------------------------- I2C ---------------------------------- */


#ifdef HAL_I2C_MODULE_ENABLED

/**
 * @brief Essential to sync up with I2C.
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param hi2c
 * @details
 * This is called from inside of HAL when an I2C transmission done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default TX interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::I2c::TransmitCompleteCallback() function.
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef * hi2c)
{
    // Poll all I2C master tx related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
    if (murasaki::platform.i2c_master->TransmitCompleteCallback(hi2c))
        return;
#endif
}

/**
 * @brief Essential to sync up with I2C.
 * @param hi2c
 * @details
 * This is called from inside of HAL when an I2C receive done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default RX interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::Uart::ReceiveCompleteCallback() function.
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef * hi2c) {
    // Poll all I2C master rx related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
    if (murasaki::platform.i2c_master->ReceiveCompleteCallback(hi2c))
    return;
#endif
}
/**
 * @brief Essential to sync up with I2C.
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param hi2c
 * @details
 * This is called from inside of HAL when an I2C transmission done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default TX interrupt call back.
 *
 * In this call back, the I2C slave device handle have to be passed to the
 * murasaki::I2cSlave::TransmitCompleteCallback() function.
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef * hi2c)
{
    // Poll all I2C master tx related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
    if (murasaki::platform.i2c_slave->TransmitCompleteCallback(hi2c))
    return;
#endif
}

/**
 * @brief Essential to sync up with I2C.
 * @param hi2c
 * @details
 * This is called from inside of HAL when an I2C receive done interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default RX interrupt call back.
 *
 * In this call back, the I2C slave device handle have to be passed to the
 * murasaki::I2cSlave::ReceiveCompleteCallback() function.
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef * hi2c) {
    // Poll all I2C master rx related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
    if (murasaki::platform.i2c_slave->ReceiveCompleteCallback(hi2c))
    return;
#endif
}

/**
 * @brief Optional error handling of I2C
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param hi2c
 * @details
 * This is called from inside of HAL when an I2C error interrupt is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default error interrupt call back.
 *
 * In this call back, the uart device handle have to be passed to the
 * murasaki::I2c::HandleError() function.
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef * hi2c) {
    // Poll all I2C master error related interrupt receivers.
    // If hit, return. If not hit,check next.
#if 0
    if (murasaki::platform.i2c_master->HandleError(hi2c))
    return;
#endif
}

#endif

/* -------------------------- GPIO ---------------------------------- */



/**
 * @brief Optional interrupt handling of EXTI
 * @ingroup MURASAKI_PLATFORM_GROUP
 * @param GPIO_P Pin number from 0 to 31
 * @details
 * This is called from inside of HAL when an EXTI is accepted.
 *
 * STM32Cube HAL has same name function internally.
 * That function is invoked whenever an relevant interrupt happens.
 * In the other hand, that function is declared as weak bound.
 * As a result, this function overrides the default error interrupt call back.
 *
 * The GPIO_P is the number of Pin. If programmmer set the pin name by CubeMX as FOO, the
 * macro to identify that EXTI is FOO_PIN
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
#if 0
    if ( B1_Pin == GPIO_Pin) {
        // release the waiting task
        if (murasaki::platform.sync_with_button != nullptr)
            murasaki::platform.sync_with_button->Release();
    }
#endif
}

/* ------------------ ASSERTION AND ERROR -------------------------- */

void CustomAssertFailed(uint8_t* file, uint32_t line)
                        {
    murasaki::debugger->Printf("Wrong parameters value: file %s on line %d\r\n",
                               file, line);
}

void CustomDefaultHandler() {
    // Call debugger's post mortem processing. Never return again.
    murasaki::debugger->DoPostMortem();
}

/* ------------------ User Function -------------------------- */
