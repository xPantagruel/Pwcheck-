//Project 1 - IZP - Password strength verification
//Created by Matej Macek (xmacek27)
//example of use : ./pwcheck LEVEL PARAM [--stats] <hesla.txt
//LEVEL - an integer in the interval [1, 4] that specifies the required security level
//PARAMETER - a positive integer that specifies an additional rule parameter
//--stats - determines whether summary statistics of the analyzed passwords
// should be displayed at the end of the program

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//function counting lenghth of each password and returning true if password is not longer than 100
bool length(char buffer[102]) {
    int count = 0;
    for (int i = 0; buffer[i] != '\0' && buffer[i] != '\n' ; ++i) {
        ++count;
    }
    if (count > 100)return true;
    return false;
}

//function checks if argument 1 is number  otherwise returns false
bool argv1_check(char *str){
    int count = 0;
    for (int i = 0; str[i] != '\0'; ++i){
        ++count;
        if(count > 1){
            return true;
        }
    }
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] < '1' || str[i] > '4') {
            return true;

        }
    }
    return false;
}

//function checks if argument 2 is not a number  otherwise returns false
bool argv2_check(char *str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] < '1' || str[i] > '9') {
            return true;
        }
    }
    return false;
}

//function checks if argument 3 is --stats otherwise returns false
bool compare(char *str) {
    char stats[8] = "--stats";
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] != stats[i]) {
            return true;
        }
    }
    return false;
}

//function searching for lowercase letters in password
bool level2_small(char buffer[102]){
    for (int i = 0; buffer[i] != '\0' ; ++i) {
        if(buffer[i] >= 'a' && buffer[i] <= 'z'){
            return true;
        }
    }
    return false;
}

//function searching for capital letters in password
bool level2_big(char buffer[102]){
    for (int i = 0; buffer[i] != '\0' ; ++i) {
        if(buffer[i] >= 'A' && buffer[i] <= 'Z'){
            return true;
        }
    }
    return false;
}

//function searching for symbols in password
bool level2_symbols(char buffer[102]){
    for (int i = 0; buffer[i] != '\0' ; ++i) {
        if((buffer[i] >= ' ' && buffer[i] <= '/')
           || (buffer[i] >= ':' && buffer[i] <= '@')
           || (buffer[i] >= '[' && buffer[i] <= '`')
           || (buffer[i] >= '{' && buffer[i] <= '~') ){
            return true;
        }
    }
    return false;
}

//function searching for numbers in password
bool level2_num(char buffer[102]){
    for (int i = 0; buffer[i] != '\0' ; ++i) {
        if(buffer[i] >= '0' && buffer[i] <= '9'){
            return true;
        }
    }
    return false;
}

//the function checks:
// whether the password meets the given number of groups according to the argument
bool level2(char buffer[102], long argv2){
    int count = 0;
    int argv = argv2;
    if(argv2 > 4){
        argv = 4;
    }
    if(level2_small(buffer))   ++count;
    if(level2_big(buffer))     ++count;
    if(level2_num(buffer))     ++count;
    if(level2_symbols(buffer)) ++count;
    if(count >= argv)return true;
    return false;
}

//the function checks:
//whether the same characters appear in the password in a row of the number according to the argument
bool level3 (char buffer[102], long argv2){
    int j = 1;

    for(int i = 0; buffer[i] != '\0' ; ++i) {
        int count = j;

        if (buffer[i] != buffer[i+1] ){
            j = 1;
        }
        if (buffer[i] == buffer[i+1]) ++j;

        if (count >= argv2){
            return false;
        }
    }
    return true;
}
//the function checks:
//if there are two identical substrings of length according to the argument in the password
bool level4 (char buffer[102], long argv2){
    int d = 1;

    for (int i = 0; buffer[i] != '\0' ; ++i) {
        int i1 = i + 1;
        for (int j = i +1; buffer[j] != '\0'; ++j) {
            if( d <= argv2){
                d = 1;
                i1 = i + 1;
            }

            if (buffer[i] == buffer[j] ){
                for (int k = j+1;buffer[k] != '\0'; ++k) {
                    if (buffer[k] == buffer[i1]) {
                        i1 = i1 + 1;
                        d = d + 1;
                    }
                    if( d >= argv2){
                        return false;
                    }
                }
            }

        }

    }
    return true;
}

