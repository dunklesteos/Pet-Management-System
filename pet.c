#include "pet.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Pet* Load_PetData(char *fileName, int *recordSize) {
    struct Pet *petData = NULL;
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        *recordSize = 0;
        return NULL;
    }

    char line[200];
    int count = 0;
    while(fgets(line, 200, file) != NULL) {
        if(strlen(line) > 1) {
            count++;
        }
    }

    *recordSize = count;
    petData = malloc(count * sizeof(struct Pet));
    rewind(file);

    struct Pet *tmp = petData;
    for (int i = 0; i < count; i++) {
        if(fgets(line, 200, file) != NULL) {
            char *piece = strtok(line, "-");
            sscanf(piece, "%d", &tmp->petID);

            piece = strtok(NULL, "-");
            strcpy(tmp->name, piece);

            piece = strtok(NULL, "-");
            sscanf(piece, "%d", &tmp->age);

            piece = strtok(NULL, "-");
            tmp->gender = *piece;

            piece = strtok(NULL, "-");
            strcpy(tmp->vStatus, piece);

            piece = strtok(NULL, "-\n");
            strcpy(tmp->vDate, piece);

            tmp++;
        }
    }
    fclose(file);
    return petData;
}

void Save_PetData(char *fileName, struct Pet *petData, int recordSize) {
    if (petData == NULL || recordSize == 0) return;
    FILE *file = fopen(fileName, "w");
    if (file == NULL) return;

    for (int i = 0; i < recordSize; i++) {
        fprintf(file, "%d-%s-%d-%c-%s-%s\n",
                petData[i].petID, petData[i].name, petData[i].age,
                petData[i].gender, petData[i].vStatus, petData[i].vDate);
    }
    fclose(file);
}

struct Pet* Add_Pet(struct Pet *petData, int *recordSize, char *name, int age, char gender, char vaccine) {
    struct Pet p1;
    if(*recordSize == 0) {
        p1.petID = 101;
    } else {
        p1.petID = (petData[*recordSize - 1].petID) + 1;
    }

    strncpy(p1.name, name, 24);
    p1.name[24] = '\0';
    p1.age = age;
    p1.gender = gender;

    if (vaccine == 'n') {
        strcpy(p1.vStatus, "nonvaccinated");
        strcpy(p1.vDate, "*");
    } else {
        strcpy(p1.vStatus, "vaccinated");
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        sprintf(p1.vDate, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    }

    *recordSize = *recordSize + 1;
    struct Pet *tmp = realloc(petData, (*recordSize) * (sizeof(struct Pet)));
    if(tmp != NULL) {
        petData = tmp;
        petData[*recordSize - 1] = p1;
    }
    return petData;
}

void Sort(struct Pet *petData, int recordSize) {
    if(petData == NULL || recordSize <= 1) return;
    struct Pet tmp;
    for (int i = 0; i < recordSize - 1; i++) {
        for (int j = 0; j < (recordSize - i) - 1; j++) {
            if (strcmp(petData[j].name, petData[j+1].name) > 0) {
                tmp = petData[j];
                petData[j] = petData[j+1];
                petData[j+1] = tmp;
            }
        }
    }
}

struct Pet* Delete_Pet(struct Pet *petData, int *recordSize, int targetID) {
    if (petData == NULL || *recordSize == 0) return petData; // Veri yoksa geri dön

    int foundIndex = -1;
    // 1. Adım: ID'yi bul
    for (int i = 0; i < *recordSize; i++) {
        if (petData[i].petID == targetID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        return petData; // ID bulunamadıysa diziyi aynen geri gönder
    }

    // 2. Adım: Bulunan elemanın üzerine, sağındaki tüm elemanları sola kaydırarak yaz
    for (int i = foundIndex; i < *recordSize - 1; i++) {
        petData[i] = petData[i + 1];
    }

    *recordSize = *recordSize - 1; // Kayıt sayısını düşür

    // 3. Adım: Belleği küçült
    if (*recordSize == 0) {
        free(petData); // Eğer son eleman silindiyse belleği tamamen temizle
        return NULL;
    }

    struct Pet *tmp = realloc(petData, (*recordSize) * sizeof(struct Pet));
    if (tmp != NULL) {
        petData = tmp;
    }

    return petData;
}


int Update_Vaccination(struct Pet *petData, int recordSize, int targetID, char *newDate) {
    if (petData == NULL || recordSize == 0) return 0; // ID bulunamadı / Veri yok

    int day = 0, month = 0, year = 0;

    // 1. Adım: String formatını kontrol et
    if (sscanf(newDate, "%d/%d/%d", &day, &month, &year) != 3) {
        return -2; // Geçersiz format
    }

    // 2. Adım: Temel Ay ve Yıl sınır kontrolü
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        return -2;
    }

    // 3. Adım: Standart ayların gün sayıları
    int daysInMonths[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // 4. Adım: Artık Yıl Kontrolü (Leap Year)
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        daysInMonths[2] = 29; // Artık yıl ise Şubat 29 çeker
    }

    // 5. Adım: Gün sınır kontrolü
    if (day < 1 || day > daysInMonths[month]) {
        return -2; // Geçersiz gün
    }

    // 6. Adım: ID'yi ara ve bilgileri güncelle
    for (int i = 0; i < recordSize; i++) {
        if (petData[i].petID == targetID) {
            strcpy(petData[i].vStatus, "vaccinated");
            strncpy(petData[i].vDate, newDate, 14);
            petData[i].vDate[14] = '\0';
            return 1; // Başarılı güncelleme
        }
    }

    return 0; // ID bulunamadı
}