#include <stdio.h>
#include <Windows.h>
#include "Server.c"

void NapTien();
void RutTien();
void TaoTaiKhoan();
void ManHinhKhoaThe();
void ManHinhMoThe();
void ManHinhDangNhap();
void ManHinhTrangChu();

NhanVien nhanVien;
int main() {
    SetConsoleOutputCP(65001); //Dùng để hiển thị tiếng việt
    ManHinhDangNhap();
    return 0;
}
void ManHinhDangNhap()
{
    char TaiKhoan[4];
    char MatKhau[20];
    printf("-------------ĐĂNG NHẬP------------\n");
    printf("Nhập vào tài khoản: ");
    scanf("%s", &TaiKhoan);
    printf("Nhập vào mật khẩu: ");
    scanf("%s", &MatKhau);
    int trangThaiDangNhap = LoginNhanVien(TaiKhoan, MatKhau);
    if (trangThaiDangNhap == 0) //Đăng nhập thất bại
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Tài khoản hoặc mật khẩu không chính xác!\n" ANSI_COLOR_RESET);
        ManHinhDangNhap(); //Cho đăng nhập lại
    }
    else // Đăng nhập thành công
    {
        system("cls"); //Xoá màn hình
        nhanVien = getNhanVienByID(TaiKhoan);
        ManHinhTrangChu();
    }
}
void ManHinhTrangChu()
{
    system("cls");
    printf("Chào %s\n", nhanVien.HoVaTen);

    printf("1. Nạp tiền vào tài khoản\n");
    printf("2. Rút tiền tại ngân hàng\n");
    printf("3. Tạo tài khoản\n");
    printf("4. Khoá thẻ\n");
    printf("5. Mở thẻ\n");
    printf("6. Đăng xuất\n");

    printf("Nhập sự lựa chọn: ");
    int LuaChon;
    scanf("%d", &LuaChon);

    switch (LuaChon)
    {
    case 1:
        system("cls"); //Xoá màn hình
        NapTien();
        break;
    case 2:
        system("cls"); //Xoá màn hình
        RutTien();
        break;
    case 3:
        system("cls"); //Xoá màn hình
        TaoTaiKhoan();
        break;
    case 4:
        system("cls"); //Xoá màn hình
        ManHinhKhoaThe();
        break;
    case 5:
        system("cls"); //Xoá màn hình
        ManHinhMoThe();
        break;
    case 6:
        system("cls"); //Xoá màn hình
        ManHinhDangNhap();
        break;
    default:
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
        break;
    }
}
void NapTien()
{
    char SoTaiKhoan[20];
    printf("-------------NẠP TIỀN------------\n");
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    char TenNguoiNhan[30];
    strcpy(TenNguoiNhan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên người nhận: %s\n", TenNguoiNhan);

    float SoTien;
    printf("Nhập số tiền cần nạp: ");
    scanf("%f", &SoTien);

    int XacNhan;
    printf("Bạn có muốn tiếp tục? (1 - Tiếp tục | 0 - Quay lại trang chủ): ");
    scanf("%d", &XacNhan);

    if (XacNhan == 1)
    {
        printf(ANSI_COLOR_GREEN "Nạp tiền thành công!\n" ANSI_COLOR_RESET);
        CongTien(SoTaiKhoan, SoTien);
        system("pause");
        ManHinhTrangChu();
    }
    else
    {
        system("cls"); //Xoá màn hình
        NapTien();
    }
    
}
void RutTien()
{
    printf("-------------RÚT TIỀN------------\n");

    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(SoTaiKhoan));
    int SoTien;
    printf("Nhập số tiền cần rút: ");
    scanf("%d", &SoTien);
    while (SoTien % 10000 != 0 || SoTien < 50000)
    {
        printf(ANSI_COLOR_RED "Số tiền phải chia hết cho 10.000 và lớn hơn 50.000\n" ANSI_COLOR_RESET);
        printf("Nhập số tiền cần rút: ");
        scanf("%d", &SoTien);
    }
    if (SoTien > KiemTraSoDu(SoTaiKhoan))
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        RutTien();
    } else
    {
        //Trừ tiền trong tài khoản
        TruTien(SoTaiKhoan, SoTien);
        printf("Đã rút thành công %.2f VND\n", SoTien);
        printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(SoTaiKhoan));
        system("pause");
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
}
void TaoTaiKhoan()
{
    User user;
    printf("-------------TẠO TÀI KHOẢN------------\n");
    printf("Nhập Họ và tên: ");
}
void ManHinhKhoaThe()
{
    printf("-------------KHOÁ THẺ------------\n");
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);

    if (getUserByID(SoTaiKhoan).TrangThai == 0)
    {
        printf("Thẻ này đã bị khoá!");
        system("pause");
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    else
    {
        int XacNhan;
        printf("Bạn có muốn khoá thẻ? (1 - Tiếp tục | 0 - Quay lại trang chủ): ");
        scanf("%d", &XacNhan);
        if (XacNhan == 1)
        {
            KhoaThe(SoTaiKhoan);
            printf("Khoá thẻ thành công!");
            system("pause");
            system("cls"); //Xoá màn hình
            ManHinhTrangChu();
        }
        else
        {
            system("cls"); //Xoá màn hình
            ManHinhTrangChu();
        }
    }
    
}
void ManHinhMoThe()
{
    printf("-------------MỞ THẺ------------\n");
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);

    if (getUserByID(SoTaiKhoan).TrangThai == 1)
    {
        printf("Thẻ này chưa bị khoá!");
        system("pause");
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    else
    {
        int XacNhan;
        printf("Bạn có muốn mở thẻ? (1 - Tiếp tục | 0 - Quay lại trang chủ): ");
        scanf("%d", &XacNhan);
        if (XacNhan == 1)
        {
            MoThe(SoTaiKhoan);
            printf("Mở thẻ thành công!");
            system("pause");
            system("cls"); //Xoá màn hình
            ManHinhTrangChu();
        }
        else
        {
            system("cls"); //Xoá màn hình
            ManHinhTrangChu();
        }
    }
}