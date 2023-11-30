#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<aio.h>
#include <string.h>
#include<time.h>
#include <dirent.h>

//modificari 3


off_t getlungimeafisieruluioriginal(const char *linkPath,const char path[]) {
    char resolvedPath[4096];  // Adjust the size according to your needs
    ssize_t resolvedSize = readlink(linkPath, resolvedPath, sizeof(resolvedPath) - 1);
    if (resolvedSize == -1) {
        perror("Eroare la citirea fisierlui link(legatura simbolica)");
        exit(EXIT_FAILURE);

    }
    resolvedPath[resolvedSize] = '\0';
	//printf("fisier original %s\n",resolvedPath);
	char path1[100];
	memcpy(path1, path, strlen(path)+1);
	strcat(path1,resolvedPath);
	memcpy(resolvedPath, path1, strlen(path1)+1);
	//printf("fisier original %s\n",resolvedPath);
    struct stat targetStat;
    if (stat(resolvedPath, &targetStat) == -1) {

        perror("Eroare la determinarea lungimi fisierului target");

        exit(EXIT_FAILURE);
    }

    return targetStat.st_size;
}
void procesare_fisier(char denumire_fisier1[], char path[],int tip)

{
char denumire_fisier[50];
int lungime;
int inaltime;
int input;

memcpy(denumire_fisier, denumire_fisier1, strlen(denumire_fisier1)+1);
	strcat(denumire_fisier,path);
	if(tip!=2)
	{
	input =open(denumire_fisier,O_RDONLY);
		if(input == -1 )
		{
			perror("Eroare la deschidere");
			exit(-1);
		}
		ssize_t bytes=0;
		char aux[100];
		bytes=read(input,aux,18);
			if(bytes == -1)
			{
				perror("eroare la citire");
				exit(-5);
			}
		bytes=read(input,&lungime,4 );
		if(bytes == -1)
			{
				perror("eroare la citire");
				exit(-6);
			}
		bytes=read(input,&inaltime,4 );
		if(bytes == -1)
			{
				perror("eroare la citire");
				exit(-6);
			}

		}
		struct stat file_stat;
		fstat(input,&file_stat);
  
		int size=file_stat.st_size; //dimensiune: <dimensiune in octeti>
  
		 int id_owner =file_stat.st_uid; //identificatorul utilizatorului
		//st_mtim = file_stat.st_mtim; //timpul ultimei modificari: 28.10.2023
  
		char mtime[30];
		time_t t = file_stat.st_mtime;
		struct tm lt;
    localtime_r(&t, &lt); /* convert to struct tm */
    strftime(mtime, sizeof mtime,"%d %b %Y ", &lt);
		int contor_legaturi =file_stat.st_nlink; //contorul de legaturi: <numar legaturi>
			char stru[4]="",strg[4]="",stro[4]="";
			char r='R',w='W',no='-',x='X';
         int fileMode = file_stat.st_mode;
        /* Check owner permissions */
        if ((fileMode & S_IRUSR) && (fileMode & S_IREAD))
        	strncat(stru,&r,1);
        	else
        	strncat(stru,&no,1);
        if ((fileMode & S_IWUSR) && (fileMode & S_IWRITE)) 
           strncat(stru,&w,1);
        	else
        	strncat(stru,&no,1);
        if ((fileMode & S_IXUSR) && (fileMode & S_IEXEC))
          strncat(stru,&x,1);
        	else
        	strncat(stru,&no,1);
        /* Check group permissions */
        if ((fileMode & S_IRGRP) && (fileMode & S_IREAD))
          strncat(strg,&r,1);
        	else
        	strncat(strg,&no,1);
        if ((fileMode & S_IWGRP) && (fileMode & S_IWRITE))
          strncat(strg,&w,1);
        	else
        	strncat(strg,&no,1);
        if ((fileMode & S_IXGRP) && (fileMode & S_IEXEC))
          strncat(strg,&x,1);
        	else
        	strncat(strg,&no,1);
        /* check other user permissions */
        if ((fileMode & S_IROTH) && (fileMode & S_IREAD))
        strncat(stro,&r,1);
        	else
        	strncat(stro,&no,1);
        if ((fileMode & S_IWOTH) && (fileMode & S_IWRITE))
       strncat(stro,&w,1);
        	else
        	strncat(stro,&no,1);
        if ((fileMode & S_IXOTH) && (fileMode & S_IEXEC))
	strncat(stro,&x,1);
        	else
        	strncat(stro,&no,1);
	//dimesinue legatura

		char buffer1[800];
		if(tip == 1)
		{
			sprintf(buffer1,"nume fisier bmp:  %s\n inaltime:  %d \n lungime: %d \n dimensiune: %d \n indentificatorul utilizatorului: %d\n timpul ultimei modificari : %s\n contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii : %s\n \n",denumire_fisier,inaltime,lungime,size,id_owner,mtime,contor_legaturi,stru,strg,stro);
		}
		else if(tip == 2)
		{
			sprintf(buffer1,"nume director:  %s\n  indentificatorul utilizatorului: %d\n  drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii : %s\n \n\n",denumire_fisier,id_owner,stru,strg,stro);
		}
		else if(tip==4)
		{
			sprintf(buffer1,"nume fisier obisnuit :  %s\n  dimensiune: %d \n indentificatorul utilizatorului: %d\n timpul ultimei modificari : %s\n contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii : %s\n\n",denumire_fisier,size,id_owner,mtime,contor_legaturi,stru,strg,stro);
		}
		else if(tip == 3)
		{
			off_t dimensiuneTarget=getlungimeafisieruluioriginal(denumire_fisier,denumire_fisier1);
			sprintf(buffer1,"nume legatura:  %s\n  dimensiune: %d \n  dimensiune fisier: %ld \n  drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii : %s\n\n",denumire_fisier,size,(long)dimensiuneTarget,stru,strg,stro);
    }
		size_t nbyte;
		nbyte = strlen(buffer1);
		int fd=open("statistica.txt",O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
		//eroare daca nu se deschide
		if(fd == -1 )
		{
			perror("Eroare la deschidere");
			exit(-1);
		}	
		write(fd,buffer1,nbyte);
			if(tip!=2)
	{
	int c=close(input);
		if(c == -1)
		{
			perror("Eroare la inchidere");
			exit(-3);
		}	
}
}
int is_directory(char denumire_fisier1[], char path[]) {
char denumire_fisier[50];
	memcpy(denumire_fisier, denumire_fisier1, strlen(denumire_fisier1)+1);
	strcat(denumire_fisier,path);
    struct stat fileStat;
    if (stat(denumire_fisier, &fileStat) == -1) {
        perror("Eroare la determinarea informatiilor din fisier");
        exit(1);
    }
    return S_ISDIR(fileStat.st_mode);
}
int is_link(char denumire_fisier1[], char path[]) {
char denumire_fisier[50];
	memcpy(denumire_fisier, denumire_fisier1, strlen(denumire_fisier1)+1);

	strcat(denumire_fisier,path);

    struct stat fileStat;
    if (lstat(denumire_fisier, &fileStat) == -1) {
        perror("Eroare la luarea de informati din fisier");
        exit(1);
    }
    return S_ISLNK(fileStat.st_mode);
}
int main(int argc, char * argv[])
{

	if(argc != 2)/home/student


	{
		printf("Usage /.program  <director_intrare> <director_iesire>\n");
		return 1;
	}

	int output=creat("statistica.txt",S_IRUSR|S_IWUSR|S_IWOTH);
		if(output == -1)

		{

			perror("Eroare la crearea fisierului statistica.txt");
		}
		close(output);
	DIR *folder;
    struct dirent *entry;
    folder = opendir(argv[1]);
    if(folder == NULL)
    {
        perror("Nu se poate citi directorul");
        return(1);

    }

    while( (entry=readdir(folder)) )

    {
           if(strcmp(entry->d_name,".")==0 || strcmp(entry->d_name,"..")==0)

              continue;

struct stat sb;

    	int tip=0;

    	stat(entry->d_name, &sb);

            char *dot = strrchr(entry->d_name, '.');   
    	char *extensie = dot +1;
    	 if (is_directory(argv[1],entry->d_name))  //caz 2
       {
    			tip=2;    // este un director
    	}
    	else if(strcmp(extensie,"bmp")==0)  //caz 1
    		{
    			tip=1;			//este de tip .bmp
    		}
    	else if (is_link(argv[1],entry->d_name)) //caz 3
    		{
    			tip=3;    //legatura simbolica
    		}
    	else 



    		{
    			tip=4;   //fisier obisnuit
    		}


              procesare_fisier(argv[1],entry->d_name,tip);  //functia de procesare
    }


    closedir(folder);



   

return 0;



}
