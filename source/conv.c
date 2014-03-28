/*
 * File:    conv.c
 * Author:  nenad
 * Details: Converting functions
 *
 * Created on March 16, 2014, 7:11 PM
 */

/*
 * File:    bsp.c
 * Author:  nenad
 * Details: Board Support Package
 *
 * Created on February 8, 2014, 11:12 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "conv.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static const uint8_t asciiToHexTable[] = {
    0x0u,   0x1u,   0x2u,   0x3u,   0x4u,   0x5u,   0x6u,   0x7,
    0x8u,   0x9u,   0xau,   0xbu,   0xcu,   0xdu,   0xeu,   0xfu
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

uint8_t convToLower(unsigned char character) {

    if ((character >= 'A') && (character <= 'Z')) {
        character |= 0x20u;
    }

    return (character);
}

uint8_t convAsciiToHex(unsigned char character) {

    character = convToLower(character);
    
    if ((character >= '0') && (character <= '9')) {

        return (asciiToHexTable[character - '0']);
    }
    if ((character >= 'a') && (character <= 'f')) {

        return (asciiToHexTable[character - 'a' + 0xau]);
    }

    return (0);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of conv.c
 ******************************************************************************/