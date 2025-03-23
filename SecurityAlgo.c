#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Function to compute Greatest Common Divisor (GCD)
int gcd(int a, int b) 
{
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

//Function to compute modular exponentiation (base^exp % mod)
long long modExp(long long base, long long exp, long long mod) 
{
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) 
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

//Function to compute modular inverse (Extended Euclidean Algorithm)
long long modInverse(long long e, long long phi) 
{
    long long m0 = phi, t, q;
    long long x0 = 0, x1 = 1;
    while (e > 1) {
        q = e / phi;
        t = phi;
        phi = e % phi, e = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

//RSA Key Generation
void generateKeys(long long *n, long long *e, long long *d) 
{
    long long p = 61, q = 53;
    *n = p * q;
    long long phi = (p - 1) * (q - 1);
    *e = 17;
    while (gcd(*e, phi) != 1) {
        (*e)++;
    }
    *d = modInverse(*e, phi);
}

//Encrypting character using RSA
long long encryptChar(char ch, long long e, long long n) 
{
    return modExp((long long)ch, e, n);
}

//Decrypting character using RSA
char decryptChar(long long cipher, long long d, long long n) 
{
    return (char)modExp(cipher, d, n);
}

// Encrypting file
void encryptFile(const char *inputFile, const char *outputFile, long long e, long long n) 
{
    FILE *inFile = fopen(inputFile, "rb");
    FILE *outFile = fopen(outputFile, "w");
    if (!inFile || !outFile) 
    {
        printf("Error opening file!\n");
        return;
    }
    char ch;
    while (fscanf(inFile, "%c", &ch) != EOF)  //reading the file character by character
    {
        fprintf(outFile, "%lld ", encryptChar(ch, e, n));   //encrypting each character
    }
    fclose(inFile);
    fclose(outFile);    //closing the file
    printf("File encrypted successfully!\n");
}

//Decrypting file
void decryptFile(const char *inputFile, const char *outputFile, long long d, long long n) 
{
    FILE *inFile = fopen(inputFile, "r");
    FILE *outFile = fopen(outputFile, "wb");
    if (!inFile || !outFile)    //checks if file with encryption exists or not
    {
        printf("Encrypted File does not exist!\n");
        return;
    }
    long long encryptedChar;
    while (fscanf(inFile, "%lld", &encryptedChar) != EOF) 
    {
        fprintf(outFile, "%c", decryptChar(encryptedChar, d, n));  //decrypting each character
    }
    fclose(inFile);
    fclose(outFile);
    printf("File decrypted successfully!\n");
}
int main() 
{
    long long n, e, d;
    generateKeys(&n, &e, &d);
    
    int choice;
    char filename[256];
    printf("Enter filename: ");
    scanf("%255s", filename);
    getchar();
    FILE *file = fopen(filename, "r");
    if (!file)     //checking if input file exists
    {
        printf("File does not exist\n");
    }
    else
    {
        char setPassword[256]; 
        printf("Create password:");
        scanf("%255s", setPassword);
        while(1)
        {
            printf("Choose an option:\n");   //printing menu for user
            printf("1. Encrypt a file\n");
            printf("2. Decrypt a file\n");
            printf("3. Access protected file\n");
            printf("4. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);
            getchar();
            if (choice == 1) 
            {
                char password[256];
                printf("Enter password to access file: ");
                scanf("%255s", password);
                if (strcmp(password, setPassword) != 0) //checking if password matches
                {
                    printf("Incorrect password. Access denied.\n");
                    continue;
                }
                else        //if password matches, encryption is done
                    encryptFile(filename, "encrypted.txt", e, n);
            } 
            else if (choice == 2) 
            {
                decryptFile("encrypted.txt", "decrypted.txt", d, n);  //function call for decryption
            } 
            else if (choice == 3) 
            {
                char password[256];
                printf("Enter password: ");
                scanf("%255s", password);
                if (strcmp(password, setPassword) != 0) //checking if password matches for access
                {
                    printf("Incorrect password. Access denied.\n");
                    continue;
                }
                FILE *file = fopen(filename, "r");
                if (!file) 
                {
                    printf("Error: Could not open the file.\n");
                    return 1;
                }
                printf("\n--- File Content ---\n");
                char ch;
                while ((ch = fgetc(file)) != EOF) 
                {
                    putchar(ch);  //printing file content
                }
                printf("\n--- End of File ---\n");
                fclose(file);
            } 
            else if (choice == 4)
            {
                exit(0);
            }
            else
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
