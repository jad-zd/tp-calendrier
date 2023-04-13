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
void add_data(Data* data) {
}

//Enlève la donnée _data_ de la base de donnée
void delete_data(Data* data) {
}

//Affiche le planning
char* see_all(char* answer) {
}

int main(int argc, char** argv) {
  return 0;
}
