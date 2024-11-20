#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <conio.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

struct Product
{
    int id;
    char name[50];
    float price;
    int quantity;
};

struct User
{
    char username[50];
    char password[50];
    int loyaltyPoints;
};

void searchUserLoyaltyPoints()
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        printf(RED "Error opening users file!\n" RESET);
        return;
    }

    struct User user;

    char searchUsername[50];
    int found = 0;

    printf("Enter username to search for loyalty points: ");
    scanf("%s", searchUsername);

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.loyaltyPoints) != EOF)
    {
        if (strcmp(user.username, searchUsername) == 0)
        {
            found = 1;
            printf(GREEN "User found:\n" RESET);
            printf("Username: %s\n", user.username);
            printf("Loyalty Points: %d\n\n", user.loyaltyPoints);
            break;
        }
    }

    if (!found)
    {
        printf(RED "User with username '%s' not found.\n\n" RESET, searchUsername);
    }

    fclose(file);
}

void displayProducts()
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
    {
        printf(RED "Error opening products file!\n" RESET);
        return;
    }

    struct Product p;
    printf(GREEN "Product List:\n\n" RESET);
    printf("ID\tName\n");
    printf("--------------------\n");

    while (fscanf(file, "ID: %d\nName: %s\nPrice: %f\nQuantity: %d\n\n", &p.id, p.name, &p.price, &p.quantity) != EOF)
    {
        printf("%d\t%s\n", p.id, p.name);
    }

    fclose(file);
    printf("\n");
}

void showLoyaltyPoints(int *loyaltyPoints)
{
    printf(GREEN "You currently have %d loyalty point(s).\n" RESET, *loyaltyPoints);
}

void SalesHistory()
{
    struct dirent *entry;
    DIR *dir = opendir(".");

    if (dir == NULL)
    {
        printf(RED "Error opening directory!\n" RESET);
        return;
    }

    printf(GREEN "Admin Sales History (All Users):\n\n" RESET);

    while ((entry = readdir(dir)) != NULL)
    {
        if (strstr(entry->d_name, "_sales.txt") != NULL)
        {
            printf(GREEN "Sales file: %s\n" RESET, entry->d_name);

            FILE *file = fopen(entry->d_name, "r");
            if (file == NULL)
            {
                printf(RED "Error opening %s file!\n" RESET, entry->d_name);
                continue;
            }

            char ch;
            while ((ch = fgetc(file)) != EOF)
            {
                putchar(ch);
            }
            printf("\n");
            fclose(file);
        }
    }

    closedir(dir);
}

int isUsernameUnique(const char *username)
{
    FILE *file = fopen("users.txt", "r");
    if (file == NULL)
    {
        return 1;
    }

    struct User existingUser;
    while (fscanf(file, "%s %s %d", existingUser.username, existingUser.password, &existingUser.loyaltyPoints) != EOF)
    {
        if (strcmp(existingUser.username, username) == 0)
        {
            fclose(file);
            return 0;
        }
    }

    fclose(file);
    return 1;
}

