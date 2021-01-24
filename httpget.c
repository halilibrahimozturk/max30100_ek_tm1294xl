
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// XDCtools Header files
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <xdc/std.h>

/* TI-RTOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Swi.h>
#include <ti/sysbios/knl/Queue.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Idle.h>
#include <ti/sysbios/knl/Mailbox.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/sysbios/knl/Semaphore.h>
#include <ti/drivers/GPIO.h>
#include <ti/net/http/httpcli.h>
#include <ti/drivers/I2C.h>

#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/sysctl.h"

#include "Board.h"

#include <sys/socket.h>
#include <arpa/inet.h>

#define SOCKETTEST_IP     "192.168.1.55"
#define TASKSTACKSIZE     4096
#define OUTGOING_PORT     5011
#define INCOMING_PORT     5030

extern Event_Handle event0;

extern Semaphore_Handle semaphore0;
extern Semaphore_Handle semaphore1;
extern Semaphore_Handle semaphore2;
extern Semaphore_Handle semaphore3;

extern Mailbox_Handle mailbox0;
extern Mailbox_Handle mailbox1;
extern Mailbox_Handle mailbox2;
extern Mailbox_Handle mailbox3;



char   tempstr[20];                     // temperature string

uint16_t ir_data[100];
uint16_t red_data[100];
uint16_t _max30100_ir_sample[16];
uint16_t _max30100_red_sample[16];

I2C_Handle      i2c;
I2C_Params      i2cParams;
I2C_Transaction i2cTransaction;
char   tempstr1[20];
char   tempstr2[20];


uint8_t ctr=0 ;


Void Timer_ISR(UArg arg1)
{
    Semaphore_post(semaphore2);
}

Void sleepTsk(UArg arg1)
{
    Event_pend(event0, Event_Id_00, Event_Id_NONE, BIOS_WAIT_FOREVER);

    Task_sleep(50);


}


Void SWI_ISR(UArg arg1)
{

    while(1){
        Semaphore_pend(semaphore2, BIOS_WAIT_FOREVER);

        Mailbox_post(mailbox1, &ctr, BIOS_NO_WAIT);


    }
}

Void agesTask(UArg arg1)
{

    while(1){
        uint16_t hr=0;
        char age[30];
        static uint8_t tempval2;
        Semaphore_pend(semaphore3, BIOS_WAIT_FOREVER);
        Mailbox_pend(mailbox2, &age, BIOS_WAIT_FOREVER);
        int x = atoi(age);
        Mailbox_pend(mailbox0, &tempval2, BIOS_WAIT_FOREVER);
        hr += tempval2;
        if(x<=2)
        {
            if(hr>=80 && hr<=130)
            {
                System_printf("You are %d years old and your heart-rate is: %d and in normal range.(80-130)\n",x,hr);
                System_flush();
            }else{
                System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(80-130)\n",x,hr);
                               System_flush();
            }
        }
        else  if(x<=4)
        {
            if(hr>=80 && hr<=120)
            {
                System_printf("You are %d years old and your heart-rate is: %d and in normal range.(80-120)\n",x,hr);
                System_flush();
            }else{
                System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(80-120)\n",x,hr);
                               System_flush();
            }
        }
        else  if(x<=6)
                {
                    if(hr>=75 && hr<=115)
                    {
                        System_printf("You are %d years old and your heart-rate is: %d and in normal range.(75-115)\n",x,hr);
                        System_flush();
                    }else{
                        System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(75-115)\n",x,hr);
                                       System_flush();
                    }
                }
        else  if(x<=8)
                        {
                            if(hr>=70 && hr<=110)
                            {
                                System_printf("You are %d years old and your heart-rate is: %d and in normal range.(70-110)\n",x,hr);
                                System_flush();
                            }else{
                                System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(70-110)\n",x,hr);
                                               System_flush();
                            }
                        }
 else  if(x<=12)
 {
     if(hr>=65 && hr<=105)
     {
         System_printf("You are %d years old and your heart-rate is: %d and in normal range.(65-105)\n",x,hr);
         System_flush();
     }else{
         System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(65-105)\n",x,hr);
         System_flush();
     }
 }
 else  if(x<=14)
 {
     if(hr>=60 && hr<=105)
     {
         System_printf("You are %d years old and your heart-rate is: %d and in normal range.(60-105)\n",x,hr);
         System_flush();
     }else{
         System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(60-105)\n",x,hr);
         System_flush();
     }
 }
 else  if(x<=16)
  {
      if(hr>=55 && hr<=100)
      {
          System_printf("You are %d years old and your heart-rate is: %d and in normal range.(55-100)\n",x,hr);
          System_flush();
      }else{
          System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(55-100)\n",x,hr);
          System_flush();
      }
  }else if(x<=18)
  {
      if(hr>=50 && hr<=95)
      {
          System_printf("You are %d years old and your heart-rate is: %d and in normal range.(50-95)\n",x,hr);
          System_flush();
      }else{
          System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(50-95)\n",x,hr);
          System_flush();
      }
  }
  else  if(x>=18)
    {
        if(hr>=60 && hr<=100)
        {
            System_printf("You are %d years old and your heart-rate is: %d and in normal range.(60-100)\n",x,hr);
            System_flush();
        }else{
            System_printf("You are %d years old and your heart-rate is: %d and not in normal range!(60-100)\n",x,hr);
            System_flush();
        }
    }
    }
}
/*
 *
 *  ======== printError ========
 */
