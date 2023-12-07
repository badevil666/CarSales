#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

/*
#include "car.h"
#include "sale.h"
#include "feedback.h"
#include "customer.h"
*/
// Constants for user types
#define CUSTOMER 'c'
#define ADMIN 'a'

// Age limits for discount eligibility
#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 25

// Discount percentage for eligible customers
#define DISCOUNT_PERCENTAGE 0.1f

// Maximum length for customer feedback
#define MAX_FEEDBACK_LENGTH 500

// Structure definition for customer information
typedef struct customer 
{
    char name[50];  // Name of the customer
    int age;        // Age of the customer
} Customer;

// Structure definition for customer feedback
typedef struct feedback 
{
    Customer customer;        // Customer information
    char comments[200];       // Feedback comments
    int rating;               // Numeric rating (assumed integer, adjust as needed)
} Feedback;

// Structure definition for sale details
typedef struct sale 
{
    char modelName[100];      // Car model name
    float totalPrice;         // Total price of the purchase
    int discountGiven;        // Flag indicating if a discount was given
    float discountValue;      // Value of the discount applied
    int numberOfCars;         // Number of cars purchased
    Customer customer;        // Customer information
    time_t purchaseDate;      // Purchase date and time
} Sale;

// Structure definition for car details
typedef struct car 
{
    char modelName[100];      // Car model name
    int yearOfManufacture;    // Year of manufacture
    int availability;         // Number of available cars
    float price;              // Price of the car
} Car;




// Declaration of variables to store total counts and arrays for car, sale, and feedback data
unsigned int totalCars = 0;         // Total count of cars
unsigned int totalSales = 0;        // Total count of sales
unsigned int totalFeedbacks = 0;    // Total count of feedbacks

Car *cars;                           // Array to store car data
Sale *sales;                         // Array to store sale data
Feedback *feedback;                  // Array to store feedback data
;


char *msg = "Enter$";  // A message string used for prompting user input


void pressEnterToContinue() 
{
    printf("Press Enter to continue...");  // Prompting the user to press Enter
    while (getchar() != '\n');  // Loop to consume any characters until a newline character (Enter key) is encountered
    getchar();  // Consume the Enter key to clear the input buffer
}


// Function to open a file with the given file name and mode
// Returns a file pointer or NULL if the opening fails
FILE* openFile(const char *fileName, const char *mode) 
{
    FILE *file = fopen(fileName, mode);  // Opening the file with specified mode
    if (file == NULL) {
        perror("Error opening file");  // Printing an error message if the file opening fails
    }
    return file;  // Returning the file pointer (can be NULL if opening fails)
}


// Function to close a file if the file pointer is not NULL
void closeFile(FILE *file) 
{
    if (file != NULL) 
    {
        fclose(file);  // Closing the file using fclose function
    }
}


// Function to sort the cars based on the year of manufacture in descending order
void sort()
{
    for (int i = 0; i < totalCars - 1; i++) 
    {
        for (int j = 0; j < totalCars - i - 1; j++) 
        {
            // Check if the current car's year of manufacture is less than the next car's year
            if (cars[j].yearOfManufacture < cars[j + 1].yearOfManufacture) 
            {
                // Swap the positions of cars[j] and cars[j + 1] to sort in descending order
                Car temp = cars[j];
                cars[j] = cars[j + 1];
                cars[j + 1] = temp;
            }
        }
    }
}

// Function to get a character input from the user with a prompt message
char getCharFromUser(char *msg) 
{
    char ch;
    printf("%s", msg);  // Display the prompt message
    while (getchar() != '\n');  // Clear the input buffer
    scanf("%c", &ch);  // Read a single character from the user
    return ch;
}


// Function to clear the console screen. Works on Windows using "cls" command.
void clearScreen()
{
    // Uses the system-specific command to clear the console screen.
    // In this case, "cls" is used, which works on Windows systems.
    // Note: This function may not work on Linux or other non-Windows systems.
    system("cls");
}


// Function to display a prompt message to the user.
void prompt(char* msg)
{
    // Prints the provided message to the console.
    printf("%s", msg);
}