void registerUser()
{
    FILE *file = fopen("users.txt", "a");
    if (file == NULL)
    {
        printf(RED "Error opening users.txt file!\n" RESET);
        return;
    }

    struct User user;
    printf("Enter username: ");
    scanf("%s", user.username);

    if (!isUsernameUnique(user.username))
    {
        printf(RED "Username already exists. Please choose a different username.\n\n" RESET);
        fclose(file);
        return;
    }

    printf("Enter password: ");
    int i = 0;
    while (1)
    {
        char ch = getch();
        if (ch == '\r')
        {
            user.password[i] = '\0';
            break;
        }
        else if (ch == '\b' && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else
        {
            user.password[i++] = ch;
            printf("*");
        }
    }
    printf("\n\n");
    user.loyaltyPoints = 0;

    fprintf(file, "%s %s %d\n", user.username, user.password, user.loyaltyPoints);
    fclose(file);

    printf(GREEN "User registered successfully!\n\n" RESET);
}

int loginUser(char *loggedInUser, int *loyaltyPoints)
{
    char username[50], password[50];
    char storedUsername[50], storedPassword[50];
    int storedLoyaltyPoints;
    FILE *file = fopen("users.txt", "r");

    if (file == NULL)
    {
        printf(RED "Error opening users.txt file!\n" RESET);
        return 0;
    }

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    int i = 0;
    while (1)
    {
        char ch = getch();
        if (ch == '\r')
        {
            password[i] = '\0';
            break;
        }
        else if (ch == '\b' && i > 0)
        {
            i--;
            printf("\b \b");
        }
        else
        {
            password[i++] = ch;
            printf("*");
        }
    }
    printf("\n\n");

    while (fscanf(file, "%s %s %d", storedUsername, storedPassword, &storedLoyaltyPoints) != EOF)
    {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0)
        {
            strcpy(loggedInUser, username);
            *loyaltyPoints = storedLoyaltyPoints;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    printf(RED "Invalid username or password.\n\n" RESET);
    return 0;
}

void updateUserLoyaltyPoints(const char *username, int loyaltyPoints)
{
    FILE *file = fopen("users.txt", "r");
    FILE *tempFile = fopen("temp_users.txt", "w");
    struct User user;

    if (file == NULL || tempFile == NULL)
    {
        printf(RED "Error opening users.txt or temp_users.txt file!\n" RESET);
        return;
    }

    while (fscanf(file, "%s %s %d", user.username, user.password, &user.loyaltyPoints) != EOF)
    {
        if (strcmp(user.username, username) == 0)
        {
            user.loyaltyPoints = loyaltyPoints;
        }
        fprintf(tempFile, "%s %s %d\n", user.username, user.password, user.loyaltyPoints);
    }

    fclose(file);
    fclose(tempFile);
    remove("users.txt");
    rename("temp_users.txt", "users.txt");
}

int isAdmin(char *username)
{
    return strcmp(username, "admin") == 0;
}

void addProduct(struct Product p)
{
    FILE *file = fopen("products.txt", "a");
    if (file == NULL)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }
    fprintf(file, "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n\n", p.id, p.name, p.price, p.quantity);
    fclose(file);
    printf(GREEN "Product added successfully!\n\n" RESET);
}

void updateProductById(int searchId, int updateChoice, float newValue)
{
    FILE *file = fopen("products.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        printf(RED "Error opening file!\n\n" RESET);
        return;
    }

    struct Product p;
    int found = 0;

    while (fscanf(file, "ID: %d\nName: %s\nPrice: %f\nQuantity: %d\n\n", &p.id, p.name, &p.price, &p.quantity) != EOF)
    {
        if (p.id == searchId)
        {
            found = 1;
            if (updateChoice == 1)
            {
                p.price = newValue;
            }
            else if (updateChoice == 2)
            {
                p.quantity = (int)newValue;
            }
        }
        fprintf(tempFile, "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n\n", p.id, p.name, p.price, p.quantity);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf(GREEN "Product updated successfully!\n\n" RESET);
    }
    else
    {
        printf(RED "Product with ID %d not found.\n\n" RESET, searchId);
        remove("temp.txt");
    }
}

void deleteProductById(int searchId)
{
    FILE *file = fopen("products.txt", "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        printf(RED "Error opening file!\n\n" RESET);
        return;
    }

    struct Product p;
    int found = 0;

    while (fscanf(file, "ID: %d\nName: %s\nPrice: %f\nQuantity: %d\n\n", &p.id, p.name, &p.price, &p.quantity) != EOF)
    {
        if (p.id != searchId)
        {
            fprintf(tempFile, "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n\n", p.id, p.name, p.price, p.quantity);
        }
        else
        {
            found = 1;
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("products.txt");
        rename("temp.txt", "products.txt");
        printf(GREEN "Product with ID %d deleted successfully.\n\n" RESET, searchId);
    }
    else
    {
        printf(RED "Product with ID %d not found.\n\n" RESET, searchId);
        remove("temp.txt");
    }
}

int getProductById(int searchId, struct Product *product)
{
    FILE *file = fopen("products.txt", "r");
    if (file == NULL)
    {
        printf(RED "Error opening file!\n\n" RESET);
        return 0;
    }

    struct Product p;
    while (fscanf(file, "ID: %d\nName: %s\nPrice: %f\nQuantity: %d\n\n", &p.id, p.name, &p.price, &p.quantity) != EOF)
    {
        if (p.id == searchId)
        {
            *product = p;
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void saveTransactionToFile(int productId, const char *productName, float price, int quantity, float subtotal, float totalPrice)
{
    FILE *file = fopen("sales.txt", "a");
    if (file == NULL)
    {
        printf(RED "Error opening sales.txt file!\n" RESET);
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(file, "Date: %02d-%02d-%04d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(file, "Product ID: %d\nProduct Name: %s\nPrice: %.2f\nQuantity: %d\nSubtotal: %.2f\nTotal Price: %.2f\n\n",
            productId, productName, price, quantity, subtotal, totalPrice);
    fclose(file);
}

float applyDiscount(float totalPrice, int *loyaltyPoints)
{
    float discountPercentage = 0.0;

    if (*loyaltyPoints >= 10)
    {
        char redeemChoice;
        printf("You have %d loyalty points. Would you like to redeem 10 points for a 10%% discount? (y/n): ", *loyaltyPoints);
        scanf(" %c", &redeemChoice);

        if (redeemChoice == 'y' || redeemChoice == 'Y')
        {
            discountPercentage = 10.0;
            *loyaltyPoints -= 10;
            printf(GREEN "10 loyalty points redeemed for an additional 10%% discount!\n" RESET);
        }
    }

    if (discountPercentage > 0)
    {
        float discountAmount = (discountPercentage / 100) * totalPrice;
        float discountedTotal = totalPrice - discountAmount;

        printf(GREEN "Discount applied: %.2f%% (%.2f)\n" RESET, discountPercentage, discountAmount);
        printf(GREEN "Total after discount: %.2f\n\n" RESET, discountedTotal);

        return discountedTotal;
    }
    else
    {
        printf("No discount applied.\n");
        return totalPrice;
    }
}

void generateBill(char *username, int *loyaltyPoints)
{
    int productId, quantity;
    float totalPrice = 0;
    float originalTotalPrice = 0;
    char choice;

    char salesFileName[60];
    sprintf(salesFileName, "%s_sales.txt", username);

    FILE *salesFile = fopen(salesFileName, "a");
    if (salesFile == NULL)
    {
        printf(RED "Error opening user's sales file!\n" RESET);
        return;
    }

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(salesFile, "Transaction Date: %02d-%02d-%04d %02d:%02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);

    do
    {
        printf("Enter Product ID to add to the bill: ");
        scanf("%d", &productId);

        struct Product product;
        if (getProductById(productId, &product))
        {
            printf("Enter quantity for %s: ", product.name);
            scanf("%d", &quantity);

            if (quantity <= product.quantity)
            {
                float subtotal = product.price * quantity;
                printf(GREEN "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\nSubtotal: %.2f\n\n" RESET, product.id, product.name, product.price, quantity, subtotal);
                totalPrice += subtotal;
                originalTotalPrice += subtotal;

                if (subtotal >= 1000)
                {
                    *loyaltyPoints += 1;
                    printf(GREEN "You earned 1 loyalty point! Total loyalty points: %d\n" RESET, *loyaltyPoints);
                }

                product.quantity -= quantity;
                updateProductById(productId, 2, product.quantity);

                fprintf(salesFile, "Product ID: %d, Name: %s, Price: %.2f, Quantity: %d, Subtotal: %.2f\n", product.id, product.name, product.price, quantity, subtotal);
            }
            else
            {
                printf(RED "Not enough stock for %s! Available stock: %d\n\n" RESET, product.name, product.quantity);
            }
        }
        else
        {
            printf(RED "Product with ID %d not found.\n\n" RESET, productId);
        }

        printf("Do you want to add another product to the bill? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    float discountedTotalPrice = applyDiscount(totalPrice, loyaltyPoints);

    if (discountedTotalPrice < originalTotalPrice)
    {
        float discountAmount = originalTotalPrice - discountedTotalPrice;
        float discountPercentage = (discountAmount / originalTotalPrice) * 100;
        fprintf(salesFile, "Discount: %.2f%% (%.2f)\n", discountPercentage, discountAmount);
    }
    else
    {
        fprintf(salesFile, "Discount:  0%% (0.00)\n");
    }

    fprintf(salesFile, "Final Total Price: %.2f\n\n", discountedTotalPrice);

    printf(GREEN "Final Total Price (after discount): %.2f\n\n" RESET, discountedTotalPrice);

    fclose(salesFile);

    updateUserLoyaltyPoints(username, *loyaltyPoints);
}

void viewSalesData(char *username)
{
    char salesFileName[60];
    sprintf(salesFileName, "%s_sales.txt", username);

    FILE *file = fopen(salesFileName, "r");
    if (file == NULL)
    {
        printf(RED "Error opening sales file or no sales data available for %s!\n" RESET, username);
        return;
    }

    char ch;
    printf(GREEN "Sales Data for %s:\n\n" RESET, username);

    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}

// int main()
// {
//     int choice;
//     int loyaltyPoints = 0;
//     char loggedInUser[50] = "";

//     printf("---------------------------------------------\n");
//     printf("Welcome to the Product Management System\n");
//     printf("---------------------------------------------\n");
//     printf("1. Register\n");
//     printf("2. Login\n");
//     printf("Select an option (1-2): ");
//     scanf("%d", &choice);
//     printf("\n");

//     if (choice == 1)
//     {
//         registerUser();
//         return 0;
//     }
//     else if (choice == 2)
//     {
//         if (!loginUser(loggedInUser, &loyaltyPoints))
//         {
//             return 0;
//         }
//     }
//     else
//     {
//         printf(RED "Invalid choice! Exiting.\n" RESET);
//         return 0;
//     }

//     if (isAdmin(loggedInUser))
//     {

//         do
//         {
//             printf("---------------------------------\n");
//             printf("Product Management System\n");
//             printf("---------------------------------\n");
//             printf("1. Add Product\n");
//             printf("2. Update Product\n");
//             printf("3. Delete Product\n");
//             printf("4. Find Product By ID\n");
//             printf("5. View Sales Data\n");
//             printf("6. Display Products\n");
//             printf("7. Search Users for loyalty points\n");
//             printf("8. Show transaction history for users\n");
//             printf("9. Exit\n");
//             printf("Select an option (1-9): ");
//             scanf("%d", &choice);
//             printf("\n");

//             struct Product product;
//             switch (choice)
//             {
//             case 1:
//                 printf("Enter Product ID: ");
//                 scanf("%d", &product.id);
//                 printf("Enter Product Name: ");
//                 scanf("%s", product.name);
//                 printf("Enter Product Price: ");
//                 scanf("%f", &product.price);
//                 printf("Enter Product Quantity: ");
//                 scanf("%d", &product.quantity);
//                 addProduct(product);
//                 break;

//             case 2:
//             {
//                 int searchId, updateChoice;
//                 float newValue;
//                 printf("Enter Product ID to update: ");
//                 scanf("%d", &searchId);
//                 printf("What do you want to update? (1 for Price, 2 for Quantity): ");
//                 scanf("%d", &updateChoice);
//                 printf("Enter new value: ");
//                 scanf("%f", &newValue);
//                 updateProductById(searchId, updateChoice, newValue);
//             }
//             break;

//             case 3:
//             {
//                 int searchId;
//                 printf("Enter Product ID to delete: ");
//                 scanf("%d", &searchId);
//                 deleteProductById(searchId);
//             }
//             break;

//             case 4:
//             {
//                 struct Product product;
//                 int searchId;

//                 printf("Enter product ID to search: ");
//                 scanf("%d", &searchId);

//                 if (getProductById(searchId, &product))
//                 {
//                     printf(GREEN "Product found:\n" RESET);
//                     printf(GREEN "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n" RESET, product.id, product.name, product.price, product.quantity);
//                 }
//                 else
//                 {
//                     printf(RED "Product with ID %d not found.\n" RESET, searchId);
//                 }
//             }
//             break;

//             case 5:
//                 SalesHistory();
//                 break;

//             case 6:
//                 displayProducts();
//                 break;

//             case 7:
//                 searchUserLoyaltyPoints();
//                 break;

//             case 8:
//                 char username[50];

//                 printf("Enter username: ");
//                 scanf("%s", username);

//                 viewSalesData(username);
//                 break;

//             case 9:
//                 printf(GREEN "Exiting the program.\n" RESET);
//                 exit(EXIT_SUCCESS);
//                 break;

//             default:
//                 printf(RED "Invalid choice! Please select a valid option.\n" RESET);
//             }

//             printf("\n");
//         } while (choice != 9);
//     }

//     else
//     {
//         do
//         {
//             printf("---------------------------------\n");
//             printf("User Menu\n");
//             printf("---------------------------------\n");
//             printf("1. Find Product By ID\n");
//             printf("2. Generate Bill\n");
//             printf("3. View My Transaction History\n");
//             printf("4. Show Loyalty Points\n");
//             printf("5. Display Products\n");
//             printf("6. Exit\n");
//             printf("Select an option (1-6): ");
//             scanf("%d", &choice);
//             printf("\n");

//             switch (choice)
//             {
//             case 1:
//                 struct Product product;
//                 int searchId;

//                 printf("Enter product ID to search: ");
//                 scanf("%d", &searchId);

//                 if (getProductById(searchId, &product))
//                 {
//                     printf(GREEN "Product found:\n" RESET);
//                     printf(GREEN "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n" RESET, product.id, product.name, product.price, product.quantity);
//                 }
//                 else
//                 {
//                     printf(RED "Product with ID %d not found.\n" RESET, searchId);
//                 }
//                 break;
//             case 2:
//                 generateBill(loggedInUser, &loyaltyPoints);
//                 break;
//             case 3:
//                 viewSalesData(loggedInUser);
//                 break;
//             case 4:
//                 showLoyaltyPoints(&loyaltyPoints);
//                 break;
//             case 5:
//                 displayProducts();
//                 break;
//             case 6:
//                 printf(GREEN "Exiting the program.\n" RESET);
//                 exit(EXIT_SUCCESS);
//                 break;
//             default:
//                 printf(RED "Invalid choice! Please select a valid option.\n" RESET);
//             }
//         } while (choice != 6);
//     }

//     return 0;
// }

int main()
{
    int choice;
    int loyaltyPoints = 0;
    char loggedInUser[50] = "";

    while (1)
    {
        printf("---------------------------------------------\n");
        printf("Welcome to the Product Management System\n");
        printf("---------------------------------------------\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Select an option (1-3): ");
        scanf("%d", &choice);
        printf("\n");

        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            if (loginUser(loggedInUser, &loyaltyPoints))
            {
                if (isAdmin(loggedInUser))
                {
                    do
                    {
                        printf("---------------------------------\n");
                        printf("Admin Panel\n");
                        printf("---------------------------------\n");
                        printf("1. Add Product\n");
                        printf("2. Update Product\n");
                        printf("3. Delete Product\n");
                        printf("4. Find Product By ID\n");
                        printf("5. View Sales Data\n");
                        printf("6. Display Products\n");
                        printf("7. Search Users for Loyalty Points\n");
                        printf("8. Show Transaction History for Users\n");
                        printf("9. Logout to Main Menu\n");
                        printf("Select an option (1-9): ");
                        scanf("%d", &choice);
                        printf("\n");

                        struct Product product;
                        switch (choice)
                        {
                        case 1:
                            printf("Enter Product ID: ");
                            scanf("%d", &product.id);
                            printf("Enter Product Name: ");
                            scanf("%s", product.name);
                            printf("Enter Product Price: ");
                            scanf("%f", &product.price);
                            printf("Enter Product Quantity: ");
                            scanf("%d", &product.quantity);
                            addProduct(product);
                            break;

                        case 2:
                        {
                            int searchId, updateChoice;
                            float newValue;
                            printf("Enter Product ID to update: ");
                            scanf("%d", &searchId);
                            printf("What do you want to update? (1 for Price, 2 for Quantity): ");
                            scanf("%d", &updateChoice);
                            printf("Enter new value: ");
                            scanf("%f", &newValue);
                            updateProductById(searchId, updateChoice, newValue);
                        }
                        break;

                        case 3:
                        {
                            int searchId;
                            printf("Enter Product ID to delete: ");
                            scanf("%d", &searchId);
                            deleteProductById(searchId);
                        }
                        break;

                        case 4:
                        {
                            struct Product product;
                            int searchId;

                            printf("Enter product ID to search: ");
                            scanf("%d", &searchId);

                            if (getProductById(searchId, &product))
                            {
                                printf(GREEN "Product found:\n" RESET);
                                printf(GREEN "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n" RESET, product.id, product.name, product.price, product.quantity);
                            }
                            else
                            {
                                printf(RED "Product with ID %d not found.\n" RESET, searchId);
                            }
                        }
                        break;

                        case 5:
                            SalesHistory();
                            break;

                        case 6:
                            displayProducts();
                            break;

                        case 7:
                            searchUserLoyaltyPoints();
                            break;

                        case 8:
                        {
                            char username[50];
                            printf("Enter username: ");
                            scanf("%s", username);
                            viewSalesData(username);
                        }
                        break;

                        case 9:
                            printf(GREEN "Returning to main menu.\n" RESET);
                            break;

                        default:
                            printf(RED "Invalid choice! Please select a valid option.\n" RESET);
                        }

                        printf("\n");
                    } while (choice != 9);
                }
                else
                {
                    do
                    {
                        printf("---------------------------------\n");
                        printf("User Menu\n");
                        printf("---------------------------------\n");
                        printf("1. Find Product By ID\n");
                        printf("2. Generate Bill\n");
                        printf("3. View My Transaction History\n");
                        printf("4. Show Loyalty Points\n");
                        printf("5. Display Products\n");
                        printf("6. Logout to Main Menu\n");
                        printf("Select an option (1-6): ");
                        scanf("%d", &choice);
                        printf("\n");

                        switch (choice)
                        {
                        case 1:
                        {
                            struct Product product;
                            int searchId;

                            printf("Enter product ID to search: ");
                            scanf("%d", &searchId);

                            if (getProductById(searchId, &product))
                            {
                                printf(GREEN "Product found:\n" RESET);
                                printf(GREEN "ID: %d\nName: %s\nPrice: %.2f\nQuantity: %d\n" RESET, product.id, product.name, product.price, product.quantity);
                            }
                            else
                            {
                                printf(RED "Product with ID %d not found.\n" RESET, searchId);
                            }
                        }
                        break;

                        case 2:
                            generateBill(loggedInUser, &loyaltyPoints);
                            break;

                        case 3:
                            viewSalesData(loggedInUser);
                            break;

                        case 4:
                            showLoyaltyPoints(&loyaltyPoints);
                            break;

                        case 5:
                            displayProducts();
                            break;

                        case 6:
                            printf(GREEN "Returning to main menu.\n" RESET);
                            break;

                        default:
                            printf(RED "Invalid choice! Please select a valid option.\n" RESET);
                        }

                        printf("\n");
                    } while (choice != 6);
                }
            }
        }
        else if (choice == 3)
        {
            printf(GREEN "Exiting the program. Goodbye!\n" RESET);
            break;
        }
        else
        {
            printf(RED "Invalid choice! Please select a valid option.\n" RESET);
        }
    }

    return 0;
}
