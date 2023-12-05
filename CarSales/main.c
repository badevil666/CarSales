#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


#include "car.h"
#include "sale.h"
#include "feedback.h"
#include "customer.h"

#define CUSTOMER 'c'
#define ADMIN 'a'
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25
#define DISCOUNT_PERCENTAGE 0.1f
#define MAX_FEEDBACK_LENGTH 500

typedef struct feedback 
{
    Customer customer;  // Reuse the Customer structure
    char comments[200];
    int rating;  // Assuming an integer rating, you can adjust based on your needs
} Feedback;

typedef struct customer 
{
    char name[50];
    int age;
} Customer;

typedef struct sale 
{
    char modelName[100];
    float totalPrice;
    int discountGiven;
    float discountValue;
    int numberOfCars;
    Customer customer;
    time_t purchaseDate;
} Sale;

typedef struct car
{
    char modelName[100];
    int yearOfManufacture;
    int availability;
    float price;
} Car;



unsigned int totalCars = 0;
unsigned int totalSales = 0;
unsigned int totalFeedbacks = 0;


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
    strcpy(sales[totalSales].customer.name, customerName);
    sales[totalSales].customer.age = customerAge;
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

    clearScreen();
    printf("\n\n*************** PURCHASE RECEIPT ***************\n");
    printf("Car Model: %s\n", cars[carIndex].modelName);
    printf("Total Price: $%.2f\n", sales[totalSales - 1].totalPrice);
    
    if (sales[totalSales].discountGiven) {
        printf("Discount Applied: Yes\n");
        printf("Discount Value: $%.2f\n", sales[totalSales- 1].discountValue);
    } else {
        printf("Discount Applied: No\n");
    }

    printf("Number of Cars: %d\n", sales[totalSales - 1].numberOfCars);
    printf("Customer Name: %s\n", sales[totalSales - 1].customer.name);
    printf("Customer Age: %d\n", sales[totalSales - 1].customer.age);
    printf("Purchase Date: %s", ctime(&sales[totalSales - 1].purchaseDate));
    printf("Final Amount: $%.2f\n", price);  // Corrected the format specifier
    printf("************* THANK YOU FOR YOUR PURCHASE *************\n");  
}

void viewSalesData() 
{
    if (totalSales == 0) 
    {
        printf("No sales data available.\n");
        return;
    }

    printf("\n*************** SALES DATA ***************\n");
    printf("%-20s%-15s%-10s%-10s%-20s%-10s%-20s\n", "Car Model", "Total Price", "Discount", "Discount Value", "Customer Name", "Customer Age", "Purchase Date");

    for (int i = 0; i < totalSales; i++) 
    {
        printf("%-20s%-15.2f%-10s%-10.2f%-20s%-10d%-20s", 
               sales[i].modelName, 
               sales[i].totalPrice, 
               sales[i].discountGiven ? "Yes" : "No", 
               sales[i].discountValue, 
               sales[i].customer.name, 
               sales[i].customer.age, 
               ctime(&sales[i].purchaseDate));
    }

    printf("\n************* END OF SALES DATA *************\n");
}


void viewCustomerFeedback()
{
    int i;

    if (totalFeedbacks == 0) {
        printf("No feedbacks available.\n");
        return;
    }

    printf("\n*************** CUSTOMER FEEDBACKS ***************\n");
    printf("%-20s%-50s%-10s\n", "Customer Name", "Comments", "Rating");

    for (i = 0; i < totalFeedbacks; i++) {
        printf("%-20s%-50s%-10d\n", feedback[i].customer.name, feedback[i].comments, feedback[i].rating);
    }

    printf("\n************* END OF FEEDBACKS *************\n");
}

