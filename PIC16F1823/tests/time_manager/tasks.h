#define TASK_LENGHT 10

struct taskData {
   unsigned int8 sec; 
   unsigned int8 count_sec;
   unsigned int8 command;
   unsigned int8 state;
   unsigned int1 flag:1;
   unsigned int1 active:1;
};

typedef void (*function)(struct taskData *arg);
typedef void (*functionTest)(void);

struct taskFunc {

   struct taskData data;
   function func_time;
};


struct taskFunc *taskList[TASK_LENGHT];
struct taskData *taskTest[TASK_LENGHT];
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
} */

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

void addTest(struct taskFunc *tk) {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_LENGHT; i++) {
      if (taskTest[i] == NULL) {
         taskTest[i] = tk;
         return;
      }
   }
}

/* static void runTk (struct taskFunc *tk) {
   struct taskFunc t_tk;
   t_tk = *tk;
   function func = t_tk.func_time; 

   if (t_tk.data.count_sec == t_tk.data.sec) {
      t_tk.data.count_sec = 0;
      func (&(t_tk.data));
   }
   else {
      t_tk.data.count_sec++;
   }
   
   *tk = t_tk;
} */

static void runTs (struct taskFunc *tk) {
   struct taskFunc t_tk;
   t_tk = *tk;  
   function func = t_tk.func_time; 

   if (t_tk.data.count_sec == t_tk.data.sec) {
      t_tk.data.count_sec = 0;
      func (&(t_tk.data));
   }
   else {
      t_tk.data.count_sec++;
   }
   *tk = t_tk;
}

/* void runTasks () {
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
} */

void runTest () {
   unsigned int8 i = 0;

   for (i = 0; i < TASK_LENGHT; i++) {
      if (taskTest[i] == NULL) {
         return;
      }
       
      if (taskTest[i]->active == FALSE) {
         continue;
      }

      runTs (taskTest[i]);
   }
}

void abc (struct taskData *tk) {
   struct taskData t_tk;   
   struct taskData oso; 
   oso.active = 1;

   oso = *tk;
   int time = 0;
   
   if (time > MAX_UNANSWERED_TIME) {
      
      //bit_set (flags_control, 2);
      output_high(PIN_C5);  
   }

   if(t_tk.state > time) {
      //tempo foi resetado
      //bit_clear (flags_control, 2);
      output_low(PIN_C5);
   }
   
   t_tk.state = time;

   *tk = t_tk;
}