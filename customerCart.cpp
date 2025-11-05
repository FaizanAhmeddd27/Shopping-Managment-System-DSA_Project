#include "customerCart.h"
#include "animation.h"
#include <conio.h>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <map>
#include <set>

const int totalCart = 50;

/* ----- HASH MAP for fast product lookup ----- */
map<int, Product*> productCache;

/**
 * @brief Rebuilds the global productCache from the product linked list.
 *
 * Clears any existing entries in the global productCache and inserts a mapping
 * from each product's `proId` to its `Product*` by traversing the linked list
 * beginning at `head`.
 */
void buildProductCache() {
    productCache.clear();
    for (Product* t = head; t; t = t->next) {
        productCache[t->proId] = t;
    }
}

/**
 * @brief Presents the cart-allocation UI and assigns a cart number if available.
 *
 * Determines the number of remaining carts from the current queue and the store's capacity,
 * then displays an error message when no carts remain or shows the allocated cart number
 * with a confirmation message when a cart is available. The function blocks until a key is pressed.
 */
void cCart() {
    drawHeader("Grab a Cart");
    
    
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    
    drawAnimatedBox(30, 13, 50, 8, Color::BRIGHT_CYAN);
    
    if (remainCart <= 0) {
        typewriter("Cart Not Available Now", 40, 16, 30);
        showErrorToast("No carts available!", 60, 18);
    } else {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(38, 15);
        cout << "Please Grab a Cart \\_/";
        
        Sleep(300);
        pulseText("Your Cart Number: " + to_string(remainCart), 38, 17, 2);
        showSuccessToast("Cart allocated successfully!", 60, 19);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/**
 * @brief Visually displays the currently available shopping carts.
 *
 * Calculates the number of remaining carts, shows an error toast and returns if none are available,
 * otherwise renders numbered cart indicators in a colored grid with a short animation and waits for a key press.
 */
void displayCart() {
    drawHeader("Display Carts");
    int totalCustomer = countCustomerQueue();
    int remainCart = totalCart - totalCustomer;
    
    if (remainCart <= 0) {
        showErrorToast("No carts available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 11); 
    slowPrint("Existing carts are: ", Color::BRIGHT_YELLOW, 20);
    
    int x = 25, y = 13, perRow = 10;
    Color cartColors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    
    for (int i = 0; i < remainCart; ++i) {
        setColor(cartColors[i % 4]);
        gotoxy(x + (i % perRow) * 6, y + (i / perRow));
        cout << "[" << (i + 1) << "]";
        Sleep(30);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/**
 * @brief Computes the longest proper-prefix which is also suffix (LPS) array for a pattern.
 *
 * Builds an LPS array where each entry at index `i` is the length of the longest proper prefix
 * of `pattern[0..i]` that is also a suffix of `pattern[0..i]`.
 *
 * @param pattern The pattern string to analyze.
 * @return std::vector<int> LPS array: for each index `i`, the length of the longest proper prefix
 * that is also a suffix for the substring ending at `i`. A "proper" prefix excludes the whole substring.
 */
vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0, i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) len = lps[len - 1];
            else { lps[i] = 0; i++; }
        }
    }
    return lps;
}

/**
 * @brief Checks whether a pattern occurs inside a text using a case-insensitive substring search.
 *
 * @param text The text to search within.
 * @param pattern The substring pattern to find.
 * @return `true` if `pattern` is found in `text` (case-insensitive), `false` otherwise.
 */
bool kmpSearch(const string& text, const string& pattern) {
    string lowerText = text, lowerPattern = pattern;
    transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);
    
    int n = lowerText.length(), m = lowerPattern.length();
    vector<int> lps = computeLPS(lowerPattern);
    
    int i = 0, j = 0;
    while (i < n) {
        if (lowerText[i] == lowerPattern[j]) {
            i++; j++;
        }
        if (j == m) return true;
        else if (i < n && lowerText[i] != lowerPattern[j]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
    return false;
}

/**
 * @brief Presents an interactive UI to search products by name or partial name and displays matching entries.
 *
 * Prompts the user for a search key, performs a case-insensitive substring search across all products, and prints a formatted table of matches (ID, name, price, quantity). Shows an informational toast when no matches are found or a success toast when the search completes, then waits for a key press before returning.
 */
void searchProductUI() {
    drawHeader("Search Product - KMP Algorithm");
    
    drawAnimatedBox(15, 10, 80, 15, Color::BRIGHT_YELLOW);
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter Product name or partial: ";
    cin.ignore(); string key; getline(cin, key);
    
    showSpinner(800, 50, 13);
    
    Product* t = head; bool found = false;
    
    gotoxy(17, 14); 
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(12) << "Product ID" << setw(30) << "Product Name" << setw(10) << "Price" << setw(10) << "Qty";
    
    gotoxy(17, 15); 
    setColor(Color::BRIGHT_CYAN);
    cout << string(70, '=');
    
    int y = 16;
    Color rowColors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    int colorIdx = 0;
    
    while (t) {
        if (kmpSearch(t->proName, key)) {
            setColor(rowColors[colorIdx % 4]);
            gotoxy(17, y++);
            cout << left << setw(12) << t->proId << setw(30) << t->proName 
                 << setw(10) << t->proPrice << setw(10) << t->proNum;
            found = true;
            colorIdx++;
            Sleep(50);
        }
        t = t->next;
    }
    
    if (!found) {
        showErrorToast("Product Not Found!", 60, 18);
    } else {
        showSuccessToast("Search completed!", 60, y + 1);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/**
 * @brief Writes a timestamped plain-text receipt for a customer's purchase to disk.
 *
 * The function creates a file named "receipt_YYYYMMDD_HHMMSS.txt" in the current working
 * directory and writes customer information, a numbered list of purchased items with
 * quantities, and summary lines for total, payable amount, and payment method.
 *
 * If the file cannot be opened for writing, the function returns without creating a receipt.
 *
 * @param custName Customer's name to appear on the receipt.
 * @param items Vector of purchased items where each pair contains the product name and quantity.
 * @param total Sum of item prices before discounts or adjustments.
 * @param finalPay Final amount payable after discounts or adjustments.
 * @param paymentMethod Human-readable payment method label to record on the receipt.
 */
static void writeReceipt(const string &custName, const vector<pair<string, int>> &items,
                         double total, double finalPay, const string &paymentMethod) {
    auto t = time(nullptr); auto tm = *localtime(&t);
    char buf[64]; strftime(buf, sizeof(buf), "receipt_%Y%m%d_%H%M%S.txt", &tm);
    ofstream f(buf);
    if (!f.is_open()) return;
    f << "=== Shopping Mall Receipt ===\n";
    f << "Customer: " << custName << "\n";
    f << "Date: " << asctime(&tm);
    f << left << setw(5) << "No" << setw(30) << "Product" << setw(10) << "Qty" << setw(12) << "Price\n";
    f << string(60, '-') << "\n";
    int i = 1;
    for (auto &it : items) f << left << setw(5) << i++ << setw(30) << it.first << setw(10) << it.second << "\n";
    f << string(60, '-') << "\n";
    f << "Total: " << total << "\n";
    f << "Payable: " << finalPay << "\n";
    f << "Paid via: " << paymentMethod << "\n\nThank you!\n";
    f.close();
}

/**
 * @brief Interactive UI for creating a customer purchase, updating inventory, and processing payment.
 *
 * Presents a prompt-driven flow that lets a user select multiple products by ID and quantity,
 * validates availability, updates product stock and sold counts, records purchased items,
 * computes discounts based on the bill, enqueues the customer, persists product data, writes a timestamped receipt,
 * and displays a payment confirmation summary.
 *
 * - Skips entries for invalid product IDs or insufficient stock; if no valid items are added the function returns without completing a purchase.
 * - Applies a discount of 10% for bills greater than 1000, 5% for bills greater than 500, otherwise no discount.
 * - Captures a payment method choice and includes it in the receipt and confirmation display.
 *
 * Side effects:
 * - Mutates Product objects' `proNum` and `soldCount`.
 * - Calls enqueueCustomer(), saveAllProductToFile(), and writeReceipt().
 */
void buyProductUI() {
    drawHeader("Buy Products - Shopping Cart");
    
    if (!head) { 
        showErrorToast("There is no Product to Buy", 60, 14);
        gotoxy(30, 23); getch(); 
        return; 
    }

    buildProductCache();
    
    int number;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 11); cout << "How many different products: "; 
    setColor(Color::BRIGHT_YELLOW);
    cin >> number;
    
    int total = countProduct();
    if (number <= 0 || number > total) { 
        showErrorToast("Invalid number!", 60, 13);
        gotoxy(30, 23); getch(); 
        return; 
    }

    vector<pair<string, int>> items; 
    double bill = 0.0;
    
    for (int i = 0; i < number; ++i) {
        int id; 
        setColor(Color::BRIGHT_GREEN);
        gotoxy(20, 13 + i * 2); cout << "Product ID: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> id;
        
        Product* t = productCache[id];
        
        if (!t || productCache.find(id) == productCache.end()) { 
            showErrorToast("Product ID not found!", 60, 14 + i * 2);
            continue; 
        }
        
        int qty; 
        setColor(Color::BRIGHT_YELLOW);
        gotoxy(45, 13 + i * 2); cout << "Qty: "; 
        setColor(Color::BRIGHT_WHITE);
        cin >> qty;
        
        if (qty > t->proNum) { 
            showErrorToast("Not enough stock!", 75, 14 + i * 2);
            continue; 
        }
        
        t->proNum -= qty; 
        t->soldCount += qty;
        items.push_back({t->proName, qty});
        bill += t->proPrice * qty;
        
        showSuccessToast("Added to cart!", 75, 14 + i * 2);
    }

    if (items.empty()) {
        showErrorToast("No items in cart!", 60, 18);
        gotoxy(30, 23); getch();
        return;
    }

    string customerName, customerPhone; 
    cin.ignore();
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(20, 18 + number * 2); cout << "Enter Your name: "; 
    setColor(Color::BRIGHT_WHITE);
    getline(cin, customerName);
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(20, 19 + number * 2); cout << "Enter Phone No.: "; 
    setColor(Color::BRIGHT_WHITE);
    cin >> customerPhone;

    double totalBill = bill;
    double discount = (bill > 1000) ? 0.10 : (bill > 500) ? 0.05 : 0.0;
    double finalPay = bill * (1.0 - discount);

    Customer c{customerName, customerPhone, finalPay};
    enqueueCustomer(c);
    saveAllProductToFile();

    drawHeader("Payment Method");
    drawAnimatedBox(20, 10, 70, 12, Color::BRIGHT_GREEN);
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 12);  cout << "Select payment for " << customerName << ":";
    
    setColor(Color::BRIGHT_GREEN);
    gotoxy(25, 14); cout << "1. Easypaisa / JazzCash / Nayapay";
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 15); cout << "2. Credit/Debit Card";
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(25, 16); cout << "3. Cash Payment";
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(25, 18); cout << "Choice: ";
    char ch = getch();
    
    string method = "Unknown";
    if (ch == '1') method = "Easypaisa/JazzCash/Nayapay";
    else if (ch == '2') method = "Card";
    else if (ch == '3') method = "Cash";

    showSpinner(1000, 50, 20);
    writeReceipt(customerName, items, totalBill, finalPay, method);

    drawHeader("Payment Confirmation");
    drawAnimatedBox(20, 10, 70, 16 + items.size(), Color::BRIGHT_GREEN);
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 12); cout << "You have bought:";
    
    Color itemColors[] = {Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    for (size_t i = 0; i < items.size(); ++i) {
        setColor(itemColors[i % 4]);
        gotoxy(25, 14 + i); 
        cout << i + 1 << " | " << items[i].first << " (Qty: " << items[i].second << ")";
        Sleep(100);
    }
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(25, 16 + items.size()); cout << "Total sum: " << totalBill << " Rs";
    
    if (discount > 0) {
        setColor(Color::BRIGHT_GREEN);
        gotoxy(25, 17 + items.size());
        cout << "Discount applied: " << (int)(discount * 100) << "% (Saved: " << (int)(totalBill - finalPay) << " Rs)";
    }
    
    setColor(Color::BRIGHT_YELLOW);
    gotoxy(25, 18 + items.size()); cout << "Payable: " << finalPay << " Rs";
    
    setColor(Color::BRIGHT_MAGENTA);
    gotoxy(25, 19 + items.size()); cout << "Paid via: " << method;
    
    
    showSuccessToast("Thank you for shopping!", 60, 22 + items.size());
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 24); getch();
}

/**
 * @brief Prompt for a price range and display all products whose price falls within it.
 *
 * Prompts the user to enter a minimum and maximum price, then lists all products with
 * price >= minimum and <= maximum in a formatted table. If no products exist or no
 * products match the range, an informational message is shown. The function uses the
 * application's console UI and waits for a key press before returning.
 */
void filterByPriceRange() {
    drawHeader("Filter Products by Price - BST Concept");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    double minPrice, maxPrice;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 11); cout << "Enter minimum price: ";
    setColor(Color::BRIGHT_WHITE);
    cin >> minPrice;
    
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter maximum price: ";
    setColor(Color::BRIGHT_WHITE);
    cin >> maxPrice;
    
    showSpinner(600, 50, 13);
    
    drawAnimatedBox(10, 14, 80, 12, Color::BRIGHT_GREEN);
    
    gotoxy(12, 16);
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(12) << "ID" << setw(30) << "Product" << setw(12) << "Price" << setw(10) << "Stock";
    
    gotoxy(12, 17);
    setColor(Color::BRIGHT_CYAN);
    cout << string(70, '=');
    
    int y = 18, count = 0;
    Color rowColors[] = {Color::BRIGHT_WHITE, Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW};
    
    for (Product* t = head; t; t = t->next) {
        if (t->proPrice >= minPrice && t->proPrice <= maxPrice) {
            setColor(rowColors[count % 4]);
            gotoxy(12, y++);
            cout << left << setw(12) << t->proId 
                 << setw(30) << t->proName 
                 << setw(12) << t->proPrice 
                 << setw(10) << t->proNum;
            count++;
            Sleep(50);
        }
    }
    
    if (count == 0) {
        setColor(Color::BRIGHT_RED);
        gotoxy(25, 20);
        cout << "No products found in this price range";
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 26); getch();
}

