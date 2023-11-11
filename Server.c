#define _GNU_SOURCE
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BUFFER 1024

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

typedef struct
{
    char TaiKhoan[20];
    char NguoiThucHien[30];
    char time[20];
    float SoTien;
    char LyDo[100];

} LichSu;
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
    char SoTaiKhoan[20];
    char HoVaTen[30];
    float SoDu;
    char CCCD[13];
    int TrangThai;
    char DiaChi[100];
    char NgaySinh[11];
    char MaPin[7];
    char MatKhau[20];
} User;

/**Lấy ra User dựa trên SỐ TÀI KHOẢN*/
User getUserByID(char SoTaiKhoan[]);

/**Lấy ra Nhân Viên dựa trên SỐ TÀI KHOẢN*/
NhanVien getNhanVienByID(char SoTaiKhoan[]);

/**Tạo tài khoản*/
void createUser(User user);

/**Đăng nhập người dùng*/
int login(char username[], char password[]);

/**Đăng nhập nhân viên*/
int loginNhanVien(char username[], char password[]);

/**Cập nhật dữ liệu User*/
void updateUser(User user);

/**In thông tin User*/
void printUserData(User user);

/**Kiểm tra số dư từ SỐ TÀI KHOẢN*/
float KiemTraSoDu(char SoTaiKhoan[]);

/**Khoá thẻ theo SỐ TÀI KHOẢN*/
void KhoaThe(char SoTaiKhoan[]);

/**Cộng tiền vào tài khoản và ghi lại lịch sử*/
void CongTien(char SoTaiKhoan[], float SoTien, LichSu lichSu);

/**Trừ tiền vào tài khoản và ghi lại lịch sử*/
void TruTien(char SoTaiKhoan[], float SoTien, LichSu lichSu);

/**In lịch sử giao dịch ra màn hình*/
void InLichSu(LichSu lichSu);

/**Load lịch sử giao dịch và in ra màn hình bằng hàm InLichSu*/
void InDanhSachLichSu(char SoTaiKhoan[]);

/**Ghi lại lịch sử giao dịch*/
void GhiLaiLichSu(LichSu lichSu);

/**Chuyển User thành chuỗi*/
char *userToString(const User *user);

/**Chuyển LichSu thành chuỗi*/
char *lichSuToString(const LichSu *lichSu);

/**Thay đổi 1 dòng trong File*/
void replaceLineInFile(char *filename, int lineIndex, char *newString);

/**Ghi lại só lần mở phần mềm*/
void writeCount(char fileName[], int count)
{
    FILE *fptr;
    fptr = fopen(fileName, "w");
    if (fptr != NULL)
    {
        fprintf(fptr, "%d", count);
        fclose(fptr);
    }
}

/**Xem số lần mở phần mềm -> sau đó cộng thêm 1 lần và ghi lại*/
int readCount(char fileName[])
{
    FILE *fptr;
    fptr = fopen(fileName, "r");
    if (fptr == NULL) //first open
    {
        fclose(fptr);
        writeCount(fileName, 1);
        return 1;
    }
    int count = 1;
    fscanf(fptr, "%d", &count);
    fclose(fptr);
    writeCount(fileName, count + 1);
    return count;
}

/**Xác nhận người dùng có muốn tiếp tục không [1 - Tiếp tục | 0 - Quay lại]*/
int YeuCauXacNhan()
{
    int XacNhan;
    printf("Bạn có muốn tiếp tục?\n(1 - Tiếp tục | 0 - Quay lại): ");
    scanf("%d", &XacNhan);
    return XacNhan;
}