// Function to update metadata information by writing it to the "metadata.csv" file.
void updateMetadata() 
{
    // Open the metadata.csv file in write mode
    FILE *metadataFile = openFile("metadata.csv", "w");
    
    // Check if the file was opened successfully
    if (metadataFile != NULL) 
    {
        // Write the totalCars, totalSales, and totalFeedbacks values to the file
        fprintf(metadataFile, "%u,%u,%u\n", totalCars, totalSales, totalFeedbacks);
        
        // Close the file after writing
        closeFile(metadataFile);
    } 
    else 
    {
        // Display an error message if the file couldn't be opened
        fprintf(stderr, "Error opening metadata.csv file for writing.\n");
    }
}



// Function to add new stock by taking input from the user and updating the "cars.csv" file.
void addNewStock() 
{
    char tempModel[100];
    int tempYear;
    int tempAvail;
    float tempPrice;
    
    // Clear the input buffer
    getchar();
    
    // Prompt the user to enter the name of the model
    printf("Enter the name of the model: ");
    // Read the input string, allowing up to 99 characters excluding the newline
    scanf(" %99[^\n]", tempModel);

    // Prompt the user to enter the year of manufacture
    printf("Enter the year of manufacture: ");
    // Read the integer input for the year of manufacture
    scanf("%d", &tempYear);

    // Prompt the user to enter the available quantity
    printf("Enter the available quantity: ");
    // Read the integer input for the available quantity
    scanf("%d", &tempAvail);

    // Prompt the user to enter the price
    printf("Enter the price: ");
    // Read the floating-point input for the price
    scanf("%f", &tempPrice);

    // Check if memory for the cars array is allocated
    if (cars == NULL) 
    {
        // Display an error message and exit if memory allocation fails
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Copy the entered values to the corresponding fields in the cars array
    snprintf(cars[totalCars].modelName, sizeof(cars[totalCars].modelName), "%s", tempModel);
    cars[totalCars].yearOfManufacture = tempYear;
    cars[totalCars].availability = tempAvail;
    cars[totalCars].price = tempPrice;

    // Open the "cars.csv" file in append mode
    FILE *file = openFile("cars.csv", "a");
    // Check if the file was opened successfully
    if (file != NULL) 
    {
        // Write the entered values to the file
        fprintf(file, "%s, %d, %d, %.2f\n", tempModel, tempYear, tempAvail, tempPrice);
        // Close the file after writing
        closeFile(file);
        // Display a success message
        printf("New stock added successfully!\n");
        // Increment totalCars after successful addition
        totalCars++;  
    } 
    else 
    {
        // Display an error message if the file couldn't be opened
        fprintf(stderr, "Error opening CSV file for writing.\n");
    }
    // Update metadata after adding new stock
    updateMetadata();
}



// Function to display the available stocks in a formatted table
void viewStocks() 
{
    int i;
    
    // Sort the cars array based on the year of manufacture
    sort(); 

    // Print the header of the table
    printf("%-5s%-20s%-10s%-15s%-10s\n", "SL No", "Model Name", "Year", "Price", "Availability");

    // Iterate through the cars array and print each entry in the table
    for (i = 0; i < totalCars; i++) 
    {
        // Print the formatted row with information about each car
        printf("%-5d%-20s%-10d%-15.2f%-10d\n", i + 1, cars[i].modelName, cars[i].yearOfManufacture, cars[i].price, cars[i].availability);
    }
}


// Function to update the cars file with the current information
void updateCars() 
{
    // Open the cars.csv file in write mode
    FILE *carsFile = openFile("cars.csv", "w");
    
    // Check if the file is successfully opened
    if (carsFile != NULL) 
    {
        // Write the current information for each car to the file
        for (unsigned int i = 0; i < totalCars; ++i) 
        {
            // Format and write the information for each car
            fprintf(carsFile, "%s, %d, %d, %.2f\n", 
                cars[i].modelName, 
                cars[i].yearOfManufacture, 
                cars[i].availability, 
                cars[i].price
            );
        }
        
        // Close the file after writing
        closeFile(carsFile);
    } 
    else 
    {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error opening cars.csv file for writing.\n");
    }
}


// Function to handle the car purchase process
void buyCar() 
{
    char choice;
    char customerName[50];
    char proof;

    int carIndex, quantity, customerAge;
    float price;

    // Prompt user for their name
    getchar();
    printf("Hi, your sweet name please: ");
    scanf(" %[^\n]", customerName);  // Adjusted the format specifier to avoid buffer overflow

    // Prompt user for their age
    printf("How old are you: ");
    scanf("%d", &customerAge);

    // Check if the customer is eligible for a discount based on age
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

    // Ask the user if they want to view the available cars before purchasing
    choice = getCharFromUser("Would you like to re-view the stock before proceeding to buy..(y/n)");

    if (choice == 'y') 
    {
        viewStocks();
    }

    // Prompt the user for the serial number of the car they want to purchase
    printf("Enter the serial number of the car: ");
    scanf("%d", &carIndex);
    --carIndex;

    // Validate the quantity of cars the user wishes to purchase
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

    // Calculate the total price before and after discount
    price = cars[carIndex].price * quantity;
    printf("Amount: %.2f\n", price);  // Corrected the format specifier
    price = price * DISCOUNT_PERCENTAGE;
    printf("Final Amount after discount has been applied: %.2f\n", price);  // Corrected the format specifier

    // Update car availability and record the sale details
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
        // Write sale details to the sales.csv file
        fprintf(salesFile, "%s, %.2f, %d, %.2f, %d, %s, %d, %ld\n",
        cars[carIndex].modelName,
        cars[carIndex].price * quantity,
        sales[totalSales].discountGiven,
        price,
        quantity,
        customerName,
        customerAge,
        time(NULL)
        );

        closeFile(salesFile);  
        totalSales++;  // Increment totalSales after successful addition
        printf("\n\nCongratulations! Your purchase is completed successfully.\n\n");
    } 
    else 
    {
        fprintf(stderr, "Error opening sales.csv file for writing.\n");
    }

    // Display the purchase receipt
    clearScreen();
    printf("\n\n*************** PURCHASE RECEIPT ***************\n");
    printf("Car Model: %s\n", cars[carIndex].modelName);
    printf("Total Price: $%.2f\n", sales[totalSales - 1].totalPrice);
    
    if (sales[totalSales - 1].discountGiven) 
    {
        printf("Discount Applied: Yes\n");
        printf("Discount Value: $%.2f\n", sales[totalSales- 1].discountValue);
    } 
    else 
    {
        printf("Discount Applied: No\n");
    }

    printf("Number of Cars: %d\n", sales[totalSales - 1].numberOfCars);
    printf("Customer Name: %s\n", sales[totalSales - 1].customer.name);
    printf("Customer Age: %d\n", sales[totalSales - 1].customer.age);
    printf("Purchase Date: %s", ctime(&sales[totalSales - 1].purchaseDate));
    printf("Final Amount: $%.2f\n", price);  // Corrected the format specifier
    printf("************* THANK YOU FOR YOUR PURCHASE *************\n");  
    updateMetadata();
}


