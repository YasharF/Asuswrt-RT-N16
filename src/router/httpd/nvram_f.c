/*
    Copyright 2001, Broadcom Corporation
    All Rights Reserved.
    
    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Broadcom Corporation.
*/
/*
 * This module creates an array of name, value pairs
 * and supports updating the nvram space. 
 *
 * This module requires the following support routines
 *
 *      malloc, free, strcmp, strncmp, strcpy, strtol, strchr, printf and sprintf
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
//#include <typedefs.h>
#include <bcmnvram.h>
#include <sys/mman.h>
#ifdef REMOVE_WL600
#include <bcmutils.h>
#include <sbsdram.h>
#include <bcmendian.h>
#include <bcmnvram_f.h>
#include <flash.h>
#include <flashutl.h>
#include <osl.h>
#endif


#define MAX_LINE_SIZE 512
#define MAX_FILE_NAME 64

char *workingDir="/etc/linuxigd";


/* Linked list of environment variables */
typedef struct nvram_tuple EnvRec;
static EnvRec  curEnv;

#ifndef NOUSB
    void del_right_list(char *user);
#endif

/*
 * NOTE : The mutex must be initialized in the OS previous to the point at
 *           which multiple entry points to the nvram code are enabled 
 *
 */
#define MAX_NVRAM_SIZE 4096
#define EPI_VERSION_STR "2.4.5"

#ifndef NOUSB
/* Remove CR/LF/Space in the end of string 
 * 
 */
 
char *strtrim(char *str)
{
   int i;
   
   i=strlen(str)-1;
   	
   while(i>=0)
   {   
      if (*(str+i)==13 || *(str+i)==10 || *(str+i)==' ')
      {	     
   	 *(str+i)=0x0; 	 
      } 
      else if (*(str+i)=='"')
      {
	 *(str+i)=0x0;
   	 break;	 
      }      
      else break;
      i--;
   }
   
   return (str);
}

char *strtrim_c(char *str)
{
   int i;
  
   return(str);

#ifdef REMOVE   
   i=strlen(str)-1;
   	
   while(i>0)
   {   
      if (*(str+i)==';')
      {
   	 *(str+i)=0x0; 
   	 break;
      }         
      i--;
   }
   return (str);
#endif   
}


int nvram_set_item(EnvRec *env, char *name, char *value)
{  
   if (env->name) free(env->name);
   if (env->value) free(env->value);
   
   env->name = (char*)malloc(strlen(name)+1);
   if (!env->name) goto err;
   strcpy(env->name, name);  	
   env->value = (char*)malloc(strlen(value)+1);
   if (!env->value) goto err;
   strcpy(env->value, value);		
   return (1);
err:
   if (env->name) free(env->name);
   if (env->value) free(env->value);
   return (0);
}

int nvram_add_item(EnvRec *env, char *name, char *value)
{
   EnvRec *orig;
   	
   if (env->next!=NULL) orig = env->next;
   else orig = NULL;	
   env->next = (EnvRec*)malloc(sizeof(EnvRec));
   if (!env->next) return(0);
   env->next->name = NULL;
   env->next->value = NULL;
   if (nvram_set_item(env->next, name, value)==0) goto err;
     
#ifdef REMOVE_WL600    
   if (orig!=NULL)
      printf("Add between %s %s %s\n", env->name, env->next->name, orig->name);
#endif      
   env->next->next = orig;
   
   return(1);

err:
   if (env->next) free(env->next);
   if (orig) env->next = orig;
   return(0);                
}
#endif

/*
 * Set the value of an NVRAM variable
 * @param	name	name of variable to set
 * @param	value	value of variable
 * @return	0 on success and errno on failure
 */
int
nvram_set_x(const char *sid, const char *name, const char *value)
{            
     return(nvram_set(name, value));
}


/*
 * Get the value of an NVRAM variable
 * @param	name	name of variable to get
 * @return	value of variable or NULL if undefined
 */
char*
nvram_get_x(const char *sid, const char *name)
{	
   return(nvram_safe_get(name));
}