void printUserData(User user)
{
    printf("Số tài khoản:             %s\n", user.SoTaiKhoan);
    printf("Họ và tên:                %s\n", user.HoVaTen);
    printf("Số dư:                    ");
    printf(ANSI_COLOR_GREEN "%.2f\n"ANSI_COLOR_RESET, user.SoDu);
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
    fprintf(file, "%s", userToString(&user));
    fclose(file);
}
void updateUser(User user)
{
    replaceLineInFile("Users.txt", user.ViTri, userToString(&user));
}
int login(char username[], char password[])
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
int loginNhanVien(char username[], char password[])
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
    updateUser(user);
}
void MoThe(char SoTaiKhoan[])
{
    User user = getUserByID(SoTaiKhoan);
    user.TrangThai  = 1;
    updateUser(user);
}
void CongTien(char SoTaiKhoan[], float SoTien, LichSu lichSu)
{
    User user = getUserByID(SoTaiKhoan);
    user.SoDu += SoTien;
    updateUser(user);
    GhiLaiLichSu(lichSu);
}
void TruTien(char SoTaiKhoan[], float SoTien, LichSu lichSu)
{
    User user = getUserByID(SoTaiKhoan);
    user.SoDu -= SoTien;
    updateUser(user);
    GhiLaiLichSu(lichSu);
}
void InLichSu(LichSu lichSu)
{
    printf("Thời gian:                 %s\n", lichSu.time);
    printf("Số tiền:                   ");
    if (lichSu.SoTien < 0)
        printf(ANSI_COLOR_RED "%.2f\n" ANSI_COLOR_RESET, lichSu.SoTien);
    else printf(ANSI_COLOR_GREEN "%.2f\n" ANSI_COLOR_RESET, lichSu.SoTien);
    
    printf("Lý Do:                     %s\n", lichSu.LyDo);
    printf("Người thực hiện:           %s\n\n", lichSu.NguoiThucHien);
}
void InDanhSachLichSu(char SoTaiKhoan[])
{
    system("cls"); //Xoá màn hình
    printf("-------------LỊCH SỬ GIAO DỊCH------------\n\n");
    printf("Số tài khoản: %s\n", SoTaiKhoan);
    printf("Chủ tài khoản: %s\n\n", getUserByID(SoTaiKhoan).HoVaTen);

    FILE * fp;
    char * line = NULL; 
    size_t len = 0;
    ssize_t read;
    fp = fopen("LichSu.txt", "r");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    
    fp = fopen("LichSu.txt", "r");

    int count = 0;
    if (size > 0) 
        while ((read = getline(&line, &len, fp)) != -1) {
            char *token = strtok(line, "\t");
            if (strcmp(token, SoTaiKhoan) != 0)
                continue;
            LichSu lichSu;
            int i = 0;
            while (token != NULL)
            {        
                if (i == 1)
                    strcpy(lichSu.time, token);
                else if (i == 2)
                    lichSu.SoTien = atof(token);
                else if (i == 3)
                    strcpy(lichSu.LyDo, token);
                else if (i == 4)
                    strcpy(lichSu.NguoiThucHien, token);
                token = strtok(NULL, "\t");
                i++;
            }
            lichSu.NguoiThucHien[strlen(lichSu.NguoiThucHien) - 1] = '\0';
            InLichSu(lichSu);
            count++;
        }

    fclose(fp);
    if (line)
        free(line);
    if (count == 0)
        printf(ANSI_COLOR_RED "Chưa có giao dịch nào\n" ANSI_COLOR_RESET);
    system("pause"); //Dừng màn hình
}
void GhiLaiLichSu(LichSu lichSu)
{
    time_t timer;
    char buffer[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    strcpy(lichSu.time, buffer);

    // printf("%s", LichSuToString(&lichSu));
    FILE *file = fopen("LichSu.txt", "a");
    fprintf(file, "%s", lichSuToString(&lichSu));
    fclose(file);
}
char *userToString(const User *user) 
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
char *lichSuToString(const LichSu *lichSu) 
{
    int requiredSize = snprintf(NULL, 0, "%s\t%s\t%f\t%s\t%s\n", lichSu->TaiKhoan, lichSu->time, lichSu->SoTien, lichSu->LyDo, lichSu->NguoiThucHien);

    char *result = (char *)malloc(requiredSize + 1);
    if (result) {
        snprintf(result, requiredSize + 1, "%s\t%s\t%f\t%s\t%s\n", lichSu->TaiKhoan, lichSu->time, lichSu->SoTien, lichSu->LyDo, lichSu->NguoiThucHien);
    }
    return result;
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
User getUserByID(char SoTaiKhoan[])
{
    User user;
    user.TonTai = 0;
    FILE * fp;
    char * line = NULL; //value (string) of line
    size_t len = 0;
    ssize_t read; //lenght of line

    fp = fopen("Users.txt", "r");

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    
    fp = fopen("Users.txt", "r");
    int row_index = 0;
    if (size > 0) 
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

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    
    fp = fopen("AdminAccount.txt", "r");
    int row_index = 0;
    if (size > 0) 
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