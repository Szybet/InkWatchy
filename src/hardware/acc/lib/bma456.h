/*

****************************************************************************
    Copyright (C) 2017 - 2018 Bosch Sensortec GmbH

    File : bma456.h

    Date: 12 Oct 2017

    Revision : 1.1.3 $

    Usage: Sensor Driver for BMA456 sensor

****************************************************************************

    Disclaimer

    Common:
    Bosch Sensortec products are developed for the consumer goods industry.
    They may only be used within the parameters of the respective valid
    product data sheet.  Bosch Sensortec products are provided with the
    express understanding that there is no warranty of fitness for a
    particular purpose.They are not fit for use in life-sustaining,
    safety or security sensitive systems or any system or device
    that may lead to bodily harm or property damage if the system
    or device malfunctions. In addition,Bosch Sensortec products are
    not fit for use in products which interact with motor vehicle systems.
    The resale and or use of products are at the purchasers own risk and
    his own responsibility. The examination of fitness for the intended use
    is the sole responsibility of the Purchaser.

    The purchaser shall indemnify Bosch Sensortec from all third party
    claims, including any claims for incidental, or consequential damages,
    arising from any product use not covered by the parameters of
    the respective valid product data sheet or not approved by
    Bosch Sensortec and reimburse Bosch Sensortec for all costs in
    connection with such claims.

    The purchaser must monitor the market for the purchased products,
    particularly with regard to product safety and inform Bosch Sensortec
    without delay of all security relevant incidents.

    Engineering Samples are marked with an asterisk (*) or (e).
    Samples may vary from the valid technical specifications of the product
    series. They are therefore not intended or fit for resale to third
    parties or for use in end products. Their sole purpose is internal
    client testing. The testing of an engineering sample may in no way
    replace the testing of a product series. Bosch Sensortec assumes
    no liability for the use of engineering samples.
    By accepting the engineering samples, the Purchaser agrees to indemnify
    Bosch Sensortec from all claims arising from the use of engineering
    samples.

    Special:
    This software module (hereinafter called "Software") and any information
    on application-sheets (hereinafter called "Information") is provided
    free of charge for the sole purpose to support your application work.
    The Software and Information is subject to the following
    terms and conditions:

    The Software is specifically designed for the exclusive use for
    Bosch Sensortec products by personnel who have special experience
    and training. Do not use this Software if you do not have the
    proper experience or training.

    This Software package is provided `` as is `` and without any expressed
    or implied warranties,including without limitation, the implied warranties
    of merchantability and fitness for a particular purpose.

    Bosch Sensortec and their representatives and agents deny any liability
    for the functional impairment
    of this Software in terms of fitness, performance and safety.
    Bosch Sensortec and their representatives and agents shall not be liable
    for any direct or indirect damages or injury, except as
    otherwise stipulated in mandatory applicable law.

    The Information provided is believed to be accurate and reliable.
    Bosch Sensortec assumes no responsibility for the consequences of use
    of such Information nor for any infringement of patents or
    other rights of third parties which may result from its use.
    No license is granted by implication or otherwise under any patent or
    patent rights of Bosch. Specifications mentioned in the Information are
    subject to change without notice.
**************************************************************************/
/*! \file bma456.h
    \brief Sensor Driver for BMA456 sensor */
    #ifndef BMA456_H
    #define BMA456_H
    
    #ifdef __cplusplus
    extern "C" {
    #endif
    #include "bma4.h"
    
    /**\name Chip ID of BMA456 sensor */
    #define	BMA456_CHIP_ID				UINT8_C(0x16)
    
    /**\name Sensor feature size */
    #define BMA456_FEATURE_SIZE			UINT8_C(64)
    #define BMA456_ANYMOTION_EN_LEN			UINT8_C(2)
    #define BMA456_RD_WR_MIN_LEN			UINT8_C(2)
    
    /**\name Feature offset address */
    #define BMA456_ANY_NO_MOTION_OFFSET		UINT8_C(0x00)
    #define BMA456_STEP_CNTR_OFFSET			UINT8_C(0x36)
    #define BMA456_STEP_CNTR_PARAM_OFFSET	UINT8_C(0x04)
    #define BMA456_WAKEUP_OFFSET			UINT8_C(0x38)
    #define BMA456_WRIST_TILT_OFFSET		UINT8_C(0x3A)
    #define BMA456_CONFIG_ID_OFFSET			UINT8_C(0x3C)
    #define BMA456_AXES_REMAP_OFFSET		UINT8_C(0x3E)
    
    /**************************************************************/
    /**\name	Remap Axes */
    /**************************************************************/
    #define BMA456_X_AXIS_MASK			UINT8_C(0x03)
    #define BMA456_X_AXIS_SIGN_MASK			UINT8_C(0x04)
    #define BMA456_Y_AXIS_MASK			UINT8_C(0x18)
    #define BMA456_Y_AXIS_SIGN_MASK			UINT8_C(0x20)
    #define BMA456_Z_AXIS_MASK			UINT8_C(0xC0)
    #define BMA456_Z_AXIS_SIGN_MASK			UINT8_C(0x01)
    
    /**************************************************************/
    /**\name	Step Counter & Detector */
    /**************************************************************/
    /**\name Step counter enable macros */
    #define BMA456_STEP_CNTR_EN_POS			UINT8_C(4)
    #define BMA456_STEP_CNTR_EN_MSK			UINT8_C(0x10)
    #define BMA456_ACTIVITY_EN_MSK			UINT8_C(0x20)
    
    /**\name Step counter watermark macros */
    #define BMA456_STEP_CNTR_WM_MSK			UINT16_C(0x03FF)
    
    /**\name Step counter reset macros */
    #define BMA456_STEP_CNTR_RST_POS		UINT8_C(2)
    #define BMA456_STEP_CNTR_RST_MSK		UINT8_C(0x04)
    
    /**\name Step detector enable macros */
    #define BMA456_STEP_DETECTOR_EN_POS		UINT8_C(3)
    #define BMA456_STEP_DETECTOR_EN_MSK		UINT8_C(0x08)
    
    /**\name Wrist-tilt enable macros */
    #define BMA456_WRIST_TILT_EN_MSK		UINT8_C(0x01)
    
    /**\name Wakeup enable macros */
    #define BMA456_WAKEUP_EN_MSK			UINT8_C(0x01)
    
    /**\name Wake up sensitivity macros */
    #define BMA456_WAKEUP_SENS_POS			UINT8_C(1)
    #define BMA456_WAKEUP_SENS_MSK			UINT8_C(0x0E)
    
    /**\name Tap selection macro */
    #define BMA456_TAP_SEL_POS			UINT8_C(4)
    #define BMA456_TAP_SEL_MSK			UINT8_C(0x10)
    
    /**\name step count output length*/
    #define BMA456_STEP_CNTR_DATA_SIZE		UINT16_C(4)
    
    /**************************************************************/
    /**\name	Any Motion */
    /**************************************************************/
    /**\name Any motion threshold macros */
    #define BMA456_ANY_NO_MOTION_THRES_POS		UINT8_C(0)
    #define BMA456_ANY_NO_MOTION_THRES_MSK		UINT16_C(0x07FF)
    
    /**\name Any motion selection macros */
    #define BMA456_ANY_NO_MOTION_SEL_POS		UINT8_C(3)
    #define BMA456_ANY_NO_MOTION_SEL_MSK		UINT8_C(0x08)
    
    /**\name Any motion enable macros */
    #define BMA456_ANY_NO_MOTION_AXIS_EN_POS	UINT8_C(5)
    #define BMA456_ANY_NO_MOTION_AXIS_EN_MSK	UINT8_C(0xE0)
    
    /**\name Any motion duration macros */
    #define BMA456_ANY_NO_MOTION_DUR_MSK		UINT16_C(0x1FFF)
    
    /**************************************************************/
    /**\name	User macros */
    /**************************************************************/
    
    /**\name Anymotion/Nomotion axis enable macros */
    #define BMA456_X_AXIS_EN			UINT8_C(0x01)
    #define BMA456_Y_AXIS_EN			UINT8_C(0x02)
    #define BMA456_Z_AXIS_EN			UINT8_C(0x04)
    #define BMA456_ALL_AXIS_EN			UINT8_C(0x07)
    #define BMA456_ALL_AXIS_DIS			UINT8_C(0x00)
    
    /**\name Feature enable macros for the sensor */
    #define BMA456_STEP_CNTR			UINT8_C(0x01)
    /**\name Below macros are mutually exclusive */
    #define BMA456_ANY_MOTION			UINT8_C(0x02)
    #define BMA456_NO_MOTION			UINT8_C(0x04)
    #define BMA456_ACTIVITY				UINT8_C(0x08)
    #define BMA456_WRIST_TILT			UINT8_C(0x10)
    #define BMA456_WAKEUP				UINT8_C(0x20)
    
    /**\name Interrupt status macros */
    #define BMA456_STEP_CNTR_INT			UINT8_C(0x02)
    #define BMA456_ACTIVITY_INT			UINT8_C(0x04)
    #define BMA456_WRIST_TILT_INT			UINT8_C(0x08)
    #define BMA456_WAKEUP_INT			UINT8_C(0x20)
    #define BMA456_ANY_NO_MOTION_INT		UINT8_C(0x40)
    #define BMA456_ERROR_INT			UINT8_C(0x80)
    
    /**\name Activity recognition macros */
    #define BMA456_USER_STATIONARY			UINT8_C(0x00)
    #define BMA456_USER_WALKING			UINT8_C(0x01)
    #define BMA456_USER_RUNNING			UINT8_C(0x02)
    #define BMA456_STATE_INVALID			UINT8_C(0x03)
    
    /**\name Configuration selection macros */
    #define BMA456_PHONE_CONFIG			UINT8_C(0x00)
    #define BMA456_WRIST_CONFIG			UINT8_C(0x01)
    
    /**\name Step counter parameter settings(1-25) for phone */
    #define BMA456_PHONE_SC_PARAM_1    UINT16_C(0x132)
    #define BMA456_PHONE_SC_PARAM_2    UINT16_C(0x78E6)
    #define BMA456_PHONE_SC_PARAM_3    UINT16_C(0x84)
    #define BMA456_PHONE_SC_PARAM_4    UINT16_C(0x6C9C)
    #define BMA456_PHONE_SC_PARAM_5    UINT8_C(0x07)
    #define BMA456_PHONE_SC_PARAM_6    UINT16_C(0x7564)
    #define BMA456_PHONE_SC_PARAM_7    UINT16_C(0x7EAA)
    #define BMA456_PHONE_SC_PARAM_8    UINT16_C(0x55F)
    #define BMA456_PHONE_SC_PARAM_9    UINT16_C(0xABE)
    #define BMA456_PHONE_SC_PARAM_10   UINT16_C(0x55F)
    #define BMA456_PHONE_SC_PARAM_11   UINT16_C(0xE896)
    #define BMA456_PHONE_SC_PARAM_12   UINT16_C(0x41EF)
    #define BMA456_PHONE_SC_PARAM_13   UINT8_C(0x01)
    #define BMA456_PHONE_SC_PARAM_14   UINT8_C(0x0C)
    #define BMA456_PHONE_SC_PARAM_15   UINT8_C(0x0C)
    #define BMA456_PHONE_SC_PARAM_16   UINT8_C(0x4A)
    #define BMA456_PHONE_SC_PARAM_17   UINT8_C(0xA0)
    #define BMA456_PHONE_SC_PARAM_18   UINT8_C(0x00)
    #define BMA456_PHONE_SC_PARAM_19   UINT8_C(0x0C)
    #define BMA456_PHONE_SC_PARAM_20   UINT16_C(0x3CF0)
    #define BMA456_PHONE_SC_PARAM_21   UINT16_C(0x100)
    #define BMA456_PHONE_SC_PARAM_22   UINT8_C(0x00)
    #define BMA456_PHONE_SC_PARAM_23   UINT8_C(0x00)
    #define BMA456_PHONE_SC_PARAM_24   UINT8_C(0x00)
    #define BMA456_PHONE_SC_PARAM_25   UINT8_C(0x00)
    
    /**\name Step counter parameter settings(1-25) for wrist (Default) */
    #define BMA456_WRIST_SC_PARAM_1   UINT16_C(0x12D)
    #define BMA456_WRIST_SC_PARAM_2   UINT16_C(0x7BD4)
    #define BMA456_WRIST_SC_PARAM_3   UINT16_C(0x13B)
    #define BMA456_WRIST_SC_PARAM_4   UINT16_C(0x7ADB)
    #define BMA456_WRIST_SC_PARAM_5   UINT8_C(0x04)
    #define BMA456_WRIST_SC_PARAM_6   UINT16_C(0x7B3F)
    #define BMA456_WRIST_SC_PARAM_7   UINT16_C(0x6CCD)
    #define BMA456_WRIST_SC_PARAM_8   UINT16_C(0x4C3)
    #define BMA456_WRIST_SC_PARAM_9   UINT16_C(0x985)
    #define BMA456_WRIST_SC_PARAM_10  UINT16_C(0x4C3)
    #define BMA456_WRIST_SC_PARAM_11  UINT16_C(0xE6EC)
    #define BMA456_WRIST_SC_PARAM_12  UINT16_C(0x460C)
    #define BMA456_WRIST_SC_PARAM_13  UINT8_C(0x01)
    #define BMA456_WRIST_SC_PARAM_14  UINT8_C(0x27)
    #define BMA456_WRIST_SC_PARAM_15  UINT8_C(0x19)
    #define BMA456_WRIST_SC_PARAM_16  UINT8_C(0x96)
    #define BMA456_WRIST_SC_PARAM_17  UINT8_C(0xA0)
    #define BMA456_WRIST_SC_PARAM_18  UINT8_C(0x01)
    #define BMA456_WRIST_SC_PARAM_19  UINT8_C(0x0C)
    #define BMA456_WRIST_SC_PARAM_20  UINT16_C(0x3CF0)
    #define BMA456_WRIST_SC_PARAM_21  UINT16_C(0x100)
    #define BMA456_WRIST_SC_PARAM_22  UINT8_C(0x01)
    #define BMA456_WRIST_SC_PARAM_23  UINT8_C(0x03)
    #define BMA456_WRIST_SC_PARAM_24  UINT8_C(0x01)
    #define BMA456_WRIST_SC_PARAM_25  UINT8_C(0x0E)
    
    /*!
        @brief Any motion configuration
    */
    struct bma456_anymotion_config {
        /*! Expressed in 50 Hz samples (20 ms) */
        uint16_t duration;
        /*! Threshold value for Any-motion / No-motion detection in
            5.11g format */
        uint16_t threshold;
        /*! Indicates if No-motion or Any-motion is selected */
        uint8_t nomotion_sel;
    };
    
    /*!
        @brief Axes remapping configuration
    */
    struct bma456_axes_remap {
        uint8_t x_axis;
        uint8_t x_axis_sign;
        uint8_t y_axis;
        uint8_t y_axis_sign;
        uint8_t z_axis;
        uint8_t z_axis_sign;
    };
    
    /*!
        @brief Step counter param settings
    */
    struct bma456_stepcounter_settings {
        /*! Step Counter param 1 */
        uint16_t param1;
        /*! Step Counter param 2 */
        uint16_t param2;
        /*! Step Counter param 3 */
        uint16_t param3;
        /*! Step Counter param 4 */
        uint16_t param4;
        /*! Step Counter param 5 */
        uint16_t param5;
        /*! Step Counter param 6 */
        uint16_t param6;
        /*! Step Counter param 7 */
        uint16_t param7;
        /*! Step Counter param 8 */
        uint16_t param8;
        /*! Step Counter param 9 */
        uint16_t param9;
        /*! Step Counter param 10 */
        uint16_t param10;
        /*! Step Counter param 11 */
        uint16_t param11;
        /*! Step Counter param 12 */
        uint16_t param12;
        /*! Step Counter param 13 */
        uint16_t param13;
        /*! Step Counter param 14 */
        uint16_t param14;
        /*! Step Counter param 15 */
        uint16_t param15;
        /*! Step Counter param 16 */
        uint16_t param16;
        /*! Step Counter param 17 */
        uint16_t param17;
        /*! Step Counter param 18 */
        uint16_t param18;
        /*! Step Counter param 19 */
        uint16_t param19;
        /*! Step Counter param 20 */
        uint16_t param20;
        /*! Step Counter param 21 */
        uint16_t param21;
        /*! Step Counter param 22 */
        uint16_t param22;
        /*! Step Counter param 23 */
        uint16_t param23;
        /*! Step Counter param 24 */
        uint16_t param24;
        /*! Step Counter param 25 */
        uint16_t param25;
    };
    
    /*!
         @brief This API is the entry point.
         Call this API before using all other APIs.
         This API reads the chip-id of the sensor and sets the resolution.
    
         @param[in,out] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_init(struct bma4_dev* dev);
    
    /*!
         @brief This API is used to upload the config file to enable
         the features of the sensor.
    
         @param[in] dev : Structure instance of bma4_dev.
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_write_config_file(struct bma4_dev* dev);
    
    /*!
         @brief This API is used to get the configuration id of the sensor.
    
         @param[out] config_id : Pointer variable used to store
         the configuration id.
         @param[in] dev : Structure instance of bma4_dev.
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_get_config_id(uint16_t* config_id, struct bma4_dev* dev);
    
    
    /*!
         @brief This API sets/unsets the user provided interrupt to either
         interrupt pin1 or pin2 in the sensor.
    
         @param[in] int_line: Variable to select either interrupt pin1 or pin2.
        int_line    |   Macros
        ------------|-------------------
             0       | BMA4_INTR1_MAP
             1       | BMA4_INTR2_MAP
         @param[in] int_map : Variable to specify the interrupts.
         @param[in] enable : Variable to specify mapping or unmapping of
         interrupts.
         enable		|	Macros
        --------------------|-------------------
         0x00		|  BMA4_DISABLE
         0x01		|  BMA4_ENABLE
         @param[in] dev : Structure instance of bma4_dev.
    
         @note Below macros specify the interrupts.
         Feature Interrupts
           - BMA456_STEP_CNTR_INT
           - BMA456_ACTIVITY_INT
           - BMA456_WRIST_TILT_INT
           - BMA456_WAKEUP_INT
           - BMA456_ANY_NO_MOTION_INT
           - BMA456_ERROR_INT
    
         Hardware Interrupts
           - BMA4_FIFO_FULL_INT
           - BMA4_FIFO_WM_INT
           - BMA4_DATA_RDY_INT
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_map_interrupt(uint8_t int_line, uint16_t int_map, uint8_t enable, struct bma4_dev* dev);
    
    /*!
         @brief This API reads the bma456 interrupt status from the sensor.
    
         @param[out] int_status : Variable to store the interrupt status
         read from the sensor.
         @param[in] dev : Structure instance of bma4_dev.
    
         @note Below macros are used to check the interrupt status.
         Feature Interrupts
    
           - BMA456_STEP_CNTR_INT
           - BMA456_ACTIVITY_INT
           - BMA456_WRIST_TILT_INT
           - BMA456_WAKEUP_INT
           - BMA456_ANY_NO_MOTION_INT
           - BMA456_ERROR_INT
    
    
         Hardware Interrupts
           - BMA4_FIFO_FULL_INT
           - BMA4_FIFO_WM_INT
           - BMA4_MAG_DATA_RDY_INT
           - BMA4_ACCEL_DATA_RDY_INT
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_read_int_status(uint16_t* int_status, struct bma4_dev* dev);
    
    /*!
         @brief This API enables/disables the features of the sensor.
    
         @param[in] feature : Variable to specify the features
         which are to be set in bma456 sensor.
         @param[in] enable : Variable which specifies whether to enable or
         disable the features in the bma456 sensor
         enable		|	Macros
        --------------------|-------------------
         0x00		|  BMA4_DISABLE
         0x01		|  BMA4_ENABLE
         @param[in] dev : Structure instance of bma4_dev.
    
         @note User should use the below macros to enable or disable the
         features of bma456 sensor
           - BMA456_STEP_CNTR
           - BMA456_ANY_MOTION (or) BMA456_NO_MOTION
           - BMA456_ACTIVITY
           - BMA456_WRIST_TILT
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_feature_enable(uint8_t feature, uint8_t enable, struct bma4_dev* dev);
    
    
    /*!
         @brief This API performs x, y and z axis remapping in the sensor.
    
         @param[in] remap_data : Pointer to store axes remapping data.
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_set_remap_axes(const struct bma456_axes_remap* remap_data, struct bma4_dev* dev);
    
    /*!
         @brief This API reads the x, y and z axis remap data from the sensor.
    
         @param[out] remap_data : Pointer to store axis remap data which is read
         from the bma456 sensor.
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_get_remap_axes(struct bma456_axes_remap* remap_data, struct bma4_dev* dev);
    
    
    /*!
         @brief This API sets the watermark level for step counter
         interrupt in the sensor.
    
         @param[in] step_counter_wm : Variable which specifies watermark level
         count
         @note Valid values are from 1 to 1023
         @note Value 0 is used for step detector interrupt
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_step_counter_set_watermark(uint16_t step_counter_wm, struct bma4_dev* dev);
    
    /*!
         @brief This API gets the water mark level set for step counter interrupt
         in the sensor
    
         @param[out] step_counter_wm : Pointer variable which stores
         the water mark level read from the sensor.
         @note valid values are from 1 to 1023
         @note value 0 is used for step detector interrupt
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_step_counter_get_watermark(uint16_t* step_counter_wm, struct bma4_dev* dev);
    
    /*!
         @brief This API resets the counted steps of step counter.
    
         @param[in] dev : structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_reset_step_counter(struct bma4_dev* dev);
    
    /*!
         @brief This API gets the number of counted steps of the step counter
         feature from the sensor.
    
         @param[out] step_count : Pointer variable which stores counted steps
         read from the sensor.
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_step_counter_output(uint32_t* step_count, struct bma4_dev* dev);
    
    /*!
        @brief This API gets the output for activity feature.
    
        @param[out] activity : Pointer variable which stores activity output
        read from the sensor.
             activity |   State
        --------------|------------------------
              0x00    | BMA456_USER_STATIONARY
              0x01    | BMA456_USER_WALKING
              0x02    | BMA456_USER_RUNNING
              0x03    | BMA456_STATE_INVALID
    
        @param[in] dev : Structure instance of bma4_dev
        @return Result of API execution status
        @retval 0 -> Success
        @retval Any non zero value -> Fail
    */
    uint16_t bma456_activity_output(uint8_t* activity, struct bma4_dev* dev);
    
    /*!
        @brief This API select the platform configuration wrist(default) or phone.
    
        @param[in] platform : Variable to select wrist/phone
    
           platform  |   Macros
        -------------|------------------------
              0x00   | BMA456_PHONE_CONFIG
              0x01   | BMA456_WRIST_CONFIG
    
        @param[in] dev : Structure instance of bma4_dev
        @return Result of API execution status
        @retval 0 -> Success
        @retval Any non zero value -> Fail
    */
    uint16_t bma456_select_platform(uint8_t platform, struct bma4_dev* dev);
    
    /*!
         @brief This API gets the parameter1 to parameter7 settings of the
         step counter feature.
    
         @param[out] setting : Pointer to structure variable which stores the
         parameter1 to parameter7 read from the sensor.
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_stepcounter_get_parameter(struct bma456_stepcounter_settings* setting, struct bma4_dev* dev);
    
    /*!
         @brief This API sets the parameter1 to parameter7 settings of the
         step counter feature in the sensor.
    
         @param[in] setting : Pointer to structure variable which stores the
         parameter1 to parameter7 settings read from the sensor.
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_stepcounter_set_parameter(const struct bma456_stepcounter_settings* setting, struct bma4_dev* dev);
    
    /*!
         @brief This API enables or disables the step detector feature in the
         sensor.
    
         @param[in] enable : Variable used to enable or disable step detector
         enable		|	Macros
        --------------------|-------------------
         0x00		|  BMA4_DISABLE
         0x01		|  BMA4_ENABLE
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_step_detector_enable(uint8_t enable, struct bma4_dev* dev);
    
    /*!
         @brief This API enables the any motion feature according to the axis
         set by the user in the sensor.
    
         @param[in] axis : Variable to specify the axis of the any motion feature
         to be enabled in the sensor.
        Value    |  Axis
        ---------|-------------------------
        0x00     |  BMA456_ALL_AXIS_DIS
        0x01     |  BMA456_X_AXIS_EN
        0x02     |  BMA456_Y_AXIS_EN
        0x04     |  BMA456_Z_AXIS_EN
        0x07     |  BMA456_ALL_AXIS_EN
         @param[in] dev : Structure instance of bma4_dev
    
         @return result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_anymotion_enable_axis(uint8_t axis, struct bma4_dev* dev);
    
    /*!	@brief	This API sets the configuration of Any motion feature in
         the sensor.
    
         @param[in]	any_motion : Pointer to structure variable to specify
         the any motion feature settings.
         Structure members are provided in the table below
        @verbatim
        -------------------------------------------------------------------------
               Structure parameters    |        Description
        --------------------------------|----------------------------------------
                                       |        Defines the number of
                                       |        consecutive data points for
                                       |        which the threshold condition
               duration                |        must be respected, for interrupt
                                       |        assertion. It is expressed in
                                       |        50 Hz samples (20 ms).
                                       |        Range is 0 to 163sec.
                                       |        Default value is 5 = 100ms.
        --------------------------------|----------------------------------------
                                       |        Slope threshold value for
                                       |        Any-motion / No-motion detection
               threshold               |        in 5.11g format.
                                       |        Range is 0 to 1g.
                                       |        Default value is 0xAA = 83mg.
        --------------------------------|----------------------------------------
                                       |        Indicates if No motion (1) or
               nomotion_sel            |        Any-motion (0) is selected;
                                       |        default value is 0 Any-motion.
        -------------------------------------------------------------------------
        @endverbatim
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_set_any_motion_config(const struct bma456_anymotion_config* any_motion, struct bma4_dev* dev);
    
    /*!	@brief	This API gets the configuration of any motion feature from
         the sensor.
    
         @param[out]	any_motion : Pointer to structure variable used to store
         the any motion feature settings read from the sensor.
         Structure members are provided in the table below
        @verbatim
        -------------------------------------------------------------------------
               Structure parameters    |        Description
        --------------------------------|----------------------------------------
                                       |        Defines the number of
                                       |        consecutive data points for
                                       |        which the threshold condition
               duration                |        must be respected, for interrupt
                                       |        assertion. It is expressed in
                                       |        50 Hz samples (20 ms).
                                       |        Range is 0 to 163sec.
                                       |        Default value is 5 = 100ms.
        --------------------------------|----------------------------------------
                                       |        Slope threshold value for
                                       |        Any-motion / No-motion detection
               threshold               |        in 5.11g format.
                                       |        Range is 0 to 1g.
                                       |        Default value is 0xAA = 83mg.
        --------------------------------|----------------------------------------
                                       |        Indicates if No motion (1) or
               nomotion_sel            |        Any-motion (0) is selected;
                                       |        default value is 0 Any-motion.
        -------------------------------------------------------------------------
        @endverbatim
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_get_any_motion_config(struct bma456_anymotion_config* any_motion, struct bma4_dev* dev);
    
    /*!
         @brief This API sets the sensitivity of wakeup feature in the sensor
    
         @param[in] sensitivity : Variable used to specify the sensitivity of the
         Wakeup feature.
         Value	|  Sensitivity
         --------|-------------------------
         0x00	|  MOST SENSITIVE
         0x07	|  LEAST SENSITIVE
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_wakeup_set_sensitivity(uint8_t sensitivity, struct bma4_dev* dev);
    
    /*!
         @brief This API gets the sensitivity of wakeup feature in the sensor
    
         @param[out] sensitivity : Pointer variable which stores the sensitivity
         value read from the sensor.
         Value	|  Sensitivity
         --------|-------------------------
         0x00	|  MOST SENSITIVE
         0x07	|  LEAST SENSITIVE
         @param[in] dev : Structure instance of bma4_dev
    
         @return Result of API execution status
         @retval 0 -> Success
         @retval Any non zero value -> Fail
    */
    uint16_t bma456_wakeup_get_sensitivity(uint8_t* sensitivity, struct bma4_dev* dev);
    
    /*!
         @brief This API is used to select single/double tap
         feature in the sensor
    
         @param tap_select : Variable used to specify the single or
         double tap selection in the sensor
            tap_select  |  description
            ------------|------------------------
            0x00        |  Double tap selected
            0x01        |  single tap selected
    
         @param dev : Structure instance of bma4_dev
    
         @return results of stream_transfer operation
         @retval 0 -> Success
         @retval Any positive value mentioned in ERROR CODES -> Fail
    
    */
    uint16_t bma456_tap_selection(const uint8_t tap_select, struct bma4_dev* dev);
    
    #ifdef __cplusplus
    }
    #endif /*End of CPP guard */
    
    #endif /*End of header guard macro */