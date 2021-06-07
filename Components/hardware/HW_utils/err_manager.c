/** ************************************************************* *
 * @file        err_manager.c
 * @brief       
 * 
 * @date        2021-04-27
 * @author      Quentin Bakrim (quentin.bakrim@hotmail.fr)
 * 
 * Mines Space
 * 
 * ************************************************************* **/

/* ------------------------------------------------------------- --
   includes
-- ------------------------------------------------------------- */
#include "err_manager.h"
#include "string.h"

/* ------------------------------------------------------------- --
   variables
-- ------------------------------------------------------------- */
char err_logger[32][32] = {0};
HW_status_t HW_init_log;

/* ------------------------------------------------------------- --
   private protoypes
-- ------------------------------------------------------------- */
void ERR_MNGR_incr_err(void);
void ERR_MNGR_push_log(const char* error);


/* ============================================================= ==
   private functions
== ============================================================= */
/** ************************************************************* *
 * @brief       
 * 
 * ************************************************************* **/
void ERR_MNGR_incr_err(void)
{
    err_counter++;
}

/** ************************************************************* *
 * @brief       
 * 
 * @param       error 
 * ************************************************************* **/
void ERR_MNGR_push_log(const char* error)
{
    memcpy(err_logger[err_counter], error, strlen(error));
    ERR_MNGR_incr_err();
}

/* ============================================================= ==
   public functions
== ============================================================= */
/** ************************************************************* *
 * @brief       
 * 
 * @param       HW_init 
 * @return      uint8_t 
 * ************************************************************* **/
uint8_t ERR_MNGR_HW_init(HW_status_t HW_init)
{
    uint8_t init_error = err_counter;

    if(HW_init.BMP280)  ERR_MNGR_push_log("BMP280 init");
    if(HW_init.DS3231)  ERR_MNGR_push_log("DS3231 init");
    if(HW_init.MPU6050) ERR_MNGR_push_log("MPU6050 init");
    if(HW_init.SSD1306) ERR_MNGR_push_log("SSD1306 init");
    if(HW_init.DS18B20) ERR_MNGR_push_log("DS18B20 init");

    return (err_counter - init_error);
}

