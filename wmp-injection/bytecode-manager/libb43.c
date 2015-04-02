#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include "libb43.h"
#include "hex2int.h"
#include "dataParser.h"
#include "vars.h"
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */


void init_file(struct debugfs_file * df){

      char debugfs_path[256]="";
      char path[256]="";
      __debugfs_find(debugfs_path);

	sprintf(path,"%s%s",debugfs_path,"/mmio32read");
	df->f_mmio32read=fopen(path, "r+");

	sprintf(path,"%s%s",debugfs_path,"/mmio32write");
	df->f_mmio32write=fopen(path, "w");

	sprintf(path,"%s%s",debugfs_path,"/mmio16read");
	df->f_mmio16read=fopen(path, "r+");

	sprintf(path,"%s%s",debugfs_path,"/mmio16write");
	df->f_mmio16write=fopen(path, "w");

	sprintf(path,"%s%s",debugfs_path,"/shm16read");
	df->f_shm16read=fopen(path, "r+");
	
	sprintf(path,"%s%s",debugfs_path,"/shm16write");
	df->f_shm16write=fopen(path, "w");
	
	sprintf(path,"%s%s",debugfs_path,"/shm32read");
	df->f_shm32read=fopen(path, "r+");
	
	sprintf(path,"%s%s",debugfs_path,"/shm32write");
	df->f_shm32write=fopen(path, "w");
}

void close_file( struct debugfs_file * df){
  fclose(df->f_shm32read); 
  fclose(df->f_shm32write); 
  fclose(df->f_shm16read); 
  fclose(df->f_shm16write);
  fclose(df->f_mmio16read);
  fclose(df->f_mmio16write);
  fclose(df->f_mmio32read);
  fclose(df->f_mmio32write);
}





int read16(struct debugfs_file * df, int reg){

	/* """Do a 16bit MMIO read""" */
	char buffer[256];
	unsigned int ret=0;
	
	rewind (df->f_mmio16read);
	fprintf (df->f_mmio16read, "0x%X",reg);
	fflush(df->f_mmio16read);
	rewind (df->f_mmio16read);
	fscanf (df->f_mmio16read, "%s", buffer);
	ret=htoi(buffer);
	
	return ret;
}




void maskSet16(struct debugfs_file * df, int reg, int mask, int set){
	/* Do a 16bit MMIO mask-and-set operation */
	//printf("set = 0x%04X \t reg=%d\n",set,reg);
	mask &= 0xFFFF;
	set &= 0xFFFF;
	//printf("set & 0xFFFF = 0x%04X\n",set);

	rewind (df->f_mmio16write);
	fprintf (df->f_mmio16write, "0x%X 0x%X 0x%X",reg,mask,set);
	fflush(df->f_mmio16write);
}
void write16(struct debugfs_file * df, int reg, int value){
	/* Do a 16bit MMIO write */
	//printf("value = 0x%04X\n",value);
	maskSet16(df,reg, 0, value);

}	




void maskSet32(struct debugfs_file * df, int reg, int mask, int set){
	/* Do a 32bit MMIO mask-and-set operation */
	//printf("reg : 0x%08X, mask : 0x%08X, set : 0x%08X\n",reg,mask,set);
	mask &= 0xFFFFFFFF;
	set &= 0xFFFFFFFF;
	//printf("reg : 0x%08X, mask : 0x%08X, set : 0x%08X\n",reg,mask,set);
	
	rewind (df->f_mmio32write);
	fprintf (df->f_mmio32write, "0x%X 0x%X 0x%X",reg,mask,set);
	fflush(df->f_mmio32write);
	
}
void write32(struct debugfs_file * df, int reg, int value){
	/* Do a 16bit MMIO write */
	//printf("value = 0x%04X\n",value);
	maskSet32(df,reg, 0, value);

}	





void shmMaskSet16(struct debugfs_file * df, int routing, int offset, int mask, int set){
	mask &= 0xFFFF;
	set &= 0xFFFF;
	rewind (df->f_shm16write);
	fprintf (df->f_shm16write, "0x%X 0x%X 0x%X 0x%X",routing, offset,mask,set);
	fflush(df->f_shm16write);
}
void shmWrite16(struct debugfs_file * df, int routing, int offset, int value){
	/*"""Do a 16bit SHM write"""*/
	//printf("shmWrite16 --- value = %X\n",value);
	shmMaskSet16(df,routing,offset,0,value);
}

