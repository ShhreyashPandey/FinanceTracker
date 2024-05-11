#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CATEGORY_FOOD_DRINKS "food and drinks"
#define CATEGORY_SHOPPING "shopping"
#define CATEGORY_TRANSPORTATION "transportation"
#define CATEGORY_COMMUNICATION_PC "communication,PC"
#define CATEGORY_HEALTHCARE "healthcare"

// Structure to represent a financial transaction
typedef struct Transaction {
    int transcID;
    char description[50];
    double amount;
    char category[20];
    time_t timestamp;
    struct Transaction* left;
    struct Transaction* right;
} Transaction;

// Function to create a new transaction
Transaction* createTransaction(int transcID, const char* description, double amount, const char* category) {
    Transaction* newTransaction = (Transaction*)malloc(sizeof(Transaction));
    newTransaction->transcID = transcID;
    snprintf(newTransaction->description, sizeof(newTransaction->description), "%s", description);
    newTransaction->amount = amount;
    snprintf(newTransaction->category, sizeof(newTransaction->category), "%s", category);
    newTransaction->timestamp = time(NULL);
    newTransaction->left = NULL;
    newTransaction->right = NULL;
    return newTransaction;
}

int isTranscIDUnique(Transaction* root, int transcID) {
    if (root == NULL) {
        return 1; // Unique
    }

    if (transcID == root->transcID) {
        return 0; // Not unique
    } else if (transcID < root->transcID) {
        return isTranscIDUnique(root->left, transcID);
    } else {
        return isTranscIDUnique(root->right, transcID);
    }
}

// Function to insert a transaction into the BST based on transcID
Transaction* insertTransaction(Transaction* root, int transcID, const char* description, double amount, const char* category) {
    if (root == NULL) {
        return createTransaction(transcID, description, amount, category);
    }

    if (transcID == root->transcID) {
        printf("Error: Transaction with the same transcID already exists.\n");
        return root;
    } else if (transcID < root->transcID) {
        root->left = insertTransaction(root->left, transcID, description, amount, category);
    } else {
        root->right = insertTransaction(root->right, transcID, description, amount, category);
    }

    return root;
}

// Function to calculate the balance
double calculateBalance(Transaction* root) {
    if (root == NULL) {
        return 0;
    }

    double leftBalance = calculateBalance(root->left);
    double rightBalance = calculateBalance(root->right);

    return leftBalance + rightBalance + root->amount;
}

// Function to generate a financial report for a given time range
void generateFinancialReport(Transaction* root, time_t start, time_t end) {
    if (root == NULL) {
        return;
    }

    generateFinancialReport(root->left, start, end);

    if (root->timestamp >= start && root->timestamp <= end) {
        printf("Description: %s\n", root->description);
        printf("Amount: Rs.%.2lf\n", root->amount);
        printf("Category: %s\n", root->category);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&root->timestamp));
        printf("Date: %s\n", timestamp);
        printf("\n");
    }

    generateFinancialReport(root->right, start, end);
}

// Function to search for a category in the tree
int searchCategory(Transaction* root, const char* category) {
    if (root == NULL) {
        return 0; // Category not found
    }

    int leftResult = searchCategory(root->left, category);
    if (leftResult) {
        return 1; // Category found in the left subtree
    }

    int rightResult = searchCategory(root->right, category);
    if (rightResult) {
        return 1; // Category found in the right subtree
    }

    return (strcmp(root->category, category) == 0);
}

// Function to display categorized transactions for a given time range
int displayCategorizedTransactions(Transaction* root, const char* category, time_t start, time_t end) {
    if (root == NULL) {
        return 0;
    }

    int found = 0; // Initialize a flag

    if (displayCategorizedTransactions(root->left, category, start, end)) {
        found = 1; // Category found in the left subtree
    }

    if (strcmp(root->category, category) == 0 && root->timestamp >= start && root->timestamp <= end) {
        found = 1; // Set the flag if the category is found
        printf("Description: %s\n", root->description);
        printf("Amount: Rs.%.2lf\n", root->amount);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&root->timestamp));
        printf("Date: %s\n", timestamp);
        printf("\n");
    }

    if (displayCategorizedTransactions(root->right, category, start, end)) {
        found = 1; // Category found in the right subtree
    }

    return found;
}


// Function to perform an inorder traversal and display transactions
void displayInorder(Transaction* root) {
    if (root == NULL) {
        return;
    }

    displayInorder(root->left);

    printf("%.2lf -> ", root->amount);

    displayInorder(root->right);
}

// Function to perform a preorder traversal and display transactions
void displayPreorder(Transaction* root) {
    if (root == NULL) {
        return;
    }

    printf("%.2lf -> ", root->amount);
    
    displayPreorder(root->left);
    displayPreorder(root->right);
}

// Function to perform a postorder traversal and display transactions
void displayPostorder(Transaction* root) {
    if (root == NULL) {
        return;
    }

    displayPostorder(root->left);
    displayPostorder(root->right);

    printf("%.2lf -> ", root->amount);
}

// Function to free the memory occupied by the transaction tree
void freeTransactionTree(Transaction* root) {
    if (root == NULL) {
        return;
    }

    freeTransactionTree(root->left);
    freeTransactionTree(root->right);
    free(root);
}