void printError(char *errString, int code)
{
    System_printf("Error! code = %d, desc = %s\n", code, errString);
    BIOS_exit(code);
}

bool IIC_OpenComm(void)
{
    bool retval = false;

    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;                 // minimum speed first
    i2c = I2C_open(Board_I2C0, &i2cParams);
    if (i2c == NULL) {
        System_abort("Error Initializing I2C\n");   // error, retval is false
    }
    else {
        System_printf("I2C Initialized!\n");        // everything alright, retval is true
        retval = true;
    }
    System_flush();
    return retval;                                  // return true or false
}

void IIC_CloseComm(void)
{
    I2C_close(i2c);
    System_printf("IIC_CloseComm()   I2C closed!\n");
    System_flush();
}

void IIC_readFifo(int device_ID, uint16_t *ir_buf, uint16_t *red_buf ,char*noOfSamples)
{
        uint16_t        temperature;
        uint8_t         txBuffer[1];
        uint8_t         rxBuffer[4];
        int buf_counter = 0;
        char tmp_buf[0x10*4];
        uint8_t num_sample = 64;//(fifo_wr_ptr - fifo_rd_ptr) * 4;
        uint8_t fifo_data[64] = { 0 };
        int i = 0 ;

        /* Point to the T ambient register and read its 2 bytes */
        txBuffer[0] = 0x05;
        i2cTransaction.slaveAddress = 0x57;
        i2cTransaction.writeBuf = txBuffer;
        i2cTransaction.writeCount = 1;
        i2cTransaction.readBuf = fifo_data;
        i2cTransaction.readCount = num_sample;

        /* Take  samples and print them out onto the console */
        for(i=0; i<num_sample; i+=4){
            if (I2C_transfer(i2c, &i2cTransaction)) {
                _max30100_ir_sample[i / 4] = (fifo_data[i] << 8) | fifo_data[i + 1];
                _max30100_red_sample[i / 4] = (fifo_data[i + 2] << 8) | fifo_data[i + 3];
//                            System_printf("Loading...\n");
//                            System_flush();

//                            if(buf_counter>=100) {
//                                            quit_loop = true;
//                                            break;
//                                        }
             }
            else {
                System_printf("I2C Bus fault\n");
            }
            System_flush();
//            Task_sleep(50);
            Event_post(event0, Event_Id_00); // post Event_Id_00
        }
    }


bool IIC_writeReg(int device_ID, int addr, uint8_t val)
{
    uint8_t txBuffer[2];
    uint8_t rxBuffer[2];
    bool retval=false;

    // place parameters
    txBuffer[0] = addr;                             // register address
    txBuffer[1] = val;                              // value to be written
    i2cTransaction.slaveAddress = device_ID;        // device IIC ID
    i2cTransaction.writeBuf = txBuffer;             // buffer that holds the values to be written
    i2cTransaction.writeCount = 2;                  // 2 bytes will be sent
    i2cTransaction.readBuf = rxBuffer;              // receive buffer (in this case it is not used)
    i2cTransaction.readCount = 0;                   // no bytes to be received

    if (I2C_transfer(i2c, &i2cTransaction)) {       // start the transaction
//        System_printf("IIC_writeReg(%d,%d)\n", addr, val);
        retval = true;                              // true will be returned to indicate that transaction is successful
    }
    else {
        System_printf("I2C Bus fault\n");           // there is an error, returns false
    }
    System_flush();

    return retval;
}

