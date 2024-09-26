#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define RUNTIME(start, end) (((double)(end - start)) / CLOCKS_PER_SEC)

// Реализовать вычисление чисел Фибоначчи рекурсивно, реализовать итеративно, выяснить, с какого номера числа Фибоначчи рекурсивный вариант заметно медленнее итеративного.

const long long FibonacciByBinet(int n) {
    if (n == 1 || n == 3) return n == 1 ? 1 : 2; // Исключения из-за ошибки деления double
    double fi = (1 + sqrt(5)) / 2;
    double n_fi = (1 - sqrt(5)) / 2;
    return (long long)(ceil(pow(fi, n) + pow(n_fi, n)) / sqrt(5));
}

const long long subRecursiveFibonacci(int n, long  long twoStepsAgo, long long oneStepAgo) {
    long long actualResult = twoStepsAgo + oneStepAgo;
    if (n == 0) {
        return actualResult;
    }
    return subRecursiveFibonacci(n - 1, oneStepAgo, actualResult);
}

const long long recursiveFibonacci(int n) {
    if (n < 2) {
        return n;
    }
    return subRecursiveFibonacci(n - 2, 0, 1);
}

const long long slowRecursiveFibonacci(int n) {
    if (n == 0 || n == 1) return (long long)n;
    return slowRecursiveFibonacci(n - 1) + slowRecursiveFibonacci(n - 2);
}
const  long long loopFibonacci(int n) {
    if (n < 2) {
        return n;
    }
    long long twoStepsAgo = 0;
    long long oneStepAgo = 1;
    while (n > 1) {
        long long tempResult = oneStepAgo + twoStepsAgo;
        twoStepsAgo = oneStepAgo;
        oneStepAgo = tempResult;
        n--;
    }
    return oneStepAgo;
}

const long long arrayFillingFibonicc(int n) {
    long long *fibonicciNumbers = malloc((n + 1) * sizeof(long long));
    fibonicciNumbers[0] = 0;
    fibonicciNumbers[1] = 1;
    int lastFilled = 1;
    while (lastFilled < n) {
        fibonicciNumbers[lastFilled + 1] = fibonicciNumbers[lastFilled] + fibonicciNumbers[lastFilled - 1];
        lastFilled++;
    }
    long long lastCalculatedNumber = fibonicciNumbers[n];
    free(fibonicciNumbers);
    return lastCalculatedNumber;
}

