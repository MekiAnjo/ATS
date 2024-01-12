#include <AES.h>
#include <Arduino.h>
#include <Crypto.h>
#include <string.h>

struct TestVector {
  const char* name;
  byte key[32];
  byte plaintext[16];
  byte ciphertext[16];
};

// clang-format off
static TestVector testVectorAES128 = {
    .name        = "AES-128-ECB",
    .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    .ciphertext  = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
                    0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A}
};
static TestVector testVectorAES192 = {
    .name        = "AES-192-ECB",
    .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17},
    .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    .ciphertext  = {0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF, 0xE0,
                    0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91}
};
static TestVector testVectorAES256 = {
    .name        = "AES-256-ECB",
    .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                    0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F},
    .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
    .ciphertext  = {0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF,
                    0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89}
};
// clang-format on

AES128 aes128;
AES192 aes192;
AES256 aes256;

byte buffer[16];

unsigned long totalEncryptDecryptTime128 = 0;
unsigned long totalEncryptDecryptTime192 = 0;
unsigned long totalEncryptDecryptTime256 = 0;

unsigned long individualEncryptDecryptTime128[100];
unsigned long individualEncryptDecryptTime192[100];
unsigned long individualEncryptDecryptTime256[100];

const int numIterations = 100;

void printAsCSV(const char* algorithm, unsigned long* individualTimes) {
  Serial.print(algorithm);
  Serial.print(",");
  for (int i = 0; i < numIterations; i++) {
    Serial.print(individualTimes[i]);
    Serial.print(",");
  }
  Serial.println();
}

void setRandomPlaintext(byte* plaintext) {
  randomSeed(analogRead(0));
  for (int i = 0; i < 16; i++) {
    plaintext[i] = random(256);
  }
}

void setRandomKey(byte* key, int keySize) {
  randomSeed(analogRead(0));
  for (int i = 0; i < keySize; i++) {
    key[i] = random(256);
  }
}

void runExperiment(BlockCipher* cipher, struct TestVector* test,
                   unsigned long& totalEncryptDecryptTime,
                   unsigned long* individualEncryptDecryptTime) {
  crypto_feed_watchdog();

  for (int i = 0; i < numIterations; i++) {
    unsigned long start = micros();

    setRandomPlaintext(test->plaintext);
    setRandomKey(test->key, cipher->keySize());
    cipher->setKey(test->key, cipher->keySize());

    cipher->encryptBlock(buffer, buffer);

    cipher->decryptBlock(buffer, buffer);

    unsigned long time = micros() - start;
    individualEncryptDecryptTime[i] = time;
    totalEncryptDecryptTime += time;
  }
}

void printResults(const char* algorithm,
                  unsigned long totalEncryptDecryptTime) {
  Serial.print("Algorithm: ");
  Serial.println(algorithm);
  Serial.print("Average EncryptDecrypt Time: ");
  Serial.print(totalEncryptDecryptTime / numIterations);
  Serial.println(" microseconds");
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.println();

  runExperiment(&aes128, &testVectorAES128, totalEncryptDecryptTime128,
                individualEncryptDecryptTime128);

  // Run experiment for AES-192
  runExperiment(&aes192, &testVectorAES192, totalEncryptDecryptTime192,
                individualEncryptDecryptTime192);

  // Run experiment for AES-256
  runExperiment(&aes256, &testVectorAES256, totalEncryptDecryptTime256,
                individualEncryptDecryptTime256);

  // Print results
  printResults("AES-128", totalEncryptDecryptTime128);
  printResults("AES-192", totalEncryptDecryptTime192);
  printResults("AES-256", totalEncryptDecryptTime256);

  // Print results as CSV
  printAsCSV("AES-128", individualEncryptDecryptTime128);
  printAsCSV("AES-192", individualEncryptDecryptTime192);
  printAsCSV("AES-256", individualEncryptDecryptTime256);
}

void loop() {}