bool IIC_readReg(int device_ID, int addr, int no_of_bytes, char *buf)
{
    uint8_t txBuffer[2];
    bool retval=false;

    // addr: register number
    txBuffer[0] = addr;                             // 1 byte: register address
    i2cTransaction.slaveAddress = device_ID;        // Device Id
    i2cTransaction.writeBuf = txBuffer;             // buffer to be sent
    i2cTransaction.writeCount = 1;                  // send just register address
    i2cTransaction.readBuf = buf;                   // read into this buffer
    i2cTransaction.readCount = no_of_bytes;         // number of bytes to be read


    if (I2C_transfer(i2c, &i2cTransaction)) {
        //System_printf("IIC_readReg(%d,%d)\n", addr, buf[0]);
        retval=true;
    }
    else {
        System_printf("I2C Bus fault\n");
    }
    System_flush();

    return retval;
}


Void taskFxn(UArg arg0, UArg arg1)
{    Semaphore_pend(semaphore1, BIOS_WAIT_FOREVER);

        char buf[10];
        char buf1[10];
        char buf2[10];
        char noOfSamples[10];
        char data[10];
        uint8_t j;

        int heartRate;
        int count2 = 0;
        int pulse = 0;
        int count1 = 0;
        int sample1,sample2 = 0;
        int sample3 = 0;
        int input,sample4=0;
        int HR;
        char mode;
        int heartBeat = 0;
        int dcFiltered, filteredPrevious,previousHeartRate = 0;
        int previousBw,bwFiltered = 0;

        IIC_OpenComm();

        //get Part ID
        IIC_readReg(0x57, 0xFF, 1, buf);
        System_printf("Register 0xFF (PART_ID) = 0x%x\n", buf[0]);
        System_flush();

        //reset interrupts

        IIC_writeReg(0x57, 0x01,0x00);

        //reset mode

        IIC_writeReg(0x57, 0x06, 0x00);

        //turn off leds

        IIC_writeReg(0x57, 0x09,buf[0]);

        //clear FIFO registers

        IIC_writeReg(0x57, 0x02,0x00);
        IIC_writeReg(0x57, 0x04,0x00);
        IIC_writeReg(0x57, 0x03,0x00);

        //set sampling rate

        IIC_readReg(0x57, 0x07, 1, buf);
        buf[0]=(buf[0] & 0xe3) | (0x01 << 2); //100Hz sample rate  is 0x01
        IIC_writeReg(0x57, 0x07,buf[0]);

        //led pulse width

        IIC_readReg(0x57, 0x07, 1, buf);
        buf[0]=(buf[0] & 0xfc) | 0x03;   //    1600US_16BITS   = 0x03
        IIC_writeReg(0x57, 0x07,buf[0]);

        //set leds current
        buf[0]=0x0f << 4 | 0x0f;
       IIC_writeReg(0x57, 0x09,buf[0]);

        //set mode
        buf[0]= 0x02   ;
        IIC_writeReg(0x57, 0x06, buf[0]);

        //enable interrupts

        IIC_writeReg(0x57, 0x01,0xF0); // 1 0 0 0
            while(1) {
                IIC_readReg(0x57, 0x02, 4, buf1);
                IIC_readReg(0x57, 0x04, 4, buf2);
                noOfSamples[0]=buf1[0]-buf2[0];
//                System_printf("Number of Samples : %d\n", buf[0]);
//                System_flush();
                IIC_readFifo(0x57, ir_data, red_data,noOfSamples);
                for(j = 0; j< 16; j++){
                sprintf(data, "s:%d\n", _max30100_ir_sample[j]);
              input=  _max30100_ir_sample[j];
              // DC FILTERING
                      dcFiltered = input + (0.75 * filteredPrevious);
                      input = dcFiltered - filteredPrevious;
                      filteredPrevious = dcFiltered;
                      heartRate = input;
                      heartRate = (sample1 + sample2 + sample3 + sample4 + input)/5;
                      sample1 = sample2;
                      sample2 = sample3;
                      sample3 = sample4;
                      sample4 = input;
            //BUTTERWORTH FILTERING
                      bwFiltered = (2.452372752527856026e-1 * heartRate) + (0.50952544949442879485 *previousBw);
                      previousBw = bwFiltered;
                      heartRate = bwFiltered;
             //HEART BEATS
                      if(heartRate > previousHeartRate & heartBeat == 0){
                          heartBeat = 1;
                      }
                      if(heartRate <= previousHeartRate-20 & heartBeat == 1){
                          pulse =count1;
                          heartBeat = 0;
                          count1 = 0;
                      }
                      if(heartRate >= 50& heartBeat == 1){
                          System_printf("Heart Beats ! \n");
                          System_flush();
                      }
//                      if(heartRate <= 10){
//                          System_printf("No Heart Beats ! \n");
//                          System_flush();
//                      }
                      if (count2==20){
//                          System_printf("HR = %d\n", HR);
//                          System_flush();
                          count2=0;
                      }
                      count1++;
                      count2++;
                      previousHeartRate = heartRate;
                      HR = 1200 / pulse;

                      System_printf("Heart-Rate is : %d\n", HR);
                      System_flush();
                      Mailbox_post(mailbox0, &HR, BIOS_NO_WAIT);
                      Mailbox_post(mailbox3, &HR, BIOS_NO_WAIT);
                      Task_sleep(50);
                        }
        }
        IIC_CloseComm();

}

