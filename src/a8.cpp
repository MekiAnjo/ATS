// #include <AES.h>
// #include <Arduino.h>
// #include <CTR.h>
// #include <Crypto.h>
// #include <EAX.h>
// #include <GCM.h>
// #include <XTS.h>
// #include <string.h>

// struct TestVector {
//   const char* name;
//   byte key[32];
//   byte plaintext[16];
//   byte ciphertext[16];
// };

// // clang-format off
// static TestVector testVectorAES128 = {
//     .name        = "AES-128-ECB",
//     .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
//                     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
//     .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
//     .ciphertext  = {0x69, 0xC4, 0xE0, 0xD8, 0x6A, 0x7B, 0x04, 0x30,
//                     0xD8, 0xCD, 0xB7, 0x80, 0x70, 0xB4, 0xC5, 0x5A}
// };
// static TestVector testVectorAES192 = {
//     .name        = "AES-192-ECB",
//     .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
//                     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
//                     0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17},
//     .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
//     .ciphertext  = {0xDD, 0xA9, 0x7C, 0xA4, 0x86, 0x4C, 0xDF, 0xE0,
//                     0x6E, 0xAF, 0x70, 0xA0, 0xEC, 0x0D, 0x71, 0x91}
// };
// static TestVector testVectorAES256 = {
//     .name        = "AES-256-ECB",
//     .key         = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
//                     0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
//                     0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
//                     0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F},
//     .plaintext   = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//                     0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF},
//     .ciphertext  = {0x8E, 0xA2, 0xB7, 0xCA, 0x51, 0x67, 0x45, 0xBF,
//                     0xEA, 0xFC, 0x49, 0x90, 0x4B, 0x49, 0x60, 0x89}
// };
// // clang-format on

// AES128 aes128;
// CTR<AES128> aes128ctr;
// EAX<AES128> aes128eax;
// GCM<AES128> aes128gcm;
// XTS<AES128> aes128xts;

// AES192 aes192;
// CTR<AES192> aes192ctr;
// EAX<AES192> aes192eax;
// GCM<AES192> aes192gcm;
// XTS<AES192> aes192xts;

// AES256 aes256;
// CTR<AES256> aes256ctr;
// EAX<AES256> aes256eax;
// GCM<AES256> aes256gcm;
// XTS<AES256> aes256xts;

// byte buffer[16];

// unsigned long totalEncryptionTime128 = 0;
// unsigned long totalDecryptionTime128 = 0;
// unsigned long totalEncryptionTime192 = 0;
// unsigned long totalDecryptionTime192 = 0;
// unsigned long totalEncryptionTime256 = 0;
// unsigned long totalDecryptionTime256 = 0;

// const int numIterations = 100;

// void setRandomPlaintext(byte* plaintext) {
//   randomSeed(analogRead(0));
//   for (int i = 0; i < 16; i++) {
//     plaintext[i] = random(256);
//   }
// }

// void setRandomKey(byte* key, int keySize) {
//   randomSeed(analogRead(0));
//   for (int i = 0; i < keySize; i++) {
//     key[i] = random(256);
//   }
// }

// void runExperimentBlockCipher(BlockCipher* cipher, struct TestVector* test,
//                               unsigned long& totalEncryptionTime,
//                               unsigned long& totalDecryptionTime);

// void printResults(const char* algorithm, unsigned long totalEncryptionTime,
//                   unsigned long totalDecryptionTime);

// void runExperimentCTR(Cipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime);

// void runExperimentEAX(AuthenticatedCipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime);

// void runExperimentGCM(AuthenticatedCipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime);

// void runExperimentXTS(XTSCommon* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime);

// void setup() {
//   Serial.begin(9600);

//   Serial.print("Block Cipher Test\n");
//   // Run experiment for AES-128
//   runExperimentBlockCipher(&aes128, &testVectorAES128, totalEncryptionTime128,
//                            totalDecryptionTime128);

//   // Run experiment for AES-192
//   runExperimentBlockCipher(&aes192, &testVectorAES192, totalEncryptionTime192,
//                            totalDecryptionTime192);

//   // Run experiment for AES-256
//   runExperimentBlockCipher(&aes256, &testVectorAES256, totalEncryptionTime256,
//                            totalDecryptionTime256);

//   // Print results
//   printResults("AES-128", totalEncryptionTime128, totalDecryptionTime128);
//   printResults("AES-192", totalEncryptionTime192, totalDecryptionTime192);
//   printResults("AES-256", totalEncryptionTime256, totalDecryptionTime256);