/*
 * Get the value of an NVRAM variable
 * @param	name	name of variable to get
 * @return	value of variable or NULL if undefined
 */
char*
nvram_get_f(const char *file, const char *field)
{ 
    return(nvram_safe_get(field));
}

/*
 * Set the value of an NVRAM variable from file
 * @param	name	name of variable to get
 * @return	value of variable or NULL if undefined
 */
int 
nvram_set_f(const char *file, const char *field, const char *value)
{
    return(nvram_set(field, value));
}

/*
 * Get the value of an NVRAM variable list 
 * @param	name	name of variable to get
 *              index   index of the list, start from 1
 * @return	value of variable or NULL if undefined
 */

char *nvram_get_list_x(const char *sid, const char *name, int index)
{   
    char new_name[MAX_LINE_SIZE];
    
    sprintf(new_name, "%s%d", name, index);
    
    return(nvram_get_f(sid, new_name));	
#ifdef OLD_DESIGN	 
    static char value[MAX_LINE_SIZE];
    char *v, *buf;
    int i;
    
    strcpy(value, nvram_get_f(sid, name));    
    
    if (value[0]=='\0')    
       return(value);
       
    buf = value;
    v = strchr(buf, ';');
    /*syslog(LOG_INFO,"Get %s %s %d\n", v, buf, index);*/
    
    i = 0;
    while (v!=NULL)
    {
    	if (i==index-1)
    	{
    	    *v = '\0';  	
    	    return(buf);
    	}
    	/*syslog(LOG_INFO, "Get %s %s\n", v, buf);*/
    	buf = v+1;
    	v = strchr(buf, ';');
    	i++;
    }       
    if ( i==0 && index==1) 
       return(value);
    else if (i==(index-1))
       return(buf);   
    
    value[0] = '\0';
    return(value);   
#endif    
}

#ifdef REMOVE_NVRAM
/*
 * Add the value into the end an NVRAM variable list 
 * @param	name	name of variable to get
 * @return	0 on success and errno on failure
 */
int nvram_add_list_x(const char *sid, const char *name, const char *value)
{    
    FILE *fl, *flTemp;           
    char buf[MAX_LINE_SIZE];	
    char new_buf[MAX_LINE_SIZE];
    char filename[MAX_FILE_NAME];
    unsigned char *v, *sp;
    int found;
                    
    sprintf(filename, "%s/%s", workingDir, sid);
    if ((fl=fopen(filename,"r+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","w+"))==NULL) return 1;
                     
    found = 0; 
    while(fgets(buf, MAX_LINE_SIZE, fl))
    {    	    
       v = strchr(buf, '=');
       if (v != NULL && ((sp = strchr(buf, ' ')) == NULL || (sp > v))) {
            /* change the "name=val" string to "set name val" */                                    
            if (!strncmp(buf, name, strlen(name)) && buf[strlen(name)]=='=')
            {
            	found++;  
            }
            else if (found)
            {	
               if (value!=NULL)	
               {
               	  sprintf(new_buf, "%s%d=\"%s\"\n", name, found-1, value);
               	  fputs(new_buf, flTemp);                  
               }   
               found = 0;
            }                           
            fputs(buf,flTemp);
       }     
    }     	  
    
    fclose(fl);
    fclose(flTemp);    
 
    if ((fl=fopen(filename,"w+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","r+"))==NULL) return 1;
    
    while(fgets(buf, MAX_LINE_SIZE, flTemp))
    {    	        	                            	   
       fputs(buf, fl);                      
    }     	  
    fclose(fl); 	   
    fclose(flTemp);    
    
    return(0);	
	
	
#ifdef OLD_DESIGN		
    char buf[MAX_LINE_SIZE];
    char *orig;
    
    orig = strtrim(nvram_get_f(sid, name));
            
    if (strcmp(orig,"")==0)
       sprintf(buf, "%s;", value);
    else
       sprintf(buf, "%s%s;", orig, value);
       
    syslog(LOG_INFO,"ADD: [%x] [%s] [%s]\n", *orig, value, buf);
       
    nvram_set_x(sid, name, buf);
#endif    
}
#endif

