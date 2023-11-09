#define _GNU_SOURCE
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BUFFER_SIZE 1000
#define FILENAME_SIZE 1024
#define MAX_LINE 2048
#define BUFFER 1024

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int TonTai;
    char TaiKhoan[20];
    char MatKhau[20];
    char HoVaTen[30];
    char CCCD[13];
} NhanVien;

typedef struct
{
    int ViTri;
    int TonTai;
    char SoTaiKhoan[4];
    char HoVaTen[30];
    float SoDu;
    char CCCD[13];
    int TrangThai;
    char DiaChi[100];
    char NgaySinh[11];
    char MaPin[7];
    char MatKhau[20];
} User;

void createUser(User user);
void UpdateUser(User user);
User getUserByID(char SoTaiKhoan[]);
NhanVien getNhanVienByID(char SoTaiKhoan[]);
int Login(char username[], char password[]);
void printUserData(User user);
void replaceLineInFile(char *filename, int lineIndex, char *newString);
float KiemTraSoDu(char SoTaiKhoan[]);
void KhoaThe(char SoTaiKhoan[]);
char *UserToString(const User *user);

void printUserData(User user)
{
    printf("Số tài khoản:             %s\n", user.SoTaiKhoan);
    printf("Họ và tên:                %s\n", user.HoVaTen);
    printf("Số dư:                    %.2f\n", user.SoDu);
    printf("Số CCCD:                  %s\n", user.CCCD);

    printf("Trạng thái:               ");
    if (user.TrangThai == 0) //Bị khoá
        printf(ANSI_COLOR_RED "Bị khoá\n" ANSI_COLOR_RESET);
    else 
        printf(ANSI_COLOR_GREEN "Hoạt động\n" ANSI_COLOR_RESET);
    
    printf("Địa chỉ:                  %s\n", user.DiaChi);
    printf("Ngày sinh:                %s\n", user.NgaySinh);
}
void createUser(User user)
{
    FILE *file = fopen("Users.txt", "a");
    fprintf(file, "%s", UserToString(&user));
    fclose(file);
}
int Login(char username[], char password[])
{
    User user = getUserByID(username);
    if (user.TonTai == 0) //Tài khoản không tồn tại
        return 0;
    if (strcmp(user.MatKhau, password) == 0 && user.TrangThai == 1) //Mật khẩu đúng
        return 1;
    if (strcmp(user.MatKhau, password) == 0 && user.TrangThai == 0) //Thẻ bị khoá
        return -1;
    return 0;
}
int LoginNhanVien(char username[], char password[])
{
    NhanVien nhanVien = getNhanVienByID(username);
    if (nhanVien.TonTai == 0) //Tài khoản không tồn tại
        return 0;
    if (strcmp(nhanVien.MatKhau, password) == 0) //Mật khẩu đúng
        return 1;
    return 0;
}
float KiemTraSoDu(char SoTaiKhoan[])
{
    return getUserByID(SoTaiKhoan).SoDu;
}
void KhoaThe(char SoTaiKhoan[])
{
    User user = getUserByID(SoTaiKhoan);
    user.TrangThai  = 0;
    UpdateUser(user);
}
void MoThe(char SoTaiKhoan[])
{
    User user = getUserByID(SoTaiKhoan);
    user.TrangThai  = 1;
    UpdateUser(user);
}

void CongTien(char SoTaiKhoan[], float SoTien)
{
    User user = getUserByID(SoTaiKhoan);
    user.SoDu += SoTien;
    UpdateUser(user);
}

void TruTien(char SoTaiKhoan[], float SoTien)
{
    User user = getUserByID(SoTaiKhoan);
    user.SoDu -= SoTien;
    UpdateUser(user);
}

