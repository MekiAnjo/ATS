#include <Arduino.h>
#include <Crypto.h>
#include <SHA256.h>
#include <SHA3.h>
#include <SHA512.h>

struct TestVector {
  const char* name;
  size_t plaintextSize;
  int* plaintext;
};

size_t testSize = 1024;
const size_t numIterations = 100;

static TestVector testVectorSHA256 = {.name = "SHA-256",
                                      .plaintextSize = testSize,
                                      .plaintext = new int[testSize]};

static TestVector testVectorSHA512 = {.name = "SHA-512",
                                      .plaintextSize = testSize,
                                      .plaintext = new int[testSize]};

static TestVector testVectorSHA3_256 = {.name = "SHA3-256",
                                        .plaintextSize = testSize,
                                        .plaintext = new int[testSize]};

static TestVector testVectorSHA3_512 = {.name = "SHA3-512",
                                        .plaintextSize = testSize,
                                        .plaintext = new int[testSize]};

unsigned long sha256Times[numIterations];
unsigned long sha512Times[numIterations];
unsigned long sha3_256Times[numIterations];
unsigned long sha3_512Times[numIterations];

void generateRandomPlaintext(int* plaintext, size_t size) {
  for (size_t i = 0; i < size; i++) {
    plaintext[i] = random(0, 256);
  }
}

void printAsCSV(const char* name, unsigned long* times) {
  Serial.print(name);
  for (size_t i = 0; i < numIterations; i++) {
    Serial.print(", ");
    Serial.print(times[i]);
  }
  Serial.println();
}

void runExperiment(Hash& hash, TestVector& testVector, unsigned long* times,
                   size_t iteration) {
  generateRandomPlaintext(testVector.plaintext, testVector.plaintextSize);

  // Zeitmessung starten
  unsigned long startTime = millis();

  // Berechne den Hash
  hash.update((uint8_t*)testVector.plaintext,
              testVector.plaintextSize * sizeof(int));
  uint8_t hashValue[32];
  hash.finalize(hashValue, sizeof(hash));

  // Zeitmessung beenden
  unsigned long endTime = millis();

  // Speichere die Zeit in das Array
  times[iteration] = endTime - startTime;
}

void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Algorithmus, Zeit (ms)");

  crypto_feed_watchdog();

  for (size_t iteration = 0; iteration < numIterations; iteration++) {
    // SHA-256
    SHA256 sha256;
    runExperiment(sha256, testVectorSHA256, sha256Times, iteration);

    // SHA-512
    SHA512 sha512;
    runExperiment(sha512, testVectorSHA512, sha512Times, iteration);

    // SHA3-256
    SHA3_256 sha3_256;
    runExperiment(sha3_256, testVectorSHA3_256, sha3_256Times, iteration);

    // SHA3-512
    SHA3_512 sha3_512;
    runExperiment(sha3_512, testVectorSHA3_512, sha3_512Times, iteration);

    // Warte 100ms um zu verhindern, dass der Watchdog-Timeout ausgelöst wird
    delay(100);
  }

  // Ausgabe der CSV-Zeiten nach allen Iterationen
  printAsCSV(testVectorSHA256.name, sha256Times);
  printAsCSV(testVectorSHA512.name, sha512Times);
  printAsCSV(testVectorSHA3_256.name, sha3_256Times);
  printAsCSV(testVectorSHA3_512.name, sha3_512Times);
}

void loop() {}
