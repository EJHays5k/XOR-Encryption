#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void xorEncrypt(char* message, char* key) {
    int keyLen = strlen(key);
    if (keyLen == 0) return;
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char* filename, const char* message, const char* key) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    xorEncrypt((char*)message, (char*)key);
    fwrite(message, sizeof(char), strlen(message), file);

    fclose(file);
}

void decryptFile(const char* filename, const char* key) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: could not open \"%s\".\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    if (fileSize <= 0) {
        printf("Error: file is empty or unreadable.\n");
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(buffer, sizeof(char), fileSize, file);
    buffer[fileSize] = '\0';

    xorEncrypt(buffer, key);

    for (int i = 0; buffer[i] != '\0'; i++) {
        putchar(buffer[i]);
    }

    free(buffer);
    fclose(file);
}

int main() {
    char choice;
    char line[64];
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    fflush(stdout);
    if (fgets(line, sizeof(line), stdin) == NULL) {
        printf("Error reading input.\n");
        return 0;
    }
    choice = line[0]; // Get the first character of the input line

    if (choice == 'e') {
        char message[1024];
        char filename[256];
        char key[256];

        do {
            printf("Enter the message to encrypt: ");
            fflush(stdout);
            if (fgets(message, sizeof(message), stdin) == NULL) return 1;
            message[strcspn(message, "\r\n")] = '\0';
            if (message[0] == '\0') printf("Message cannot be empty.\n");
        } while (message[0] == '\0');

        printf("Enter the filename to save encrypted data: ");
        fflush(stdout);
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';

        do {
            printf("Enter the encryption key: ");   // "decryption" in the 'd' branch
            fflush(stdout);
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\r\n")] = '\0';
            if (key[0] == '\0') printf("Key cannot be empty.\n");
        } while (key[0] == '\0');

        encryptFile(filename, message, key);
    }
    else if (choice == 'd') {
        char filename[256];
        char key[256];

        printf("Enter the filename to read encrypted data: ");
        fflush(stdout);
        fgets(filename, sizeof(filename), stdin);
        filename[strcspn(filename, "\n")] = '\0';

        do {
            printf("Enter the encryption key: ");   // "decryption" in the 'd' branch
            fflush(stdout);
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\r\n")] = '\0';
            if (key[0] == '\0') printf("Key cannot be empty.\n");
        } while (key[0] == '\0');

        decryptFile(filename, key);
    }
    else {
        printf("Invalid choice.\n");
    }

    return 0;
}