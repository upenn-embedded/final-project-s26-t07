#ifndef RECIPE_H
#define RECIPE_H

#include <stdint.h>

typedef struct {
    const char *name;
    uint16_t ingredient1_ml;
    uint16_t ingredient2_ml;
    uint16_t ingredient3_ml;
} Recipe;

extern Recipe recipe_1;
extern Recipe recipe_2;
extern Recipe recipe_3;

#endif
