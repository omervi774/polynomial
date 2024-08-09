// polynomial
// Authored by Omer Weinbrand

// ==Description==

// The program produces threads as the number of xs in the polynomial.
// Each thread calculates its part in the polynomial and updates the result in the global variable.
// after all the threads are finished, the main theard prints the result and begins the next iteration.
// The program ends when the user enters the word done
#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <pthread.h>

#define len 512
typedef struct Calc {
    int coeff;
    int power;
    int val;
}
        calcs;
int res = 0;
pthread_mutex_t my_lock;
calcs * check_sentence(char * , int * , int * , int * , int);
int chars_to_num(char * , int, int);
void * calc_val_poly(void * calcs1);
int main() {
    printf("Hello, World!\n");
    pthread_mutex_init( & my_lock, NULL);
    char sentence[len];
    int num_of_threads = 0;
    int flag = 0;
    int val = 0;
    int status;
    do {
        printf("enter polynomial and val to put like this (polynomial, val)\n");
        fgets(sentence, len, stdin);
        sentence[strlen(sentence) - 1] = '\0';
        val = 0;
        num_of_threads = 0;
        flag = 0;
        res = 0;
        check_sentence(sentence, & num_of_threads, & flag, & val, 0);
        if (flag == 0) {
            calcs * calcs1 = check_sentence(sentence, & num_of_threads, & flag, & val, 1);
            if (calcs1 != NULL) {
                pthread_t tid[num_of_threads];
                for (int i = 0; i < num_of_threads; i++) {
                    status = pthread_create( & tid[i], NULL, calc_val_poly, & calcs1[i]);
                    if (status != 0) {
                        fprintf(stderr, "pthread_create faild\n");
                        exit(1);
                    }
                }
                for (int i = 0; i < num_of_threads; i++)
                    pthread_join(tid[i], NULL);
                res += val;
                printf("res = %d\n", res);
                free(calcs1);
                calcs1 = NULL;
            }
        }

    } while (strcmp(sentence, "done") != 0);
    pthread_mutex_destroy( & my_lock);
    return 0;

}
// if p = 0 :
// check if the sentence is proper,and count how many 'x' I have
// if p = 1 :
// create an array of calas according to the number of 'x' and for each cell calculate the coeff,power,and the val I need to put in the x
calcs * check_sentence(char * sentence, int * num_of_threads, int * flag, int * free_val, int p) {
    if (p == 0 && strcmp(sentence, "done") == 0) {
        * flag = 1;
        return NULL;
    }
    int t = -1; // denote where is , (if i have space before , = invalid input)
    int l = -1; // where is x
    int j = 0; // index of the temp
    int k = 0; // index of calcs1
    int val1 = 0;
    int num_space = 0;
    char temp[len];
    calcs * calcs1 = NULL;
    if (p == 1) {
        calcs1 = (calcs * ) malloc(sizeof(calcs) * ( * num_of_threads));
        if (calcs1 == NULL) {
            fprintf(stderr, "malloc faild\n");
            return NULL;
        }
    }
    for (int i = 0; i < strlen(sentence); i++) {
        if (p == 0 && sentence[i] == 'x') // because we get to x I can deduce that num of thread++
        {
            * num_of_threads += 1;
        }
        if (p == 0 && sentence[i] != 'x' && sentence[i] != '+' && sentence[i] != '*' &&
            sentence[i] != '^' && sentence[i] != ',' && sentence[i] != ' ' && (sentence[i] < '0' || sentence[i] > '9')) {
            * flag = 1;
            fprintf(stderr, "invalid input \n");
            return NULL;
        } else if (p == 0 && sentence[i] == ',')
            t = i;

        else if (p == 0 && sentence[i] == ' ') // count how many space the sentence has
        {
            num_space++;
            if (t == -1) {
                * flag = 1;
                fprintf(stderr, "invalid input (space before ,)\n");
            }
            if (num_space > 1) // if sentence has more than 1 space = invalid input
                *
                        flag = 1;
        } else if (p == 1 && sentence[i] != '+' && sentence[i] != ',') {
            temp[j] = sentence[i];
            if (sentence[i] == 'x') // notation where I got 'x'
            {
                l = j;
            }
            j++;

        }
        if (p == 1 && (sentence[i] == '+' || sentence[i] == ',' || i + 1 == strlen(sentence))) {
            temp[j] = '\0';

            if (l == 0) // no coeff, calculate the power of x
            {
                calcs1[k].coeff = 1;
                int power1 = chars_to_num(temp, l, 1);
                calcs1[k].power = power1;
            } else if (l > 0) // calculate coeff of x and the power of x
            {
                int coeff1 = chars_to_num(temp, l, 0);
                calcs1[k].coeff = coeff1;
                if (l + 1 == strlen(temp)) // no power for x in temp means power = 1
                    calcs1[k].power = 1;
                else {
                    int power1 = chars_to_num(temp, l, 1);
                    calcs1[k].power = power1;
                }
            }
            if (l == -1 && i + 1 < strlen(sentence)) // free val and no need to update struct so k--(in the end of the method k++ so its even)
            {
                * free_val = chars_to_num(temp, l, 2);
                k--;
            }

            if (l == -1 && i + 1 == strlen(sentence)) {
                val1 = chars_to_num(temp, l, 3);
            }
            l = -1;
            j = 0;
            k++;
            strcpy(temp, "");
        }
    }
    if (p == 0)
        return NULL;

    for (int i = 0; i < * num_of_threads; i++)
        calcs1[i].val = val1;
    return calcs1;
}
// calculate the coeff,power and the val to put of x
int chars_to_num(char * sentence, int l, int p) {
    int val = 0;
    int start;
    int end;
    if (p == 0) // for calculate coeff
    {
        start = 0;
        end = l - 1;
    } else if (p == 1) // for calculate power
    {
        start = l + 2;
        end = strlen(sentence);
    } else if (p == 2) // for calculate free val
    {
        start = 0;
        end = strlen(sentence);
    } else {
        start = 1;
        end = strlen(sentence);
    }
    for (int i = start; i < end; i++) {
        if (val == 0)
            val = (int)(sentence[i] - 48);
        else
            val = val * 10 + (int)(sentence[i] - 48);
    }
    return val;
}
// each thread calculate his part of the polynomial using the argument of the function ->
// calcs[i] for every thread that store the coeff,power,and the val to put of the 'x'
void * calc_val_poly(void * calcs1) {
    calcs * cal = (calcs * ) calcs1;
    int val = cal -> val;
    int power = cal -> power;
    int coeff = cal -> coeff;
    int res1 = 1;
    for (int i = 0; i < power; i++)
        res1 *= val;

    res1 *= coeff;
    pthread_mutex_lock( & my_lock);
    res += res1;
    pthread_mutex_unlock( & my_lock);
    //pthread_exit(NULL);
    return NULL;
}
