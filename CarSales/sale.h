#ifndef SALE_H
#define SALE_H
#include <time.h>
#include "customer.h"

struct Sale {
    char modelName[100];
    float totalPrice;
    int discountGiven;
    float discountValue;
    int numberOfCars;
    struct Customer customer;
    time_t purchaseDate;
};
#endif // SALE_H