void getCustomerFeedback() 
{
    char customerName[50];
    char comments[200];
    int rating;

    printf("Hi, your sweet name please: ");
    scanf("%49[^\n]", customerName);

    printf("Enter your comments: ");
    scanf("%199[^\n]", comments);

    printf("Enter your rating (1-5): ");
    scanf("%d", &rating);

    // Allocate memory for feedback array
    if (feedback == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(feedback[totalFeedbacks].customer.name, customerName);
    snprintf(feedback[totalFeedbacks].comments, sizeof(feedback[totalFeedbacks].comments), "%s", comments);
    feedback[totalFeedbacks].rating = (rating >= 1 && rating <= 5) ? rating : 0;

    FILE *file = openFile("feedbacks.csv", "a");
    if (file != NULL) {
        fprintf(file, "%s,%s,%d\n", customerName, comments, rating);
        closeFile(file);
        printf("Feedback submitted successfully!\n");
        totalFeedbacks++;  // Increment totalFeedbacks after successful addition
    } else {
        fprintf(stderr, "Error opening CSV file for writing.\n");
    }   
}

void retrieveDataFromFile()
{

}

void readMetadataFromFile() 
{
    FILE *file = openFile("metadata.csv", "r");
    if (file != NULL) 
    {
        fscanf(file, "%u,%u,%u\n", &totalCars, &totalSales, &totalFeedbacks);
        closeFile(file);
    } 
    else 
    {
        fprintf(stderr, "Error opening metadata.csv file for reading.\n");
    }
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
    for (int i = 0; i < totalCars - 1; i++) 
    {
        for (int j = 0; j < totalCars - i - 1; j++) 
        {
            if (cars[j].yearOfManufacture > cars[j + 1].yearOfManufacture) 
            {
                // Swap cars[j] and cars[j + 1]
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }

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

char getCharFromUser(char *msg) {
    char ch;
    printf("%s", msg);
    while (getchar() != '\n');
    scanf("%c", &ch);
    return ch;
}


int main() 
{
    readMetadataFromFile();

    // Allocate memory for the arrays
    cars = (Car *)malloc(2 * totalCars * sizeof(Car));
    sales = (Sale *)malloc(2 * totalSales * sizeof(Sale));
    feedback = (Feedback *)malloc(2 * totalFeedbacks * sizeof(Feedback));

    if (cars == NULL || sales == NULL || feedback == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    retrieveDataFromFile();

    int choice;
    char userType;

    printf("Are you a customer (c) or admin (a): ");

    while (getchar() != '\n');

    scanf("%c", &userType);

    switch (userType) 
    {
        case ADMIN:
                    while (true)
                    {
                        printf("1] Add new Stock.\n");
                        printf("2] View Sales Data.\n");
                        printf("3] EXIT\n");
                        printf("Enter your choice: ");
                        prompt(msg);

                        while (getchar() != '\n');
                        scanf("%d", &choice);

                        switch (choice) 
                        {
                            case 1: addNewStock();
                                    break;
                            case 2: viewSalesData(); 
                                    break;
                            case 3: goto exit;  // Using goto to break out of the loop
                            default: printf("Invalid choice\n");
                        }
                    }
                    break;
        case CUSTOMER:
                       while (true) 
                       {
                        printf("1] View Stocks.\n");
                        printf("2] Buy a car.\n");
                        printf("3] View or add Customer feedback.\n");
                        printf("4] Exit\n");
                        printf("Enter your choice: ");
                        prompt(msg);

                        while (getchar() != '\n');
                        scanf("%d", &choice);

                        clearScreen();

                        switch (choice) 
                        {
                            case 1: viewStocks(); 
                                    break;
                            case 2: buyCar(); 
                                    break;
                            case 3: customerFeedback(); 
                                    break;
                            case 4: goto exit;  // Using goto to break out of the loop
                            default: printf("Invalid choice\n");
                        }
                        
                    }
                    break;
        default:
            // Invalid user type
                    fprintf(stderr, "Invalid user type. Exiting.\n");
                    exit(EXIT_FAILURE);
    }

exit:
    // Free allocated memory before exiting
    free(cars);
    free(sales);
    free(feedback);

    return 0;
}