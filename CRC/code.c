#include <stdio.h>
#include <string.h>

void xor_divide(const char *dividend, const char *divisor, char *remainder) {
    int n = strlen(divisor);
    int len = strlen(dividend);

    char temp[256];
    strcpy(temp, dividend);          // work copy

    for (int i = 0; i <= len - n; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < n; j++) {
                temp[i + j] = (temp[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }
    // last n-1 bits are remainder
    strncpy(remainder, temp + len - (n - 1), n - 1);
    remainder[n - 1] = '\0';
}

void compute_crc(const char *data, const char *generator,
                 char *crc_out, char *tx_out) {
    int k = strlen(generator);
    char augmented[256];

    // data + (k-1) zeros
    strcpy(augmented, data);
    for (int i = 0; i < k - 1; i++)
        augmented[strlen(data) + i] = '0';
    augmented[strlen(data) + k - 1] = '\0';

    xor_divide(augmented, generator, crc_out);

    // transmitted = data + crc
    strcpy(tx_out, data);
    strcat(tx_out, crc_out);
}

int verify_crc(const char *received, const char *generator) {
    char rem[256];
    xor_divide(received, generator, rem);

    for (int i = 0; rem[i] != '\0'; i++)
        if (rem[i] != '0')
            return 0;       // error detected
    return 1;               // no error
}

int main() {
    char data[256], gen[256];

    printf("Enter data bits     : ");
    scanf("%s", data);
    printf("Enter generator bits: ");
    scanf("%s", gen);

    char crc[256], transmitted[256];
    compute_crc(data, gen, crc, transmitted);

    printf("CRC       : %s\n", crc);
    printf("Transmitted frame: %s\n", transmitted);

    // verify (correct)
    printf("Verify transmitted (no error): %s\n",
           verify_crc(transmitted, gen) ? "OK" : "ERROR");

    // example: flip one bit to test detection
    char tampered[256];
    strcpy(tampered, transmitted);
    tampered[1] = (tampered[1] == '0') ? '1' : '0';   // flip bit 1
    printf("Tampered frame: %s\n", tampered);
    printf("Verify tampered: %s\n",
           verify_crc(tampered, gen) ? "OK" : "ERROR");

    return 0;
}
