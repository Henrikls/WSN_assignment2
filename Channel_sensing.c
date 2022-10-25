#include "contiki.h"
#include "CC2420.h"
#include "sys/log.h"

#define LOG_MODULE "Sensing Log"
#define LOG_LEVEL LOG_LEVEL_DBG
//constants for amount of channels and samples
#define CNLS 16
#define SMPLS 500

PROCESS(main_process, "main_process");

AUTOSTART_PROCESSES(&main_process);

PROCESS_THREAD(main_process, ev, data)
{
    PROCESS_BEGIN();
    LOG_DBG("Init - main \n");

    //initialize the module
    cc2420_init();
    //make a list which holds the values for the different dbm measurements
    int list[CNLS];
    int dbm_sum;
    int i; 
    //Turn on radio
    cc2420_on();
    for(i = 11; i <= 26; i++){
        //select channel
        cc2420_set_channel(i);
        dbm_sum = 0;
        for(int j = 0; j < SMPLS; j++){
            //store measured dbm and sum it up 
            dbm_sum += cc2420_rssi();
        }
        //store the average in a list from 0 to 16
        list[i-11] = dbm_sum/SMPLS;
    }
    //turn off radio 
    cc2420_off();

    //find the best channel aka find highest number
    int best_dbm = list[0];
    int best_cnl = 0;
    for (i = 0; i < CNLS; i++)
    {
        if(list[i] > best_dbm)
        {
            best_dbm = list[i];
            best_cnl = 11 + i;
        }
        //print the averages and their associated channels
        printf("average = %d of channel = %d \n", list[i], i);
    }
    printf("Best average = %d of channel = %d \n", best_dbm, best_cnl);
    cc2420_set_channel(best_cnl);

    LOG_DBG("Main finish \n");
    PROCESS_END();
}
