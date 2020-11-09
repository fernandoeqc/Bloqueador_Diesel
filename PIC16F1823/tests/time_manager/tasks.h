#define TASK_WIDTH 10

typedef void (*funcao)(unsigned int argumento);

struct task{
   unsigned int seg; 
   unsigned int conta_seg;
   unsigned int comando;
   unsigned int1 flag:1;
   unsigned int1 ativo:1;
   funcao func;
};

struct task *taskList[TASK_WIDTH];

void timeManager (struct task *tmp, funcao func) {
   int x;
   struct task b_tmp;
   b_tmp = *tmp;

   if (b_tmp.conta_seg == b_tmp.seg) {
      b_tmp.conta_seg = 0;

      if (func != NULL) {
         func(b_tmp.comando);
      }
   }
   b_tmp.conta_seg++;

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
   funcao func = tk->func; 
   if (tk->conta_seg == tk->seg) {
      func (tk->comando);
      tk->conta_seg = 0;
   }
   tk->conta_seg++;
}

void runTasks () {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_WIDTH; i++) {
      if (taskList[i] == NULL) {
         return;
      }
      runTk (taskList[i]);
   }
}


void ativaMotor(int estado)
{  
   output_toggle(PIN_C4);
}

void ativaBat(int argumento)
{
   output_toggle(PIN_C5);
}