//   Serial.print("CTR Cipher Mode Test\n");
//   // Run experiment for AES-128
//   runExperimentCTR(&aes128ctr, &testVectorAES128, totalEncryptionTime128,
//                    totalDecryptionTime128);

//   // Run experiment for AES-192
//   runExperimentCTR(&aes192ctr, &testVectorAES192, totalEncryptionTime192,
//                    totalDecryptionTime192);

//   // Run experiment for AES-256
//   runExperimentCTR(&aes256ctr, &testVectorAES256, totalEncryptionTime256,
//                    totalDecryptionTime256);

//   // Print results
//   printResults("AES-128-CTR", totalEncryptionTime128, totalDecryptionTime128);
//   printResults("AES-192-CTR", totalEncryptionTime192, totalDecryptionTime192);
//   printResults("AES-256-CTR", totalEncryptionTime256, totalDecryptionTime256);

//   Serial.print("EAX Cipher Mode Test\n");
//   // Run experiment for AES-128
//   runExperimentEAX(&aes128eax, &testVectorAES128, totalEncryptionTime128,
//                    totalDecryptionTime128);

//   // Run experiment for AES-192
//   runExperimentEAX(&aes192eax, &testVectorAES192, totalEncryptionTime192,
//                    totalDecryptionTime192);

//   // Run experiment for AES-256
//   runExperimentEAX(&aes256eax, &testVectorAES256, totalEncryptionTime256,
//                    totalDecryptionTime256);

//   // Print results
//   printResults("AES-128-EAX", totalEncryptionTime128, totalDecryptionTime128);
//   printResults("AES-192-EAX", totalEncryptionTime192, totalDecryptionTime192);
//   printResults("AES-256-EAX", totalEncryptionTime256, totalDecryptionTime256);

//   Serial.print("GCM Cipher Mode Test\n");
//   // Run experiment for AES-128
//   runExperimentGCM(&aes128gcm, &testVectorAES128, totalEncryptionTime128,
//                    totalDecryptionTime128);

//   // Run experiment for AES-192
//   runExperimentGCM(&aes192gcm, &testVectorAES192, totalEncryptionTime192,
//                    totalDecryptionTime192);

//   // Run experiment for AES-256
//   runExperimentGCM(&aes256gcm, &testVectorAES256, totalEncryptionTime256,
//                    totalDecryptionTime256);

//   // Print results
//   printResults("AES-128-GCM", totalEncryptionTime128, totalDecryptionTime128);
//   printResults("AES-192-GCM", totalEncryptionTime192, totalDecryptionTime192);
//   printResults("AES-256-GCM", totalEncryptionTime256, totalDecryptionTime256);

//   Serial.print("XTS Cipher Mode Test\n");
//   // Run experiment for AES-128
//   runExperimentXTS(&aes128xts, &testVectorAES128, totalEncryptionTime128,
//                    totalDecryptionTime128);

//   // Run experiment for AES-192
//   runExperimentXTS(&aes192xts, &testVectorAES192, totalEncryptionTime192,
//                    totalDecryptionTime192);

//   // Run experiment for AES-256
//   runExperimentXTS(&aes256xts, &testVectorAES256, totalEncryptionTime256,
//                    totalDecryptionTime256);

//   // Print results
//   printResults("AES-128-XTS", totalEncryptionTime128, totalDecryptionTime128);
//   printResults("AES-192-XTS", totalEncryptionTime192, totalDecryptionTime192);
//   printResults("AES-256-XTS", totalEncryptionTime256, totalDecryptionTime256);
// }

// void loop() {}

// void runExperimentBlockCipher(BlockCipher* cipher, struct TestVector* test,
//                               unsigned long& totalEncryptionTime,
//                               unsigned long& totalDecryptionTime) {
//   crypto_feed_watchdog();

//   Serial.print(test->name);
//   Serial.print(" Set Key ... \n");
//   for (int i = 0; i < numIterations; i++) {
//     cipher->setKey(test->key, cipher->keySize());
//   }

//   Serial.print(test->name);
//   Serial.print(" Encrypt ... \n");
//   unsigned long startEncryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->encryptBlock(buffer, buffer);
//   }
//   unsigned long endEncryptionTime = micros();
//   totalEncryptionTime += endEncryptionTime - startEncryptionTime;

