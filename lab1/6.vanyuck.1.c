#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int size = 4;

int charToInt(char a){
    return a - '0';
}

int sum(int a, int b, int overflow){
    return (overflow ^ (a ^ b));
}

int getOverflow(int a, int b, int overflow){
    return  ((a & b) | (overflow & (a ^ b)));
}

int* add(int* num1, int* num2){
    int *result = malloc(size*sizeof(int));
    int overflow = 0;

    for(int i = size - 1; i>=0; i--){
       result[i] = sum(num1[i], num2[i], overflow);
       overflow = getOverflow(num1[i], num2[i], overflow);
    }

    if(num2[0] == 0 && num1[0] == 0 && result[0] == 1){           // расширение сетки в 2 раза
        size *= 2;
        int *newResult = malloc(size*sizeof(int));  
        for (int i = size / 2, j = 0; i < size; i++, j++)
        {
            newResult[i] = result[j];
        }
        return newResult;   
    }

    if (num1[0] == 1 && num2[0] == 1 && result[0] == 0){       // расширение сетки в 2 раза
        size *= 2;
        int *newResult = malloc(size*sizeof(int));  
        for (int i = 0, j = 0; i < size; i++)
        {
            if (i < size / 2){
                newResult[i] = 1;
            }
            else {
                newResult[i] = result[j];
                j++;
            } 
        }
        return newResult;  
    }

    return result;
}

int* getNegative(int *arr){
    for(int i = 0; i < size; i++){
        arr[i] = !arr[i]; 
    }
    int *tempArr = malloc(size*sizeof(int)); 
    tempArr[size - 1] = 1;              
    arr = add(arr, tempArr);
    return arr;
}

int* multiple(int* num1, int* num2){
    int* A = malloc((2 * size + 1) *sizeof(int));
    int* S = malloc((2 * size + 1) *sizeof(int));
    int* P = malloc((2 * size + 1) *sizeof(int));

    for(int i = 0; i < size; i++){
        A[i] = num1[i];
    }

    int* tempArr = getNegative(num1);
    for(int i = 0; i < size; i++)    {
        S[i] = tempArr[i];
    }

    for(int i = size, j = 0; i < 2 * size; i++, j++){
        P[i] = num2[j];
    }

    for (int i = 0; i < size; i++){
        if(P[2 * size - 1] == 0 && P[2 * size] == 1){
            size = 2 * size + 1;
            P = add(A, P);
            size = (size - 1) / 2;
        }
        if(P[2 * size - 1] == 1 && P[2 * size] == 0){
            size = 2 * size + 1;
            P = add(S, P);
            size = (size - 1) / 2;
        }

        int prev = P[0];                //сдвиг вправо
        P[0] = P[2 * size -1];
        for(int i = 1; i < 2 * size + 1; i++){
            int temp = P[i];
            P[i] = prev;
            prev = temp;
        }
    }
    int* result = malloc((2 * size) *sizeof(int));
    for(int i = 0; i < 2 * size; i++){
        result[i] = P[i];
    }
    size *= 2;
    return result;
}

void printArr(int* arr){
    for(int i = 0; i < size; i++){
        printf("%d", arr[i]);
    }
    printf("\n");
}

int* getAdditionalCode(int* arr){
    if (arr[0] == 1){
        for(int i = 1; i < size; i++){ 
            arr[i] = !arr[i]; 
        }
        int *tempArr = malloc(size*sizeof(int)); 
        tempArr[size - 1] = 1;              
        arr = add(arr, tempArr);
    }
    return arr;
}


int main(){
    char *intput1 = malloc(size*sizeof(char));
    char *intput2 = malloc(size*sizeof(char));
    int *number1 = malloc(size*sizeof(int));
    int *number2 = malloc(size*sizeof(int));

    printf("Enter two numbers  in binary code:\n");
   
    scanf("%s", intput1);

    for(int i = 0; i < size; i++){
        number1[i] = charToInt(intput1[i]); 
    }

    number1 = getAdditionalCode(number1);

    scanf("%s", intput2); 

    for(int i = 0; i < size; i++){
        number2[i] = charToInt(intput2[i]);
    }

    number2 = getAdditionalCode(number2);

    printf("Number1 in additional code:  ");
    printArr(number1);

    printf("Number2 in additional code:  ");
    printArr(number2);

    int operationNum;
    printf("1 --- Add \n2 --- Sub \n3 --- Multiplication\n");
    scanf("%d", &operationNum);

    switch(operationNum){
        case 1 :
            printf("The result of adding in additional code:");
            int *outputAdd = add(number1, number2);
            printArr(outputAdd);
            printf("\n");
            break;
        case 2 :
            printf("The result of subtraction in addiltional code:");
            number2 = getNegative(number2);
            int *outputSub = add(number1, number2);
            printArr(outputSub);
            printf("\n");
            break;
        case 3 :
            printf("The result of multiplication in additional code:");
            int *outputMult = multiple(number1, number2);
            printArr(outputMult);
            printf("\n");
            break;
    }
    
    return 0;
}