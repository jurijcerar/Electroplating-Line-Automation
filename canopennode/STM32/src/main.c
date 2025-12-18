/*
 * CANopen main program file.
 *
 * This file is a template for other microcontrollers.
 *
 * @file        main.c
 * @author      Janez Paternoster
 * @copyright   2004 - 2015 Janez Paternoster
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <https://github.com/CANopenNode/CANopenNode>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * CANopenNode is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Following clarification and special exception to the GNU General Public
 * License is included to the distribution terms of CANopenNode:
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library. Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 *
 * As a special exception, the copyright holders of this library give
 * you permission to link this library with independent modules to
 * produce an executable, regardless of the license terms of these
 * independent modules, and to copy and distribute the resulting
 * executable under terms of your choice, provided that you also meet,
 * for each linked independent module, the terms and conditions of the
 * license of that module. An independent module is a module which is
 * not derived from or based on this library. If you modify this
 * library, you may extend this exception to your version of the
 * library, but you are not obliged to do so. If you do not wish
 * to do so, delete this exception statement from your version.
 */

#include "CANopen.h"
#include "application.h"
#include "hw_config.h"
#include <stdio.h>

#define TMR_TASK_INTERVAL (1000)   /* Interval of tmrTask thread in microseconds */
#define INCREMENT_1MS(var) (var++) /* Increment 1ms variable in tmrTask */

/* Global variables and objects */
volatile uint16_t CO_timer1ms = 0U; /* variable increments each millisecond */

int main(void)
{
  Set_System();

  printf("[LOG] Starting CANOpenNode ...\r\n");
  
  CO_NMT_reset_cmd_t reset = CO_RESET_NOT;
  CO_CANmodule_t CANmodule;
  CANmodule.CANbaseAddress = CAN1;

  programStart();

  // increase variable each startup. Variable is stored in EEPROM
  OD_powerOnCounter++;
  
    while (reset != CO_RESET_APP)
    {

        // CANopen communication reset - initialize CANopen objects
        CO_ReturnError_t err;
        uint16_t timer1msPrevious;

        // disable CAN and CAN interrupts
        CO_CANmodule_disable(&CANmodule);
        NVIC_DisableIRQ(TIM2_IRQn);
        communicationReset();

        // initialize CANopen & CAN interrupts
        err = CO_init(CAN1_BASE /* CAN module address */, 0x05 /* NodeID */, 20 /* bit rate */); /*global var should be on defined*/

        if (err != CO_ERROR_NO)
        {
            printf("[ERROR] Could not initialize CAN bus\r\n");
            while (1);
        }

        // Timer on
        TIM_Cmd(TIM2, ENABLE);
        // Timer interrupt on
        NVIC_EnableIRQ(TIM2_IRQn);

         // Start CAN
        CO_CANsetNormalMode(CO->CANmodule[0]);

        reset = CO_RESET_NOT;
        timer1msPrevious = CO_timer1ms;

        printf("[LOG] Entering main active loop\r\n");

        while (reset == CO_RESET_NOT)
        {
            // loop for normal program execution
            uint16_t timer1msCopy, timer1msDiff;

            timer1msCopy = CO_timer1ms;
            timer1msDiff = timer1msCopy - timer1msPrevious;
            timer1msPrevious = timer1msCopy;

            // CANopen process
            reset = CO_process(CO, timer1msDiff, NULL);

            // Nonblocking application code may go here.
            programAsync(timer1msDiff);
        }
    }

    // program exit
    printf("[LOG] Stopping CANOpenNode ...\r\n");
    // stop threads/functions
    programEnd();

    // delete objects from memory
    CO_delete(CAN1_BASE /* CAN module address */);

    // reset system
    printf("[LOG] Resetting System\r\n");
    NVIC_SystemReset();

    return 0;
}


void TIM2_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); /* Clear the Update pending bit */
    INCREMENT_1MS(CO_timer1ms);

    if (CO->CANmodule[0]->CANnormal)
    {
        bool syncWas;

        /* Process Sync and read inputs */
        syncWas = CO_process_SYNC_RPDO(CO, TMR_TASK_INTERVAL);

        /* Further I/O or nonblocking application code may go here. */
        program1ms();

        /* Write outputs */
        CO_process_TPDO(CO, syncWas, TMR_TASK_INTERVAL);
    }

}

#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE {
  USART_SendData(EVAL_COM1, (uint8_t) ch);
  
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