// Function to display sales data
void viewSalesData() 
{
    // Check if there is any sales data available
    if (totalSales == 0) 
    {
        printf("No sales data available.\n");
        return;
    }

    // Display header for sales data
    printf("\n*************** SALES DATA ***************\n");
    printf("%-20s%-15s%-10s%-10s%-20s%-10s%-20s\n", "Car Model", "Total Price", "Discount", "Discount Value", "Customer Name", "Customer Age", "Purchase Date");

    // Loop through each sale and display relevant information
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

    // Display the end of sales data
    printf("\n************* END OF SALES DATA *************\n");

    // Update metadata after viewing sales data
    updateMetadata();
}



// Function to display customer feedback
void viewCustomerFeedback()
{
    int i;

    // Check if there is any feedback available
    if (totalFeedbacks == 0) {
        printf("No feedbacks available.\n");
        return;
    }

    // Display header for customer feedback
    printf("\n*************** CUSTOMER FEEDBACKS ***************\n");
    printf("%-20s%-50s%-10s\n", "Customer Name", "Comments", "Rating");

    // Loop through each feedback and display relevant information
    for (i = 0; i < totalFeedbacks; i++) {
        printf("%-20s%-50s%-10d\n", feedback[i].customer.name, feedback[i].comments, feedback[i].rating);
    }

    // Display the end of feedbacks
    printf("\n************* END OF FEEDBACKS *************\n");
}