/*
 * Add the value into the end an NVRAM variable list 
 * @param	name	name of variable to get
 * @return	0 on success and errno on failure
 */
int nvram_add_lists_x(const char *sid, const char *name, const char *value, int count)
{    
#ifndef REMOVE_NVRAM	
    char name1[32], name2[32];
  
#ifdef NOUSB
    strcpy(name1, name);
#else 
    findNVRAMName(sid, name, name1);
#endif

    if (name[0]!='\0')
    {	
        sprintf(name2, "%s%d", name1, count);
    	nvram_set(name2, value);
    }	
#else                
    FILE *fl, *flTemp;           
    char buf[MAX_LINE_SIZE];	
    char new_buf[MAX_LINE_SIZE];
    char new_buf1[MAX_LINE_SIZE];
    char filename[MAX_FILE_NAME];
    unsigned char *v, *sp;
    int found;

    sprintf(filename, "%s/%s", workingDir, sid);
    if ((fl=fopen(filename,"r+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","w+"))==NULL) return 1;
                            
    if (count==0)
        sprintf(new_buf, "%s=", name);
    else 
        sprintf(new_buf, "%s%d=", name, count-1);

    sprintf(new_buf1, "%s%d=", name, count);
        
    found = 0;     
    
    while(fgets(buf, MAX_LINE_SIZE, fl))
    {    	    
       v = strchr(buf, '=');
       if (v != NULL && ((sp = strchr(buf, ' ')) == NULL || (sp > v))) {
            /* change the "name=val" string to "set name val" */
            if (!strncmp(buf, new_buf, strlen(new_buf))) /* Find the previous record */
            {   
               if (!found) /* If not added yet */
               {	 
                  fputs(buf, flTemp); /* Write this record */
               	        
                  if (value!=NULL)    /* Add new record */
                  {
               	     sprintf(new_buf, "%s%d=\"%s\"\n", name, count, value);
               	     fputs(new_buf, flTemp);
               	     found = 1;
               	     sprintf(new_buf, "%s%d=", name, count); /* Maintain the buf of new reocrd */
               	  }          
               }               
            }
            else if (strncmp(buf, new_buf1, strlen(new_buf1))!=0)
	       fputs(buf,flTemp);
       }     
    }     	  
    
    fclose(fl);
    fclose(flTemp);    
 
    if ((fl=fopen(filename,"w+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","r+"))==NULL) return 1;
    
    while(fgets(buf, MAX_LINE_SIZE, flTemp))
    {    	        	                            	   
       fputs(buf, fl);                      
    }     	  
    fclose(fl); 	   
    fclose(flTemp);    
#endif    
    return(0);	
	
	
#ifdef OLD_DESIGN		
    char buf[MAX_LINE_SIZE];
    char *orig;
    
    orig = strtrim(nvram_get_f(sid, name));
            
    if (strcmp(orig,"")==0)
       sprintf(buf, "%s;", value);
    else
       sprintf(buf, "%s%s;", orig, value);
       
    syslog(LOG_INFO,"ADD: [%x] [%s] [%s]\n", *orig, value, buf);
       
    nvram_set_x(sid, name, buf);
#endif    
}


#ifdef REMOVE_NVRAM
/*
 * Delete the value from an NVRAM variable list 
 * @param	name	name of variable list
 *              index   index of variable list
 * @return	0 on success and errno on failure
 */
int nvram_del_list_x(const char *sid, const char *name, int index)
{
#ifdef REMOVE_WL600	
    char value[MAX_LINE_SIZE];
    char *v, *buf;
    int i;	   
       
    strcpy(value, nvram_get_f(sid, name));
    
    /*syslog(LOG_INFO, "Get list : %s %d\n", value, index);*/
    
    if (value[0]=='\0')
       return(1);
       
    buf = value;
    v = strchr(buf, ';');

    i = 0;
    while (v!=NULL)
    {
    	/* syslog(LOG_INFO, "Check list : %s %d %d\n", value, index, i);*/
    	if (i==index-1)
    	{
    	    *v++ = '\0';  
    	    strcpy(buf, v);
    	    /*syslog(LOG_INFO, "Set list : %s\n", value);*/
    	    nvram_set_x(sid, name, value);
    	    return(0);
    	}
    	buf = v+1;
    	v = strchr(buf, ';');
    	i++;
    }            
    return(1);     
#endif      	
}
#endif

