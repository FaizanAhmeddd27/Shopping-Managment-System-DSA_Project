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

// Advanced DSA algorithms
Product* binarySearchById(int targetId);  // Binary Search O(log n)
Product* mergeSortProducts(Product* node, bool sortByPrice=false);  // Merge Sort O(n log n)
void sortProductsByName();
void sortProductsByPrice();
void displayTopSellers(int topN = 5);  // Using Heap/Priority Queue

#endif // MANAGER_H