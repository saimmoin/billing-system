#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

float applyDiscount(float totalPrice)
{
    float discountPercentage;
    printf("Enter discount percentage (e.g., 10 for 10%%, or 0 if no discount): ");
    scanf("%f", &discountPercentage);

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

void generateBill()
{
    int productId, quantity;
    float totalPrice = 0;
    float originalTotalPrice = 0;
    char choice;

    FILE *salesFile;

    salesFile = fopen("sales.txt", "a");
    if (salesFile == NULL)
    {
        printf(RED "Error opening sales file!\n" RESET);
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

    float discountedTotalPrice = applyDiscount(totalPrice);

    if (discountedTotalPrice < originalTotalPrice)
    {
        float discountAmount = originalTotalPrice - discountedTotalPrice;
        float discountPercentage = (discountAmount / originalTotalPrice) * 100;
        fprintf(salesFile, "Discount: %.2f%% (%.2f)\n", discountPercentage, discountAmount);
    }
    else
    {
        fprintf(salesFile, "Discount:  %.2f%% (%.2f)\n");
    }

    fprintf(salesFile, "Final Total Price: %.2f\n\n", discountedTotalPrice);

    printf(GREEN "Final Total Price (after discount): %.2f\n\n" RESET, discountedTotalPrice);

    fclose(salesFile);
}

void viewSalesData()
{
    FILE *file = fopen("sales.txt", "r");
    if (file == NULL)
    {
        printf(RED "Error opening sales.txt file or no sales data available!\n" RESET);
        return;
    }

    char ch;
    printf(GREEN "Past Sales Data:\n\n" RESET);

    while ((ch = fgetc(file)) != EOF)
    {
        putchar(ch);
    }

    fclose(file);
}

int main()
{
    int choice;
    struct Product product;

    do
    {
        printf("Product Management System\n");
        printf("1. Add Product\n");
        printf("2. Update Product\n");
        printf("3. Delete Product\n");
        printf("4. Find Product By ID\n");
        printf("5. Generate Bill\n");
        printf("6. View Sales Data\n");
        printf("7. Exit\n");
        printf("Select an option (1-7): ");
        scanf("%d", &choice);
        printf("\n");

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
            generateBill();
            break;

        case 6:
            viewSalesData();
            break;

        case 7:
            printf(GREEN "Exiting the program.\n" RESET);
            exit(EXIT_SUCCESS);
            break;

        default:
            printf(RED "Invalid choice! Please select a valid option.\n" RESET);
        }

        printf("\n");
    } while (choice != 7);

    return 0;
}