//The function counts the length of the password and returns it
int letters_size(char buffer[102]) {
    int count = 0;
    for (int i = 0; buffer[i] != '\0'; ++i) {
        if( buffer[i] != '\n')++count;
    }
    return count;
}

//there is array full of zeros and then funkcion that
//according to the value of the password character in the ascii tab overrides 0 from that place to 1
char array[256] = {0};
bool different_chars1(char buffer[102]) {
    for (int i = 0; buffer[i] != '\0'; ++i) {
        array[(int) buffer[i]] = 1;

    }
    return true;
}

//function counting all 1 in array
int different_chars2(){
    int count = 0;
    for (int i = 0; i <= 256 ; ++i){
        if (array[i] == 1 ){
            ++count;
        }
    }
    return count;
}

//the main function that uses the other functions to determine
// whether the password meets the corresponding Level and parameter and optionally print out statistics
int main(int argc, char *argv[]) {

    //main conditions for argument
    if(argc < 3){
        fprintf(stderr, "error");
        return -1;
    }

    if(argc > 4){
        fprintf(stderr, "error");
        return -1;
    }

    if(argv1_check(argv[1])){
        fprintf(stderr, "error");
        return -1;
    }

    if(argv2_check(argv[2])){
        fprintf(stderr, "error");
        return -1;
    }
    if (argc == 4) {
        if (compare(argv[3])) {
            fprintf(stderr, "error");
            return -1;
        }
    }

    char *ptr;
    long argv1;
    long argv2;
    char buffer[102];
    double numofpsw = 0;
    double allsize = 0;
    int min = 100;

    //argument using strol to converts the initial part of the string to a long int value
    argv1 = strtol(argv[1], &ptr, 30);
    argv2 = strtol(argv[2], &ptr, 30);

    //a cycle that checks the value of the argument
    //and according to it passes the password through the given levels and print them if they meet the levels
    while (fgets(buffer, 102, stdin) != NULL) {

        if(length(buffer)){
            fprintf(stderr, "error");
            return -1;
        }

        if (argv1 == 1) {
            if (level2_big(buffer)
                && level2_small(buffer)) {
                printf("%s", buffer);
            }
        }

        if (argv1 == 2) {
            if (level2_big(buffer)
                && level2_small(buffer)
                && level2(buffer, argv2)) {
                printf("%s", buffer);
            }
        }

        if (argv1 == 3) {
            if (level2_big(buffer)
                && level2_small(buffer)
                && level2(buffer, argv2)
                && level3(buffer, argv2)) {
                printf("%s", buffer);
            }
        }

        if (argv1 == 4) {
            if (level2_big(buffer)
                && level2_small(buffer)
                && level2(buffer, argv2)
                && level3(buffer, argv2)
                && level4(buffer, argv2)) {
                printf("%s", buffer);
            }
        }
        //counting all characters in password and added them to a double allsize
        if (letters_size(buffer) != 0) {
            allsize = allsize + letters_size(buffer);
        }
        if (allsize == 0) {
            allsize = letters_size(buffer);
        }

        //counting how many passwords we have
        ++numofpsw;

        //condition that overwrites the int min if size of password is smaller than min
        if (letters_size(buffer) < min) {
            min = letters_size(buffer);
        }

        //according to the value of the password character in the ascii tab overrides 0 from that place to 1 in array
        different_chars1(buffer);
    }

    //condition for listing statistics
    if (argc == 4) {
        int nchars;
        double avg;

        //calculating average length of all passwords
        avg = allsize / numofpsw;

        if (array[10] == 1) {
            array[10] = 0;
        }

        //stores to int nchars number of different characters in all passwords
        nchars = different_chars2();

        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", nchars);
        printf("Minimalni delka: %d\n", min);
        printf("Prumerna delka: %.1f\n", avg);
    }
}


