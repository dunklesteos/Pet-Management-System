#ifndef PET_H
#define PET_H

#include <stdio.h>

// --- VERI YAPILARI ---
struct Pet {
    int petID;
    char name[25];
    int age;
    char gender;
    char vStatus[15];
    char vDate[15];
};

// --- FONKSIYON TANIMLARI ---
struct Pet* Load_PetData(char *fileName, int *recordSize);
void Save_PetData(char *fileName, struct Pet *petData, int recordSize);
struct Pet* Add_Pet(struct Pet *petData, int *recordSize, char *name, int age, char gender, char vaccine);
void Sort(struct Pet *petData, int recordSize);
struct Pet* Delete_Pet(struct Pet *petData, int *recordSize, int targetID);

// YENİ EKLENEN AŞILAMA FONKSİYONU
int Update_Vaccination(struct Pet *petData, int recordSize, int targetID, char *newDate);

#endif // PET_H