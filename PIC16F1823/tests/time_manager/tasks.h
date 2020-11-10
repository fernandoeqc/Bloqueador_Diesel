#define TASK_WIDTH 10

typedef void (*funcao)(struct task *argumento);

struct task{
   unsigned int sec; 
   unsigned int count_sec;
   unsigned int command;
   unsigned int1 flag:1;
   unsigned int1 active:1;
   funcao func;
};

struct task *taskList[TASK_WIDTH];

void timeManager (struct task *tmp, funcao func) {
   int x;
   struct task b_tmp;
   b_tmp = *tmp;

   if (b_tmp.count_sec == b_tmp.sec) {
      b_tmp.count_sec = 0;

      if (func != NULL) {
         func(*tmp);
      }
   }
   b_tmp.count_sec++;

   *tmp = b_tmp;
}

void initTasks () {
   unsigned int8 i = 0;
   
   for (i = 0; i < TASK_WIDTH; i++) {
      taskList[i] = NULL;
   }
}

void addTask (struct task *tk) {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_WIDTH; i++) {
      if (taskList[i] == NULL) {
         taskList[i] = tk;
         return;
      }
   }
}

static void runTk (struct task *tk) {
   struct task t_tk = *tk;
   funcao func = t_tk.func; 

   if (t_tk.count_sec == t_tk.sec) {
      t_tk.count_sec = 0;
      func (tk);
      t_tk.flag = TRUE;
   }
   else {
      t_tk.count_sec++;
   }
   
   *tk = t_tk;
}

void runTasks () {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_WIDTH; i++) {
      if (taskList[i] == NULL) {
         return;
      }
       
      if (taskList[i]->active == FALSE) {
         continue;
      }

      runTk (taskList[i]);
   }
}


void ativaMotor(struct tasks *tk) { 
   struct task t_tk = *tk;

   if (t_tk.command == 'O') {
      output_low(MOTOR1);
      output_high(MOTOR2);
      t_tk.active = FALSE;
   }
   else if (t_tk.command == 'C') {
      output_low(MOTOR2);
      output_high(MOTOR1);
      t_tk.active =  FALSE;
   }
   else if (t_tk.command == 'W') {

   }

   *tk = t_tk;
}

void ativaBat(struct task *tk) {
   struct task t_tk = *tk;

   output_toggle(PIN_C5);

   *tk = t_tk;
}

