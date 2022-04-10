/* 
    A simple library for simulating general PLC functionality.
*/
#include <string>
#define STRING_NAME_LEN 20

/* Variables and enums */
enum VariableType { BOOL, BYTE, INT, FLOAT };
enum GPIOMode { GPIO_INPUT, GPIO_OUTPUT };
enum TimerMode { ONDelay, OFFDelay };

typedef struct sVariable Variable;
typedef struct sGPIO GPIO;
typedef struct sPeripheryType PeripheryType;
typedef struct sPeripheryInstance PeripheryInstance;
typedef struct sLine Line;
typedef struct sLineReader LineReader;
typedef struct sInstruction Instruction;
typedef struct sCPU CPU;

/* Function pointers */
typedef int (*PeripheryType_Create)(CPU *cpu,Line *line, PeripheryType *p_type);
typedef int (*PeripheryType_Set)(CPU *cpu, Line *line, PeripheryInstance *p_instance);
typedef void (*PeripheryType_Loop)();
typedef int (*PeripheryInstance_Create)(const char[]);
typedef int (*PeripheryInstance_Set)(PeripheryInstance *periph_instance);
typedef int (*Instruction_Parse)(CPU *cpu,Line *line);
typedef int (*GetNextLine)(std::ifstream& src,  Line* curr_line);
typedef int (*Variable_Set)(CPU *cpu, Line *line);


typedef struct sVariable {
    char name[STRING_NAME_LEN];
    VariableType type;
    Variable_Set set;
    int ivalue;
    float fvalue;
} Variable;

typedef struct sGPIO{
    int pin;
    GPIOMode mode;
    Variable *var;
} GPIO;

typedef struct sTimer {
    bool EN = false;
    bool running = false;
    int presetTime; //delay length of timer
    int accumulatedTime=0; //how much has passed since timer began
    time_t lastMillis = 0; //time of last call
    int timerMode; //onDelay, offDelay, pulse
    int lastInputState=0;
    GPIO *input; //input pin
    GPIO *output; //output pin
}Timer;

typedef struct sESPNOW {
    //not implemented yet
}ESPNOW;

//SETUPDONE instruction f.e before it all config , after it the main program
typedef struct sPeripheryType {
    char name[STRING_NAME_LEN];
    PeripheryType_Create create;
    PeripheryType_Set set;
    PeripheryType_Loop loop;
    int update_freq;
} PeripheryType;

typedef struct sPeripheryInstance {
    char name[STRING_NAME_LEN];
    PeripheryType *type;
    void* peripheryData;
} PeripheryInstance;

typedef struct sLine{
    int line_number;
    char instruction[20];
    char args[10][15];
    int args_count;
} Line;

typedef struct sLineReader{
    char sourceName[20];
    int lineNumber;
    Line curr_line;
    GetNextLine get_next_line;
} LineReader;

typedef struct sInstruction {
    char name[20];
    Instruction_Parse instr;
} Instruction;

typedef struct sCPU {
    int PC;
    Variable vars[15];
    int variable_index=0;
    PeripheryType periph_types[15];
    PeripheryInstance periph_instances[15];
    int instance_index=0;
    Instruction instructions[15];
} CPU;








