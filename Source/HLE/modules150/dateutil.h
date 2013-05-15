

int leap (int year)
{
    if((year%100== 0 && year%400==0)||  (year%4==0))
       
                return 1;
            else
                return 0;
}
/* can be done that way too but no portable?:P
int GetDayOfWeek(int year, int month, int day)
{
	SYSTEMTIME st;
 	FILETIME ft;
 	st.wYear = year;
	st.wMonth = month;
 	st.wDay = day;
	st.wHour = 0;
 	st.wMinute = 0;
 	st.wSecond = 0;
 	st.wMilliseconds = 0;
	SystemTimeToFileTime(&st,&ft);
 	FileTimeToSystemTime(&ft,&st);
 	return st.wDayOfWeek;
 }
*/
//http://en.wikipedia.org/wiki/Doomsday_Algorithm
int getDayNumber(int year,int month,int day)
{
    int dm=0;
	int dn=0;
                
    if (leap(year)==0)
    {      
		if(month==1)
             dm=0;
        if(month==2)
              dm=31;  
        if(month==3)
              dm=59;
        if(month==4)
               dm=90;
        if(month==5)
               dm=120;
        if(month==6)
               dm=151;
        if(month==7)
               dm=181;
         if(month==8)
               dm=212;
         if(month==9)
               dm=243;
         if(month==10)
                dm=273;
         if(month==11)
                dm=304;
         if(month==12)
                dm=334;
	}                                 
     else    
     {          
		 if(month==1)
                dm=0;
          if(month==2)
                dm=31;                        
          if(month==3)
                dm=60;
          if(month==4)
               dm=91;
          if(month==5)
               dm=121;
          if(month==6)
               dm=152;
          if(month==7)
               dm=182;  
          if(month==8)
               dm=213;
          if(month==9)
               dm=244;
          if(month==10)
               dm=274;
          if(month==11)
               dm=304;
          if(month==12)
               dm=335;
	 }  
       
        dn=dm+day;
       
      return dn;
     
}  

#
unsigned int daysformonth(unsigned int month, unsigned int year)
{
   /*  This function returns the number of days for the given month in the given year    */

    return (30 + (((month & 9) == 8) || ((month & 9) == 1)) - (month == 2) - (!(((year % 4) == 0) && (((year % 100) != 0) || ((year % 400) == 0))) && (month == 2)));

}