/*
 * Delete the value from an NVRAM variable list 
 * @param	name	name of variable list
 *              index   index of variable list
 * @return	0 on success and errno on failure
 */
int nvram_del_lists_x(const char *sid, const char *name, int *delMap)
{
#ifndef REMOVE_NVRAM

    FILE *fp;
    char names[32], oname[32], nname[32], *oval, *nval;
    int oi, ni, di;
   
#ifdef NOUSB
    strcpy(names, name);
#else 
    findNVRAMName(sid, name, names);
#endif

#ifndef NOUSB
    if(!strcmp(name,"acc_username"))
    {
	int i=0;
	char delname[32],namevalue[32];
	//char *namevalue;

	while(delMap[i]!=-1)
	{
		sprintf(delname,"%s%d",name,delMap[i]);
//Yau
printf("delname=%s\n",delname);
		strcpy(namevalue,nvram_get(delname));
		//namevalue=nvram_get(delname);
//Yau
printf("del_list_x:: name: %s = %s\n",delname,namevalue);

		del_right_list(namevalue);
		i++;
	}
    }
#endif

    if (names[0]!='\0')
    {	
	oi=0;
	ni=0;
	di=0;
	while(1)
	{
		sprintf(oname, "%s%d", names, oi);		
		sprintf(nname, "%s%d", names, ni);

		oval = nvram_get(oname);
		nval = nvram_get(nname);

		if (oval==NULL) break;

		printf("d: %d %d %d %d\n", oi, ni, di, delMap[di]);
		if (delMap[di]!=-1&&delMap[di]==oi)
		{
			oi++;
			di++;
		}
		else
		{
			nvram_set(nname, oval);
			ni++;
			oi++;
		}	
	}
    }	                
#else
    FILE *fl, *flTemp;           
    char buf[MAX_LINE_SIZE];	
    char new_buf[MAX_LINE_SIZE];
    char filename[MAX_FILE_NAME];
    unsigned char *v, *sp;
    int found, count, i, del;
                    
    sprintf(filename, "%s/%s", workingDir, sid);
    if ((fl=fopen(filename,"r+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","w+"))==NULL) return 1;
                    
    count = 0;
    
    del = 0;   
                             
    while(fgets(buf, MAX_LINE_SIZE, fl))
    {    	    
       v = strchr(buf, '=');
       if (v != NULL && ((sp = strchr(buf, ' ')) == NULL || (sp > v))) {
            /* change the "name=val" string to "set name val" */                                    
            if (!strncmp(buf, name, strlen(name)) && buf[strlen(name)]!='=')
            {
            	i = strlen(name);
            	
            	if (buf[i]>='0' && buf[i]<='9')
            	    found = buf[i] - '0';
            	else
            	    continue;
            	        
            	if (buf[i+1]>='0' && buf[i+1]<='9')
            	{
            	    found = found*10 + buf[i+1] - '0';    
            	    
            	    if (buf[i+2]>='0' && buf[i+2]<='9')
            	        found = found*10 + buf[i+2] - '0';    
            	    else if (buf[i+2]!='=')
            	        continue;    
            	}    
            	else if (buf[i+1]!='=')
            	    continue;
            	
            	/*printf("Del : %d %d %d\n", del, delMap[del], found);	*/
            	                	                	   
            	if ( delMap[del]!=-1 && delMap[del]==found)            
            	{            	    
            	    del++;	
                    continue;                                
                }   
                                  	
                v++;
                                   
                if (*v=='"')
                  v++;      
                              	
                sprintf(new_buf, "%s%d=\"%s\"\n", name, count, v);
               	fputs(new_buf, flTemp);                                                                     
               	
                count++;
            }            
            else             
            {	
                fputs(buf,flTemp);
            }                           
           
       }     
    }     	  
    
    fclose(fl);
    fclose(flTemp);    
 
    if ((fl=fopen(filename,"w+"))==NULL) return 1;
    if ((flTemp=fopen("/tmp/temp.cfg","r+"))==NULL) return 1;
    
    while(fgets(buf, MAX_LINE_SIZE, flTemp))
    {    	        	                            	   
       fputs(buf, fl);                      
    }     	  
    fclose(fl); 	   
    fclose(flTemp);    
#endif
    
    return(0);	
			
#ifdef REMOVE_WL600	
    char value[MAX_LINE_SIZE];
    char *v, *buf;
    int i;	   
       
    strcpy(value, nvram_get_f(sid, name));
    
    /*syslog(LOG_INFO, "Get list : %s %d\n", value, index);*/
    
    if (value[0]=='\0')
       return(1);
       
    buf = value;
    v = strchr(buf, ';');

    i = 0;
    while (v!=NULL)
    {
    	/* syslog(LOG_INFO, "Check list : %s %d %d\n", value, index, i);*/
    	if (i==index-1)
    	{
    	    *v++ = '\0';  
    	    strcpy(buf, v);
    	    /*syslog(LOG_INFO, "Set list : %s\n", value);*/
    	    nvram_set_x(sid, name, value);
    	    return(0);
    	}
    	buf = v+1;
    	v = strchr(buf, ';');
    	i++;
    }            
    return(1);     
#endif      	
}