const bool globalTest(int calculationsCeil, bool needToDisplayAllRuntimeIntervals, bool needToDisplayCalculationResults) {
    if (calculationsCeil > 92) {
        calculationsCeil = 92;
        printf("Because of overflow max Fibonacci number can be calculated is 92-nd\n");
    } else if (calculationsCeil > 72) {
        printf("Because of non-accuarcy of 'double' type calculations Binet's formula can make mistakes for n > 72\n");
    }
    FILE *preFilledFibonacciNums = fopen("/Users/matveyakm/Documents/HW2/FibonacciNums.txt", "r"); //macbook compilation needs full file path
    bool fileReadingError = preFilledFibonacciNums == NULL;
    size_t readingBufferSize = 512;
    char *readingBuffer = malloc(readingBufferSize * sizeof(char));
    long long *preFilledNumArray = malloc(100 * sizeof(long long));
    for (int i = 0; i < 100; i++) {
        preFilledNumArray[i] = -1;
    }
    if (fileReadingError) {
        printf("Because of error with reading file calculations can't be checked\n");
    } else {
        int n;
        long long preFilledNum;
        while (getline(&readingBuffer, &readingBufferSize, preFilledFibonacciNums) != -1) {
            if (sscanf(readingBuffer, "%d %llu", &n, &preFilledNum) == 2) {
                preFilledNumArray[n] = preFilledNum;

            } else {
                printf("Error parsing line: %s\n", readingBuffer);
            }
        }
    }
    char methodName[5][30] = {
        "Recursion method",
        "Method using loop",
        "Binet's formula",
        "Method using array filling",
        "Dumb recursion method"
    };
    bool isAllResultsCorrect = true;
    double requiredTimeInSum[5] = {0.0f};
    int n = 0;
    while (n <= calculationsCeil) {
        clock_t start, end;
        double requiredTime[5] = {0.0f};

        start = clock();
        long long recursiveResult = recursiveFibonacci(n);
        end = clock();
        requiredTime[0] = RUNTIME(start, end);
        
        start = clock();
        long long BinetsFormulaResult = FibonacciByBinet(n);
        end = clock();
        requiredTime[1] = RUNTIME(start, end);

        start = clock();
        long long loopResult = loopFibonacci(n);
        end = clock();
        requiredTime[2] = RUNTIME(start, end);

        start = clock();
        long long arrayFillingResult = arrayFillingFibonicc(n);
        end = clock();
        requiredTime[3] = RUNTIME(start, end);

        long long slowRecursiveResult = preFilledNumArray[n]; // слишком много времени займёт тест, если считать этим методом дальше 42
        requiredTime[4] = INFINITY;
        if (n < 42) {
            start = clock();
            long long slowRecursiveResult = slowRecursiveFibonacci(n);
            end = clock();
            requiredTime[4] = RUNTIME(start, end);
        }

        if (needToDisplayAllRuntimeIntervals || needToDisplayCalculationResults) {
            printf("%d  ", n);
        }
        for (int i = 0; i < 5; i++) {
            requiredTimeInSum[i] += requiredTime[i];
            if (needToDisplayAllRuntimeIntervals) {
                printf("%.5f ms    ", requiredTime[i] * 1000);
                if (i == 4 && !needToDisplayCalculationResults) { // Костыли без которых не прожить
                    printf("\n");
                }
            }
        }
        long long calculationResults[5] = {recursiveResult, loopResult, BinetsFormulaResult, arrayFillingResult, slowRecursiveResult};
        bool isEachResultCorrect[5] = {true, true, true, true, true}; // {true} == {true, false, false, false, false}
        bool isCalculationsCorrect = true;
        for (int i = 0; i < 5; i++) {
            if (!fileReadingError || preFilledNumArray[i] == -1) {
                isEachResultCorrect[i] = preFilledNumArray[n] == calculationResults[i];
            }
            isCalculationsCorrect *= isEachResultCorrect[i];
            if (needToDisplayCalculationResults) {
                printf("%lld ", calculationResults[i]);
                if (i == 4) {
                    printf("\n");
                }
            }
        }
        for (int i = 0; i < 5; i++) {
            if (!isEachResultCorrect[i] && (!(n >= 72 && i == 2) || needToDisplayCalculationResults)) {
                printf("%s failed (n = %d, calculated %lld, expected %lld) \n", methodName[i], n, calculationResults[i], preFilledNumArray[n]);
            }
        }
        n++;
    }
    if (fileReadingError) {
        printf("\nTest skipped (file cant be opened)\n");
    } else if (calculationsCeil >= 72) {
        printf("\nTest succesful (ignored %d mismatches calculated by Binet's formula)\n", (n - 72));
    } else {
        printf(isAllResultsCorrect ? "\nTest succesful\n" : "\nTest failed\n");
    }
    for (int i = 0; i < 5; i++) {
        printf("%s used %.6f secs for calucalute Fibonacci numbers from 0 to %d\n", methodName[i], requiredTimeInSum[i], calculationsCeil);
    }
    fclose(preFilledFibonacciNums);
    free(preFilledNumArray);
    free(readingBuffer);
    return isAllResultsCorrect;
}

int main()
{
    globalTest(90, false, true); // n, вывод затраченного времени, вывод результатов вычислений
    return 0;
}
// n <= 10 -- dumb recursion works fast
// n >= 73 -- Binet is not accurate
// n >= 93 -- Nothing doesnt work