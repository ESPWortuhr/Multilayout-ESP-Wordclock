#include <Arduino.h>

/*
 * Layout Front
 *
 * E S K I S T L F Ü N F
 * Z E H N Z W A N Z I G
 * D R E I V I E R T E L
 * N A C H A P P Y V O R
 * H A L B I R T H D A Y
 * D R Z W Ö L F Ü N F X
 * Z E H N E U N D R E I
 * Z W E I N S I E B E N
 * E L F V I E R A C H T
 * S E C H S I U H R Y E
 *
 */

#define USE_DREIVIERTEL


unsigned int matrix[10][11] = {
    {0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10},
    {21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11},
    {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32},
    {43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33},
    {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54},
    {65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55},
    {66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76},
    {87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77},
    {88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98},
    {109,108,107,106,105,104,103,102,101,100,99}
};


unsigned int smatrix[110] = {
    0,   1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
    33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
    55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65,
    66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
    77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
    88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
    99,100,101,102,103,104,105,106,107,108,109
};


static inline void es_ist()
{
    //Es
    led_set(0);
    led_set(1);

    //Ist
    led_set(3);
    led_set(4);
    led_set(5);
}


static inline void nach()
{
    led_set(43);
    led_set(42);
    led_set(41);
    led_set(40);
}


static inline void vor()
{
    led_set(33);
    led_set(34);
    led_set(35);
}


static inline void viertel()
{
    led_set(32);
    led_set(31);
    led_set(30);
    led_set(29);
    led_set(28);
    led_set(27);
    led_set(26);
}


static inline void dreiviertel()
{
    led_set(32);
    led_set(31);
    led_set(30);
    led_set(29);
    led_set(28);
    led_set(27);
    led_set(26);
    led_set(25);
    led_set(24);
    led_set(23);
    led_set(22);
}


static inline void fuenf()
{
    led_set(7);
    led_set(8);
    led_set(9);
    led_set(10);
}


static inline void zehn()
{
    led_set(21);
    led_set(20);
    led_set(19);
    led_set(18);
}


static inline void zwanzig()
{
    led_set(17);
    led_set(16);
    led_set(15);
    led_set(14);
    led_set(13);
    led_set(12);
    led_set(11);
}


static inline void halb()
{
    led_set(44);
    led_set(45);
    led_set(46);
    led_set(47);
}


static inline void eins()
{
    led_set(85);
    led_set(84);
    led_set(83);
    led_set(82);
}


static inline void uhr()
{
    led_set(101);
    led_set(102);
    led_set(103);
}


static inline void happy_birthday()
{
    // happy
    led_set(40);
    led_set(39);
    led_set(38);
    led_set(37);
    led_set(36);

    // happy
    led_set(47);
    led_set(48);
    led_set(49);
    led_set(50);
    led_set(51);
    led_set(52);
    led_set(53);
    led_set(54);
}


static inline void h_ein()
{
    led_set(85);
    led_set(84);
    led_set(83);
}


static inline void h_zwei()
{
    led_set(87);
    led_set(86);
    led_set(85);
    led_set(84);
}


static inline void h_drei()
{
    led_set(73);
    led_set(74);
    led_set(75);
    led_set(76);
}


static inline void h_vier()
{
    led_set(91);
    led_set(92);
    led_set(93);
    led_set(94);
}


static inline void h_fuenf()
{
    led_set(59);
    led_set(58);
    led_set(57);
    led_set(56);
}


static inline void h_sechs()
{
    led_set(109);
    led_set(108);
    led_set(107);
    led_set(106);
    led_set(105);
}


static inline void h_sieben()
{
    led_set(82);
    led_set(81);
    led_set(80);
    led_set(79);
    led_set(78);
    led_set(77);
}


static inline void h_acht()
{
    led_set(95);
    led_set(96);
    led_set(97);
    led_set(98);
}


static inline void h_neun()
{
    led_set(69);
    led_set(70);
    led_set(71);
    led_set(72);
}


static inline void h_zehn()
{
    led_set(66);
    led_set(67);
    led_set(68);
    led_set(69);
}


static inline void h_elf()
{
    led_set(88);
    led_set(89);
    led_set(90);
}


static inline void h_zwoelf()
{
    led_set(63);
    led_set(62);
    led_set(61);
    led_set(60);
    led_set(59);

}


static inline void h_droelf()
{
    led_set(65);
    led_set(64);
    led_set(61);
    led_set(60);
    led_set(59);

}
