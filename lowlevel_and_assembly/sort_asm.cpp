/**DigiPen Institute of technology, all rights reserved.**\
 Author     Jimi Huard
 Class      CS 315
 Assignment 2
 Description
   Optimize a C subroutine or section of C code using x86 inline assembly. 
   Please use bubble sort as the code to optimize but if you find another one 
   such as quick sort, insertion sort, binary sort and etc. to optimize, please 
   email hwu@digipen.edu to get agreement.
\**DigiPen Institute of technology, all rights reserved.**/

#include <iostream>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#define ASSERT(x, msg) do { \
  if (x) ; \
  else MessageBoxA(0, msg, msg, MB_OK) ; \
} while (0)

#define ERROR_IF(x, msg) do { \
  if (x) MessageBoxA(0, msg, msg, MB_OK) ; \
} while (0)

/*
  sorts an array least to greatest. (Bubble Sort algorithm)
*/
static void BubbleSort(int array[], int arraySize) {
/* C/C++ implementation  
  int i, j ;
  for (i = 0; i < arraySize; ++i)
    for (j = i + 1; j < arraySize; ++j)
      if (array[i] > array[j])
        std::swap(array[i], array[j]) ;
*/
  // eax = array[i]
  // ebx = array[j]
  // esi = i
  // ecx = j
  // edx = temp (swap)
  __asm {
    mov esi, -1
    
  BubbleSort_Loop_Outer:
    cmp esi, [arraySize]
    jge BubbleSort_End
      
    inc esi
    mov eax, [array]
    mov eax, [eax + TYPE int * esi]
    mov ecx, esi
  BubbleSort_Loop_Inner:
    inc ecx
    cmp ecx, [arraySize]
    jge BubbleSort_Loop_Outer

    mov ebx, [array]
    mov ebx, [ebx + TYPE int * ecx]

    cmp ebx, eax
    jge BubbleSort_Loop_Inner
    
    mov edx, [array]
    mov [edx + TYPE int * ecx], eax
    mov [edx + TYPE int * esi], ebx
    mov eax, ebx

    jmp BubbleSort_Loop_Inner

  BubbleSort_End:

  }
}

/*
  Returns whether or not the given array is sorted
  least to greatest.
*/
static bool IsSorted(int const array[], int arraySize) {
  for (--arraySize; arraySize; --arraySize) 
    if (array[arraySize] < array[arraySize - 1])
      return false ;
  return true ;
}

/*
  The entry point for the program.
*/
int main(void) {
  int array[100] ;
  int const arraySize = sizeof(array) / sizeof(*array) ;

  // try base case, pre sorted array.
  for (int i = 0; i < arraySize; ++i)
    array[i] = i ;

  BubbleSort(array, arraySize) ;
  ASSERT(IsSorted(array, arraySize), "Failure") ;

  // try end case, reverse sorted array.
  for (int i = 0; i < arraySize; ++i)
    array[arraySize - i - 1] = i ;

  BubbleSort(array, arraySize) ;
  ASSERT(IsSorted(array, arraySize), "Failure") ;

  // try 1000 random combinations
  for (int j = 0; j < 1000; ++j) {
    for (int i = 0; i < arraySize; ++i) 
      array[i] = rand() % 1000 ;

    BubbleSort(array, arraySize) ;
    ASSERT(IsSorted(array, arraySize), "Failure") ;
  }

  return 0 ;
}