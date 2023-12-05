#ifndef SALE_H
#define SALE_H
#include <time.h>
#include "customer.h"

typedef struct sale {
    char modelName[100];
    float totalPrice;
    int discountGiven;
    float discountValue;
    int numberOfCars;
    customer Customer;
    time_t purchaseDate;
} Sale;
#endif // SALE_H