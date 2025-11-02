#ifndef MANAGER_H
#define MANAGER_H

#include "animation.h"
#include <bits/stdc++.h>
using namespace std;

struct Product {
    int proId;
    string proName;
    double proPrice;
    int proNum;
    int soldCount;
    Product *next;
    Product(int id=0, const string &n="", double p=0.0, int q=0)
        : proId(id), proName(n), proPrice(p), proNum(q), soldCount(0), next(nullptr) {}
};

extern Product *head;

// product functions
int countProduct();
void addProduct();
void displayProduct();
void modifyProduct();
void deleteProduct();
void saveNewProductToFile(Product *temp);
void saveAllProductToFile();
void loadProductFromFile();
void deleteList();

// algorithms (merge sort)
Product* mergeSortProducts(Product* node, bool sortByPrice=false);
void sortProductsByName();
void sortProductsByPrice();
void displayTopSellers(int topN = 5);

#endif // MANAGER_H