User getUserByID(char SoTaiKhoan[])
{
    //https://stackoverflow.com/questions/3501338/c-read-file-line-by-line 
    User user;
    user.TonTai = 0;
    FILE * fp;
    char * line = NULL; //value (string) of line
    size_t len = 0;
    ssize_t read; //lenght of line

    fp = fopen("Users.txt", "r");
    int row_index = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, "\t");
        row_index++;
        if (strcmp(token, SoTaiKhoan) != 0)
            continue;
        user.TonTai = 1;
        user.ViTri = row_index - 1;
        strcpy(user.SoTaiKhoan, token);
        int i = 0;
        while (token != NULL)
        {        
            if (i == 1)
                strcpy(user.HoVaTen, token);
            else if (i == 2)
                user.SoDu = atof(token);
            else if (i == 3)
                strcpy(user.CCCD, token);
            else if (i == 4)
                user.TrangThai = atoi(token);
            else if (i == 5)
                strcpy(user.DiaChi, token);
            else if (i == 6)
                strcpy(user.NgaySinh, token);
            else if (i == 7)
                strcpy(user.MaPin, token);
            else if (i == 8)
                strcpy(user.MatKhau, token);
            token = strtok(NULL, "\t");
            i++;
        }
        user.MatKhau[strlen(user.MatKhau) - 1] = '\0';
        fclose(fp);
        return user;
    }

    fclose(fp);
    if (line)
        free(line);
    return user;
}

NhanVien getNhanVienByID(char TaiKhoan[])
{
    //https://stackoverflow.com/questions/3501338/c-read-file-line-by-line 
    NhanVien nhanVien;
    nhanVien.TonTai = 0;
    FILE * fp;
    char * line = NULL; //value (string) of line
    size_t len = 0;
    ssize_t read; //lenght of line

    fp = fopen("AdminAccount.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);
    int row_index = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, "\t");
        row_index++;
        if (strcmp(token, TaiKhoan) != 0)
            continue;
        strcpy(nhanVien.TaiKhoan, token);
        nhanVien.TonTai = 1;
        int i = 0;
        while (token != NULL)
        {        
            if (i == 1)
                strcpy(nhanVien.MatKhau, token);
            else if (i == 2)
                strcpy(nhanVien.HoVaTen, token);
            else if (i == 3)
                strcpy(nhanVien.CCCD, token);
            token = strtok(NULL, "\t");
            i++;
        }
        nhanVien.CCCD[strlen(nhanVien.CCCD) - 1] = '\0';
        return nhanVien;
    }

    fclose(fp);
    if (line)
        free(line);
    exit(EXIT_SUCCESS);
    return nhanVien;
}
char *UserToString(const User *user) 
{
    int requiredSize = snprintf(NULL, 0, "%s\t%s\t%f\t%s\t%d\t%s\t%s\t%s\t%s\n",
                                user->SoTaiKhoan, user->HoVaTen, user->SoDu, user->CCCD, user->TrangThai, user->DiaChi, user->NgaySinh, user->MaPin, user->MatKhau);

    char *result = (char *)malloc(requiredSize + 1);
    if (result) {
        snprintf(result, requiredSize + 1, "%s\t%s\t%f\t%s\t%d\t%s\t%s\t%s\t%s\n",
                                user->SoTaiKhoan, user->HoVaTen, user->SoDu, user->CCCD, user->TrangThai, user->DiaChi, user->NgaySinh, user->MaPin, user->MatKhau);
    }
    return result;
}
void UpdateUser(User user)
{
    replaceLineInFile("Users.txt", user.ViTri, UserToString(&user));
}

void replaceLineInFile(char *filename, int lineIndex, char *newString) {
    FILE *file, *temp;
    char buffer[512];

    file = fopen(filename, "r");
    temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf("Error opening files!\n");
        return;
    }
    int count = 0;
    while ((fgets(buffer, 512, file)) != NULL) {
        if (lineIndex == count)
            strcpy(buffer, newString);
        count++;
        fputs(buffer, temp);
    }

    fclose(temp);
    fclose(file);
    fclose(file);

    if(remove(filename) != 0)
        perror("Error deleting file");
    rename("temp.txt", filename);
}
