#ifndef LS7366R_H
#define LS7366R_H

#include <Arduino.h>

/*
    This class is for connecting an Arduino with a LS7366R quadrature encoder chip via the Arduino's SPI interface.

    https://lsicsi.com/datasheets/LS7366R.pdf
*/

namespace LS7366R
{
    //////// STATUS REGISTER DEFINITIONS ////////
    // Status byte meaning
    // 7  6  5   4   3   2   1   0
    // CY BW CMP IDX CEN PLS U/D S

    #define STR_CY 0b10000000
    #define STR_BW 0b01000000
    #define STR_CMP 0b00100000
    #define STR_IDX 0b00010000
    #define STR_CEN 0b00001000
    #define STR_PLS 0b00000100
    #define STR_UD 0b00000010
    #define STR_SIGN 0b00000001

    //////// IR DEFINITIONS, COMBINE 1 OP WITH 1 SELECT ////////
    #define CLR 0b00000000
    #define RD 0b01000000
    #define WR 0b10000000
    #define LD 0b11000000

    #define MDR0 0b00001000
    #define MDR1 0b00010000
    #define DTR 0b00011000
    #define CNTR 0b00100000
    #define OTR 0b00101000
    #define STR 0b00110000

    //////// MDR0 DEFINITIONS ////////
    #define MDR0_NONQUAD 0b00000000
    #define MDR0_X1QUAD 0b00000001
    #define MDR0_X2QUAD 0b00000010
    #define MDR0_X4QUAD 0b00000011
    #define MDR0_FREE_CNT 0b00000000
    #define MDR0_SING_CNT 0b00000100
    #define MDR0_RNG_LIMIT_CNT 0b00001000
    #define MDR0_MODN_CNT 0b00001100

    #define MDR0_IDX_DISABLE 0b00000000
    #define MDR0_IDX_LOAD_CNTR 0b00010000
    #define MDR0_IDX_RSET_CNTR 0b00100000
    #define MDR0_IDX_LOAD_OTR 0b00110000

    #define MDR0_IDX_ASYNC 0b00000000
    #define MDR0_IDX_SYNC 0b01000000
    #define MDR0_FILTR_CLK_DIV1 0b00000000
    #define MDR0_FILTR_CLK_DIV2 0b10000000

    //////// MDR1 DEFINITIONS ////////
    #define MDR1_CNT_4BYTE 0b00000000
    #define MDR1_CNT_3BYTE 0b00000001
    #define MDR1_CNT_2BYTE 0b00000010
    #define MDR1_CNT_1BYTE 0b00000011

    #define MDR1_CNT_ENABLE 0b00000000
    #define MDR1_CNT_DISABLE 0b00000100

    #define MDR1_FLAG_NONE 0b00000000
    #define MDR1_FLAG_IDX 0b00010000
    #define MDR1_FLAG_CMP 0b00100000
    #define MDR1_FLAG_BW 0b01000000
    #define MDR1_FLAG_CY 0b10000000

    #define MDR0_DEFAULT_CONFIG MDR0_X1QUAD | MDR0_FREE_CNT | MDR0_IDX_DISABLE | MDR0_FILTR_CLK_DIV1
    #define MDR1_DEFAULT_CONFIG MDR1_CNT_4BYTE | MDR1_FLAG_NONE

    class QuadratureEncoder
    {
        public:

            QuadratureEncoder(int enc_select, byte mdr0_config, byte mdr1_config);

            void init();

            byte status();

            long count();

            void clearCount();

            // Status methods
            bool statusCounterOverflow(byte status);

            bool statusCounterUnderflow(byte status);

            bool statusCounterEqualDtr(byte status);

            bool statusIndex(byte status);

            bool statusCountEnabled(byte status);

            bool statusPowerLoss(byte status);

            // 0: count down, 1: count up. Avoid use of "forward" or "reverse" since
            // these have different meanings depending on context
            bool statusDirection(byte status);

            bool directionUp(byte status);

            bool directionDown(byte status);

            bool statusSign(byte status); // 1: negative, 0: positive

        private:

            int m_enc_select;
            int m_counter_size;
            byte m_mdr0_config;
            byte m_mdr1_config;
    };

} // end LS7366R namespace

#endif // LS7366R_H