// Function to get customer feedback
void getCustomerFeedback() 
{
    char customerName[50];
    char comments[200];
    int rating;

    // Prompt user for their name
    getchar();
    printf("Hi, your sweet name please: ");
    scanf(" %[^\n]", customerName);

    // Prompt user for their comments
    printf("Enter your comments: ");
    scanf(" %[^\n]", comments);

    // Prompt user for their rating (1-5)
    printf("Enter your rating (1-5): ");
    scanf(" %d", &rating);

    // Allocate memory for feedback array
    if (feedback == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Copy user input into feedback array
    strcpy(feedback[totalFeedbacks].customer.name, customerName);
    snprintf(feedback[totalFeedbacks].comments, sizeof(feedback[totalFeedbacks].comments), "%s", comments);
    
    // Validate and set the rating
    feedback[totalFeedbacks].rating = (rating >= 1 && rating <= 5) ? rating : 0;

    // Open the feedbacks.csv file in append mode
    FILE *file = openFile("feedbacks.csv", "a");
    if (file != NULL) {
        // Write feedback information to the file
        fprintf(file, "%s, %s, %d\n", customerName, comments, rating);
        closeFile(file);
        printf("Feedback submitted successfully!\n");
        totalFeedbacks++;  // Increment totalFeedbacks after successful addition
    } 
    else 
    {
        fprintf(stderr, "Error opening CSV file for writing.\n");
    }   
    
    // Update metadata after feedback submission
    updateMetadata();
}


// Function to retrieve cars from the cars.csv file
void retrieveCarsFromFile()
{
    // Open the cars.csv file in read mode
    FILE *carsFile = openFile("cars.csv", "r");
    if (carsFile == NULL) 
    {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error opening cars.csv file for reading.\n");
        return;
    }

    // Iterate through the cars array and read data from the file
    for (unsigned int i = 0; i < totalCars; ++i) 
    {
        // Use fscanf to read data from the file, format specified based on CSV structure
        if (fscanf(carsFile, "%99[^,],%d,%d,%f\n", cars[i].modelName, &cars[i].yearOfManufacture, &cars[i].availability, &cars[i].price) != 4) 
        {
            // Print an error message and exit if there's an issue reading data
            fprintf(stderr, "Error reading data from cars.csv.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file after reading data
    closeFile(carsFile);
}


// Function to retrieve sales data from the sales.csv file
void retrieveSalesFromFile() 
{
    // Open the sales.csv file in read mode
    FILE *salesFile = openFile("sales.csv", "r");
    if (salesFile == NULL) 
    {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error opening sales.csv file for reading.\n");
        return;
    }

    // Iterate through the sales array and read data from the file
    for (unsigned int i = 0; i < totalSales; ++i) 
    {
        // Use fscanf to read data from the file, format specified based on CSV structure
        if (fscanf(salesFile, "%99[^,],%f,%d,%f,%d,%49[^,],%d,%ld\n", sales[i].modelName, &sales[i].totalPrice, &sales[i].discountGiven, &sales[i].discountValue, &sales[i].numberOfCars, sales[i].customer.name, &sales[i].customer.age, &sales[i].purchaseDate) != 8) 
        {
            // Print an error message and exit if there's an issue reading data
            fprintf(stderr, "Error reading data from sales.csv.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file after reading data
    closeFile(salesFile);
}


// Function to retrieve feedback data from the feedbacks.csv file
void retrieveFeedbacksFromFile() 
{
    // Open the feedbacks.csv file in read mode
    FILE *feedbackFile = openFile("feedbacks.csv", "r");
    if (feedbackFile == NULL) 
    {
        // Print an error message if the file cannot be opened
        fprintf(stderr, "Error opening feedbacks.csv file for reading.\n");
        return;
    }

    // Iterate through the feedback array and read data from the file
    for (unsigned int i = 0; i < totalFeedbacks; ++i) 
    {
        // Use fscanf to read data from the file, format specified based on CSV structure
        if (fscanf(feedbackFile, "%49[^,],%199[^,],%d\n", feedback[i].customer.name, feedback[i].comments, &feedback[i].rating) != 3) 
        {
            // Print an error message and exit if there's an issue reading data
            fprintf(stderr, "Error reading data from feedbacks.csv.\n");
            exit(EXIT_FAILURE);
        }
    }

    // Close the file after reading data
    closeFile(feedbackFile);
}

// Function to retrieve all data (cars, sales, feedbacks) from their respective CSV files
void retrieveDataFromFile()
{
    // Call individual functions to retrieve data for each entity
    retrieveCarsFromFile();
    retrieveSalesFromFile();
    retrieveFeedbacksFromFile();
}


// Function to read metadata (totalCars, totalSales, totalFeedbacks) from the metadata.csv file
void readMetadataFromFile() 
{
    // Open the metadata.csv file in read mode
    FILE *file = openFile("metadata.csv", "r");
    
    // Check if the file is opened successfully
    if (file != NULL) 
    {
        // Read the values of totalCars, totalSales, and totalFeedbacks from the file
        fscanf(file, "%u,%u,%u\n", &totalCars, &totalSales, &totalFeedbacks);
        
        // Close the file after reading
        closeFile(file);
    } 
    else 
    {
        // Print an error message if there's an issue opening the file
        fprintf(stderr, "Error opening metadata.csv file for reading.\n");
    }
}





// Main function for the car dealership management system
int main() 
{
    // Clear the screen and read metadata information from the file
    clearScreen();
    readMetadataFromFile();

    // Allocate memory for the arrays based on metadata values
    cars = (Car *)malloc(2 * totalCars * sizeof(Car));
    sales = (Sale *)malloc(2 * totalSales * sizeof(Sale));
    feedback = (Feedback *)malloc(2 * totalFeedbacks * sizeof(Feedback));

    // Check if memory allocation is successful
    if (cars == NULL || sales == NULL || feedback == NULL) 
    {
        // Print an error message and exit if memory allocation fails
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    // Retrieve data from files (cars.csv, sales.csv, feedbacks.csv)
    retrieveDataFromFile();

    int choice;
    char userType;

    // Prompt the user to select their role (customer or admin)
    printf("Are you a customer (c) or admin (a): ");

    // Flush the standard input buffer to avoid issues with newline characters
    fflush(stdin);
    
    // Read the user type (customer or admin)
    scanf("%c", &userType);

    // Clear the screen for a clean interface
    clearScreen();

    // Switch statement to handle admin-specific actions
    switch (userType) 
    {
        case 'a':
            while (true)  // Infinite loop for admin actions
            {
                // Display admin menu options
                printf("1] Add new Stock.\n");
                printf("2] View Sales Data.\n");
                printf("3] EXIT\n\n");
                printf("Enter your choice\n ");
                prompt(msg);

                // Flush the standard input buffer to avoid issues with newline characters
                fflush(stdin);
                
                // Read the choice input
                scanf("%d", &choice);

                // Clear the screen for a clean interface
                clearScreen();

                // Switch statement to handle admin menu choices
                switch (choice) 
                {
                    case 1: 
                        addNewStock();
                        break;

                    case 2: 
                        viewSalesData(); 
                        break;

                    case 3: 
                        goto exit;  // Using goto to break out of the loop

                    default: 
                        printf("Invalid choice\n");
                }
            }
            break;
            // Switch statement to handle customer-specific actions
    
        case 'c':
            while (true)  // Infinite loop for customer actions
            {
                // Display customer menu options
                clearScreen();
                printf("1] View Stocks.\n");
                printf("2] Buy a car.\n");
                printf("3] View or add Customer feedback.\n");
                printf("4] Exit\n");
                printf("Enter your choice\n");
                prompt(msg);

                // Flush the standard input buffer to avoid issues with newline characters
                fflush(stdin);

                // Read the choice input
                scanf("%d", &choice);

                // Clear the screen for a clean interface
                clearScreen();

                // Switch statement to handle customer menu choices
                switch (choice) 
                {
                    case 1: 
                        viewStocks(); 
                        break;

                    case 2: 
                        buyCar(); 
                        
                        // Prompt for feedback if the user chooses to give feedback
                        printf("Would you like to give us your feedback about our service(y/n) : ");
                        fflush(stdin);
                        scanf(" %c", &userType);
                        if (userType == 'y')
                            getCustomerFeedback();
                        break;

                    case 3: 
                        viewCustomerFeedback(); 
                        break;

                    case 4: 
                        goto exit;  // Using goto to break out of the loop

                    default: 
                        printf("Invalid choice\n");
                }
                
                // Prompt the user to press Enter to continue
                pressEnterToContinue();
            }
            break;

        default:
            // Invalid user type
            fprintf(stderr, "Invalid user type. Exiting.\n");
            exit(EXIT_FAILURE);
    }

    // Label for the exit point of the program
    exit:
    // Update the cars information before exiting
    updateCars();

    // Free allocated memory before exiting
    free(cars);
    free(sales);
    free(feedback);

    return 0;
}
