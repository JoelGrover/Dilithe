#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100

struct Item {
    int id;
    char name[50];
    float price;
};

struct Item store[MAX_ITEMS];
int itemCount = 0;

struct Purchase {
    int itemID;
    char itemName[50];
    int quantity;
    float cost;
};

struct Purchase purchases[MAX_ITEMS];
int purchaseCount = 0;

void addItem() {
    if (itemCount >= MAX_ITEMS) {
        printf("Store is full. Cannot add more items.\n");
        return;
    }

    struct Item newItem;

    printf("Enter item id: ");
    scanf("%d", &newItem.id);
    printf("Enter item name: ");
    scanf("%s", newItem.name);
    printf("Enter item price: ");
    scanf("%f", &newItem.price);

    store[itemCount++] = newItem;

    printf("Item added successfully.\n");
}

void modifyItem() {
    int id;
    int i;

    printf("Enter item id to modify: ");
    scanf("%d", &id);

    for (i = 0; i < itemCount; i++) {
        if (store[i].id == id) {
            printf("Enter new item name: ");
            scanf("%s", store[i].name);
            printf("Enter new item price: ");
            scanf("%f", &store[i].price);

            printf("Item modified successfully.\n");
            return;
        }
    }

    printf("Item not found.\n");
}

void listItems() {
    int i;

    if (itemCount == 0) {
        printf("No items found in the store.\n");
        return;
    }

    printf("Item List:\n");
    printf("ID\tName\t\tPrice\n");
    printf("--------------------------------\n");

    for (i = 0; i < itemCount; i++) {
        printf("%d\t%s\t\t%.2f\n", store[i].id, store[i].name, store[i].price);
    }
}

void generateBill() {
    int id;
    int quantity;
    int i;
    float total = 0.0;

    printf("Enter item id for billing (0 to stop): ");
    scanf("%d", &id);

    while (id != 0) {
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        for (i = 0; i < itemCount; i++) {
            if (store[i].id == id) {
                float itemTotal = store[i].price * quantity;
                printf("Item: %s\tPrice: %.2f\tQuantity: %d\tTotal: %.2f\n", store[i].name, store[i].price, quantity, itemTotal);
                total += itemTotal;

                // Store purchase information
                purchases[purchaseCount].itemID = id;
                strcpy(purchases[purchaseCount].itemName, store[i].name);
                purchases[purchaseCount].quantity = quantity;
                purchases[purchaseCount].cost = itemTotal;
                purchaseCount++;

                break;
            }
        }

        printf("Enter item id for billing (0 to stop): ");
        scanf("%d", &id);
    }

    printf("-------------------------------\n");
    printf("Current Bill Total: %.2f\n", total);
}

void savePurchases(FILE *userFile) {
    fprintf(userFile, "\nPurchase History:\n");
    fprintf(userFile, "Item ID\tItem Name\tQuantity\tCost\n");
    fprintf(userFile, "----------------------------------------\n");
    
    for (int i = 0; i < purchaseCount; i++) {
        fprintf(userFile, "%d\t\t%s\t\t%d\t\t%.2f\n", purchases[i].itemID, purchases[i].itemName, purchases[i].quantity, purchases[i].cost);
    }
}

int main() {
    char userName[50];
    char phoneNumber[15];
    
    printf("Welcome to the Departmental Store Management\n");
    printf("-----------------------------------------\n");
    printf("Enter your name: ");
    scanf("%s", userName);
    printf("Enter your phone number: ");
    scanf("%s", phoneNumber);
    
    // Create a filename based on the user's phone number and name
    char fileName[70];
    snprintf(fileName, sizeof(fileName), "%s_%s.txt", phoneNumber, userName);
    
    FILE *userFile = fopen(fileName, "a+"); // Open for appending
    if (userFile == NULL) {
        printf("Error creating user file.\n");
        return 1;
    }
    
    fseek(userFile, 0, SEEK_END);
    if (ftell(userFile) == 0) {
        // Save user's name and phone number only if the file is empty
        fprintf(userFile, "User: %s\nPhone Number: %s\n\n", userName, phoneNumber);
    }
    
    fseek(userFile, 0, SEEK_SET);

    int choice;

    do {
        printf("\nMain Menu\n");
        printf("-----------------------------\n");
        printf("1. Add Item\n");
        printf("2. Modify Item\n");
        printf("3. List Items\n");
        printf("4. Generate Bill\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                modifyItem();
                break;
            case 3:
                listItems();
                break;
            case 4:
                generateBill();
                savePurchases(userFile); // Save purchase history
                purchaseCount = 0; // Reset purchase history
                break;
            case 5:
                printf("Thank You for Visiting!\n");
                fclose(userFile);  // Close the user's file
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (1);

    fclose(userFile);
    return 0;
}