//   Serial.print(test->name);
//   Serial.print(" Decrypt ... \n");
//   unsigned long startDecryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->decryptBlock(buffer, buffer);
//   }
//   unsigned long endDecryptionTime = micros();
//   totalDecryptionTime += endDecryptionTime - startDecryptionTime;
//   Serial.println("");
// }

// void runExperimentCTR(Cipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime) {
//   crypto_feed_watchdog();

//   Serial.print(test->name);
//   Serial.print(" Set Key ... \n");
//   for (int i = 0; i < numIterations; i++) {
//     cipher->setKey(test->key, cipher->keySize());
//   }

//   Serial.print(test->name);
//   Serial.print(" Encrypt ... \n");
//   unsigned long startEncryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->encrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endEncryptionTime = micros();
//   totalEncryptionTime += endEncryptionTime - startEncryptionTime;

//   Serial.print(test->name);
//   Serial.print(" Decrypt ... \n");
//   unsigned long startDecryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->decrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endDecryptionTime = micros();
//   totalDecryptionTime += endDecryptionTime - startDecryptionTime;
//   Serial.println("");
// }

// void runExperimentEAX(AuthenticatedCipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime) {
//   crypto_feed_watchdog();

//   Serial.print(test->name);
//   Serial.print(" Set Key ... \n");
//   for (int i = 0; i < numIterations; i++) {
//     cipher->setKey(test->key, cipher->keySize());
//   }

//   Serial.print(test->name);
//   Serial.print(" Encrypt ... \n");
//   unsigned long startEncryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->encrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endEncryptionTime = micros();
//   totalEncryptionTime += endEncryptionTime - startEncryptionTime;

//   Serial.print(test->name);
//   Serial.print(" Decrypt ... \n");
//   unsigned long startDecryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->decrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endDecryptionTime = micros();
//   totalDecryptionTime += endDecryptionTime - startDecryptionTime;
//   Serial.println("");
// }

// void runExperimentGCM(AuthenticatedCipher* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime) {
//   crypto_feed_watchdog();

//   Serial.print(test->name);
//   Serial.print(" Set Key ... \n");
//   for (int i = 0; i < numIterations; i++) {
//     cipher->setKey(test->key, cipher->keySize());
//   }

//   Serial.print(test->name);
//   Serial.print(" Encrypt ... \n");
//   unsigned long startEncryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->encrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endEncryptionTime = micros();
//   totalEncryptionTime += endEncryptionTime - startEncryptionTime;

//   Serial.print(test->name);
//   Serial.print(" Decrypt ... \n");
//   unsigned long startDecryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->decrypt(buffer, buffer, sizeof(test->plaintext));
//   }
//   unsigned long endDecryptionTime = micros();
//   totalDecryptionTime += endDecryptionTime - startDecryptionTime;
//   Serial.println("");
// }

// void runExperimentXTS(XTSCommon* cipher, struct TestVector* test,
//                       unsigned long& totalEncryptionTime,
//                       unsigned long& totalDecryptionTime) {
//   crypto_feed_watchdog();

//   Serial.print(test->name);
//   Serial.print(" Set Key ... \n");
//   for (int i = 0; i < numIterations; i++) {
//     cipher->setKey(test->key, cipher->keySize() * 2);
//   }

//   Serial.print(test->name);
//   Serial.print(" Encrypt ... \n");
//   unsigned long startEncryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->encryptSector(buffer, buffer);
//   }
//   unsigned long endEncryptionTime = micros();
//   totalEncryptionTime += endEncryptionTime - startEncryptionTime;

//   Serial.print(test->name);
//   Serial.print(" Decrypt ... \n");
//   unsigned long startDecryptionTime = micros();
//   for (int i = 0; i < numIterations; i++) {
//     cipher->decryptSector(buffer, buffer);
//   }
//   unsigned long endDecryptionTime = micros();
//   totalDecryptionTime += endDecryptionTime - startDecryptionTime;
//   Serial.println("");
// }

// void printResults(const char* algorithm, unsigned long totalEncryptionTime,
//                   unsigned long totalDecryptionTime) {
//   Serial.print("Algorithm: ");
//   Serial.println(algorithm);
//   Serial.print("Average Encryption Time: ");
//   Serial.print(totalEncryptionTime / numIterations);
//   Serial.println(" microseconds");
//   Serial.print("Average Decryption Time: ");
//   Serial.print(totalDecryptionTime / numIterations);
//   Serial.println(" microseconds");
//   Serial.println();
// }