void shmRead16_char(struct debugfs_file * df, int routing, int offset, char * buffer){
	rewind (df->f_shm16read);
	fprintf (df->f_shm16read, "0x%X 0x%X",routing,offset);
	fflush(df->f_shm16read);
	rewind (df->f_shm16read);
	fscanf (df->f_shm16read, "%s", buffer);
}

unsigned int shmRead16(struct debugfs_file * df, int routing, int offset ){
	char buffer[256];
	unsigned int int_buffer=0;
	
	rewind (df->f_shm16read);
	fprintf (df->f_shm16read, "0x%X 0x%X",routing,offset);
	fflush(df->f_shm16read);
	rewind (df->f_shm16read);
	fscanf (df->f_shm16read, "%s", buffer);
	
	
	//int_buffer = *((int *)buffer);
	//printf("buffer=%s \t int_buffer = %X\n",buffer,int_buffer);
	int_buffer=htoi(buffer);
	
	//printf("buffer=%s \t int_buffer = %X\n",buffer,int_buffer);
	return int_buffer;
}

void shmRead32(struct debugfs_file * df, int routing, int offset, char * buffer){
	rewind (df->f_shm32read);
	fprintf (df->f_shm32read, "0x%X 0x%X",routing,offset);
	fflush(df->f_shm32read);
	rewind (df->f_shm32read);
	fscanf (df->f_shm32read, "%s", buffer);
}

unsigned int shmRead32_int(struct debugfs_file * df, int routing, int offset){
	unsigned int buffer_int=0;
	char buffer[256];
	
	rewind (df->f_shm32read);
	fprintf (df->f_shm32read, "0x%X 0x%X",routing,offset);
	fflush(df->f_shm32read);

	rewind (df->f_shm32read);
	fscanf (df->f_shm32read, "%s", buffer);
	
	buffer_int=htoi(buffer);
	//printf("buffer=%s \t int_buffer = %X\n",buffer,buffer_int);
	return buffer_int;
}


void shmWrite32(struct debugfs_file * df, int routing, int offset, int value){
/* Do a 32bit SHM write */
	shmMaskSet32(df, routing, offset, 0, value);
}

void shmMaskSet32(struct debugfs_file * df, int routing, int offset, int mask, int set){
	/* Do a 32bit SHM mask-and-set operation */
	mask &=0xFFFFFFFF;
	set &=0xFFFFFFFF;
	rewind (df->f_shm32write);
	fprintf (df->f_shm32write, "0x%X 0x%X 0x%X 0x%X",routing, offset, mask, set);
	fflush(df->f_shm32write);
}

void __debugfs_find(char *path){
	FILE * mtab;
	char * ret;
	char * tmp_path="/sys/kernel/debug/b43";
	DIR *dp;
	struct dirent *ep;
	dp = opendir (tmp_path);
	
	char * phy="";
	
	if (dp != NULL) {
		while (ep = readdir (dp)){
			char dev[3];
			memcpy(dev, &ep->d_name[0], 3 );
			dev[3] = '\0';
			if (!strcmp(dev,"phy")){
				phy=ep->d_name;
				break;
			}
		}
 		sprintf( path, "%s/%s",tmp_path,phy );
		(void) closedir (dp);
		
	}
	else{
		perror ("Couldn't open the directory");
		exit(1);
	}
}

void shmSharedRead(struct debugfs_file * df){
  
  char * ret="";
   
  char buffer[256]="";
  char tmp_buff[256]="";  
  int offset;
  printf("Shared memory:\n");
  for (offset=0; offset < 4096; offset+=4){
  //for (offset=0; offset < 512; offset+=4){

    //unsigned int buffer_int = shmRead32_int(df, B43_SHM_SHARED, offset);

    if (offset%16==0){
      printf("\n");
      printf("0x%04X:\t", offset);
    }  

    shmRead32( df, B43_SHM_SHARED, offset, buffer);
    
    sprintf(tmp_buff,"%c%c%c%c", buffer[8], buffer[9],buffer[6],buffer[7]);
    printf("%s ", tmp_buff);

    sprintf(tmp_buff,"%c%c%c%c", buffer[4], buffer[5],buffer[2],buffer[3]);
    printf("%s ", tmp_buff);

  }

  printf("\n");

}


void getGprs(struct debugfs_file * df){
	/* Returns an array of 64 ints. One for each General Purpose register. */
	unsigned int reg=0;
	int i=0;
	printf("General purpose registers:\n");

	for (i=0;i<64;i++){
		reg = shmRead16(df,B43_SHM_REGS, i);
		printf("r%02u: %04X  ",i,reg);
		if ((i-3)%4==0)
			printf("\n");
	}
}

