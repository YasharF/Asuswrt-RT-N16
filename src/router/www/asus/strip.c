#include <stdio.h>

main(int argc, char *argv[])
{
	FILE *fp;
	char buf[1024], *ptr;
	int i;
	int meta_skip=0;

	fp=fopen(argv[1], "r");
	if (fp)
	{
		while(fgets(buf, sizeof(buf), fp))
		{
			if (buf[0]=='\r' || buf[0]=='\n')
				continue;
			
			if(strstr(buf, "meta") && strstr(buf, "charset"))
			{
				meta_skip++;
				if(meta_skip>1) continue;
			}

			for(i=strlen(buf)-1;i>=0;i--)
			{	
				if (buf[i]=='\r'||buf[i]=='\n') continue;
				if (buf[i]==' ') 
				{
					buf[i]='\r';
					buf[i+1]='\n';
					buf[i+2]=0;
				}
				else break;
			}	
			if (buf[0]=='\r' || buf[0]=='\n')
				continue;
			ptr=strstr(buf, "//");
			
			if (ptr!=NULL && !strstr(buf,"//-->")) 
			{
				if(!(buf!=ptr && *(ptr-1)==':'))
				{
					*ptr='\r';
					*(ptr+1)='\n';
					*(ptr+2)=0;
				}
			}	
			if (buf[0]=='\r' || buf[0]=='\n')
				continue;	
			if (buf[0]=='{' /*|| buf[0]=='}'*/)
			{
				if (buf[1]=='\r'||buf[1]=='\n') buf[1] = 0;
			}
			i=0;
			while(buf[i]=='\t')
			{
				i++;
			}
			while(buf[i]==' ')
			{
				i++;
			}
			
			
			// remove tab
			
			//i=strlen(buf);
			//if (buf[i-1]=='\r' || buf[i-1]=='\n')
			//	buf[i-1]=0;
			//i=strlen(buf);
			//if (buf[i-1]=='\r' || buf[i-1]=='\n')
			//	buf[i-1]=0;	
			printf("%s", buf+i);
		}
		fclose(fp);
	}
}
