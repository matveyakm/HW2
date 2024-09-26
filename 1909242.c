#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define ARRAY_LENTH(array) (sizeof(array) / sizeof((array)[0]))
#define RUNTIME(start, end) (((double)(end - start)) / CLOCKS_PER_SEC)

// Написать сортировки пузырьком и подсчётом. С помощью функции clock() из time.h сравнить времена их работы на массиве из 100000 элементов.

void swap(int *array,int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void bubbleSort(int *array, int arrayLenth) {
    bool wasSwapped = true;
    for (int i = 0; i < arrayLenth; i++) {
        if (!wasSwapped) return;
        wasSwapped = false;
        for (int j = arrayLenth - 1; j > i; j--) {
            if (array[j] < array[j - 1]) {
                swap(array, j, j - 1);
                wasSwapped = true;
            }
        }
    }
}

void elementCountingSort(int *array, int arrayLenth) {
    int minElement = array[0];
    int maxElement = array[0];
    for (int i = 0; i < arrayLenth; i++) {
        if (array[i] > maxElement) {
            maxElement = array[i];
        }
        if (array[i] < minElement) {
            minElement = array[i];
        }
    }
    int *countOfElements = malloc((maxElement - minElement + 1) * sizeof(int));
    for (int i = 0; i < maxElement - minElement + 1; i++) {
        countOfElements[i] = 0;
    }
    for (int i = 0; i < arrayLenth; i++) {
        countOfElements[array[i] - minElement]++;
    }
    int currentPositionOfFillingIterator = 0;
    for (int i = 0; i < maxElement - minElement + 1; i++) {
        int j = 0;
        while (j < countOfElements[i]) {
            array[currentPositionOfFillingIterator] = i + minElement;
            currentPositionOfFillingIterator++;
            j++; 
        }
    }
    free(countOfElements);
}

void randomizeArray(int *array, int arrayLenth, int seed) {
    if (seed == 0) {
        srand(time(NULL));
    } else {
        srand(seed);
    }
    for (int i = 0; i < arrayLenth; i++) {
        array[i] = rand() % 1000;
    }
}

const bool defaultTest(bool needToCheckBubble) { // Функции бы передавать в аргументе...
    int defaultArray[] = {52, 144, 0, -2, 6, 3, 7, 3, 99};
    int defaultArrayLenth = ARRAY_LENTH(defaultArray);
    if (needToCheckBubble) {
        bubbleSort(defaultArray, defaultArrayLenth);
    } else {
        elementCountingSort(defaultArray, defaultArrayLenth);
    }
    int expectingResult[] = {-2, 0, 3, 3, 6, 7, 52, 99, 144};
    bool isTestSuccesful = true;
    for (int i = 0; i < defaultArrayLenth; i++) {
        isTestSuccesful *= defaultArray[i] == expectingResult[i];
    }
    return isTestSuccesful;
}

const bool resultTest(int *arraySortedByBubble, int *arraySortedByElementCounting, int arraysLenth) {
    bool isTestSucceful = arraySortedByBubble[0] == arraySortedByElementCounting[0];
    for (int i = 1; i < arraysLenth; i++) {
        if (arraySortedByBubble[i-1] > arraySortedByBubble[i] || arraySortedByBubble[i] != arraySortedByElementCounting[i]) {
            isTestSucceful = false;
        }
    }
    return isTestSucceful;
}

const bool globalTest(int arrayLenth) {
    bool isTestSuccesful = defaultTest(false) * defaultTest(true); // comapare with prefilled data 
    int seed[] = {52, 666, 4, 89, 5843, 101, 123, 5, 54358, 33, 947};
    int countOfTests = ARRAY_LENTH(seed);
    int *arrayNeedToTestBubble = malloc(arrayLenth * sizeof(int));
    int *arrayNeedToTestElementCounting = malloc(arrayLenth * sizeof(int));
    double bubbleRuntime = 0;
    double elementCountingRuntime = 0;
    clock_t start, end;
    for (int seedPeaker = 0; seedPeaker < countOfTests; seedPeaker++) { 
        randomizeArray(arrayNeedToTestBubble, arrayLenth, seed[seedPeaker]);
        memcpy(arrayNeedToTestElementCounting,arrayNeedToTestBubble, arrayLenth * sizeof(int));
        
        start = clock();
        bubbleSort(arrayNeedToTestBubble, arrayLenth);
        end = clock();
        bubbleRuntime += RUNTIME(start, end);

        start = clock();
        elementCountingSort(arrayNeedToTestElementCounting, arrayLenth);
        end = clock();
        elementCountingRuntime += RUNTIME(start, end);
        bool localTestSuccesful = resultTest(arrayNeedToTestBubble, arrayNeedToTestElementCounting, arrayLenth);
        if (!localTestSuccesful) {
            printf("Test with seed [%d] failed.\n", seed[seedPeaker]);
        }
        isTestSuccesful *= localTestSuccesful;
    }
    printf(isTestSuccesful ? "Test succesful" : "Test failed");
    printf(" [%d elements | %d tests]\n", arrayLenth, countOfTests);
    printf("Average runtime of bubble sort: %.7f\n", bubbleRuntime / ARRAY_LENTH(seed));
    printf("Average runtime of element counting sort: %.7f\n", elementCountingRuntime / ARRAY_LENTH(seed));
    
    free(arrayNeedToTestBubble);
    free(arrayNeedToTestElementCounting);

    return isTestSuccesful;
}

int main()
{
    int arrayLenth = 100000;
    int *arrayNeedToSort = malloc(arrayLenth * sizeof(int));
    int *arrayCopy = malloc(arrayLenth * sizeof(int));
    randomizeArray(arrayNeedToSort, arrayLenth, 0);
    memcpy(arrayCopy, arrayNeedToSort, arrayLenth * sizeof(int));

    clock_t start, end;

    start = clock();
    bubbleSort(arrayCopy, arrayLenth); // ~16 sec
    end = clock();
    printf("Bubble sorting used %.6f sec\n", RUNTIME(start,end));

    start = clock();
    elementCountingSort(arrayNeedToSort, arrayLenth); // ~0.005 sec
    end = clock();
    printf("Element counting used %.6f sec\n", RUNTIME(start,end));

    printf(resultTest(arrayNeedToSort, arrayCopy, arrayLenth) ? "Local test succesful\n" : "Local test failed\n");
    
    globalTest(10000);

    free(arrayNeedToSort);
    free(arrayCopy);
    return 0;
}