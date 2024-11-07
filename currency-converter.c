#include <stdio.h>
int main()
{
    int choice;
    float amount;
    printf("Currency Converter\n");
    printf("1. Dollar to PKR\n");
    printf("2. PKR to Dollar\n");
    printf("Select an option (1/2): ");
    scanf("%d", &choice);
    printf("Enter amount: ");
    scanf("%f", &amount);

    if (choice == 1)
    {
        printf("%.2f USD is equal to %.2f PKR\n", amount, amount * 270);
    }
    else if (choice == 2)
    {
        printf("%.2f PKR is equal to %.2f USD\n", amount, amount / 270);
    }
    else
    {
        printf("Invalid choice!\n");
    }
    return 0;
}
