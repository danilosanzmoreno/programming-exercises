#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

/*
 * Complete the 'dynamicArray' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */

/*
 * To return the integer array from the function, you should:
 *     - Store the size of the array to be returned in the result_count variable
 *     - Allocate the array statically or dynamically
 *
 * For example,
 * int* return_integer_array_using_static_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     static int a[5] = {1, 2, 3, 4, 5};
 *
 *     return a;
 * }
 *
 * int* return_integer_array_using_dynamic_allocation(int* result_count) {
 *     *result_count = 5;
 *
 *     int *a = malloc(5 * sizeof(int));
 *
 *     for (int i = 0; i < 5; i++) {
 *         *(a + i) = i + 1;
 *     }
 *
 *     return a;
 * }
 *
 */
int* dynamicArray(int n, int queries_rows, int queries_columns, int** queries, 
    int* result_count) 
{
    int last_answer = 0;
    int** seq_list = calloc(n, sizeof(int*));
    int* seq_list_position = calloc(n, sizeof(int));
    int* seq_list_size = calloc(n, sizeof(int));
    int* result = malloc(1024 * sizeof(int));
    int result_size = 1024;
    int seq = 0;

    *result_count = 0;
    for (int i = 0; i < queries_rows; i++) {
        seq = (queries[i][1] ^ last_answer) % n;
        if (queries[i][0] == 1) {
            if ( seq_list[seq] == 0 ) {
                seq_list_size[seq] = 1024;
                seq_list[seq] = malloc(seq_list_size[seq] * sizeof(int));
            }
            else if (seq_list_position[seq] >= seq_list_size[seq]) {
                seq_list_size[seq] <<= 1;
                seq_list[seq] = realloc(seq_list[seq], seq_list_size[seq] 
                    * sizeof(int));              
            }
            seq_list[seq][seq_list_position[seq]++] = queries[i][2];
        }        
        else if (queries[i][0] == 2) {
            last_answer = seq_list[seq][queries[i][2] % seq_list_position[seq]];
            if ( *result_count >= result_size) {
                result_size <<= 1;
                result = realloc(result, result_size * sizeof(int));
            }
            result[(*result_count)++] = last_answer;
        }
        else {
            printf("Error: input doesn't match.");
            exit(EXIT_FAILURE);
        }
    }
    return result;
} 

int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    /**
     * Get the integers as strings separated by spaces
     */
    char** first_multiple_input = split_string(rtrim(readline()));

    /**
     * Set n as first integer 
     */
    int n = parse_int(*(first_multiple_input + 0));

    /**
     * Set q as second integer 
     */
    int q = parse_int(*(first_multiple_input + 1));

    int** queries = malloc(q * sizeof(int*));

    /**
     * Alloc q lines into queries list of 3 items each input 
     */
    for (int i = 0; i < q; i++) {
        *(queries + i) = malloc(3 * (sizeof(int)));

        char** queries_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 3; j++) {
            int queries_item = parse_int(*(queries_item_temp + j));

            *(*(queries + i) + j) = queries_item;
        }
    }

    int result_count;
    int* result = dynamicArray(n, q, 3, queries, &result_count);

    //for (int i = 0; i < result_count; i++) {
    //    fprintf(fptr, "%d", *(result + i));
//
    //    if (i != result_count - 1) {
    //        fprintf(fptr, "\n");
    //    }
    //}
//
    //fprintf(fptr, "\n");
//
    //fclose(fptr);

    return 0;
}

/**
 * char* readline() - Read a line from stdin
 * Read a line from stdin, the default space allocated for the line is 1kB but
 * if the input istring is trimmed continue  doubling the allocated space to 
 * be able to allocate the whole line
 * Return: pointer to string trimmed
 */
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    /**
     * Continue reading from the cursor from previous iteration
     */
    while (true) {
        char* cursor = data + data_length;
        /**
         * Read a line or (alloc_length - data_length) characters from input 
         * and save into cursor
         */
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        /**
         * Add the line read length to data_length
         */
        data_length += strlen(cursor);

        /**
         * if data_length is less than allco_length or last character is line
         * end, the line has been read
         */
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        /**
         * If data_length is greater than allocated memory double alloc length
         * and reallocate space
         */
        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    /**
     * Replace end of line by NULL if necessary
     * readjust data pointer reserved memory
     */
    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

/**
 * char* ltrim(char* str) -Trim left spaces in string
 * @str: string to trim
 * If string pointer is 0 return null, if string lenght is 0 return string, 
 * get next characters while pointer to string is space and is no null
 * Return: pointer to string trimmed
 */
char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

/**
 * char* rtrim(char* str) -Trim right spaces in string
 * @str: string to trim
 * If string pointer is 0 return null, if string lenght is 0 return string, 
 * get a pointer to last character of string and get previous characters while
 * they are spaces. when one character no space is found set next character as
 * null.
 * Return: pointer to string trimmed
 */
char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

/**
 * char** split_string(char* str) - Split string
 * @str: string to split
 * Get the first token until the space of string, then the splits array of
 * strings is reallocated and add add the last string pointer "token" to splits
 * Return: pointer of pointer where there are all tokens 
 */
char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");  
    }

    return splits;
}

/**
 * int parse_int(char* str) - Parse str to int
 * @str: string to parse int
 * Take the string and converts to llong in base 10, if displacement pointer 
 * doesn't match with NULL exit failure
 * Return: integer parsed 
 */
int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}