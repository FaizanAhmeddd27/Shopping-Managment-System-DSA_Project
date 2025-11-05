#ifndef CUSTOMERQUEUE_H
#define CUSTOMERQUEUE_H

#include "animation.h"
#include <bits/stdc++.h>
using namespace std;

struct Customer {
    string cname;
    string cphone;
    double cbill;
};

// Queue operations (FIFO - First In First Out)
void enqueueCustomer(const Customer &c);
void dequeueCustomer();
int countCustomerQueue();
void loadCustomerFromFile();
void saveAllCustomerToFile();
void customerListDisplay();

#endif // CUSTOMERQUEUE_H