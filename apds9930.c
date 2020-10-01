#include "apds9930.h"
#include "iic.h"

/**
 * @brief       check APDS9930 Device Address
 * @param       NONE
 * @return      NONE
*/
//static void apds9930_check(void)
//{
//    if (i2c_CheckDevice(APDS9930_I2C_ADDR))
//    {
//        while (1)
//        {
//            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//            HAL_Delay(100);
//            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//            HAL_Delay(100);
//        }
//    }
//}

/**
 * @brief       write APDS9930 register data
 * @param       address   register Address
 * @param       dat   write data
 * @return      NONE
*/
void apds9930_WriteRegData(uint8_t address, uint8_t dat)
{
    i2c_Start();

    i2c_SendByte((APDS9930_I2C_ADDR << 1) & (0xFE));
    i2c_WaitAck();

    i2c_SendByte(REPEATED_BYTE | address);
    i2c_WaitAck();

    i2c_SendByte(dat);
    i2c_WaitAck();

    i2c_Stop();
}

/**
 * @brief Writes a single byte to the I2C device (no register)
 *
 * @param[in] val the 1-byte value to write to the I2C device
 * @return True if successful write operation. False otherwise.
 */
void apds9930_wireWriteByte(uint8_t val)
{
    i2c_Start();

    i2c_SendByte((APDS9930_I2C_ADDR << 1) & (0xFE));
    i2c_WaitAck();

    i2c_SendByte(val);
    i2c_WaitAck();

    i2c_Stop();
}

/**
 * @brief       read APDS9930 register data
 * @param       address   register Address
 * @return      register data
*/
uint8_t apds9930_readRegData(uint8_t address)
{
    uint8_t recv_data;

    i2c_Start();
    i2c_SendByte((APDS9930_I2C_ADDR << 1) & (0xFE));
    i2c_WaitAck();

    i2c_SendByte(AUTO_INCREMENT | address);
    i2c_WaitAck();

    i2c_Start();
    i2c_SendByte((APDS9930_I2C_ADDR << 1) | (0x01));
    i2c_WaitAck();

    recv_data = i2c_ReadByte(0);

    i2c_Stop();

    return (uint8_t)recv_data;
}

/**
 * @brief       init APDS9930
 * @param       NONE
 * @return      NONE
*/
void apds9930_init(void)
{
    uint8_t id = 0;
    
    /*init i2c gpio*/
    iic_gpio_init();

    /*i2c stop*/
    i2c_Stop();

    /*read apds9930 id*/
    id = apds9930_readRegData(APDS9930_ID);
    printf("0x%x \r\n",id);
    
    if(id != 0x39)
        while(1);

    /* Set ENABLE register to 0 (disable all features) */
    apds9930_setMode(ALL,OFF);

    /*set apds9930 registers*/

    apds9930_WriteRegData(APDS9930_ATIME, DEFAULT_ATIME);   //set ATIME
    apds9930_WriteRegData(APDS9930_WTIME, DEFAULT_WTIME);   //set WTIME
    apds9930_WriteRegData(APDS9930_PPULSE, DEFAULT_PPULSE); //set PPULSE
    apds9930_WriteRegData(APDS9930_POFFSET, DEFAULT_POFFSET); //set POFFSET
    apds9930_WriteRegData(APDS9930_CONFIG, DEFAULT_CONFIG);   //clear CONFIG

    apds9930_setLEDDriver(DEFAULT_PDRIVE);
    apds9930_setProximityGain(DEFAULT_PGAIN);
    apds9930_setAmbientLightGain(DEFAULT_AGAIN);
    apds9930_setProximityDiode(DEFAULT_PDIODE);

    // apds9930_setProximityIntLowThreshold(DEFAULT_PILT);
    // apds9930_setProximityIntHighThreshold(DEFAULT_PIHT);
    apds9930_setLightIntLowThreshold(DEFAULT_AILT);
    apds9930_setLightIntHighThreshold(DEFAULT_AIHT);

    apds9930_WriteRegData(APDS9930_PERS, DEFAULT_PERS); //Enable and Powerup apds9930

    HAL_Delay(500); //delay 20ms
}

/**
 * @brief       read APDS9930   Mode
 * @param       NONE
 * @return      enable_value
*/
uint8_t apds9930_getMode(void)
{
    uint8_t enable_value;

    enable_value = apds9930_readRegData(APDS9930_ENABLE);

    return enable_value;
}

/**
 * @brief       set APDS9930 mode
 * @param       mode which feature to enable
 * @param       enable ON (1) or OFF (0)
 * @return      enable_value
*/
void apds9930_setMode(uint8_t mode, uint8_t enable)
{
    uint8_t reg_val;

    reg_val = apds9930_getMode();

    if(reg_val == ERROR)
        while(1);

    enable = enable & 0x01;

    if(mode >= 0 && mode <= 6)
    {
        if(enable){
            reg_val |= (1<<mode);
        }else{
            reg_val &= ~(1<<mode);
        }
    } else if(mode == ALL)
    {
        if(enable){
            reg_val = 0x7F;
        } else {
            reg_val = 0x00;
        }
    }

    apds9930_WriteRegData(APDS9930_ENABLE,reg_val);

}