void getLinkRegs(struct debugfs_file * df){
	unsigned int reg=0;
	int i=0;
	int tmp_val=0;
	printf("Link registers:\n");
	for (i=0;i<4;i++){
		reg = read16(df, 0x4D0 + (i * 2));
		printf("lr%u: %04X  ",i,reg);
		
	}
	printf("\n");
}
void getOffsetRegs(struct debugfs_file * df){
/*"""Returns an array of 7 ints. One for each Offset Register."""*/
	unsigned int reg=0;
	int i;
	printf("Offset registers:\n");
	for (i=0;i<7;i++){
		reg = read16(df, 0x4C0 + (i * 2));
		printf("off%u: %04X  ",i,reg);
		if ((i-3)%4==0)
		  printf("\n");
	}
	printf("\n");

}


void shmReadStateTime(struct debugfs_file * df,  char * file_name){
  
 	char buffer[80];
	char time_stamp_char[128][32];  
	char state_num_char[128][32];
	char exit_transition_char[128][32];
	int offset;
	int enable_store=0;
	long int time_stamp_1=1;  
	long int state_num_1=0;
	long int exit_transition_1=0;
	long int time_stamp_2=1;  
	long int state_num_2=0;
	long int exit_transition_2=0;
	long int state_time=0;
	long int last_time_stamp=0;
  
	time_t rawtime;
	struct tm * timeinfo;
  	int i,j;

	FILE * log_state_time;
	log_state_time = fopen(file_name, "w+");
	
	//printf("name file %s\n",file_name);
	
	for(j=0; j<300; j++){
		printf("%d\n", j);
		  
		#define REGION_DEBUG_START	3072	
		#define REGION_DEBUG_STOP 	4048
		i=0;
		for (offset=REGION_DEBUG_START; offset < REGION_DEBUG_STOP; offset+=4){
		  
		      shmRead32( df, B43_SHM_SHARED, offset, buffer);
		      sprintf(time_stamp_char[i],"%c%c%c%c%c%c%c%c",buffer[6],buffer[7],buffer[8],buffer[9],buffer[2],buffer[3],buffer[4],buffer[5]);
		      //printf("%s - %ld \n", time_stamp_char[i], strtol(time_stamp_char[i], NULL, 16));
		  
		      offset+=4;
		  		  
		      shmRead32( df, B43_SHM_SHARED, offset, buffer);
		      sprintf(state_num_char[i],"%c%c%c%c",buffer[6],buffer[7],buffer[8],buffer[9]);
		      //printf("%s ", state_num_char);
		      sprintf(exit_transition_char[i],"%c%c",buffer[4],buffer[5]);
		      //printf("%s ", exit_transition_char);
		      
		      i++;
		}
		
		i=0;
		state_num_1 = strtol(state_num_char[i], NULL, 16);
		exit_transition_1 = strtol(exit_transition_char[i], NULL, 16);
		time_stamp_1 = strtol(time_stamp_char[i], NULL, 16);
		i++;      
		
		for (offset=(REGION_DEBUG_START+8); offset < REGION_DEBUG_STOP; offset+=4){    
		      //printf("\n");
		      //printf("%d - 0x%04X:\t", i, offset);
		  
		      state_num_2 = strtol(state_num_char[i], NULL, 16);
		      //printf("%ld \t", state_num);

		      exit_transition_2 = strtol(exit_transition_char[i], NULL, 16);
		      //printf("%ld \t", exit_transition);

		      offset+=4;

		      time_stamp_2 = strtol(time_stamp_char[i], NULL, 16);
		      //printf("%ld \t\n", time_stamp);

		      if( time_stamp_1 > last_time_stamp && time_stamp_2 > time_stamp_1) {
			  last_time_stamp = time_stamp_1;
			  state_time = time_stamp_2 - time_stamp_1;
			  printf("%d - %d - 0x%04X:\t", j, i, offset);
		  
			  time(&rawtime);
			  timeinfo = localtime(&rawtime);
			  //2014 01 26 11 17 15
			  strftime (buffer,80,"%G%m%d%H%M%S",timeinfo);	
			  //printf("%s \t%ld\t %ld\t %ld\t %ld\t %ld\n", buffer, state_num_1, exit_transition_1, time_stamp_1, time_stamp_2, state_time);
			  printf("%s \t%ld\t %ld\t %ld\t %ld\n", buffer, state_num_1, exit_transition_1, time_stamp_2, state_time);
			  fprintf(log_state_time, "%s,%ld,%ld,%ld\n", buffer, state_num_1, exit_transition_1, state_time);
			  fflush(log_state_time);	      
		      }
		      time_stamp_1 = time_stamp_2;
		      state_num_1 = state_num_2;
		      exit_transition_1 = exit_transition_2;
		      i++;
		      
		}
			
		sleep(1);
	}
	
	fclose(log_state_time);
	printf("\n");

}



