#include <stdio.h>
#include <Windows.h>
#include "Server.c"

void RutTien();
void ChuyenKhoan();
void ThongTin();
void ManHinhKhoaThe();
void ManHinhDangNhap();
void ManHinhTrangChu();
User mUser;

int main() {
    SetConsoleOutputCP(65001); //Dùng để hiển thị tiếng việt
    printf("Bạn đã mở chương trình %d lần...", readCount("ATM_open.txt"));
    Sleep(2000);
    system("cls"); //Xoá màn hình
    ManHinhDangNhap();
    return 0;
}
void ManHinhDangNhap()
{
    // char TaiKhoan[4] = "001";
    // char MatKhau[20] = "mk1";
    char TaiKhoan[4];
    char MatKhau[20];
    printf("-------------ĐĂNG NHẬP------------\n");
    printf("Nhập vào tài khoản: ");
    scanf("%s", &TaiKhoan);
    printf("Nhập vào mật khẩu: ");
    scanf("%s", &MatKhau);
    int trangThaiDangNhap = Login(TaiKhoan, MatKhau);
    if (trangThaiDangNhap == 0) //Đăng nhập thất bại
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Tài khoản hoặc mật khẩu không chính xác!\n" ANSI_COLOR_RESET);
        ManHinhDangNhap(); //Cho đăng nhập lại
    }
    else if (trangThaiDangNhap == -1) //Thẻ bị khoá
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Thẻ của bạn đã bị khoá. Vui lòng đến ngân hàng để mở khoá!\n" ANSI_COLOR_RESET);
        ManHinhDangNhap(); //Cho đăng nhập lại
    }
    else // Đăng nhập thành công
    {
        system("cls"); //Xoá màn hình
        mUser = getUserByID(TaiKhoan);
        ManHinhTrangChu();
    }
}
void ManHinhTrangChu()
{
    system("cls");
    printf("Chào %s\n", mUser.HoVaTen);

    printf("1. Rút tiền\n");
    printf("2. Chuyển khoản\n");
    printf("3. Thông tin tài khoản\n");
    printf("4. Khoá thẻ\n");
    printf("5. Đăng xuất\n");

    printf("Nhập sự lựa chọn: ");
    int LuaChon;
    scanf("%d", &LuaChon);

    switch (LuaChon)
    {
    case 1:
        system("cls"); //Xoá màn hình
        RutTien();
        break;
    case 2:
        system("cls"); //Xoá màn hình
        ChuyenKhoan();
        break;
    case 3:
        system("cls"); //Xoá màn hình
        ThongTin();
        break;
    case 4:
        system("cls"); //Xoá màn hình
        ManHinhKhoaThe();
        break;
    case 5:
        system("cls"); //Xoá màn hình
        ManHinhDangNhap();
        break;
    default:
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
        break;
    }
}
void RutTien()
{
    printf("-------------RÚT TIỀN------------\n");
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.SoTaiKhoan));
    long SoTien;
    printf("Nhập số tiền cần rút: ");
    scanf("%ld", &SoTien);
    while ((SoTien % 10000 != 0 || SoTien < 50000) && SoTien != 0)
    {
        printf(ANSI_COLOR_RED "Số tiền phải chia hết cho 10.000 và lớn hơn 50.000\n" ANSI_COLOR_RESET);
        printf("Nhập số tiền cần rút: ");
        scanf("%ld", &SoTien);
    }
    if (SoTien == 0)
    {
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
        return;
    }
    if (SoTien > KiemTraSoDu(mUser.SoTaiKhoan))
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        RutTien();
    } else
    {
        int DemLanNhapMaPin = 0;
        char MaPin[7];
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);
        while (strcmp(MaPin, mUser.MaPin) != 0)
        {
            DemLanNhapMaPin++;
            if (DemLanNhapMaPin == 5)
            {
                KhoaThe(mUser.SoTaiKhoan);
                system("cls"); //Xoá màn hình
                printf(ANSI_COLOR_RED "Tài khoản đã bị khoá do nhập mã Pin sai 5 lần!\n");
                system("pause");
                return;
            }
            
            printf(ANSI_COLOR_RED "Mã PIN không đúng! (Còn %d lần)\n" ANSI_COLOR_RESET, 5 - DemLanNhapMaPin);
            printf("Nhập mã Pin: ");
            scanf("%s", &MaPin);
        }
        //Trừ tiền trong tài khoản
        TruTien(mUser.SoTaiKhoan, SoTien);
        printf(ANSI_COLOR_GREEN "Bạn đã rút thành công %ld VND\n" ANSI_COLOR_RESET, SoTien);
        printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.SoTaiKhoan));
        system("pause");
        ManHinhTrangChu();
    }
    
}
void ChuyenKhoan()
{
    printf("-------------CHUYỂN KHOẢN------------\n");
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.SoTaiKhoan));

    char SoTaiKhoan[20];
    printf("Nhập số tài khoản cần chuyển: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản cần chuyển: ");
        scanf("%s", &SoTaiKhoan);
    }

    char TenNguoiNhan[30];
    strcpy(TenNguoiNhan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên người nhận: %s\n", TenNguoiNhan);

    float SoTien;
    printf("Nhập số tiền cần chuyển: ");
    scanf("%f", &SoTien);
    while (SoTien > KiemTraSoDu(mUser.SoTaiKhoan))
    {
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        printf("Nhập số tiền cần chuyển: ");
        scanf("%f", &SoTien);
    } 
     
    int DemLanNhapMaPin = 0;
    char MaPin[7];
    printf("Nhập mã Pin: ");
    scanf("%s", &MaPin);
    while (strcmp(MaPin, mUser.MaPin) != 0)
    {
        DemLanNhapMaPin++;
        if (DemLanNhapMaPin == 5)
        {
            KhoaThe(mUser.SoTaiKhoan);
            system("cls"); //Xoá màn hình
            printf(ANSI_COLOR_RED "Tài khoản đã bị khoá do nhập mã Pin sai 5 lần!\n");
            system("pause");
            return;
        }
        printf(ANSI_COLOR_RED "Mã PIN không đúng! (Còn %d lần)\n" ANSI_COLOR_RESET, 5 - DemLanNhapMaPin);
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);
    }
    printf("Bạn có chắc muốn chuyển %.2f đến %s? (1 - Tiếp tục | 0 - Huỷ): ", SoTien, TenNguoiNhan);
    int XacNhan;
    scanf("%d", &XacNhan);
    if (XacNhan == 1)
    {
        //Cộng tiền cho người nhận
        CongTien(SoTaiKhoan, SoTien);
        TruTien(mUser.SoTaiKhoan, SoTien);
        // Trừ tiền bản thân
        printf(ANSI_COLOR_GREEN "Chuyển khoản thành công!\n" ANSI_COLOR_RESET);
        system("pause");
        ManHinhTrangChu();
    }
    else
    {
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    
}
void ThongTin()
{
    printf("-------------THÔNG TIN TÀI KHOẢN------------\n");
    printUserData(getUserByID(mUser.SoTaiKhoan));
    system("pause");
    system("cls"); //Xoá màn hình
    ManHinhTrangChu();
}
void ManHinhKhoaThe()
{
    printf("-------------KHOÁ THẺ------------\n");
    printf("Bạn có chắc khoá thẻ? (1 - Tiếp tục | 0 - Huỷ): ");
    int XacNhan;
    scanf("%d", &XacNhan);
    int DemLanNhapMaPin = 0;
    char MaPin[7];
    printf("Nhập mã Pin: ");
    scanf("%s", &MaPin);
    while (strcmp(MaPin, mUser.MaPin) != 0)
    {
        DemLanNhapMaPin++;
        if (DemLanNhapMaPin == 5)
        {
            KhoaThe(mUser.SoTaiKhoan);
            system("cls"); //Xoá màn hình
            printf(ANSI_COLOR_RED "Tài khoản đã bị khoá do nhập mã Pin sai 5 lần!\n");
            system("pause");
            return;
        }
        
        printf(ANSI_COLOR_RED "Mã PIN không đúng! (Còn %d lần)\n" ANSI_COLOR_RESET, 5 - DemLanNhapMaPin);
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);
    }
    KhoaThe(mUser.SoTaiKhoan);
    system("cls"); //Xoá màn hình
    printf(ANSI_COLOR_RED "Tài khoản của bạn đã bị khoá!\n" ANSI_COLOR_RESET);
    system("pause"); //Dừng màn hình
    system("cls"); //Xoá màn hình
    ManHinhDangNhap();
}