bool sendData2Server(char *serverIP, int serverPort, char *data, int size)
{
    int sockfd, connStat, numSend;
    bool retval=false;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == -1) {
        System_printf("Socket not created");
        close(sockfd);
        return false;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));  // clear serverAddr structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);     // convert port # to network order
    inet_pton(AF_INET, serverIP, &(serverAddr.sin_addr));

    connStat = connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(connStat < 0) {
        System_printf("sendData2Server::Error while connecting to server\n");
    }
    else {
        numSend = send(sockfd, data, size, 0);       // send data to the server
        if(numSend < 0) {
            System_printf("sendData2Server::Error while sending data to server\n");
        }
        else {
            retval = true;      // we successfully sent the temperature string
        }
    }
    System_flush();
    close(sockfd);
    return retval;
}

Void clientSocketTask(UArg arg0, UArg arg1)
{
    while(1) {
        // wait for the semaphore that httpTask() will signal
        // when temperature string is retrieved from api.openweathermap.org site
        //
        Semaphore_pend(semaphore0, BIOS_WAIT_FOREVER);

        GPIO_write(Board_LED0, 1); // turn on the LED

        // connect to SocketTest program on the system with given IP/port
        // send hello message whihc has a length of 5.
        //
        if(sendData2Server(SOCKETTEST_IP, OUTGOING_PORT, tempstr, strlen(tempstr))) {
            System_printf("clientSocketTask:: Temperature is sent to the server\n");
            System_flush();
        }

        GPIO_write(Board_LED0, 0);  // turn off the LED
    }
}

void getTimeStr(char *str)
{
    // dummy get time as string function
    // you may need to replace the time by getting time from NTP servers
    //
    strcpy(str, "2021-01-07 12:34:56");
}

float getTemperature(void)
{
    // dummy return
    //
    return atof(tempstr);
}

