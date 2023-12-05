#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "constants.h"

typedef struct car 
{
    char modelName[100];
    unsigned int yearOfManufacture;
    unsigned int availability;
    float price;
} Car;

unsigned int totalCars;

Car *cars;

char *msg = "Enter$";

void addNewStock()
{
    char tempModel[100];
    int tempYear;
    int tempAvail;
    float tempPrice;
    
    printf("Enter the name of the model : ");
    scanf("%[^\n]", &tempModel);

    printf("Enter the year of manufacture: ");
    scanf("%d", &tempYear);

    printf("Enter the available quantity: ");
    scanf("%d", &tempAvail);

    printf("Enter the price: ");
    scanf("%f", &tempPrice);

    totalCars++;

    snprintf(cars[totalCars].modelName, sizeof(cars[totalCars].modelName), "%s", tempModel);
    cars[totalCars].yearOfManufacture = tempYear;
    cars[totalCars].availability = tempAvail;
    cars[totalCars].price = tempPrice;

    FILE *file = openFile("cars.csv", "a");
    if(file != NULL)
    {
        fprintf(file, "%s,%d,%d,%.2f\n", tempModel, tempYear, tempAvail, tempPrice);
        closeFile(file);
        printf("New stock added successfully!\n");   
    }
    else
    {
        printf("Error opening CSV file for writing.\n");
    }

}

void viewStocks()
{
    int i;
    sort();
    
    printf("%-5s%-20s%-10s%-15s%-10s\n", "SL No", "Model Name", "Year", "Price", "Availability");
    
    for(i = 0; i < totalCars; i++)
    {
        printf("%-5s%-20s%-10s%-15s%-10s\n", i + 1, cars[i].modelName, cars[i].yearOfManufacture, cars[i].price, cars[i].availability);
    }
}

void buyCar()
{
    char choice;
    char customerName[50];

    int carIndex, quantity;
    choice = getCharFromUser("Would you like to re-view the stock before proceeding to buy..\n");
    prompt(msg);

    scanf("%c", &choice);

    if(choice == 'y')
    {
        viewStocks();
    }

    printf("Enter the serial number of the car : ");
    scanf("%d", &carIndex);
    do
    {   
        printf("The available number of cars are %d\n", cars[carIndex].availability);

        printf("How many of them do you wish to purchase : ");
        scanf("%d", &quantity);  

    }while(quantity > 1 && quantity < cars[carIndex].availability);

    


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

FILE* openFile(const char *fileName, const char *mode)
{
    FILE *file = fopen(fileName, mode);
    if(file == NULL)
    {
        perror("Error opening file");
    }
    return file;
}

void closeFile(FILE *file)
{
    if(file != NULL)
    {
        fclose(file);
    }    
}

char getCharFromUser(char *msg)
{
    char ch;
    printf("%s", msg);
    scanf("%c", &ch);
    return ch;
}

void main()
{
    retrieveDataFromFile();

    int choice;
    char userType;

    
    printf("Are you a customer(c) or admin(a) : ");
    scanf("%c", &userType);
    if(userType == ADMIN)
    {
        while(true)
        {
            printf("1] Add new Stock.\n");
            printf("2] View Sales Data. \n");
        }
        
    }
    else
    {
        while(true)
        {

            printf("What would you like to do.\n\n");
            printf("1] View Stocks.\n");
            printf("2] Buy a car.\n");
            printf("3] View or add Customer feedback.\n\n");
            printf("Enter your choice \n");
            prompt(msg);
            scanf("%d", choice);

            clearScreen();

            switch(choice)
            {
                case 1 : viewStocks();
                         break;
                
                case 2 : buyCar();
                         break;

                case 3 : customerFeedback();
                         break;

                default : printf("<<<<<-----Invalid choice----->>>>>"); 
    
            }


        }
    }
    

}