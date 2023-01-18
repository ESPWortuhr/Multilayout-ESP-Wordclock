#include "Uhrtype.hpp"

/*
 * Layout Front
 *
 * I T L I S A S A M P M
 * A C Q U A R T E R D C
 * T W E N T Y F I V E X
 * H A L F S T E N F T O
 * P A S T E R U N I N E
 * O N E S I X T H R E E
 * F O U R F I V E T W O
 * E I G H T E L E V E N
 * S E V E N T W E L V E
 * T E N S E O'C L O C K
 *
 */

#define USE_DREIVIERTEL

class En10x11_t : public iUhrType {
public:
    virtual const bool hasZwanzig() override { return true; }

    //------------------------------------------------------------------------------

    virtual const bool hasTwentyfive() override { return true; }

    //------------------------------------------------------------------------------

    void show(uint8_t text) override {
        switch (text) {

        case es_ist:
            // Es
            Letter_set(0); // I
            Letter_set(1); // T
            // Ist
            Letter_set(3); // I
            Letter_set(4); // S
            break;
        case h_ein:
            // One
            Letter_set(65); // O
            Letter_set(64); // N
            Letter_set(63); // E
            break;
        case h_zwei:
            // Two
            Letter_set(74); // T
            Letter_set(75); // W
            Letter_set(76); // O
            break;
        case h_drei:
            // Three
            Letter_set(59); // T
            Letter_set(58); // H
            Letter_set(57); // R
            Letter_set(56); // E
            Letter_set(55); // E
            break;
        case h_vier:
            // Four
            Letter_set(66); // F
            Letter_set(67); // O
            Letter_set(68); // U
            Letter_set(69); // R
            break;
        case h_fuenf:
            // Five
            Letter_set(70); // F
            Letter_set(71); // I
            Letter_set(72); // V
            Letter_set(73); // E
            break;
        case h_sechs:
            // Six
            Letter_set(62); // S
            Letter_set(61); // I
            Letter_set(60); // X
            break;
        case h_sieben:
            // Seven
            Letter_set(88); // S
            Letter_set(89); // E
            Letter_set(90); // V
            Letter_set(91); // E
            Letter_set(92); // N
            break;
        case h_acht:
            // Eight
            Letter_set(87); // E
            Letter_set(86); // I
            Letter_set(85); // G
            Letter_set(84); // H
            Letter_set(83); // T
            break;
        case h_neun:
            // Nine
            Letter_set(51); // N
            Letter_set(52); // I
            Letter_set(53); // N
            Letter_set(54); // E
            break;
        case h_zehn:
            // Ten
            Letter_set(109); // T
            Letter_set(108); // E
            Letter_set(107); // N
            break;
        case h_elf:
            // Eleven
            Letter_set(82); // E
            Letter_set(81); // L
            Letter_set(80); // E
            Letter_set(79); // V
            Letter_set(78); // E
            Letter_set(77); // N
            break;
        case h_zwoelf:
            // Twelve
            Letter_set(93); // T
            Letter_set(94); // W
            Letter_set(95); // E
            Letter_set(96); // L
            Letter_set(97); // V
            Letter_set(98); // E
            break;
        case fuenf:
            // Five
            Letter_set(28); // F
            Letter_set(29); // I
            Letter_set(30); // V
            Letter_set(31); // E
            break;
        case zehn:
            // Ten
            Letter_set(38); // T
            Letter_set(37); // E
            Letter_set(36); // N
            break;
        case a_quarter:
            // A Quater
            Letter_set(21); // A
            Letter_set(13); // Q
            Letter_set(14); // U
            Letter_set(15); // A
            Letter_set(16); // R
            Letter_set(17); // T
            Letter_set(18); // E
            Letter_set(19); // R
            break;
        case viertel:
            // Quater
            Letter_set(13); // Q
            Letter_set(14); // U
            Letter_set(15); // A
            Letter_set(16); // R
            Letter_set(17); // T
            Letter_set(18); // E
            Letter_set(19); // R
            break;
        case zwanzig:
            // Twenty
            Letter_set(22); // T
            Letter_set(23); // W
            Letter_set(24); // E
            Letter_set(25); // N
            Letter_set(26); // T
            Letter_set(27); // Y
            break;
        case twentyfive:
            // Twentyfive
            Letter_set(22); // T
            Letter_set(23); // W
            Letter_set(24); // E
            Letter_set(25); // N
            Letter_set(26); // T
            Letter_set(27); // Y
            Letter_set(28); // F
            Letter_set(29); // I
            Letter_set(30); // V
            Letter_set(31); // E
            break;
        case halb:
            // Half
            Letter_set(43); // H
            Letter_set(42); // A
            Letter_set(41); // L
            Letter_set(40); // F
            break;
        case nach:
            // Past
            Letter_set(44); // P
            Letter_set(45); // A
            Letter_set(46); // S
            Letter_set(47); // T
            break;
        case vor:
            // To
            Letter_set(34); // T
            Letter_set(33); // O
            break;
        case uhr:
            // O'Clock
            Letter_set(104); // O'
            Letter_set(103); // C
            Letter_set(102); // L
            Letter_set(101); // O
            Letter_set(100); // C
            Letter_set(99);  // K
            break;
        default:
            break;
        };
    };
};

En10x11_t _en10x11;