void shmReadActivateTime(struct debugfs_file * df,  char * file_name){
  
 	char buffer[80];
	char time_stamp_char[128][32];  
	char state_num_char[128][32];
	char exit_transition_char[128][32];
	int offset;
	int enable_store=1;
	long int time_stamp_1=1;  
	long int time_stamp_2=1;  
	long int state_num=0;
	long int state_num_1=0;
	long int state_num_2=0;
	long int state_time=0;
	long int exit_transition=0;
	long int last_time_stamp=0;
  
	time_t rawtime;
	struct tm * timeinfo;
  	int i,j;

	FILE * log_state_time;
	log_state_time = fopen(file_name, "a+");
	
	//printf("inside name file %s\n",file_name);


	#define REGION_DEBUG_START_1	3408	
	#define REGION_DEBUG_STOP_1 	3936

	
	//for(j=0; j<30; j++){
		printf("%d\n", j);
		  
		i=0;
		for (offset=REGION_DEBUG_START_1; offset < REGION_DEBUG_STOP_1; offset+=4){
		      
		      shmRead32( df, B43_SHM_SHARED, offset, buffer);
		      sprintf(time_stamp_char[i],"%c%c%c%c%c%c%c%c",buffer[6],buffer[7],buffer[8],buffer[9],buffer[2],buffer[3],buffer[4],buffer[5]);
		      printf("%s - %ld \t", time_stamp_char[i], strtol(time_stamp_char[i], NULL, 16));      
		      offset+=4;
		      
		      shmRead32( df, B43_SHM_SHARED, offset, buffer);
		      sprintf(state_num_char[i],"%c%c%c%c",buffer[6],buffer[7],buffer[8],buffer[9]);
		      printf("%s - %ld\n", state_num_char[i], strtol(state_num_char[i], NULL, 16));
		      //sprintf(exit_transition_char[i],"%c%c",buffer[4],buffer[5]);
		      //printf("%s ", exit_transition_char);
		      //printf("\n");		      
		      i++;
		}
		
		i=0;
		for (offset=REGION_DEBUG_START_1; offset < REGION_DEBUG_STOP_1; offset+=4){    
		      //printf("\n");
		      //printf("%d - 0x%04X:\t", i, offset);
		  
		      state_num_1 = strtol(state_num_char[i], NULL, 16);
		      offset+=4;
		      time_stamp_1 = strtol(time_stamp_char[i], NULL, 16);
		      printf("	%ld \t", time_stamp_1);
		      
		      offset+=4;
		      i++;

		      state_num_2 = strtol(state_num_char[i], NULL, 16);
		      offset+=4;
		      time_stamp_2 = strtol(time_stamp_char[i], NULL, 16);
		      printf("	%ld \t", time_stamp_2);
		      
		      printf(" = %ld \n", time_stamp_2-time_stamp_1);

		      if( time_stamp_1 > last_time_stamp) {
			      
			      last_time_stamp = time_stamp_2;
			      state_time = time_stamp_2 - time_stamp_1;
			  
			      if(state_time>0){
					time(&rawtime);
					timeinfo = localtime(&rawtime);
					//2014 01 26 11 17 15
					strftime (buffer,80,"%G%m%d%H%M%S",timeinfo);	
					printf("%d - %d - 0x%04X:\t", j, i, offset);
					//printf("%s \t%ld\t %d\t %ld\t %ld\t %ld\n", buffer, 0, 0, time_stamp_1, time_stamp_2, state_time);
					fprintf(log_state_time, "%s,%ld,%d,%ld\n", buffer, state_num_2, 0, state_time);
					fflush(log_state_time);
			      }
		      }
			 
		      i++;
		      
		}
		
	//	sleep(10);
	//}
	
		
	
	fclose(log_state_time);
	
}




