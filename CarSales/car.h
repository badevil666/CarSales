#ifndef CAR_H
#define CAR_H

typedef struct car {
    char modelName[100];
    int yearOfManufacture;
    int availability;
    float price;
} Car;

void viewStocks();
void addNewStock();

#endif // CAR_H