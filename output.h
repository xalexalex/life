#define X 40
#define Y 40
#define DELAY 200
#define ONEIN 5
#define ALIVE 'O'
#define DEAD ' '
#define MSG_EXIT 1

void cleanup(void);
void init(void);
void output(char field[Y][X]);
int process_input(void);
