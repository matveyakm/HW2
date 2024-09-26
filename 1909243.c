#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#define ARRAY_LENTH(array) (sizeof(array) / sizeof((array)[0]))
#define RUNTIME(start, end) (((double)(end - start)) / CLOCKS_PER_SEC)

// Реализовать возведение в целую степень — в лоб (за линейное время) и за О(log n).

// Результат возведения в степень чисел, меньших или равных нулю, соответствует результату pow() из math.h

const double absInDouble(double a) {
    return a >= 0 ? a : -a;
}

const double classicPower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    bool exponentIsPositive = exponent > 0;
    exponent = exponentIsPositive ? exponent : -exponent;
    double convertedBase = (double)base;
    while (exponent > 1) {
        convertedBase *= base;
        exponent--;
    }
    return exponentIsPositive ? convertedBase : 1 / convertedBase; 
}

const long positiveExponentRecursivePower(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    if (exponent % 2 == 1) {
        return (long)base * positiveExponentRecursivePower(base, exponent - 1); 
    } else {
        long halfExponentResult = positiveExponentRecursivePower(base, exponent / 2);
        return halfExponentResult * halfExponentResult;
    }
}

const double recursivePower(int base, int exponent) // ~logN
{
    bool exponentIsPositive = exponent > 0;
    exponent = exponentIsPositive ? exponent : -exponent;
    double absResult = (double)positiveExponentRecursivePower(base, exponent);
    return exponentIsPositive ? absResult : 1 / absResult; 
}
const double average(double *array, int lenth) {
    double arraySum = 0;
    for (int i = 0; i < lenth; i++) {
        arraySum += array[i];
    }
    return arraySum / (double)lenth;
}

const bool compareWithLib(int base, int exponent, double resultNeedToCheck, int orderOfAccuracy) {
    double libAnswer = pow((double)base, (double)exponent);
    if (isinf(libAnswer) && isinf(resultNeedToCheck)) {
        return true;
    }
    return absInDouble(libAnswer - resultNeedToCheck) < pow(10, orderOfAccuracy);
}

const bool globalTest(int maxExponent, int orderOfAccuracy, bool needToDisplayFails) {
    int base[] = {-3, -2, -1, 0, 1, 2, 3, 4, 5};
    int basesLenth = ARRAY_LENTH(base);
    bool isTestSuccesful = true;
    int countOfSuccesfulTests = 0;
    int countOfTests = 0;
    double classicPowerRuntime = 0;
    double recursivePowerRuntime = 0;
    double libPowerRuntime = 0;
    for (int basePeaker = 0; basePeaker < basesLenth; basePeaker++) {
        for (int i = -3; i <= maxExponent; i++) {
            clock_t start, end;

            start = clock();
            double classicResult = classicPower(base[basePeaker], i);
            end = clock();
            classicPowerRuntime += RUNTIME(start, end);

            start = clock();
            double recursiveResult = recursivePower(base[basePeaker], i);
            end = clock();
            recursivePowerRuntime += RUNTIME(start, end);

            start = clock();
            double libResult = pow((double) base[basePeaker], (double)i);
            end = clock();
            libPowerRuntime += RUNTIME(start, end);

            bool isResultsCorrect = compareWithLib(base[basePeaker], i, classicResult, orderOfAccuracy) * compareWithLib(base[basePeaker], i, recursiveResult, orderOfAccuracy);
            if (!isResultsCorrect) {
                if (needToDisplayFails) {
                    printf("Failed. %d ** %d :: %f %f %f\n", base[basePeaker], i, classicResult, recursiveResult, libResult);
                }
                isTestSuccesful = false;
            } else {
                countOfSuccesfulTests++;
            }
            countOfTests++;
        }
    }
    double averageClassicPowerRuntime = (classicPowerRuntime / countOfTests) * 1000;
    double averageRecursivePowerRuntime = (recursivePowerRuntime / countOfTests) * 1000;
    double averageLibPowerRuntime = (libPowerRuntime / countOfTests) * 1000;
    printf(isTestSuccesful ? "Test succesful" : "Test failed");
    printf(" [%d/%d tests succesful]\n", countOfSuccesfulTests, countOfTests);
    printf("Average runtime of classic power: %.7fms\n", averageClassicPowerRuntime);
    printf("Average runtime of recursive power: %.7fms\n", averageRecursivePowerRuntime);
    printf("Average runtime of lib power: %.7fms\n", averageLibPowerRuntime);
    return isTestSuccesful;
}

int main()
{
    const int base = -2;
    const int exponent = -2;
    printf("%f %f\n", classicPower(base, exponent), recursivePower(base, exponent));

    globalTest(20, -8, true);

    return 0;
}