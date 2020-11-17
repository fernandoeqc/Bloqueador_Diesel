#define TASK_LENGHT 10

static int8 number_task = 0;


typedef int1 (*function)(void);

struct taskFunc {
   unsigned int8 sec; 
   unsigned int8 count_sec;
   unsigned int1 active:1;
   unsigned int1 flag:1;
   function func;
}controlValve,
controlMotor,
endMotor;


struct taskFunc *taskList[TASK_LENGHT];
/* 
void timeManager (struct taskFunc *tmp, function func_time) {
   int x;
   struct taskFunc b_tmp;
   b_tmp = *tmp;

   if (b_tmp.data.count_sec == b_tmp.data.sec) {
      b_tmp.data.count_sec = 0;

      if (func_time != NULL) {
         func_time(&b_tmp.data);
      }
   }
   b_tmp.data.count_sec++;

   *tmp = b_tmp;
} 


void initTasks () {
   unsigned int8 i = 0;
   
   for (i = 0; i < TASK_LENGHT; i++) {
      taskList[i] = NULL;
   }
}
*/

void addTask (struct taskFunc *tk) {

   taskList[number_task] = tk;
   number_task++;
}

 
static void runTk (struct taskFunc *tk) {
   function func = tk->func; 

   if (tk->count_sec == tk->sec) {
      tk->count_sec = 0;
      tk->flag = func();
   }
   else {
      tk->count_sec++;
   }
}

void runTasks () {
   unsigned int8 i = 0;

   for (i = 0; i < number_task; i++) {
      if (taskList[i] == NULL) {
         return;
      }
       
      if (taskList[i]->active == FALSE) {
         continue;
      }

      runTk (taskList[i]);
   }
}