// Helper function to find the node with the smallest transcID in a subtree
Transaction* findMin(Transaction* node) {
    Transaction* current = node;
    while (current && current->left != NULL){
                current = current->left;
    return current;}
}

// Function to delete a transaction based on transcID
Transaction* deleteTransaction(Transaction* root, int transcID) {
    if (root == NULL) {
        printf("Transaction with the specified transcID does not exist.\n");
        return root;
    }

    if (transcID < root->transcID) {
        root->left = deleteTransaction(root->left, transcID);
    } else if (transcID > root->transcID) {
        root->right = deleteTransaction(root->right, transcID);
    } else {
        // Node with the specified transcID found, delete it
        if (root->left == NULL) {
            Transaction* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Transaction* temp = root->left;
            free(root);
            return temp;
        }

        // Node with two children, get the in-order successor (smallest in the right subtree)
        Transaction* temp = findMin(root->right);

        // Copy the in-order successor's data to this node
        root->transcID = temp->transcID;
        snprintf(root->description, sizeof(root->description), "%s", temp->description);
        root->amount = temp->amount;
        snprintf(root->category, sizeof(root->category), "%s", temp->category);

        // Delete the in-order successor
        root->right = deleteTransaction(root->right, temp->transcID);
    }

    return root;
}

int main() {
    Transaction* transactionTree = NULL;

    int choice;
    do {
        printf("\n\nFinance Tracker Menu:\n");
        printf("1. Add Transaction\n");
        printf("2. Display Total Transactions\n");
        printf("3. Generate Financial Report for last 1 month\n");
        printf("4. Display Categorized Transactions for last 1 month\n");
        printf("5. Display Inorder Traversal\n");
        printf("6. Display Preorder Traversal\n");
        printf("7. Display Postorder Traversal\n");
        printf("8. Delete Transaction by transcID\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int transcID;
                char description[50];
                double amount;
                char category[20];
                
                printf("Enter transcID: ");
                scanf("%d", &transcID);
                
                if (isTranscIDUnique(transactionTree, transcID)) {
                    printf("Enter description: ");
                    scanf(" %[^\n]", description);
                    printf("Enter amount: ");
                    scanf("%lf", &amount);
                    
                    int categoryChoice;
                    printf("Enter category (choose from the following):\n");
                    printf("1. %s\n", CATEGORY_FOOD_DRINKS);
                    printf("2. %s\n", CATEGORY_SHOPPING);
                    printf("3. %s\n", CATEGORY_TRANSPORTATION);
                    printf("4. %s\n", CATEGORY_COMMUNICATION_PC);
                    printf("5. %s\n", CATEGORY_HEALTHCARE);
                    
                    do {
                        printf("Enter the number corresponding to your category choice: ");
                        scanf("%d", &categoryChoice);
                    
                        switch (categoryChoice) {
                            case 1:
                                snprintf(category, sizeof(category), "%s", CATEGORY_FOOD_DRINKS);
                                break;
                            case 2:
                                snprintf(category, sizeof(category), "%s", CATEGORY_SHOPPING);
                                break;
                            case 3:
                                snprintf(category, sizeof(category), "%s", CATEGORY_TRANSPORTATION);
                                break;
                            case 4:
                                snprintf(category, sizeof(category), "%s", CATEGORY_COMMUNICATION_PC);
                                break;
                            case 5:
                                snprintf(category, sizeof(category), "%s", CATEGORY_HEALTHCARE);
                                break;
                            default:
                                printf("Invalid category choice. Please try again.\n");
                        }
                    } while (categoryChoice < 1 || categoryChoice > 5);
                    
                    transactionTree = insertTransaction(transactionTree, transcID, description, amount, category);
                } else {
                    printf("Transaction with the same transcID already exists. Please enter a unique transcID.\n");
                }
                break;
            }
            case 2:
                // Calculate and display the balance (total)
                double totalBalance = calculateBalance(transactionTree);
                printf("Total Transaction: Rs.%.2lf\n", totalBalance);
                break;
            case 3: {
                time_t currentTime = time(NULL);
                time_t oneMonthAgo = currentTime - 30 * 24 * 60 * 60;  // 30 days in seconds
                generateFinancialReport(transactionTree, oneMonthAgo, currentTime);
                break;
            }
            case 4: {
                char category[20];
                printf("Enter category: ");
                scanf(" %[^\n]", category);

                time_t currentTime = time(NULL);
                time_t oneMonthAgo = currentTime - 30 * 24 * 60 * 60;  // 30 days in seconds
                int found = displayCategorizedTransactions(transactionTree, category, oneMonthAgo, currentTime);
                if (!found) {
                    printf("Category doesn't exist!!\n");
                }
                break;
            }
            case 5: {
                // Display transactions in inorder
                displayInorder(transactionTree);
                break;
            }
            case 6: {
                // Display transactions in preorder
                displayPreorder(transactionTree);
                break;
            }
            case 7: {
                // Display transactions in postorder
                displayPostorder(transactionTree);
                break;
            }
            case 8: {
                int transcIDToDelete;
                printf("Enter transcID to delete: ");
                scanf("%d", &transcIDToDelete);
                transactionTree = deleteTransaction(transactionTree, transcIDToDelete);
                printf("Deletion Done");
                break;
            }
            case 9:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    // Free memory
    freeTransactionTree(transactionTree);

    return 0;
}

    
       

