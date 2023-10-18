#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hangman.h"

char secret_word[20] = "";
char coded_word[20] = "";
char guessed_letters[30] = "";
char message[4][100] = {"", "", "", "       "};
char help[100] = "";
char warning[100] = "";
int errors = 0, hidden_letters;
Hangman stickman = {"   ", "   ", " ", " ", " ", " ", " "};

//Escolhe a palavra secreta
void find_word() { 
    srand(time(NULL));

    int i;   
    int position = (rand() % TOTAL_WORDS) + 1;

    FILE *arquivo = fopen("palavras.txt", "r");
    if(arquivo) {
        for(i = 0; i < position; i++){
            fscanf(arquivo, "%s", secret_word);
        }
    }

    hidden_letters = strlen(secret_word);
    generate_coded_word(hidden_letters);
}

//Gera a sequencia de tracos
void generate_coded_word(int length) {
    int cont;

    for (cont = 0; cont < length; cont++)
        coded_word[cont] = '_';
    
    coded_word[length] = '\0';
}

//Mostra a forca e pede uma letra
void guess() {
    char input[30];

    if(hidden_letters <= 3) 
        strcpy(help, "\e[46;30m ⚑ To guess the entire word, input 'word' \e[0m");

    print_hangman();
    strcpy(warning, "");

    printf("\t🎯 Guess a letter: ");
    scanf(" %s", input);

    if(strcmp(input, "word") == 0) {
        guess_word();
        return;
    }

    switch (check_guess(strlen(guessed_letters), input)) {
        case TOO_LONG:
            strcpy(warning, "\e[43;30m ⚠ Input only one letter at a time \e[0m");
            break;
        case INVALID:
            strcpy(warning, "\e[43;30m ⚠ Invalid character \e[0m");
            break;
        case FOUND:
            strcpy(warning, "\e[43;30m ⚠ Repeated letter \e[0m");
            break;
        case NOT_FOUND:
            check_letter(input[0]);
    }
}

//Verifica os possiveis erros no input
int check_guess(int total_guessed, char input[]) {
    if(strlen(input) != 1)
        return TOO_LONG;
    
    char letter = input[0];

    if(letter < 97 || letter > 122) {
        if(letter >= 65 && letter <= 90) 
            letter += 32;
        else
            return INVALID;
    }

    int i;
    for (i = 0; i < total_guessed; i++)
        if(guessed_letters[i] == letter)
            return FOUND;

    strncat(guessed_letters, &letter, 1);
    return NOT_FOUND;
}

//Verifica se a letra esta na palavra secreta e a substitui na sequencia de tracos
void check_letter(char letter) {
    int i, cont = 0;

    for (i = 0; i < strlen(secret_word); i++) 
        if (secret_word[i] == letter) {
            coded_word[i] = letter;
            cont++;
            hidden_letters--;
        }

    if(cont == 0)
        errors++;
}

//Verifica se a palavra ja foi descoberta
int check_word() {
    if(!strcmp(coded_word, secret_word))
        return FINISH;
    else
        return CONTINUE;
}

//Mostra mensagem de vitoria
void end_win() {
    strcpy(message[0], "\e[0;32m       ╖ ╓ ╔═╗ ╖ ╓   ╖ ╓ ╥ ╖ ╥ \e[0m");
    strcpy(message[1], "\e[0;32m       ╚╦╝ ║ ║ ║ ║   ║╥║ ║ ║╲║ \e[0m");
    strcpy(message[2], "\e[0;32m        ╨  ╚═╝ ╚═╝   ╚╩╝ ╨ ╨ ╙ \e[0m");
    strcpy(message[3], "--- The secret word indeed was '");
    strcat(message[3], secret_word);
    strcat(message[3], "' ---");
    strcpy(coded_word, "");
    strcpy(help, "");

    print_hangman();
}

//Verifica se o usuario perdeu
int check_errors() {
    if(errors == TOTAL_ERRORS)
        return FINISH;
    else if(errors < TOTAL_ERRORS)
        return CONTINUE;

    for(errors = 1; errors < 7; errors++)
        update_hangman();
    return FINISH;
}

//Mostra mensagem de derrota
void end_loss() {
    strcpy(message[0], "\e[0;31m   ╖ ╓ ╔═╗ ╖ ╓   ╥   ╔═╗ ╔═╕ ╔═╕ \e[0m");
    strcpy(message[1], "\e[0;31m   ╚╦╝ ║ ║ ║ ║   ║   ║ ║ ╚═╗ ╠╡  \e[0m");
    strcpy(message[2], "\e[0;31m    ╨  ╚═╝ ╚═╝   ╚═╛ ╚═╝ ╘═╝ ╚═╛ \e[0m");
    strcpy(message[3], "--- The secret word was '");
    strcat(message[3], secret_word);
    strcat(message[3], "' ---");
    strcpy(coded_word, "");
    strcpy(help, "");

    print_hangman();
}

//Imprime a forca
void print_hangman() {
    update_hangman();

    system("clear");
    printf("\t   ╔══╗         ");          print_guessed(strlen(guessed_letters));
    printf("\t   ║ %s                 \n", stickman.head_1);
    printf("\t   ║ %s       %s        \n", stickman.head_2, message[0]);
    printf("\t   ║ %s%s%s       %s    \n", stickman.left_arm, stickman.body, stickman.right_arm, message[1]);
    printf("\t   ║ %s %s       %s     \n", stickman.left_leg, stickman.right_leg, message[2]);
    printf("\t   ║           %s     %s\n", message[3], help);
    printf("\t╒══╩══╕        %s     %s\n", coded_word, warning);
    printf("\n");
}

//Desenha as partes do stickman
void update_hangman() {
    switch(errors) {
        case 1:
            strcpy(stickman.head_1, "╭┴╮");
            strcpy(stickman.head_2, "╰─╯");
            break;
        case 2:
            strcpy(stickman.head_2, "╰┬╯");
            strcpy(stickman.body, "│");
            break;
        case 3:
            strcpy(stickman.left_arm, "╱");
            break;
        case 4:
            strcpy(stickman.right_arm, "╲");
            break;
        case 5:
            strcpy(stickman.left_leg, "╱");
            break;
        case 6:
            strcpy(stickman.right_leg, "╲");
            break;
    }
}

//Imprime as letras que ja foram tentadas
void print_guessed(int length) {
    int i;

    printf("Guessed letters: %c", guessed_letters[0]);
    for(i = 1; i < length; i++)
        printf(" - %c", guessed_letters[i]);
    printf("\n");
}

//Recebe a tentativa do usuario de adivinhar a palavra inteira
void guess_word() {
    char guessing[30];

    strcpy(warning, "");
    strcpy(help, "");
    print_hangman();

    printf("\t🚩 Guess a word: ");
    scanf(" %s", guessing);
    strcpy(coded_word, guessing);

    errors = 7;
}