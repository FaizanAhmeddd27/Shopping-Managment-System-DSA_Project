#ifndef CUSTOMERCART_H
#define CUSTOMERCART_H

#include "manager.h"
#include "customerQueue.h"
#include <bits/stdc++.h>
using namespace std;

void cCart();
void displayCart();
void searchProductUI();  // Uses KMP Algorithm for pattern matching
void buyProductUI();     // Uses Hash Map for O(1) product lookup

#endif // CUSTOMERCART_H