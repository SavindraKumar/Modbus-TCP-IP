//! @addtogroup Main
//! @brief Initialize Modbus application and TCP Interface from main application
//! @{
//!
//****************************************************************************/
//! @file main.c
//! @brief Start of application
//! @author Savindra Kumar(savindran1989@gmail.com)
//! @bug No known bugs.
//!
//****************************************************************************/
//                           Includes
//****************************************************************************/
//standard header files
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//user defined files
#include "mbap_conf.h"
#include "mbap_user.h"

#include "../tcp_server/tcp.h"

//****************************************************************************/
//                           Defines and typedefs
//****************************************************************************/

//****************************************************************************/
//                           external variables
//****************************************************************************/

//****************************************************************************/
//                           Local variables
//****************************************************************************/

//****************************************************************************/
//                           Local Functions
//****************************************************************************/

//****************************************************************************/
//                    G L O B A L  F U N C T I O N S
//****************************************************************************/
//
//! @brief main function
//! @param[in]  None
//! @return     int
//
int main(void)
{
    mu_Init();
    tcp_Init();

    return 0;
}//end main

//****************************************************************************/
//                           L O C A L  F U N C T I O N S
//****************************************************************************/

//****************************************************************************/
//                             End of file
//****************************************************************************/
/** @}*/