#ifndef NOUSB
int nvram_add_right_list(const char *name, const char *value1, const char *value2)
{
	int count, list_no;
	char rright[205], wright[205], username[205];
	char onlist_num[2];

	count = atoi(nvram_get("sh_edit_x"));
	list_no = atoi(nvram_get_i("sh_acc_onlist_num",count));
//Yau
	strcpy(username, nvram_get_i("sh_acc_user", count));

	sprintf(username, "%s%s;",username, value1);
//Yau
	nvram_set_i("sh_acc_user",count,username);

	if( strcmp("1",value2)==0) // read only
	{
		strcpy(rright, nvram_get_i("sh_rright", count));
	        sprintf(rright, "%s%s;", rright, value1);
	        nvram_set_i("sh_rright",count,rright);
//Yau
	}
	else // read/write
	{
                strcpy(rright, nvram_get_i("sh_rright", count));
                sprintf(rright, "%s%s;", rright, value1);
                nvram_set_i("sh_rright",count,rright);

                strcpy(wright, nvram_get_i("sh_wright", count));
                sprintf(wright, "%s%s;", wright, value1);
                nvram_set_i("sh_wright",count,wright);
//Yau
	}

	sprintf(onlist_num,"%d",(list_no+1));
	nvram_set_i("sh_acc_onlist_num", count, onlist_num);
	nvram_set("sh_acc_onlist_num_x", onlist_num);
	nvram_set("sh_acc_user_x", username);
	return(0);
}

