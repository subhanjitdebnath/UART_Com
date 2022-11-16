#include "IO_OptionCard_FI_SENT.h"
void SetFrequencyDutyCycle(float Frequency,float Duty)
{
	TIM1->ARR=(int)(1000000.0/Frequency);
	TIM1->CCR1 = (Duty*TIM1->ARR)/100;
}
void ProcessDataReceived(char* Receive,char* freq,char* duty)
{
	int i,j;
	if(Receive[0]!=NULL)
		  {
			  //memset(freq,0,5);
			  //memset(duty,0,5);
			  i=0;
			  while(i<10)
			  {
				 freq[i]='\0';
				 duty[i]='\0';
				 i++;
			  }
			  i=0;
			  j=0;
			  while(Receive[i]!='-')
			  {
				  freq[j]=Receive[i];
				  i++;
				  j++;
			  }
			  i++;
			  j=0;
			  while(Receive[i]!='\0' && Receive[i]!='\n')
			  {
				  duty[j]=Receive[i];
				  i++;
				  j++;
			  }
			  //freq[4]='\n';
			 // duty[4]='\n';
		  }

}
