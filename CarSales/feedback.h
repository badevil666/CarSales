#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "customer.h"  // Include the customer header

typedef struct feedback 
{
    Customer customer;  // Reuse the Customer structure
    char comments[200];
    int rating;  // Assuming an integer rating, you can adjust based on your needs
} Feedback;
#endif // FEEDBACK_H