/**
 * @brief       Starts the light (Ambient/IR) sensor on the APDS-9930
 * @param       interrupts true to enable hardware interrupt on high or low lighte
 * @return      NONE
*/
void apds9930_enableLightSensor(bool interrupts)
{
    apds9930_setAmbientLightGain(DEFAULT_AGAIN);

    if(interrupts){
        apds9930_setAmbientLightIntEnable(1);
    } else {
        apds9930_setAmbientLightIntEnable(0);
    }

    apds9930_enablePower();

    apds9930_setMode(AMBIENT_LIGHT,1);
}

/**
 * @brief       Ends the light sensor on the APDS-9930
 * @param       NONE
 * @return      NONE
*/
void apds9930_disableLightSensor(void)
{
    apds9930_setAmbientLightIntEnable(0);

    apds9930_setMode(AMBIENT_LIGHT,0);
}


/**
 * @brief       Ends the light sensor on the APDS-9930
 * @param       NONE
 * @return      NONE
*/
uint8_t apds9930_getAmbientLightGain(void)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_CONTROL);

    val &= 0x03;

    return val;
}


/**
 * @brief       read APDS9930   Ch0 light
 * @param       NONE
 * @return      light
*/
uint16_t apds9930_readCh0Light(void)
{
    uint8_t val_byte[2];

    val_byte[0] = apds9930_readRegData(APDS9930_Ch0DATAL);
    val_byte[1] = apds9930_readRegData(APDS9930_Ch0DATAH);

    return (uint16_t)((val_byte[0]) + (uint16_t)(val_byte[1]*256));
}

/**
 * @brief       read APDS9930   Ch1 light
 * @param       NONE
 * @return      light
*/
uint16_t apds9930_readCh1Light(void)
{
    uint8_t val_byte[2];

    val_byte[0] = apds9930_readRegData(APDS9930_Ch1DATAL);
    val_byte[1] = apds9930_readRegData(APDS9930_Ch1DATAH);

    return (uint16_t)((val_byte[0]) + (uint16_t)(val_byte[1]*256));
}

/**
 * @brief       read APDS9930   Proximity
 * @param       NONE
 * @return      Proximity
*/
uint16_t apds9930_readProximity(void)
{
    uint8_t val_byte[2];

    val_byte[0] = apds9930_readRegData(APDS9930_PDATAL);
    val_byte[1] = apds9930_readRegData(APDS9930_PDATAH);

    return (uint16_t)(val_byte[1] + (uint16_t)(val_byte[0] << 8));
}

/**
 * @brief       Sets the low threshold for ambient light interrupts
 * @param       threshold  interrupts threshold
 * @return      NONE
*/
void apds9930_setLightIntLowThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;

    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    

    apds9930_WriteRegData(APDS9930_AILTL, val_low);
    apds9930_WriteRegData(APDS9930_AILTH, val_high);
}

/**
 * @brief       Sets the high threshold for ambient light interrupts
 * @param       threshold  interrupts threshold
 * @return      NONE
*/
void apds9930_setLightIntHighThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;

    val_low = (threshold & 0x00FF);
    val_high = (threshold & 0xFF00) >> 8;

    apds9930_WriteRegData(APDS9930_AIHTL, val_low);
    apds9930_WriteRegData(APDS9930_AIHTH, val_high);
}

/**
 * @brief       get light
 * @param       light_gain:0-->AGAIN_1X 1-->AGAIN_8X  2-->AGAIN_16X  3-->AGAIN_120X
 * @return      light value
*/
float apds9930_readAmbientLightLux(uint8_t light_gain)
{
    uint16_t ch0, ch1;
    uint8_t x[4] = {1, 8, 16, 120};
    float ALSIT = 2.73f * (256 - DEFAULT_ATIME);
    float iac;
    float lpc;

    ch0 = apds9930_readCh0Light();
    ch1 = apds9930_readCh1Light();

    if ((ch0 - ALS_B * ch1) > (ALS_C * ch0 - ALS_D * ch1))
    {
        iac = ch0 - ALS_B * ch1;
    }
    else
    {
        iac = ALS_C * ch0 - ALS_D * ch1;
    }

    if (iac < 0)
        iac = 0;

    lpc = (GA * DF) / (ALSIT * x[apds9930_getAmbientLightGain()]);

    return iac * lpc;
}

