#include <stdio.h>
#include <string.h>

void xorEncrypt(char *message, char *key) {
    int keyLen = strlen(key);
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, const char *message, const char *key) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    xorEncrypt((char *)message, (char *)key);
    fwrite(message, sizeof(char), strlen(message), file);

    fclose(file);
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
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
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    scanf(" %c", &choice);

    if (choice == 'e') {
        char message[1024];
        char filename[256];
        char key[256];

        printf("Enter the message to encrypt: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        printf("Enter the filename to save encrypted data: ");
        scanf("%s", filename);

        printf("Enter the encryption key: ");
        scanf("%s", key);

        encryptFile(filename, message, key);
    } else if (choice == 'd') {
        char filename[256];
        char key[256];

        printf("Enter the filename to read encrypted data: ");
        scanf("%s", filename);

        printf("Enter the decryption key: ");
        scanf("%s", key);

        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}