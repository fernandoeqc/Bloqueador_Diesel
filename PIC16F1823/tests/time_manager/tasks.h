#define TASK_LENGHT 10

struct taskData {
   unsigned int sec; 
   unsigned int count_sec;
   unsigned int command;
   unsigned int1 flag:1;
   unsigned int1 active:1;
};

typedef void (*function)(struct taskData *argumento);

struct taskFunc {
   struct taskData data;
   function func_time;
   function func_flag;
};


struct taskFunc *taskList[TASK_LENGHT];

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

void addTask (struct taskFunc *tk) {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_LENGHT; i++) {
      if (taskList[i] == NULL) {
         taskList[i] = tk;
         return;
      }
   }
}

static void runTk (struct taskFunc *tk) {
   struct taskFunc t_tk;
   t_tk = *tk;
   function func_time = t_tk.func_time; 

   if (t_tk.data.count_sec == t_tk.data.sec) {
      t_tk.data.count_sec = 0;
      func_time (&tk->data);
   }
   else {
      t_tk.data.count_sec++;
   }
   
   *tk = t_tk;
}

void runTasks () {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_LENGHT; i++) {
      if (taskList[i] == NULL) {
         return;
      }
       
      if (taskList[i]->data.active == FALSE) {
         continue;
      }

      runTk (taskList[i]);
   }
}