/* ---------- NEW: WISHLIST (Set/HashSet DSA) ---------- */
set<int> wishlist;

/**
 * @brief Prompt for a product ID and add that product to the global wishlist if present.
 *
 * Prompts the user to enter a product ID, validates that products exist and the ID corresponds
 * to an existing product, then inserts the ID into the global wishlist if it is not already present.
 * Displays appropriate success, informational, or error messages and awaits a key press before returning.
 */
void addToWishlist() {
    drawHeader("Add to Wishlist - HashSet Implementation");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    int id;
    setColor(Color::BRIGHT_CYAN);
    gotoxy(20, 12); cout << "Enter Product ID to add to wishlist: ";
    setColor(Color::BRIGHT_WHITE);
    cin >> id;
    
    buildProductCache();
    
    if (productCache.find(id) == productCache.end()) {
        showErrorToast("Product not found!", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    if (wishlist.find(id) != wishlist.end()) {
        showInfoToast("Product already in wishlist!", 60, 14);
    } else {
        wishlist.insert(id);
        showSuccessToast("Added to wishlist!", 60, 14);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 23); getch();
}

/**
 * @brief Display the current wishlist of products in a formatted, colorized UI.
 *
 * If the global wishlist is empty, shows an informational toast and returns after a key press.
 * Otherwise rebuilds the product cache, opens a bordered results box, prints a header row
 * (ID, Product, Price) and then lists each wishlist product found in the cache with
 * alternating colors and a short delay between lines. Ends by waiting for a key press.
 */
void viewWishlist() {
    drawHeader("My Wishlist - Set Data Structure");
    
    if (wishlist.empty()) {
        showInfoToast("Your wishlist is empty", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    buildProductCache();
    
    drawAnimatedBox(10, 11, 80, 10 + wishlist.size(), Color::BRIGHT_MAGENTA);
    
    gotoxy(12, 13);
    setColor(Color::BRIGHT_YELLOW);
    cout << left << setw(12) << "ID" << setw(35) << "Product" << setw(12) << "Price";
    
    gotoxy(12, 14);
    setColor(Color::BRIGHT_CYAN);
    cout << string(65, '=');
    
    int y = 15;
    Color colors[] = {Color::BRIGHT_CYAN, Color::BRIGHT_GREEN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    int idx = 0;
    
    for (int id : wishlist) {
        if (productCache.find(id) != productCache.end()) {
            Product* p = productCache[id];
            setColor(colors[idx % 4]);
            gotoxy(12, y++);
            cout << left << setw(12) << p->proId 
                 << setw(35) << p->proName 
                 << setw(12) << p->proPrice;
            idx++;
            Sleep(80);
        }
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, y + 2); getch();
}

/**
 * @brief Presents top product recommendations to the user.
 *
 * Displays up to five cheapest products that have available stock in a bordered, animated UI box.
 * If no products exist, shows an error toast and returns. For each recommended product, prints
 * the product name, price, and stock with color variations and a short delay between entries,
 * then waits for a key press before returning.
 */
void showRecommendations() {
    drawHeader("Product Recommendations - Graph Algorithm");
    
    if (!head) {
        showErrorToast("No products available", 60, 14);
        gotoxy(30, 23); getch();
        return;
    }
    
    // Recommend top 5 cheapest products
    priority_queue<pair<double, Product*>, vector<pair<double, Product*>>, greater<pair<double, Product*>>> minHeap;
    
    for (Product* t = head; t; t = t->next) {
        if (t->proNum > 0) {
            minHeap.push({t->proPrice, t});
        }
    }
    
    drawAnimatedBox(10, 10, 75, 13, Color::BRIGHT_YELLOW);
    
    gotoxy(12, 12);
    setColor(Color::BRIGHT_YELLOW);
    cout << "TOP RECOMMENDED PRODUCTS (Best Value):";
    
    gotoxy(12, 14);
    setColor(Color::BRIGHT_CYAN);
    cout << left << setw(30) << "Product" << setw(12) << "Price" << setw(12) << "Stock";
    
    gotoxy(12, 15);
    cout << string(60, '=');
    
    int y = 16, count = 0;
    Color colors[] = {Color::BRIGHT_GREEN, Color::BRIGHT_CYAN, Color::BRIGHT_YELLOW, Color::BRIGHT_MAGENTA};
    
    while (!minHeap.empty() && count < 5) {
        auto item = minHeap.top(); minHeap.pop();
        Product* p = item.second;
        
        setColor(colors[count % 4]);
        gotoxy(12, y++);
        cout << left << setw(30) << p->proName 
             << setw(12) << p->proPrice 
             << setw(12) << p->proNum;
        count++;
        Sleep(100);
    }
    
    setColor(Color::BRIGHT_WHITE);
    gotoxy(30, 26); getch();
}