Void serverSocketTask(UArg arg0, UArg arg1)
{
    static uint8_t tempval2, tempAverage2;

    float newTemp;

    int serverfd, new_socket, valread, len;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[30];
    char outstr[30], tmpstr[30];
    bool quit_protocol;

    int age_comes =0;

    char *s1, *s2;

    serverfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverfd == -1) {
        System_printf("serverSocketTask::Socket not created.. quiting the task.\n");
        return;     // we just quit the tasks. nothing else to do.
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(INCOMING_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Attaching socket to the port
    //
    if (bind(serverfd, (struct sockaddr *)&serverAddr,  sizeof(serverAddr))<0) {
         System_printf("serverSocketTask::bind failed..\n");

         // nothing else to do, since without bind nothing else works
         // we need to terminate the task
         return;
    }
    if (listen(serverfd, 3) < 0) {

        System_printf("serverSocketTask::listen() failed\n");
        // nothing else to do, since without bind nothing else works
        // we need to terminate the task
        return;
    }

    while(1) {

        len = sizeof(clientAddr);
        if ((new_socket = accept(serverfd, (struct sockaddr *)&clientAddr, &len))<0) {
            System_printf("serverSocketTask::accept() failed\n");
            continue;               // get back to the beginning of the while loop
        }

        System_printf("Accepted connection\n"); // IP address is in clientAddr.sin_addr
        System_flush();

        // task while loop
        //
        quit_protocol = false;
        do {

            // let's receive data string
            if((valread = recv(new_socket, buffer, 10, 0))<0) {

                // there is an error. Let's terminate the connection and get out of the loop
                //
                close(new_socket);
                break;
            }

            // let's truncate the received string
            //
            buffer[10]=0;
            if(valread<10) buffer[valread]=0;

            System_printf("Message received: %s\n", buffer);

            if(!strcmp(buffer, "HELLO")) {
                strcpy(outstr,"GREETINGS 200\n");
                send(new_socket , outstr , strlen(outstr) , 0);
                System_printf("Server <-- GREETINGS 200\n");
            }
            else if(!strcmp(buffer, "GETTIME")) {
                uint8_t myTime;
                float floatTime;
                getTimeStr(tmpstr);
                Semaphore_post(semaphore2);
                Mailbox_pend(mailbox1, &myTime, BIOS_WAIT_FOREVER);
                floatTime = (float)(myTime);
                sprintf(outstr, "OK %5.2f\n", floatTime);
                send(new_socket , outstr , strlen(outstr) , 0);
            }
            else if(!strcmp(buffer, "HEARTRATE")) {
                uint16_t tot2=0;
                int i;
                for (i = 0; i < 10; i++) {
                    Semaphore_post(semaphore1);
                    Mailbox_pend(mailbox0, &tempval2, BIOS_WAIT_FOREVER);

                    tot2 += tempval2;

                    }
                 tempAverage2 = tot2 /10;
                 newTemp = (float)(tempAverage2);

                sprintf(outstr, "OK %5.2f\n", newTemp);
                send(new_socket , outstr , strlen(outstr) , 0);
            }
            else if(!strcmp(buffer, "SHUTDOWN")) {
                quit_protocol = true;     // it will allow us to get out of while loop
                strcpy(outstr, " SYSTEM CLOSING");
                send(new_socket , outstr , strlen(outstr) , 0);
            }
            else if(!strcmp(buffer, "AGE")) {
                                 strcpy(outstr,"Please , enter your age : \n");
                                 send(new_socket , outstr , strlen(outstr) , 0);
                                 age_comes=1;
                        }


            else if( age_comes ) {
//                System_printf("AGE : : : :: : : %s n" , buffer);
//                      System_flush();
                uint16_t tot2=0;
                int i;
                for (i = 0; i < 10; i++) {
                    Semaphore_post(semaphore1);
                    Mailbox_pend(mailbox0, &tempval2, BIOS_WAIT_FOREVER);
                    Semaphore_post(semaphore3);
                    Mailbox_post(mailbox2, &buffer, BIOS_NO_WAIT);
                    tot2 += tempval2;
                }
                tempAverage2 = tot2 /10;
                newTemp = (float)(tempAverage2);
                sprintf(outstr, "OK %5.2f\n", newTemp);
                send(new_socket , outstr , strlen(outstr) , 0);
                age_comes = 0;
            }
        }
        while(!quit_protocol);
        System_flush();
        close(new_socket);
        BIOS_exit(1);
    }
    close(serverfd);
    return;
}

bool createTasks(void)
{
    static Task_Handle taskHandle1, taskHandle2, taskHandle3, taskHandle4,taskHandle5;
    Task_Params taskParams;
    Error_Block eb;

    Error_init(&eb);



    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 1;
    taskHandle2 = Task_create((Task_FuncPtr)clientSocketTask, &taskParams, &eb);

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 1;
    taskHandle3 = Task_create((Task_FuncPtr)serverSocketTask, &taskParams, &eb);

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 1;
    taskHandle4 = Task_create((Task_FuncPtr)taskFxn, &taskParams, &eb);

    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 1;
    taskHandle5 = Task_create((Task_FuncPtr)agesTask, &taskParams, &eb);

    Task_Params_init(&taskParams);
       taskParams.stackSize = TASKSTACKSIZE;
       taskParams.priority = 1;
       taskHandle5 = Task_create((Task_FuncPtr)sleepTsk, &taskParams, &eb);


       Task_Params_init(&taskParams);
               taskParams.stackSize = TASKSTACKSIZE;
               taskParams.priority = 1;
               taskHandle1 = Task_create((Task_FuncPtr)SWI_ISR, &taskParams, &eb);

    if (taskHandle1 == NULL ||  taskHandle2 == NULL || taskHandle3 == NULL || taskHandle4 == NULL) {
        printError("netIPAddrHook: Failed to create HTTP, Socket and Server Tasks\n", -1);
        return false;
    }

    return true;
}
//  This function is called when IP Addr is added or deleted
//
void netIPAddrHook(unsigned int IPAddr, unsigned int IfIdx, unsigned int fAdd)
{
    // Create a HTTP task when the IP address is added
    if (fAdd) {
        createTasks();
    }
}

int main(void)
{
    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initEMAC();
    Board_initI2C();

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the HTTP GET example\nSystem provider is set to "
            "SysMin. Halt the target to view any SysMin contents in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();


    /* Start BIOS */
    BIOS_start();

    return (0);
}
