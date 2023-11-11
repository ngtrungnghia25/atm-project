#include "Server.c"

/**Mở màn hình đăng nhập*/
void ManHinhDangNhap();

/**Mở màn hình trang chủ*/
void ManHinhTrangChu();

/**Mở màn hình rút tiền*/
void RutTien();

/**Mở màn hình chuyển khoản*/
void ChuyenKhoan();

/**Mở màn hình in thông tin khách hàng*/
void ThongTin();

/**Mở màn hình khoá thẻ*/
void ManHinhKhoaThe();

/**Lưu lại người dùng khi đã đăng nhập*/
User mUser;

/**Chương trình chính*/
int main() {
    SetConsoleOutputCP(65001); //Dùng để hiển thị tiếng việt
    printf("Bạn đã mở chương trình %d lần...\n", readCount("ATM_open.txt"));
    system("pause"); //Dừng màn hình
    system("cls"); //Xoá màn hình
    ManHinhDangNhap();
    return 0;
}
void ManHinhDangNhap()
{
    char TaiKhoan[20];
    char MatKhau[20];
    printf("-------------ĐĂNG NHẬP------------\n");
    printf("Nhập vào tài khoản: ");
    scanf("%s", &TaiKhoan);
    printf("Nhập vào mật khẩu: ");
    scanf("%s", &MatKhau);
    int trangThaiDangNhap = login(TaiKhoan, MatKhau);
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
    printf("Chào %s\nSố tài khoản: %s\n\n", mUser.HoVaTen, mUser.SoTaiKhoan);

    printf("1. Rút tiền\n");
    printf("2. Chuyển khoản\n");
    printf("3. Lịch sử giao dịch\n");
    printf("4. Thông tin tài khoản\n");
    printf("5. Khoá thẻ\n");
    printf("6. Đăng xuất\n");

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
        InDanhSachLichSu(mUser.SoTaiKhoan);
        ManHinhTrangChu();
        break;
    case 4:
        system("cls"); //Xoá màn hình
        ThongTin();
        break;
    case 5:
        system("cls"); //Xoá màn hình
        ManHinhKhoaThe();
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
void RutTien()
{
    printf("-------------RÚT TIỀN------------\n");
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(mUser.SoTaiKhoan));

    //Nhập số tiền
    long SoTien;
    printf("Nhập số tiền cần rút: ");
    scanf("%ld", &SoTien);

    //Kiểm tra số tiền có lớn hơn 50000 và chia hết cho 10.000 không (sai nhập lại). Nếu nhập 0 thì quay lại
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
    
    //Kiểm tra số dư đủ không
    if (SoTien > KiemTraSoDu(mUser.SoTaiKhoan)) //Số dư không đủ
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        RutTien();
    } else //Số dư đủ 
    {
        int DemLanNhapMaPin = 0; //Đếm số lần nhập mã PIN
        
        //Nhập mã PIN;
        char MaPin[7];
        printf("Nhập mã Pin: ");
        scanf("%s", &MaPin);

        //Nhập sai, yêu cầu nhập lại (Khoá sau 5 lần)
        while (strcmp(MaPin, mUser.MaPin) != 0)
        {
            DemLanNhapMaPin++;
            if (DemLanNhapMaPin == 5) //Sai 5 lần (Khoá thẻ) -> Đăng xuất
            {
                KhoaThe(mUser.SoTaiKhoan);
                system("cls"); //Xoá màn hình
                printf(ANSI_COLOR_RED "Tài khoản đã bị khoá do nhập mã Pin sai 5 lần!\n");
                system("pause"); //Dừng màn hình
                system("cls"); //Xoá màn hình
                ManHinhDangNhap();
                return;
            }
            
            //Nhập lại mã PIN
            printf(ANSI_COLOR_RED "Mã PIN không đúng! (Còn %d lần)\n" ANSI_COLOR_RESET, 5 - DemLanNhapMaPin);
            printf("Nhập mã Pin: ");
            scanf("%s", &MaPin);
        }

        //Ghi lại lịch sử
        LichSu lichSu;
        strcpy(lichSu.TaiKhoan, mUser.SoTaiKhoan);
        lichSu.SoTien = -SoTien;
        strcpy(lichSu.LyDo, "Rút tiền");
        strcpy(lichSu.NguoiThucHien, mUser.HoVaTen);

        //Trừ tiền trong tài khoản
        TruTien(mUser.SoTaiKhoan, SoTien, lichSu);

        //Thông báo cho người dùng sau đó quay lại Trang Chủ
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
        LichSu lichSu;
        strcpy(lichSu.TaiKhoan, SoTaiKhoan);
        lichSu.SoTien = SoTien;
        char LyDo[100] = "Nhận tiền từ ";
        strcat(LyDo, mUser.HoVaTen);
        strcpy(lichSu.LyDo, LyDo);
        strcpy(lichSu.NguoiThucHien, mUser.HoVaTen);

        CongTien(SoTaiKhoan, SoTien, lichSu);

        // Trừ tiền bản thân
        LichSu lichSu1;
        strcpy(lichSu1.TaiKhoan, mUser.SoTaiKhoan);
        lichSu1.SoTien = (-SoTien);
        char LyDo1[100] = "Chuyển tiền đến ";
        strcat(LyDo1, TenNguoiNhan);
        strcpy(lichSu1.LyDo, LyDo1);
        strcpy(lichSu1.NguoiThucHien, mUser.HoVaTen);

        TruTien(mUser.SoTaiKhoan, SoTien, lichSu1);

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
    printf("Bạn có chắc muốn khoá thẻ?\n(1 - Tiếp tục | 0 - Huỷ): ");
    int XacNhan;
    scanf("%d", &XacNhan);
    if (XacNhan != 1)
    {
        ManHinhTrangChu();
        return;
    }
    
    KhoaThe(mUser.SoTaiKhoan);
    system("cls"); //Xoá màn hình
    printf(ANSI_COLOR_RED "Tài khoản của bạn đã bị khoá!\n" ANSI_COLOR_RESET);
    system("pause"); //Dừng màn hình
    system("cls"); //Xoá màn hình
    ManHinhDangNhap();
}
