#define POK_NEEDS_TIMER 1
#define POK_NEEDS_THREADS 1

#define POK_NEEDS_PARTITIONS 1

#define POK_NEEDS_DEBUG 1
#define POK_NEEDS_CONSOLE 1

#define POK_NEEDS_LIBMATH 1
//#define POK_NEEDS_ZERO_DIVISION_EXCEPTION 1

/* Configuration from kernel starts*/
#define POK_NEEDS_THREAD_SLEEP 1
/* Configuration from kernel ends*/

#define POK_NEEDS_LIBC_STDLIB 1
#define POK_CONFIG_NEEDS_FUNC_MEMCPY         1
#define POK_CONFIG_NEEDS_FUNC_MEMSET         1
#define POK_CONFIG_NEEDS_FUNC_MEMCMP         1
#define POK_CONFIG_NEEDS_FUNC_STRCMP         1
#define POK_CONFIG_NEEDS_FUNC_STRNCMP        1
#define POK_CONFIG_NEEDS_FUNC_STRNCASECMP    1
#define POK_CONFIG_NEEDS_FUNC_STRCPY         1
#define POK_CONFIG_NEEDS_FUNC_STRNCPY        1
#define POK_CONFIG_NEEDS_FUNC_STRLEN         1
#define POK_CONFIG_NEEDS_FUNC_STREQ          1
#define POK_CONFIG_NEEDS_FUNC_ITOA           1
#define POK_CONFIG_NEEDS_FUNC_UDIVDI3        1

#define POK_NEEDS_MIDDLEWARE 1

#define POK_NEEDS_ARINC653_PARTITION 1
#define POK_NEEDS_ARINC653_PROCESS 1
#define POK_NEEDS_ARINC653_ERROR 1
#define POK_NEEDS_ARINC653_SAMPLING 1
#define POK_NEEDS_ARINC653_QUEUEING 1
#define POK_NEEDS_ARINC653_TIME 1

#define POK_CONFIG_OPTIMIZE_FOR_GENERATED_CODE 1

#define POK_NEEDS_BUFFERS 1
#define POK_NEEDS_ARINC653_BUFFER 1
#define POK_CONFIG_NB_BUFFERS pok_config_nb_buffers
#define POK_CONFIG_BUFFER_DATA_SIZE pok_config_buffer_data_size

#define POK_NEEDS_BLACKBOARDS 1
#define POK_NEEDS_ARINC653_BLACKBOARD 1
#define POK_CONFIG_NB_BLACKBOARDS pok_config_nb_blackboards
#define POK_CONFIG_BLACKBOARD_DATA_SIZE pok_config_blackboard_data_size

#define POK_NEEDS_SEMAPHORES 1
#define POK_NEEDS_ARINC653_SEMAPHORE 1
#define POK_CONFIG_ARINC653_NB_SEMAPHORES pok_config_arinc653_nb_semaphores

#define POK_NEEDS_EVENTS 1
#define POK_NEEDS_ARINC653_EVENT 1
#define POK_CONFIG_ARINC653_NB_EVENTS pok_config_arinc653_nb_events
#define POK_CONFIG_NB_EVENTS pok_config_nb_events

#define POK_CONFIG_NB_THREADS pok_config_nb_threads

extern unsigned pok_config_nb_buffers;
extern unsigned pok_config_buffer_data_size;
extern unsigned pok_config_nb_blackboards;
extern unsigned pok_config_blackboard_data_size;
extern unsigned pok_config_arinc653_nb_semaphores;
extern unsigned pok_config_arinc653_nb_events;
extern unsigned pok_config_nb_events;
extern unsigned pok_config_nb_threads;
