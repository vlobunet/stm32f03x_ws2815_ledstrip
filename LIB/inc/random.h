#ifndef __INC_LIB8TION_RANDOM_H
#define __INC_LIB8TION_RANDOM_H

/// @ defgroup Random Fast генераторы случайных чисел
/// Быстрые 8- и 16-битные случайные числа без знака.
/// Значительно быстрее, чем Arduino random (), но также несколько менее случайный.
/// Вы можете добавить энтропию.

#include <stdlib.h>
#include <stdint.h>

#define LIB8STATIC __attribute__ ((unused)) static inline
// X(n+1) = (2053 * X(n)) + 13849)
#define FASTLED_RAND16_2053  ((uint16_t)(2053))
#define FASTLED_RAND16_13849 ((uint16_t)(13849))

#if defined(LIB8_ATTINY)
#define APPLY_FASTLED_RAND16_2053(x) (x << 11) + (x << 2) + x
#else
#define APPLY_FASTLED_RAND16_2053(x) (x * FASTLED_RAND16_2053)
#endif

/// начальное число случайных чисел
extern uint16_t rand16seed;

/// Генерация 8-битного случайного числа
LIB8STATIC uint8_t random8() {
    rand16seed = APPLY_FASTLED_RAND16_2053(rand16seed) + FASTLED_RAND16_13849;
    // возвращаем сумму старшего и младшего байтов
    // для лучшего смешивания и непоследовательной корреляции
    return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) + ((uint8_t)(rand16seed >> 8)));
}

/// Генерация 16-битного случайного числа
LIB8STATIC uint16_t random16() {
    rand16seed = APPLY_FASTLED_RAND16_2053(rand16seed) + FASTLED_RAND16_13849;
    return rand16seed;
}

/// Генерируем 8-битное случайное число от 0 до lim
/// @param lim верхняя граница результата
LIB8STATIC uint8_t random8_lim(uint8_t lim) {
    uint8_t r = random8();
    r = (r * lim) >> 8;
    return r;
}

/// Генерация 8-битного случайного числа в заданном диапазоне
/// @param min нижняя граница случайного числа
/// @param lim верхняя граница случайного числа
LIB8STATIC uint8_t random8_range(uint8_t min, uint8_t lim) {
    uint8_t delta = lim - min;
    uint8_t r = random8_lim(delta) + min;
    return r;
}

/// Генерируем 16-битное случайное число от 0 до lim
/// @param lim верхняя граница результата
LIB8STATIC uint16_t random16_lim( uint16_t lim) {
    uint16_t r = random16();
    uint32_t p = (uint32_t)lim * (uint32_t)r;
    r = p >> 16;
    return r;
}

/// Генерируем 16-битное случайное число в заданном диапазоне
/// @param min нижняя граница случайного числа
/// @param lim верхняя граница случайного числа
LIB8STATIC uint16_t random16_range( uint16_t min, uint16_t lim) {
    uint16_t delta = lim - min;
    uint16_t r = random16_lim( delta) + min;
    return r;
}

/// Устанавливаем 16-битное начальное число, используемое для генератора случайных чисел
LIB8STATIC void random16_set_seed( uint16_t seed) {
    rand16seed = seed;
}

/// Получить текущее начальное значение для генератора случайных чисел
LIB8STATIC uint16_t random16_get_seed() {
    return rand16seed;
}

/// Добавляем энтропию в генератор случайных чисел
LIB8STATIC void random16_add_entropy( uint16_t entropy) {
    rand16seed += entropy;
}

#endif