void shmReadZigbeeRx(struct debugfs_file * df,  char * file_name){
  
 	char buffer[80];
	char rx_code[500][2];  
	char rx_seq[500][2];  
	char rx_value[500][4];
	long int busy_value[500];
	long int code_int[500];
	long int seq_int[500];
	
	int offset;
	
	time_t rawtime;
	struct tm * timeinfo;
  	int i,j;

	printf("name file %s\n",file_name);

	FILE * log_zigbee_rx;
	log_zigbee_rx = fopen(file_name, "w+");
	
	for(j=0; j<300; j++){
		printf("%d\n", j);
		  
		#define REGION_DEBUG_START_3	3400	
		#define REGION_DEBUG_STOP_3 	3940
		i=0;
		for (offset=REGION_DEBUG_START_3; offset < REGION_DEBUG_STOP_3; offset+=4){
		      shmRead32( df, B43_SHM_SHARED, offset, buffer);
		      sprintf(rx_code[i],"%c%c",buffer[8],buffer[9]);
		      sprintf(rx_seq[i],"%c%c",buffer[6],buffer[7]);
		      sprintf(rx_value[i],"%c%c%c%c",buffer[4],buffer[5],buffer[2],buffer[3]);
		      busy_value[i] = strtol(rx_value[i], NULL, 16);
		      code_int[i] = strtol(rx_code[i], NULL, 16);
		      seq_int[i] = strtol(rx_seq[i], NULL, 16);
		      //printf("%s - %s - %ld\n", rx_code[i], rx_seq[i], strtol(rx_value[i], NULL, 16));
		      printf("%ld - %ld - %ld\n", code_int[i], seq_int[i], busy_value[i]);
		      
		      
		      if(strcmp(rx_seq[i],"EE")){
			time(&rawtime);
			timeinfo = localtime(&rawtime);
			//2014 01 26 11 17 15
			strftime (buffer,80,"%G%m%d%H%M%S",timeinfo);	
			fprintf(log_zigbee_rx, "%s,%ld,%ld,%ld\n", buffer, code_int[i], seq_int[i], busy_value[i]);
			fflush(log_zigbee_rx);	      
		      }
		   
		      i++;
		      
		}
			
		sleep(4);
	}
	
	fclose(log_zigbee_rx);
	printf("\n");

}

/* ******************
 * FUNCTION TO READ metaMAC parameters
 * 
 * work: the function runs for 120 seconds and every second change the bytecode slot protocol
 * needed : you need load bytecode protocol in both WMP slots
 * usage: to work this function you need run bytecode-manager with option --read-slot file_name
 * example : ./bytecode-manager --read-slot log_slot.csv
 * 
 * ***************** */