int nvram_remove_right_list(int *delMap)
{
        int sh_edit, i=0, j, tmp2=0, list_no;
	char *user, *p;
        char rright[205], wright[205], username[205];
	char tmp_user[205];
    	char buf_user[205];
    	char buf_rright[205];
    	char buf_wright[205];
	char onlist_num[2];
                                                                                                                                              
        sh_edit = atoi(nvram_get("sh_edit_x"));
	list_no = atoi(nvram_get_i("sh_acc_onlist_num",sh_edit));
//Yau
printf("++++ remove_right_list ++++ %d\n",sh_edit);

        strcpy(buf_user, nvram_get_i("sh_acc_user", sh_edit));
        strcpy(buf_rright, nvram_get_i("sh_rright", sh_edit));
        strcpy(buf_wright, nvram_get_i("sh_wright", sh_edit));
//find user name and del it
	while(delMap[i]!=-1)
	{
		j=0;
		tmp2=delMap[i]-i;
	        strcpy(tmp_user, buf_user);
	        user = strtok(tmp_user, ";");
		if(tmp2!=0)
		{
	        	while(user=strtok(NULL,";"))
        		{
				j++;
				if(tmp2==j) break;
			}
		}
//Yau
//del the user name
		p=strstr(buf_user, user);
		*p='\0';
		strcpy(username, buf_user);
		sprintf(username,"%s%s",username,p+strlen(user)+1);
                strcpy(buf_user, username);

                p=strstr(buf_rright, user);
                *p='\0';
                strcpy(rright, buf_rright);
                sprintf(rright,"%s%s",rright,p+strlen(user)+1);
                strcpy(buf_rright, rright);

                p=strstr(buf_wright, user);
		if(p!=NULL)
		{
                *p='\0';
                strcpy(wright, buf_wright);
                sprintf(wright,"%s%s",wright,p+strlen(user)+1);
                strcpy(buf_wright, wright);
		}

		i++;
	}

        sprintf(onlist_num,"%d",list_no-i);
	//Set to Nvram
        nvram_set_i("sh_acc_onlist_num",sh_edit, onlist_num);
	nvram_set("sh_acc_onlist_num_x", onlist_num);
	nvram_set_i("sh_acc_user",sh_edit,buf_user);
	nvram_set_i("sh_rright",sh_edit,buf_rright);
	nvram_set_i("sh_wright",sh_edit,buf_wright);
	nvram_set("sh_acc_user_x",buf_user);

        return(0);
}


int nvram_modify_right_list(char *user, int right)
{
        int sh_edit;
        char *p;
        char wright[205];
        char buf_wright[205];
                                                                                                                                              
        sh_edit = atoi(nvram_get("sh_edit_x"));
//Yau
printf("++++ modify_right_list ++++ %d\n",sh_edit);
                                                                                                                                              
        strcpy(buf_wright, nvram_get_i("sh_wright", sh_edit));
	if(right==1)//read only
	{
                p=strstr(buf_wright, user);
                if(p!=NULL)
                {
	                *p='\0';
        	        strcpy(wright, buf_wright);
                	sprintf(wright,"%s%s",wright,p+strlen(user)+1);
                	strcpy(buf_wright, wright);
		}
	}
	else // read/write
	{
                p=strstr(buf_wright, user);
                if(p==NULL)
                {
        	        sprintf(buf_wright, "%s%s;", buf_wright, user);
		}
	}
	//write to nvram
        nvram_set_i("sh_wright",sh_edit,buf_wright);

	return (0);
}

// 2005.12.26 yau add for: when del the users in the user list 
// also need to del these in all access right lists
void del_right_list(char *deluser)
{
        int  sh_num, sh_modify=0i;
        char *p;
        char wright[205], username[205];
        char buf_user[205];
        //char buf_rright[205];
        char buf_wright[205];
                               
	sh_num = atoi(nvram_get("sh_num"));

printf("del User Name: %s, \n", deluser);

	while(sh_modify < sh_num)
	{
        	strcpy(buf_user, nvram_get_i("sh_acc_user", sh_modify));
	        //strcpy(buf_rright, nvram_get_i("sh_rright", sh_modify));
	        strcpy(buf_wright, nvram_get_i("sh_wright", sh_modify));
//del the user
                p=strstr(buf_user, deluser);
		if(p!=NULL)
		{
	                *p='\0';
        	        strcpy(username, buf_user);
                	sprintf(username,"%s%s",username,p+strlen(deluser)+1);
	                strcpy(buf_user, username);
		}

                p=strstr(buf_wright, deluser);
                if(p!=NULL)
                {
	                *p='\0';
        	        strcpy(wright, buf_wright);
                	sprintf(wright,"%s%s",wright,p+strlen(deluser)+1);
	                strcpy(buf_wright, wright);
                }
	        //Set to Nvram
        	nvram_set_i("sh_acc_user",sh_modify,buf_user);
	        nvram_set_i("sh_rright",sh_modify,buf_user);
        	nvram_set_i("sh_wright",sh_modify,buf_wright);
                sh_modify++;
        }
}
#endif
