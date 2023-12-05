#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "constants.h"
#include "car.h"
#include "sale.h"
#include "feedback.h"
#include "customer.h"

unsigned int totalCars;
unsigned int totalSales;
unsigned int totalFeedbacks;


Car *cars;
Sale *sales;
Feedback *feedback;


char *msg = "Enter$";

void addNewStock() {
    char tempModel[100];
    int tempYear;
    int tempAvail;
    float tempPrice;

    printf("Enter the name of the model : ");
    scanf("%99[^\n]", tempModel);

    printf("Enter the year of manufacture: ");
    scanf("%d", &tempYear);

    printf("Enter the available quantity: ");
    scanf("%d", &tempAvail);

    printf("Enter the price: ");
    scanf("%f", &tempPrice);

    // Allocate memory for cars array
    if (cars == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(cars[totalCars].modelName, sizeof(cars[totalCars].modelName), "%s", tempModel);
    cars[totalCars].yearOfManufacture = tempYear;
    cars[totalCars].availability = tempAvail;
    cars[totalCars].price = tempPrice;

    FILE *file = openFile("cars.csv", "a");
    if (file != NULL) 
    {
        fprintf(file, "%s,%d,%d,%.2f\n", tempModel, tempYear, tempAvail, tempPrice);
        closeFile(file);
        printf("New stock added successfully!\n");
        totalCars++;  // Increment totalCars after successful addition
    } else 
    {
        fprintf(stderr, "Error opening CSV file for writing.\n");
    }
}


void viewStocks() {
    int i;
    sort(); 

    printf("%-5s%-20s%-10s%-15s%-10s\n", "SL No", "Model Name", "Year", "Price", "Availability");

    for (i = 0; i < totalCars; i++) {
        printf("%-5d%-20s%-10d%-15.2f%-10d\n", i + 1, cars[i].modelName, cars[i].yearOfManufacture, cars[i].price, cars[i].availability);
    }
}


void buyCar() {
    char choice;
    char customerName[50];
    char proof;

    int carIndex, quantity, customerAge;
    float price;

    printf("Hi, your sweet name please: ");
    scanf("%49[^\n]", customerName);  // Adjusted the format specifier to avoid buffer overflow

    printf("How old are you: ");
    scanf("%d", &customerAge);

    printf("Do you have any valid ID as proof of your age (y/n): ");
    scanf(" %c", &proof);  // Added a space before %c to consume the newline character

    if (proof == 'y' && customerAge >= DISCOUNT_MIN_AGE && customerAge <= DISCOUNT_MAX_AGE) 
    {
        printf("Hurray! You are eligible for a discount.\n");
        // You might want to store this information in the sales record as well
        sales[totalSales].discountGiven = 1;
    } 
    else 
    {
        printf("You are not eligible for a discount.\n");
        sales[totalSales].discountGiven = 0;
    }

    choice = getCharFromUser("Would you like to re-view the stock before proceeding to buy..\n");
    prompt(msg);

    if (choice == 'y') 
    {
        viewStocks();
    }

    printf("Enter the serial number of the car: ");
    scanf("%d", &carIndex);
    do 
    {   
        printf("The available number of cars is %d\n", cars[carIndex].availability);

        printf("How many of them do you wish to purchase: ");
        scanf("%d", &quantity); 

        if (quantity > 0 && quantity <= cars[carIndex].availability) 
        {
            break;
        } 
        else 
        {
            printf("Invalid quantity. Please choose a valid quantity.\n");
        } 

    } while (quantity > 0 && quantity <= cars[carIndex].availability);

    price = cars[carIndex].price * quantity;

    printf("Amount: %.2f\n", price);  // Corrected the format specifier

    price = price * DISCOUNT_PERCENTAGE;

    printf("Final Amount after discount has been applied: %.2f\n", price);  // Corrected the format specifier

    printf("\n\nCongratulations! Your purchase is completed successfully.\n\n");

    cars[carIndex].availability -= quantity;
    
    strcpy(sales[totalSales].modelName, cars[carIndex].modelName);
    sales[totalSales].totalPrice = cars[carIndex].price * quantity;
    
    sales[totalSales].discountValue = price;
    sales[totalSales].numberOfCars = quantity;
    strcpy(sales[totalSales].Customer.name, customerName);
    sales[totalSales].Customer.age = customerAge;
    sales[totalSales].purchaseDate = time(NULL);
    // Open the sales.csv file in append mode
    FILE *salesFile = openFile("sales.csv", "a");
    if (salesFile != NULL) 
    {
        fprintf(salesFile, "%s,%.2f,%d,%.2f,%d,%s,%d,%ld\n",
            cars[carIndex].modelName,
            cars[carIndex].price * quantity,
            sales[totalSales].discountGiven,
            price,
            quantity,
            customerName,
            customerAge,
            time(NULL)
        );

        closeFile(salesFile);  // Assuming closeFile doesn't have additional logic
        totalSales++;  // Increment totalSales after successful addition
        printf("\n\nCongratulations! Your purchase is completed successfully.\n\n");
    } 
    else 
    {
        fprintf(stderr, "Error opening sales.csv file for writing.\n");
    }
}

void viewSalesData()
{

}

void customerFeedback()
{

}

void retrieveDataFromFile()
{

}
void prompt(char* msg)
{
    printf("%s", msg);
}

void clearScreen()
{
    system("clear");
}

void sort()
{


}

FILE* openFile(const char *fileName, const char *mode) {
    FILE *file = fopen(fileName, mode);
    if (file == NULL) {
        perror("Error opening file");
    }
    return file;
}

void closeFile(FILE *file) {
    if (file != NULL) {
        fclose(file);
    }
}

#include <stdio.h>

char getCharFromUser(char *msg) {
    char ch;
    printf("%s", msg);
    while (getchar() != '\n');
    scanf("%c", &ch);
    return ch;
}


void main() {
    retrieveDataFromFile();

    int choice;
    char userType;
    
    printf("Are you a customer(c) or admin(a) : ");

    
    while (getchar() != '\n');

    scanf("%c", &userType);
    
    if (userType == 'a') {
        while (true) {
            printf("1] Add new Stock.\n");
            printf("2] View Sales Data.\n");
            
            
            break;
        }
    } else {
        while (true) {
            printf("What would you like to do.\n\n");
            printf("1] View Stocks.\n");
            printf("2] Buy a car.\n");
            printf("3] View or add Customer feedback.\n");
            printf("Enter your choice: ");

            
            while (getchar() != '\n');

            scanf("%d", &choice);

            clearScreen();

            switch (choice) {
                case 1: viewStocks(); break;
                case 2: buyCar(); break;
                case 3: customerFeedback(); break;
                default: printf("<<<<<-----Invalid choice----->>>>>\n");
            }

            
            break;
        }
    }
}