void readSlotTimeValue(struct debugfs_file * df,  char * file_name){
  
 	int i,j,k,l;
	unsigned int count_slot;
	unsigned int count_slot_old;
	unsigned int count_slot_var;
	unsigned int packet_to_transmit;
	unsigned int my_transmission;
	unsigned int succes_transmission;
	unsigned int other_transmission;
	
	long int usec;
	long int usec_from_start;
	long int usec_from_current;
	
	struct timeval starttime, finishtime;
	struct timeval start7slot, finish7slot;
	
	struct options opt;
	
	printf("name file %s\n",file_name);

	FILE * log_slot_time;
	log_slot_time = fopen(file_name, "w+");
	fprintf(log_slot_time, "num-row,num-read,um-from-start-real,um-from-start-compute,um-diff-time,bytecode-protocol,count-slot,count-slot-var,packet_to_transmit,my_transmission,succes_transmission,other_transmission,\n");
	gettimeofday(&starttime, NULL);
	usec_from_current = 0;
	start7slot= starttime;
	k=0;
	
	for(l=0; l<120; l++){ 		//it runs for 120 seconds
	  
		
		if( (l%2) == 0) 	//for this experiment every second change bytecode slot
		  opt.active = "1";
		else
		  opt.active = "2";
		
		
	//	gettimeofday(&start7slot, NULL);	
		//activation
		writeAddressBytecode(df,&opt);	//change bytecode slot - generally we need from 20ms to 80ms to do it
	/*
		gettimeofday(&finish7slot, NULL);	    
		usec=(finish7slot.tv_sec-start7slot.tv_sec)*1000000;
		usec+=(finish7slot.tv_usec-start7slot.tv_usec);
		printf("%d - %ld\n", j, usec);
		sleep(1);
	*/	
	
		count_slot_old = 0x000F & shmRead16(df, B43_SHM_REGS, COUNT_SLOT);	//get current time slot number
		for(j=0; j<84; j++){//we read metaMAC parameters every 12ms, so we complete 84 cycle in a second 
		
			//this code read metaMAC parameters
		  
			//delay
			usleep(7000);
		  
			//read meta-MAC value, generally we need 5ms to do it
			count_slot_old = count_slot & 0x000F;
			count_slot = shmRead16(df, B43_SHM_REGS, COUNT_SLOT);
			packet_to_transmit = shmRead16(df, B43_SHM_SHARED, PACKET_TO_TRANSMIT);
			my_transmission = shmRead16(df, B43_SHM_SHARED, MY_TRANSMISSION);
			succes_transmission = shmRead16(df, B43_SHM_SHARED, SUCCES_TRANSMISSION);
			other_transmission = shmRead16(df, B43_SHM_SHARED, OTHER_TRANSMISSION);
			
			//compute cycle time
			gettimeofday(&finish7slot, NULL);	    
			usec=(finish7slot.tv_sec-start7slot.tv_sec)*1000000;
			usec+=(finish7slot.tv_usec-start7slot.tv_usec);
			usec_from_start = (finish7slot.tv_sec-starttime.tv_sec)*1000000;
			usec_from_start += (finish7slot.tv_usec-starttime.tv_usec);
			start7slot = finish7slot;
			
			// print debug values
			printf("%d - %ld\n", j, usec);
			printf("count_slot:0x%04X - packet_to_transmit:0x%04X - my_transmission:0x%04X - succes_transmission:0x%04X - other_transmission:0x%04X\n", count_slot, packet_to_transmit, my_transmission, succes_transmission, other_transmission);
			//printf("%d - %d - %s,%d,%d,%ld\n", i, count_change, buffer, 251, 0,usec);    
			  
			// check if cycle time is over, we must be sure to read at least every 16ms
			if ( count_slot_old == (count_slot & 0x000F) | usec > 16000 | j==0)
			{
				// if last cicle is over 16ms or if we change bytecode, we fill time sloc with 0, no information for this slot time
				printf("read error\n");
				if(usec > 100000)
				  exit(1);
				while(1){
					fprintf(log_slot_time,"%d,%d,%ld,%ld,%ld,%c,%d,0,0,0,0,0\n",
						k, j, usec_from_start, usec_from_current, usec, *opt.active, count_slot & 0x000F);	
					k++;
					usec_from_current += 2200;		
					if(usec_from_current > usec_from_start )
					    break;
				}    
				fflush(log_slot_time);
			}
			else
			{
				// we extract metaMAC parametes from registers and put it in the log file
				count_slot_var = count_slot_old;
				for(i=0; i<7; i++)	// we get a maximum of 7 time slots, to safe, we not get the current 
				{
					fprintf(log_slot_time,"%d,%d,%ld,%ld,%ld,%c,%d,%d,%01x,%01x,%01x,%01x\n",
						k, j, usec_from_start, usec_from_current, usec, *opt.active, count_slot & 0x000F, count_slot_var, 
						(packet_to_transmit>>count_slot_var) & 0x0001, (my_transmission>>count_slot_var) & 0x0001, 
						(succes_transmission>>count_slot_var) & 0x0001, (other_transmission>>count_slot_var) & 0x0001);	
					k++;
					usec_from_current += 2200;
					if(count_slot_var==7)	// we increase module 7
					    count_slot_var=0;
					else
					    count_slot_var++;
					
					if(count_slot_var == (count_slot & 0x000F) ) //we read to the last slot time
					    break;
				}    
				fflush(log_slot_time);	      
			}
		}
		
	}
	
	fclose(log_slot_time);
	printf("read successful\n");

}




void getRaw(struct debugfs_file * df){

}
void getPc(struct debugfs_file * df){

}


void ucodeStop(struct debugfs_file * df){
  //Unconditionally stop the microcode PSM
  
  maskSet32(df, B43_MMIO_MACCTL, ~B43_MACCTL_PSM_RUN, 0);
  
  
}
void ucodeStart(struct debugfs_file * df){
  /*	Unconditionally start the microcode PSM. This will restart the
	microcode on the current PC. It will not jump to 0. Warning: This will
	unconditionally restart the PSM and ignore any driver-state!
  */
  maskSet32(df, B43_MMIO_MACCTL, ~0, B43_MACCTL_PSM_RUN);
  
}


