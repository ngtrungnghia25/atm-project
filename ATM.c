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
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.ID));
    int SoTien;
    printf("Nhập số tiền cần rút: ");
    scanf("%d", &SoTien);
    while (SoTien % 10000 != 0 || SoTien < 50000)
    {
        printf(ANSI_COLOR_RED "Số tiền phải chia hết cho 10.000 và lớn hơn 50.000\n" ANSI_COLOR_RESET);
        printf("Nhập số tiền cần rút: ");
        scanf("%d", &SoTien);
    }
    if (SoTien > KiemTraSoDu(mUser.ID))
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        RutTien();
    } else
    {
        //Trừ tiền trong tài khoản
        TruTien(mUser.ID, SoTien);
        printf("Bạn đã rút thành công %.2f VND\n", SoTien);
        printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.ID));
        system("pause");
        ManHinhTrangChu();
    }
    
}
void ChuyenKhoan()
{
    printf("-------------CHUYỂN KHOẢN------------\n");
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.ID));

    char ID[20];
    printf("Nhập số tài khoản cần chuyển: ");
    scanf("%s", &ID);
    while (getUserByID(ID).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản cần chuyển: ");
        scanf("%s", &ID);
    }

    char TenNguoiNhan[30];
    strcpy(TenNguoiNhan, getUserByID(ID).HoVaTen);
    printf("Tên người nhận: %s\n", TenNguoiNhan);

    float SoTien;
    printf("Nhập số tiền cần chuyển: ");
    scanf("%f", &SoTien);
    while (SoTien > KiemTraSoDu(mUser.ID))
    {
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        printf("Nhập số tiền cần chuyển: ");
        scanf("%f", &SoTien);
    } 
     
    char MaPin[7];
    printf("Nhập mã Pin: ");
    scanf("%s", &MaPin);
    while (strcmp(MaPin, mUser.MaPin) != 0)
    {
        printf(ANSI_COLOR_RED "Mã PIN không đúng!\n" ANSI_COLOR_RESET);
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);
    }
    printf("Bạn có chắc muốn chuyển %.2f đến %s? (1 - Tiếp tục | 0 - Huỷ): ", SoTien, TenNguoiNhan);
    int XacNhan;
    scanf("%d", &XacNhan);
    if (XacNhan == 1)
    {
        //Cộng tiền cho người nhận
        // Trừ tiền bản thân
        printf(ANSI_COLOR_GREEN "Chuyển khoản thành công!\n" ANSI_COLOR_RESET);
        system("pause");
        ManHinhTrangChu();
    }
    else
    {
        system("cls"); //Xoá màn hình
        ChuyenKhoan();
    }
    
}
void ThongTin()
{
    printf("-------------THÔNG TIN TÀI KHOẢN------------\n");
    printUserData(mUser);
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
    char MaPin[7];
    printf("Nhập mã Pin: ");
    scanf("%s", &MaPin);
    while (strcmp(MaPin, mUser.MaPin) != 0)
    {
        printf(ANSI_COLOR_RED "Mã PIN không đúng!\n" ANSI_COLOR_RESET);
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);
    }

}