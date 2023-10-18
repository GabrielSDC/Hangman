#define TOTAL_ERRORS 6
#define TOTAL_WORDS 100

typedef struct {
    char head_1[10];
    char head_2[10];
    char body[10];
    char left_arm[10];
    char right_arm[10];
    char left_leg[10];
    char right_leg[10];
} Hangman;

typedef enum {FINISH, CONTINUE, FOUND, NOT_FOUND, INVALID, TOO_LONG} Return_values;

void find_word();
int  generate_random();
void generate_coded_word(int);
void guess();
int  check_guess(int, char []);
void print_hangman();
void print_guessed(int);
void update_hangman();
void check_letter(char);
int  check_word();
void end_win();
int  check_errors();
void end_loss();
void guess_word();
