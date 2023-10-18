#include <stdio.h>
#include <stdlib.h>
#include "hangman.h"

//inicializa o jogo
void init() {
    find_word();

    while(1) {
        guess();

        if (check_word() == FINISH) {
            end_win();
            break;
        } 
        else if (check_errors() == FINISH) {
            end_loss();
            break;    
        }
    }
}

int main() {
    init();
    return 0;
}