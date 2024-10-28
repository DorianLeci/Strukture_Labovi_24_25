#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_OPENING_FILE -1
#define BUFFER_SIZE 1024

struct _person;
typedef struct _person *position;

typedef struct _person {
char fname[50], lname[50];
int birth_year;
position next;
}person;

position create_person(char* fname, char* lname, int birth_year);
int prepend_list(position head, char* fname, char* lname, int birth_year);
int print_list(position first);
position find_last(position head);
int append_list(position head, char* fname, char* lname, int birth_year);
position find_by_lname(position first, char* lname);

int FileOpen(position head,char *filename);

int Odabir_Prepend(position head,char *fname,char *lname,int birth_year,int br_osoba,char *filename);
int Odabir_Apend(position head,char *fname,char *lname,int birth_year,int br_osoba,char *filename);

int ObrisiEl(position head,position ref);
position FindPrev(position head,position ref);

int main() {
    int rezultat_funkcije,i,vel_liste;
    char lname[50];
    position prezime,ref;
    position head=(position)malloc(sizeof(person));

    if(head==NULL){
        printf("\nNeuspješno alociranje memorije head-a\n");
        return -1;
    }
    head->next=NULL;

    rezultat_funkcije=FileOpen(head,"podatci.txt");

    if(rezultat_funkcije==ERROR_OPENING_FILE){
        printf("\nNeuspješna provedba funkcije\n");
    }

    printf("\nIspis vezane liste\n");
    print_list(head->next);

    printf("\nUnesi prezime koje zelis pretražiti\n");
    getchar();
    fgets(lname,50,stdin);
    vel_liste=strlen(lname);

    lname[vel_liste-1]='\0';

    prezime=find_by_lname(head->next,lname);

    if(prezime){
        printf("\nPronađeno je prezime: %s\n",prezime->lname);
    }
    else if(!prezime){
        printf("\nNije pronađeno prezime\n");
    }

    //////////////////////

    printf("\nOdaberi element koji želiš obrisati.Unesi prezime\n");

    for(i=0;i<vel_liste;i++){
        lname[i]='\0'; //resetiranje liste
    }

    fgets(lname,50,stdin);
    vel_liste=strlen(lname);

    lname[vel_liste-1]='\0';

    ref=find_by_lname(head->next,lname);

    if(ref){
        printf("\nUspješno pronađeno\n");
        printf("Ref: %s\n",ref->lname);
        ObrisiEl(head,ref);
        printf("\nLista nakon brisanja elementa\n");
        print_list(head->next);
    }

   else{
    printf("\nNije pronađeno prezime.Brisanje elementa nije moguće\n");
   }




    
    free(head);

    return 0;


}

int FileOpen(position head,char *filename){ //ucitavanje podataka iz filea i pozivanje ostalih funkcija
    int broj_osoba=0;
    char fname[50],lname[50];
    int birth_year;
    int odabir;
    char buffer[BUFFER_SIZE];
    FILE *fp=fopen(filename,"r");

    if(!fp){
        printf("\nPogreška pri otvaranju datoteke\n");
        return ERROR_OPENING_FILE;
    }
    	while(fgets(buffer, BUFFER_SIZE, fp)!=NULL) {
			broj_osoba++;
	}
    printf("\nBroj osoba: %d\n",broj_osoba);

    printf("\nOdaberi hoćeš li dodavati elemente na početak(0) ili kraj(1) liste\n");
    scanf("%d",&odabir);

    if(odabir==0){
        Odabir_Prepend(head,fname,lname,birth_year,broj_osoba,filename);
    }

    else if(odabir==1){
        Odabir_Apend(head,fname,lname,birth_year,broj_osoba,filename);
    }

    else{
        printf("\nNetočan unos\n");
        return -1;
    }

    fclose(fp);

    return 0;



}

position create_person(char* fname, char* lname,int birth_year) {

position new_person = NULL;

new_person = (position)malloc(sizeof(person));
if (!new_person) {
printf("\nNije uspješno malociranje\n");
return NULL;
}
strcpy(new_person->fname, fname);
strcpy(new_person->lname, lname);
new_person->birth_year = birth_year;
new_person->next = NULL;

return new_person;

}

int prepend_list(position head, char* fname, char* lname, int birth_year) {
position new_person = NULL;
position temp;

new_person = create_person(fname, lname, birth_year);

if (!new_person) {
return -1;
}
temp = head->next;
new_person->next = temp;
head->next = new_person;

return 0;

}

int print_list(position first) {
position temp = first;

while (temp != NULL) {
printf("%s %s %d\n", temp->fname, temp->lname, temp->birth_year);
temp = temp->next;
}

return 0;
}

position find_last(position head) {
position temp = head;
while (temp->next != NULL) {
temp = temp->next;
}

return temp;
}

int append_list(position head,char *fname,char *lname,int birth_year) {
position new_person = NULL;
position last = NULL;

new_person = create_person(fname, lname, birth_year);

if (!new_person) {
return -1;
}

last = find_last(head);
last->next = new_person;
new_person->next = NULL;

return 0;

}

position find_by_lname(position first, char* lname) {
position temp = first;

while (temp != NULL && strcmp(lname,temp->lname)!=0) {
temp = temp->next;
}
if (temp == NULL) {
return NULL;
}
return temp;
}

int Odabir_Prepend(position head,char *fname,char *lname,int birth_year,int br_osoba,char *filename){
    int i,j,vel_liste;
    FILE *fp=fopen(filename,"r");

    if(!fp){
        printf("\nPogreška u otvaranju datoteke\n");
        return ERROR_OPENING_FILE;
    }

            for(i=0;i<br_osoba;i++){
            fscanf(fp,"%s %s %d",fname,lname,&birth_year);

            prepend_list(head,fname,lname,birth_year); //ponovno stvaranje člana u vezanoj listi

            //ciscenje listi
            vel_liste=strlen(fname);
            for(j=0;j<vel_liste;j++){
                fname[j]='\0';
            }

            vel_liste=strlen(lname);
            for(j=0;j<vel_liste;j++){
                lname[j]='\0';
            }

        }
    fclose(fp);

    return 0;
}

int Odabir_Apend(position head,char *fname,char *lname,int birth_year,int br_osoba,char *filename){
    int i,j,vel_liste;
    FILE *fp=fopen(filename,"r");

    if(!fp){
        printf("\nPogreška u otvaranju datoteke\n");
        return ERROR_OPENING_FILE;
    }

            for(i=0;i<br_osoba;i++){
            fscanf(fp,"%s %s %d",fname,lname,&birth_year);

            append_list(head,fname,lname,birth_year); //ponovno stvaranje člana u vezanoj listi

            //ciscenje listi
            vel_liste=strlen(fname);
            for(j=0;j<vel_liste;j++){
                fname[j]='\0';
            }

            vel_liste=strlen(lname);
            for(j=0;j<vel_liste;j++){
                lname[j]='\0';
            }

        }

    fclose(fp);

    return 0;
}

int ObrisiEl(position head,position ref){

    position prethodni=FindPrev(head,ref);

    if(!prethodni){
        return -1;
    }

    prethodni->next=ref->next;
    free(ref);

    return 0;
}

position FindPrev(position head,position ref){
    position temp=head;

    while(temp->next!=NULL && temp->next!=ref){
        temp=temp->next;
    }

    if(!temp->next){
        printf("\nNe postoji prethodni\n");
        return NULL;
    }

    return temp;
}



