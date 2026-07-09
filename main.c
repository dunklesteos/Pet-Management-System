#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "include/raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

// Kendi yazdığımız pet kütüphanesini dahil ediyoruz
#include "pet.h"

typedef enum {
    STATE_MENU = 0,
    STATE_ADD,
    STATE_DISPLAY,
    STATE_DELETE,
    STATE_VACCINATE
} AppState;

int main(void) {
    char globalFileName[50] = "PetData.txt";
    int recordSize = 0;
    struct Pet *petData = Load_PetData(globalFileName, &recordSize);

    // Pencerenin sağ üstündeki genişletme tuşunu aktif et ve pencereyi aç
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1100, 750, "Pet Management System v1.3");
    SetTargetFPS(60);

    AppState currentState = STATE_MENU;

    // --- ADD EKRANI DEĞİŞKENLERİ ---
    char inputName[25] = "";
    char inputAge[5] = "";
    char genderChoice = 'M';
    char vaccineChoice = 'n';

    bool nameEditMode = false;
    bool ageEditMode = false;

    int activeGender = 0;
    int activeVaccine = 1;

    // --- SİLME EKRANI DEĞİŞKENLERİ ---
    char inputDeleteID[10] = "";
    bool deleteEditMode = false;
    int deleteStatus = 0;

    // --- AŞILAMA EKRANI DEĞİŞKENLERİ ---
    char inputVacID[10] = "";
    char inputVacDate[15] = "";
    bool vacIDEditMode = false;
    bool vacDateEditMode = false;
    int vacStatus = 0; // 0: Bekliyor, 1: Başarılı, -1: Bulunamadı, -2: Geçersiz Tarih

    // Yazı boyutunu dev ekrana uygun olarak 22 yaptık
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

    while (!WindowShouldClose()) {
        // Her karede güncel ekran boyutlarını alıp dinamik ortalama yapıyoruz
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        int btnWidth = 360;
        int btnHeight = 60;
        int centerX = (screenWidth - btnWidth) / 2;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ==================== 1. ANA MENÜ EKRANI ====================
        if (currentState == STATE_MENU) {
            DrawText("PET MANAGEMENT SYSTEM", (screenWidth - MeasureText("PET MANAGEMENT SYSTEM", 42)) / 2, 80, 42, MAROON);

            if (GuiButton((Rectangle){ centerX, 160, btnWidth, btnHeight }, "1 - Add New Pet")) currentState = STATE_ADD;
            if (GuiButton((Rectangle){ centerX, 245, btnWidth, btnHeight }, "2 - Sort Pet List")) {
                Sort(petData, recordSize);
                currentState = STATE_DISPLAY;
            }
            if (GuiButton((Rectangle){ centerX, 330, btnWidth, btnHeight }, "3 - Display Pet List")) currentState = STATE_DISPLAY;
            if (GuiButton((Rectangle){ centerX, 415, btnWidth, btnHeight }, "4 - Delete Pet By ID")) {
                currentState = STATE_DELETE;
                deleteStatus = 0;
            }
            if (GuiButton((Rectangle){ centerX, 500, btnWidth, btnHeight }, "5 - Vaccinate Pet By ID")) {
                currentState = STATE_VACCINATE;
                vacStatus = 0;
                strcpy(inputVacID, "");
                strcpy(inputVacDate, "");
            }
            if (GuiButton((Rectangle){ centerX, 585, btnWidth, btnHeight }, "6 - Save and Exit")) break;

            DrawText(TextFormat("Total Loaded Records: %d", recordSize), 50, screenHeight - 60, 24, DARKGRAY);

            // ==================== 2. PET EKLEME EKRANI ====================
        } else if (currentState == STATE_ADD) {
            DrawText("ADD NEW PET REGISTER", (screenWidth - MeasureText("ADD NEW PET REGISTER", 36)) / 2, 60, 36, DARKGRAY);

            int formX = (screenWidth - 500) / 2;

            DrawText("Pet Name:", formX, 180, 24, BLACK);
            if (GuiTextBox((Rectangle){ formX + 180, 175, 320, 45 }, inputName, 25, nameEditMode)) nameEditMode = !nameEditMode;

            DrawText("Pet Age:", formX, 260, 24, BLACK);
            if (GuiTextBox((Rectangle){ formX + 180, 255, 140, 45 }, inputAge, 4, ageEditMode)) ageEditMode = !ageEditMode;

            DrawText("Gender:", formX, 340, 24, BLACK);
            GuiToggleGroup((Rectangle){ formX + 180, 335, 140, 45 }, "M;F", &activeGender);
            if (activeGender == 0) genderChoice = 'M'; else genderChoice = 'F';

            DrawText("Vaccinated Today?:", formX - 50, 420, 24, BLACK);
            GuiToggleGroup((Rectangle){ formX + 200, 415, 170, 45 }, "Yes;No", &activeVaccine);
            if (activeVaccine == 0) vaccineChoice = 'y'; else vaccineChoice = 'n';

            if (GuiButton((Rectangle){ (screenWidth - 280) / 2, 550, 280, 60 }, "SAVE PET")) {
                int finalAge = atoi(inputAge);
                petData = Add_Pet(petData, &recordSize, inputName, finalAge, genderChoice, vaccineChoice);

                strcpy(inputName, "");
                strcpy(inputAge, "");
                currentState = STATE_MENU;
            }

            if (GuiButton((Rectangle){ 40, 40, 110, 45 }, "< Back")) currentState = STATE_MENU;

            // ==================== 3. LİSTELEME EKRANI ====================
        } else if (currentState == STATE_DISPLAY) {
            DrawText("REGISTERED PET LIST", (screenWidth - MeasureText("REGISTERED PET LIST", 36)) / 2, 50, 36, DARKGRAY);

            int startY = 140;
            int tableX = (screenWidth - 980) / 2;
            if (tableX < 40) tableX = 40;

            DrawText("ID        NAME                 AGE       GENDER       VAC STATUS           VAC DATE", tableX, startY, 20, MAROON);
            DrawLine(tableX, startY + 30, tableX + 980, startY + 30, LIGHTGRAY);

            for (int i = 0; i < recordSize; i++) {
                int currentY = startY + 50 + (i * 42);
                if (currentY > screenHeight - 80) break;

                DrawText(TextFormat("%d", petData[i].petID), tableX, currentY, 20, BLACK);
                DrawText(petData[i].name, tableX + 90, currentY, 20, BLACK);
                DrawText(TextFormat("%d", petData[i].age), tableX + 310, currentY, 20, BLACK);
                DrawText(TextFormat("%c", petData[i].gender), tableX + 440, currentY, 20, BLACK);
                DrawText(petData[i].vStatus, tableX + 570, currentY, 20, BLACK);
                DrawText(petData[i].vDate, tableX + 810, currentY, 20, BLACK);
            }

            if (GuiButton((Rectangle){ 40, 40, 110, 45 }, "< Back")) currentState = STATE_MENU;

            // ==================== 4. SİLME EKRANI ====================
        } else if (currentState == STATE_DELETE) {
            DrawText("DELETE PET BY ID", (screenWidth - MeasureText("DELETE PET BY ID", 36)) / 2, 80, 36, DARKGRAY);

            int formX = (screenWidth - 400) / 2;

            DrawText("Enter Pet ID:", formX, 250, 24, BLACK);
            if (GuiTextBox((Rectangle){ formX + 160, 240, 200, 50 }, inputDeleteID, 10, deleteEditMode)) deleteEditMode = !deleteEditMode;

            if (GuiButton((Rectangle){ (screenWidth - 200) / 2, 340, 200, 60 }, "DELETE")) {
                int targetID = atoi(inputDeleteID);
                int oldSize = recordSize;

                petData = Delete_Pet(petData, &recordSize, targetID);

                if (recordSize < oldSize) {
                    deleteStatus = 1;
                    strcpy(inputDeleteID, "");
                } else {
                    deleteStatus = -1;
                }
            }

            if (deleteStatus == 1) {
                DrawText("Pet deleted successfully!", (screenWidth - MeasureText("Pet deleted successfully!", 24)) / 2, 450, 24, DARKGREEN);
            } else if (deleteStatus == -1) {
                DrawText("Error: Pet ID not found!", (screenWidth - MeasureText("Error: Pet ID not found!", 24)) / 2, 450, 24, RED);
            }

            if (GuiButton((Rectangle){ 40, 40, 110, 45 }, "< Back")) currentState = STATE_MENU;

            // ==================== 5. AŞILAMA EKRANI ====================
        } else if (currentState == STATE_VACCINATE) {
            DrawText("VACCINATE PET BY ID", (screenWidth - MeasureText("VACCINATE PET BY ID", 36)) / 2, 60, 36, DARKGRAY);

            int formX = (screenWidth - 500) / 2;

            // ID Girişi
            DrawText("Enter Pet ID:", formX, 180, 24, BLACK);
            if (GuiTextBox((Rectangle){ formX + 200, 175, 200, 45 }, inputVacID, 10, vacIDEditMode)) vacIDEditMode = !vacIDEditMode;

            // Tarih Girişi
            DrawText("Vaccine Date:", formX, 260, 24, BLACK);
            if (GuiTextBox((Rectangle){ formX + 200, 255, 240, 45 }, inputVacDate, 12, vacDateEditMode)) vacDateEditMode = !vacDateEditMode;

            DrawText("(Format: DD/MM/YYYY)", formX + 200, 310, 16, DARKGRAY);

            // Güncelleme Butonu
            if (GuiButton((Rectangle){ (screenWidth - 260) / 2, 390, 260, 60 }, "VACCINATE PET")) {
                int targetID = atoi(inputVacID);

                // Her tıklamada durum mesajını sıfırla ki eski uyarının üstüne yazabilsin
                vacStatus = 0;

                int result = Update_Vaccination(petData, recordSize, targetID, inputVacDate);

                if (result == 1) {
                    vacStatus = 1; // Başarılı
                    strcpy(inputVacID, "");
                    strcpy(inputVacDate, "");
                } else if (result == -2) {
                    vacStatus = -2; // Geçersiz Tarih Hatası
                } else {
                    vacStatus = -1; // ID Bulunamadı Hatası
                }
            }

            // Geri Bildirim Mesajları (Tam olarak burada anlık basılacak)
            if (vacStatus == 1) {
                DrawText("Vaccination status updated successfully!", (screenWidth - MeasureText("Vaccination status updated successfully!", 24)) / 2, 500, 24, DARKGREEN);
            } else if (vacStatus == -1) {
                DrawText("Error: Pet ID not found!", (screenWidth - MeasureText("Error: Pet ID not found!", 24)) / 2, 500, 24, RED);
            } else if (vacStatus == -2) {
                DrawText("Error: Invalid Date! Check days (1-31) and months (1-12).", (screenWidth - MeasureText("Error: Invalid Date! Check days (1-31) and months (1-12).", 24)) / 2, 500, 24, RED);
            }

            if (GuiButton((Rectangle){ 40, 40, 110, 45 }, "< Back")) {
                currentState = STATE_MENU;
                vacStatus = 0; // Menüye dönerken temizle
            }
        }

        EndDrawing();
    }

    // Çıkışta verileri diske kaydet ve belleği serbest bırak
    Save_PetData(globalFileName, petData, recordSize);
    if (petData != NULL) free(petData);

    CloseWindow();
    return 0;
}