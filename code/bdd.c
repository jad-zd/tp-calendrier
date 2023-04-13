#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bdd.h"
#include "utils.h"

//Nom du fichier contenant les données
static const char* DATA = "data";

//Retourne une string à partir d'un Day 
char* day_to_string(enum Day d) {
  switch (d) {
    case MON: return "Lundi";
    case TUE: return "Mardi";
    case WED: return "Mercredi";
    case THU: return "Jeudi";
    case FRI: return "Vendredi";
    case SAT: return "Samedi";
    case SUN: return "Dimanche";
    case NONE: return "Not a day";
  }
}

//Retourne un Day à partir d'un string
//dans le cas où la string ne correspond pas à un jour, on renvoie NONE
enum Day string_to_day(char* dd) {
  char d[LINE_SIZE]; 
  strcpy(d, dd);
  //Conversion en minuscule
  for (int i = 0; i < strlen(d); i++) 
    d[i] = tolower(d[i]);
  
  if (strcmp("lundi", d) == 0) return MON;
  else if (strcmp("mardi", d) == 0) return TUE;
  else if (strcmp("mercredi", d) == 0) return WED;
  else if (strcmp("jeudi", d) == 0) return THU;
  else if (strcmp("vendredi", d) == 0) return FRI;
  else if (strcmp("samedi", d) == 0) return SAT;
  else if (strcmp("dimanche", d) == 0) return SUN;
  else return NONE;
}


// Libère la mémoire d'un pointeur vers Data
void data_free(Data* d) {
  free(d->name);
  free(d->activity);
  free(d);
}

//Modifie une chaîne de caratère correspondant à data
void data_format(char* l, Data* data) {
  sprintf(l, "%s,%s,%s,%d\n", 
      data->name, data->activity, 
      day_to_string(data->day), data->hour);
}


//Retourne une structure Data à partir d'une ligne de donnée
// get_data("toto,arc,lundi,4") ->  Data { "toto", "arc", MON, 4 };
// Attention il faudra libérer la mémoire vous-même après avoir utilisé
// le pointeur généré par cette fonction
Data* get_data(char* line) {
  char* parse;
  Data* data = malloc(sizeof(Data));
  char error_msg[LINE_SIZE];
  sprintf(error_msg, "Erreur de parsing pour: %s\n", line);
  
  //On s'assure que la ligne qu'on parse soit dans le mémoire autorisée en 
  // écriture
  char* l = malloc(strlen(line)+1);
  l = strncpy(l, line, strlen(line)+1);

  parse = strtok(l, ",");
  if (parse == NULL) exit_msg(error_msg, 0);
  data->name = malloc(strlen(parse)+1);
  strcpy(data->name, parse);

  parse = strtok(NULL, ",");
  if (parse == NULL) exit_msg(error_msg, 0);
  data->activity = malloc(strlen(parse)+1);
  strcpy(data->activity, parse);

  parse = strtok(NULL, ",");
  if (parse == NULL) exit_msg(error_msg, 0);
  data->day = string_to_day(parse);

  parse = strtok(NULL, "\n");
  if (parse == NULL) exit_msg(error_msg, 0);
  data->hour = atoi(parse);
  free(l); 

  return data;
  }

//La fonction _add_data_  retourne 0 si l'opération s'est bien déroulé
//sinon -1
int add_data(Data* data) {
  FILE* f; // déclaration d'un flux
  f = fopen(DATA, "a"); // ouverture du fichier DATA en mode append
  if (f == NULL) return -1; // gestion de l'erreur si on a pas de droits d'écriture
  char* l = malloc(LINE_SIZE); // initialisation d'une chaîne de caractère de taille LINE_SIZE dans heap
  data_format(l, data); // formatage de la chaîne de caractère (fonction fournie plus haut)
  fprintf(f, "%s", l); // écriture de la ligne dans le fichier
  if (fclose(f) == EOF) return -1; // gestion de l'erreur
  free(l); // libération de la mémoire
  return 0; // retourne 0 si tout s'est bien passé
}

//Enlève la donnée _data_ de la base de donnée
// La façon la plus simple de faire cela est de créer un nouveau fichier,
// y copier tout le contenu de l'ancien fichier sans la/les ligne(s) concernées [data]
// et finalement renommer ce fichier en data
int datas_are_equal(Data* data1, Data* data2) {
  int res = 
    strcmp(data1->name, data2->name) == 0
    && strcmp(data1->activity, data2->activity) == 0
    && data1->day == data2->day
    && data1->hour == data2->hour
    ;
  return res;
}

int delete_data(Data* data) {
  FILE* fold;
  FILE* fnew;
  char line[LINE_SIZE]; // initialisation d'une chaîne de caractère de taille LINE_size en tant que variable local
  Data* tmp;
  int i = 0; // à incrémenter dès qu'on supprime (ne copie pas) une ligne
  fold = fopen(DATA, "r");
  if (fold == NULL) return -1; // si on a pas les droits ou le fichier n'existe pas
  fnew = fopen("data.tmp", "w");
  if (fold == NULL) return -1;
  while(fgets(line, LINE_SIZE, fold)) { // tant qu'on peut lire une ligne
    tmp = get_data(line);
    if ( ! datas_are_equal(data, tmp)) {
      fprintf(fnew, "%s", line); // data != tmp => on copie dans fnew
    } else i++; // data == tmp => on incrémente i
  }
  if (i == 0) printf("Evennement non présent\n"); 
  if (fclose(fold) == EOF) return -1;
  if (fclose(fnew) == EOF) return -1;
  rename("data.tmp", "data"); // on renomme le fichier
  return 0;
}

//Affiche le planning
char* see_all(char* answer) {
}

int main(int argc, char** argv) {
  // test add_data(Data* data)
  Data* data = malloc(sizeof(Data));
  data->name ="toto";
  data->activity = "jouer";
  data->day = MON;
  data->hour = 14;
  int res = add_data(data);
  if (res != 0) {
    free(data);
    return res;
  }
  // test delete_data(Data* data)
  int res2 = delete_data(data);
  if (res2 != 0) {
    free(data);
    return res2;
  }
  free(data);
  return 0;
}
