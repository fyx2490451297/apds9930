#ifndef __APDS9930_H
#define __APDS9930_H

#include <stdbool.h>

/* APDS9930-INT*/
#define APDS9930_INT_PORT       GPIOA
#define APDS9930_INT_PIN        GPIO_PIN_0

/*DEBUG*/
#define DEBUG 0

/* APDS-9930 I2C address */
#define APDS9930_I2C_ADDR       0x39

/* Command register modes */
#define REPEATED_BYTE           0x80
#define AUTO_INCREMENT          0xA0
#define SPECIAL_FN              0xE0

/* Error code for returned values */
#define ERROR                   0xFF

/* Acceptable device IDs */
#define APDS9930_ID_1           0x12
#define APDS9930_ID_2           0x39

/* Misc parameters */
#define FIFO_PAUSE_TIME         30      // Wait period (ms) between FIFO reads

/* APDS-9930 registers*/
#define APDS9930_ENABLE         0x00        /*Enable of states and interrupts*/
#define APDS9930_ATIME          0x01        /*ALS ADC time*/
#define APDS9930_PTIME          0X02        /*Proximity ADC time*/
#define APDS9930_WTIME          0X03        /*Wait time*/
#define APDS9930_AILTL          0x04        /*ALS interrupt low threshold low byte*/
#define APDS9930_AILTH          0x05        /*ALS interrupt low threshold hi byte*/
#define APDS9930_AIHTL          0x06        /*ALS interrupt hi threshold low byte*/
#define APDS9930_AIHTH          0x07        /*ALS interrupt hi threshold hi byte*/
#define APDS9930_PILTL          0x08        /*Proximity interrupt low threshold low byte*/
#define APDS9930_PILTH          0x09        /*Proximity interrupt low threshold hi byte*/
#define APDS9930_PIHTL          0x0A        /*Proximity interrupt hi threshold low byte*/
#define APDS9930_PIHTH          0x0B        /*Proximity interrupt hi threshold hi byte*/
#define APDS9930_PERS           0x0C        /*Interrupt persistence filters*/
#define APDS9930_CONFIG         0x0D        /*Configuration*/
#define APDS9930_PPULSE         0x0E        /*Proximity pulse count*/
#define APDS9930_CONTROL        0x0F        /*Gain control register*/
#define APDS9930_ID             0x12        /*Device ID*/
#define APDS9930_STATUS         0x13        /*Device status*/
#define APDS9930_Ch0DATAL       0x14        /*Ch0 ADC low data register*/
#define APDS9930_Ch0DATAH       0x15        /*Ch0 ADC high data register*/
#define APDS9930_Ch1DATAL       0x16        /*Ch1 ADC low data register*/
#define APDS9930_Ch1DATAH       0x17        /*Ch1 ADC high data register*/
#define APDS9930_PDATAL         0x18        /*Proximity ADC low data register*/
#define APDS9930_PDATAH         0x19        /*Proximity ADC high data register*/
#define APDS9930_POFFSET        0x1E        /*Proximity offset register*/

/*bit fields*/
#define APDS9930_PON            0b00000001
#define APDS9930_AEN            0b00000010
#define APDS9930_PEN            0b00000100
#define APDS9930_WEN            0b00001000
#define APSD9930_AIEN           0b00010000
#define APDS9930_PIEN           0b00100000
#define APDS9930_SAI            0b01000000

/*on/off definitions*/
#define OFF                     0
#define ON                      1

/* Acceptable parameters for setMode */
#define POWER                   0
#define AMBIENT_LIGHT           1
#define PROXIMITY               2
#define WAIT                    3
#define AMBIENT_LIGHT_INT       4
#define PROXIMITY_INT           5
#define SLEEP_AFTER_INT         6
#define ALL                     7

/* LED Drive values */
#define LED_DRIVE_100MA         0
#define LED_DRIVE_50MA          1
#define LED_DRIVE_25MA          2
#define LED_DRIVE_12_5MA        3

/* Proximity Gain (PGAIN) values */
#define PGAIN_1X                0
#define PGAIN_2X                1
#define PGAIN_4X                2
#define PGAIN_8X                3

/* ALS Gain (AGAIN) values */
#define AGAIN_1X                0
#define AGAIN_8X                1
#define AGAIN_16X               2
#define AGAIN_120X              3

/* Interrupt clear values */
#define CLEAR_PROX_INT          0xE5
#define CLEAR_ALS_INT           0xE6
#define CLEAR_ALL_INTS          0xE7

/* Default values */
#define DEFAULT_ATIME           0xED
#define DEFAULT_WTIME           0xFF
#define DEFAULT_PTIME           0xFF
#define DEFAULT_PPULSE          0x08
#define DEFAULT_POFFSET         0       // 0 offset
#define DEFAULT_CONFIG          0
#define DEFAULT_PDRIVE          LED_DRIVE_100MA
#define DEFAULT_PDIODE          2
#define DEFAULT_PGAIN           PGAIN_8X
#define DEFAULT_AGAIN           AGAIN_1X
#define DEFAULT_PILT            0       // Low proximity threshold
#define DEFAULT_PIHT            50      // High proximity threshold
#define DEFAULT_AILT            0xFFFF  // Force interrupt for calibration
#define DEFAULT_AIHT            0
#define DEFAULT_PERS            0x22    // 2 consecutive prox or ALS for int.

/* ALS coefficients */
#define DF                      52
#define GA                      0.49
#define ALS_B                   1.862
#define ALS_C                   0.746
#define ALS_D                   1.291

/* State definitions */
enum {
  NOTAVAILABLE_STATE,
  NEAR_STATE,
  FAR_STATE,
  ALL_STATE
};



/* APDS9930 functions*/
void apds9930_init(void);
uint8_t apds9930_getMode(void);
void apds9930_setMode(uint8_t mode, uint8_t enable);
uint16_t apds9930_readCh0Light(void);
uint16_t apds9930_readCh1Light(void);
uint16_t apds9930_readProximity(void);
void apds9930_setLightIntLowThreshold(uint16_t threshold);
void apds9930_setLightIntHighThreshold(uint16_t threshold);
float apds9930_readAmbientLightLux(uint8_t light_gain);
void apds9930_setLEDDriver(uint8_t driver);
void apds9930_setProximityGain(uint8_t driver);
void apds9930_setAmbientLightGain(uint8_t drive);
void apds9930_setProximityDiode(uint8_t drive);
void apds9930_setProximityIntLowThreshold(uint16_t threshold);
void apds9930_setProximityIntHighThreshold(uint16_t threshold);
void apds9930_enableLightSensor(bool interrupts);
void apds9930_setAmbientLightIntEnable(uint8_t enable);
void apds9930_enablePower(void);
void apds9930_disablePower(void);
void apds9930_disableLightSensor(void);
uint8_t apds9930_getAmbientLightGain(void);
void apds9930_WriteRegData(uint8_t address, uint8_t dat);
uint8_t apds9930_readRegData(uint8_t address);
void apds9930_clearAmbientLightInt(void);
void apds9930_clearAllInts(void);
uint16_t apds9930_getLightIntLowThreshold(void);
uint16_t apds9930_getLightIntHighThreshold(void);
#endif