/**
 * @brief       Sets the LED drive strength for proximity and ALS
 * @param       drive the value (0-3) for the LED drive strength
 * @return      NONE
*/
void apds9930_setLEDDriver(uint8_t driver)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_CONTROL);

    driver &= 0x03;
    driver = driver << 6;
    val &= 0x3F;
    val |= driver;

    apds9930_WriteRegData(APDS9930_CONTROL,val);
}

/**
 * @brief       Sets the receiver gain for proximity detection
 * @param       drive the value (0-3) for the LED drive strength
 * @return      NONE
*/
void apds9930_setProximityGain(uint8_t driver)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_CONTROL);

    driver &= 0x03;
    driver = driver << 2;
    val &= 0xF3;
    val |= driver;

    apds9930_WriteRegData(APDS9930_CONTROL,val);
}

/**
 * @brief       Sets the receiver gain for the ambient light sensor (ALS)
 * @param       drive the value (0-3) for the LED drive strength
 * @return      NONE
*/
void apds9930_setAmbientLightGain(uint8_t drive)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_CONTROL);

    drive &= 0x03;
    val &= 0xFC;
    val |= drive;

    apds9930_WriteRegData(APDS9930_CONTROL,val);
}

/**
 * @brief       Selects the proximity diode
 * @param       drive the value (0-3) for the LED drive strength
 * @return      NONE
*/
void apds9930_setProximityDiode(uint8_t drive)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_CONTROL);

    drive &= 0x03;
    drive = drive << 4;
    val &= 0xCF;
    val |= drive;

    apds9930_WriteRegData(APDS9930_CONTROL,val);
}

/**
 * @brief       Selects the proximity diode
 * @param       threshold the value for irq
 * @return      NONE
*/
//void apds9930_setProximityIntLowThreshold(uint16_t threshold)
//{
//    uint8_t lo;
//    uint8_t hi;
//    hi = threshold >> 8;
//    lo = threshold & 0x00FF;

//    apds9930_WriteRegData(APDS9930_PILTL,lo);
//    apds9930_WriteRegData(APDS9930_PILTL,hi);
//}

/**
 * @brief       Selects the proximity diode
 * @param       threshold the value for irq
 * @return      NONE
*/
//void apds9930_setProximityIntHighThreshold(uint16_t threshold)
//{
//    uint8_t lo;
//    uint8_t hi;
//    hi = threshold >> 8;
//    lo = threshold & 0x00FF;

//    apds9930_WriteRegData(APDS9930_PIHTL,lo);
//    apds9930_WriteRegData(APDS9930_PIHTH,hi);
//}


/**
 * @brief Turns ambient light interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
void apds9930_setAmbientLightIntEnable(uint8_t enable)
{
    uint8_t val;

    val = apds9930_readRegData(APDS9930_ENABLE);

    enable &= 0x01;
    enable = enable << 4;
    val &= 0xEF;
    val |= enable;  

    apds9930_WriteRegData(APDS9930_ENABLE,val);
}

/**
 * @brief Clears the ambient light interrupt
 *
 * @return True if operation completed successfully. False otherwise.
 */
void apds9930_clearAmbientLightInt(void)
{
    apds9930_wireWriteByte(CLEAR_ALS_INT);
}

/**
 * @brief Clears all interrupts
 *
 * @return True if operation completed successfully. False otherwise.
 */
void apds9930_clearAllInts(void)
{
    apds9930_wireWriteByte(CLEAR_ALL_INTS);
}

/**
 * @brief       Turn the APDS-9930 on
 * @param       NONE
 * @return      NONE
*/
void apds9930_enablePower(void)
{
    apds9930_setMode(POWER,1);
}

/**
 * @brief       Turn the APDS-9930 off
 * @param       NONE
 * @return      NONE
*/
void apds9930_disablePower(void)
{
    apds9930_setMode(POWER,0);
}

/**
 * @brief       get Light Int Low Threshold
 * @param       NONE
 * @return      threshold
*/
uint16_t apds9930_getLightIntLowThreshold(void)
{
    uint8_t val_byte[2];
    uint16_t threshold=0;
    
    val_byte[0] = apds9930_readRegData(APDS9930_AILTL);
    val_byte[1] = apds9930_readRegData(APDS9930_AILTH);
    
    threshold = (uint16_t)(val_byte[0] + (uint16_t)(val_byte[1]*256));
    
    return threshold;
}

/**
 * @brief       get Light Int High Threshold
 * @param       NONE
 * @return      threshold
*/
uint16_t apds9930_getLightIntHighThreshold(void)
{
    uint8_t val_byte[2];
    uint16_t threshold=0;
    
    val_byte[0] = apds9930_readRegData(APDS9930_AIHTL);
    val_byte[1] = apds9930_readRegData(APDS9930_AIHTH);
    
    threshold = (uint16_t)(val_byte[0] + (uint16_t)(val_byte[1]*256));
    
    return threshold;
}