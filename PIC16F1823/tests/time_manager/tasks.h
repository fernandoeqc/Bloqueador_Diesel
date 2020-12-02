#define TASK_LENGHT 5

static int8 number_task = 0;


typedef void (*function)();

struct taskFunc {
   unsigned int8 sec; 
   unsigned int8 count_sec;
   function func_time;
   unsigned int1 active:1;
}contaBloq, battery, timeReceive, uart, powerIn;


struct taskFunc *taskList[TASK_LENGHT];


void addTask (struct taskFunc *tk) {

   taskList[number_task] = tk;
   number_task++;
}

 
static void runTk (struct taskFunc *tk) {
   function func = tk->func_time; 

   if (tk->count_sec == tk->sec) {
      tk->count_sec = 0;
      func ();
   }
   else {
      tk->count_sec++;
   }
}

void runTasks () {
   unsigned int8 task_ct = 0;

   for (task_ct = 0; task_ct < number_task; task_ct++) {
       
      if (taskList[task_ct]->active == FALSE) {
         continue;
      }

      runTk (taskList[task_ct]);
   }
}
