#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define ARRAY_LENTH(array) (sizeof(array) / sizeof((array)[0]))
#define RUNTIME(start, end) (((double)(end - start)) / CLOCKS_PER_SEC)

// Написать программу, которая заполняет массив случайными значениями (с использованием функции rand из stdlib.h), потом преобразует его без использования дополнительных массивов так, что в начале массива будут элементы, меньшие первого, а в конце — большие либо равные первому. Программа должна работать за линейное время.

void swap(int *array,int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

void firstToMid(int *randomArray, int arrayLenth) {
    int countOfNumsThatBiggerThanFirst = 0;
    int firstElementValue = randomArray[0];
    int i = 1;
    while (i < arrayLenth) {
        if (randomArray[i] >= firstElementValue) {
            countOfNumsThatBiggerThanFirst++;
        }
        else {
            int j = 0;
            while (j < countOfNumsThatBiggerThanFirst + 1) {
                swap(randomArray, i - j, i - j - 1);
                j++;
            }
        }
        i++;
    }
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

void printArray(int *array, int arrayLenth) {
    for (int i = 0; i < arrayLenth; i++) { 
        printf("%d ",array[i]);
    }
    printf("\n");
}

const bool resultTest(int *array, int arrayLenth, int firstElement) {
    bool afterFirstElement = false;
    bool isTestSuccesful = true;
    for (int i = 0; i < arrayLenth; i++) { 
        if (array[i] == firstElement) {
            afterFirstElement = true;
        }
        if ((array[i] < firstElement) == afterFirstElement) {
            isTestSuccesful = false;
        }
    }
    return isTestSuccesful;
}

const bool globalTest(int lenthOfTestingArray) {
    int seed[] = {52, 666, 4, 89, 5843, 101, 123, 5, 54358, 33, 947};
    int *arrayNeedToTest = malloc(lenthOfTestingArray * sizeof(int));
    bool isTestSuccesful = true;
    int countOfSuccesfulTests = 0;
    clock_t start, end;
    start = clock();
    for (int seedPeaker = 0; seedPeaker < ARRAY_LENTH(seed); seedPeaker++) { 
        randomizeArray(arrayNeedToTest, lenthOfTestingArray, seed[seedPeaker]);
        int firstElement = arrayNeedToTest[0];
        firstToMid(arrayNeedToTest, lenthOfTestingArray);
        bool localTestSuccesful = resultTest(arrayNeedToTest, lenthOfTestingArray, firstElement);
        if (!localTestSuccesful) {
            printf("Test with seed [%d] failed.\n", seed[seedPeaker]);
        } else {
            countOfSuccesfulTests++;
        }
        isTestSuccesful *= localTestSuccesful;
    }
    end = clock();
    printf(isTestSuccesful ? "Test succesful " : " Test failed ");
    printf("[%d/%d tests succesful with lenth %d | %.5f secs]", countOfSuccesfulTests, ARRAY_LENTH(seed), lenthOfTestingArray, RUNTIME(start, end));
    free(arrayNeedToTest);
    return isTestSuccesful;
}

int main()
{
    int randomArrayLenth = 8;
    int *randomArray = malloc(randomArrayLenth * sizeof(int));

    randomizeArray(randomArray, randomArrayLenth, 0);
    int firstElemenent = randomArray[0];
    printArray(randomArray, randomArrayLenth);

    firstToMid(randomArray, randomArrayLenth);
    printArray(randomArray, randomArrayLenth);
    printf(resultTest(randomArray, randomArrayLenth, firstElemenent) ? "Local test (for random nums) succesful\n" : "Local test (for random nums) failed\n");
    free(randomArray);

    globalTest(10000);

    return 0;
}