#include "Server.c"

/**Mở màn hình đăng nhập tài khoản nhân viên*/
void ManHinhDangNhap();

/**Mở màn hình trang chủ*/
void ManHinhTrangChu();

/**Mở màn hình nạp tiền cho khách hàng*/
void NapTien();

/**Mở màn hình rút tiền*/
void RutTien();

/**Mở màn hình xem lịch sử giao dịch của khác hàng*/
void XemLichSuGiaoDich();

/**Mở màn hình tạo tài khoản khách hàng*/
void TaoTaiKhoan();

/**Mở màn hình khoá thẻ khách hàng*/
void ManHinhKhoaThe();

/**Mở màn hình mở thẻ khách hàng*/
void ManHinhMoThe();

/**Lưu lại Nhân viên đã đăng nhập*/
NhanVien nhanVien;

/**Chương trình chính*/
int main() {
    SetConsoleOutputCP(65001); //Dùng để hiển thị tiếng việt
    printf("Bạn đã mở chương trình %d lần...\n", readCount("NhanVien_open.txt"));
    system("pause"); //Dừng màn hình
    system("cls"); //Xoá màn hình
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
    int trangThaiDangNhap = loginNhanVien(TaiKhoan, MatKhau);
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
    printf("3. Xem lịch sử giao dịch khách hàng\n");
    printf("4. Tạo tài khoản\n");
    printf("5. Khoá thẻ\n");
    printf("6. Mở thẻ\n");
    printf("7. Đăng xuất\n");

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
        XemLichSuGiaoDich();
        break;
    case 4:
        system("cls"); //Xoá màn hình
        TaoTaiKhoan();
        break;
    case 5:
        system("cls"); //Xoá màn hình
        ManHinhKhoaThe();
        break;
    case 6:
        system("cls"); //Xoá màn hình
        ManHinhMoThe();
        break;
    case 7:
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
    printf("-------------NẠP TIỀN------------\n");
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    //Hiện tên người nhận
    char TenNguoiNhan[30];
    strcpy(TenNguoiNhan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên người nhận: %s\n", TenNguoiNhan);

    //Yêu cầu nhập số tiền
    long SoTien;
    printf("Nhập số tiền cần nạp: ");
    scanf("%ld", &SoTien);

    //Xác nhận tiếp tục
    int XacNhan = YeuCauXacNhan();
    if (XacNhan == 1)
    {
        //Ghi lại lịch sử
        LichSu lichSu;
        strcpy(lichSu.TaiKhoan, SoTaiKhoan);
        lichSu.SoTien = SoTien;
        strcpy(lichSu.LyDo, "Nạp tiền vào tài khoản");
        strcpy(lichSu.NguoiThucHien, nhanVien.HoVaTen);

        //Cộng tiền vào tài khoản
        CongTien(SoTaiKhoan, SoTien, lichSu);

        printf(ANSI_COLOR_GREEN "Nạp tiền thành công!\n" ANSI_COLOR_RESET);
        system("pause"); //Dừng màn hình
        ManHinhTrangChu();
    }
    else
    {
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    
}
void RutTien()
{
    printf("-------------RÚT TIỀN------------\n");
    
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    //Hiện tên chủ tài khoản và số dư
    char ChuTaiKhoan[30];
    strcpy(ChuTaiKhoan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên chủ tài khoản: %s\n", ChuTaiKhoan);
    printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(SoTaiKhoan));

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
    if (SoTien > KiemTraSoDu(SoTaiKhoan)) //Số dư không đủ
    {
        system("cls"); //Xoá màn hình
        printf(ANSI_COLOR_RED "Số dư không đủ\n" ANSI_COLOR_RESET);
        RutTien();
    } 
    else //Số dư đủ
    {
        //Ghi lại lịch sử
        LichSu lichSu;
        strcpy(lichSu.TaiKhoan, SoTaiKhoan);
        lichSu.SoTien = -SoTien;
        strcpy(lichSu.LyDo, "Rút tiền tại ngân hàng");
        strcpy(lichSu.NguoiThucHien, nhanVien.HoVaTen);

        //Trừ tiền trong tài khoản
        TruTien(SoTaiKhoan, (long)SoTien, lichSu);

        printf(ANSI_COLOR_GREEN "Đã rút thành công %ld VND\n" ANSI_COLOR_RESET, SoTien);
        printf("Số dư hiện tại là %.2f VND\n", KiemTraSoDu(SoTaiKhoan));
        system("pause");
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
}
void XemLichSuGiaoDich()
{
    printf("-------------XEM LỊCH SỬ GIAO DỊCH------------\n");
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    system("cls"); //Xoá màn hình
    InDanhSachLichSu(SoTaiKhoan);
    ManHinhTrangChu();
}
void TaoTaiKhoan()
{
    User user;
    printf("-------------TẠO TÀI KHOẢN------------\n");

    const char ALLOWED[] = "0123456789";
    char random[3+1];
    int i = 0;
    int c = 0;
    int nbAllowed = sizeof(ALLOWED)-1;
    for(i = 0; i < 3; i++) {
        c = rand() % nbAllowed ;
        random[i] = ALLOWED[c];
    }
    random[3] = '\0';
    strcpy(user.SoTaiKhoan, random);
    if (getUserByID(user.SoTaiKhoan).TonTai == 1)
    {
        system("cls"); //Xoá màn hình
        TaoTaiKhoan();
        return;
    }
    

    puts(user.SoTaiKhoan);
    printf("Nhập Họ và tên: ");
    gets(user.HoVaTen);
    gets(user.HoVaTen);
    user.SoDu = 0;
    printf("Nhập CCCD: ");
    gets(user.CCCD);
    user.TrangThai = 1;
    printf("Nhập địa chỉ: ");
    gets(user.DiaChi);
    printf("Nhập ngày sinh: ");
    gets(user.NgaySinh);
    printf("Nhập mã PIN: ");
    gets(user.MaPin);
    printf("Nhập mật khẩu: ");
    gets(user.MatKhau);

    int XacNhan = YeuCauXacNhan();
    if (XacNhan == 1)
    {
        createUser(user);
        printf(ANSI_COLOR_GREEN "Tạo tài khoản thành công!\n" ANSI_COLOR_RESET);
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
void ManHinhKhoaThe()
{
    printf("-------------KHOÁ THẺ------------\n");
    char SoTaiKhoan[20];
    printf("Nhập số tài khoản: ");
    scanf("%s", &SoTaiKhoan);
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    //In tên chủ tài khoản
    char ChuTaiKhoan[30];
    strcpy(ChuTaiKhoan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên chủ tài khoản: %s\n", ChuTaiKhoan);

    if (getUserByID(SoTaiKhoan).TrangThai == 0) //Thẻ này đã bị khoá trước đó
    {
        printf(ANSI_COLOR_RED "Thẻ này đã bị khoá!\n" ANSI_COLOR_RESET);
        system("pause"); //Dừng màn hình
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    else //Thẻ này đã bị khoá trước đó
    {
        int XacNhan = YeuCauXacNhan();
        if (XacNhan == 1)
        {
            KhoaThe(SoTaiKhoan);
            printf(ANSI_COLOR_GREEN "Khoá thẻ thành công!\n" ANSI_COLOR_RESET);
            system("pause"); //Dừng màn hình
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
    while (getUserByID(SoTaiKhoan).TonTai == 0)
    {
        printf(ANSI_COLOR_RED "Số tài khoản không tồn tại!\n" ANSI_COLOR_RESET);
        printf("Nhập số tài khoản: ");
        scanf("%s", &SoTaiKhoan);
    }

    //In tên chủ tài khoản
    char ChuTaiKhoan[30];
    strcpy(ChuTaiKhoan, getUserByID(SoTaiKhoan).HoVaTen);
    printf("Tên chủ tài khoản: %s\n", ChuTaiKhoan);

    if (getUserByID(SoTaiKhoan).TrangThai == 1) // Thẻ này chưa bị khoá nên không thể mở
    {
        printf(ANSI_COLOR_RED "Thẻ này chưa bị khoá!\n" ANSI_COLOR_RESET);
        system("pause"); //Dừng màn hình
        system("cls"); //Xoá màn hình
        ManHinhTrangChu();
    }
    else
    {
        int XacNhan = YeuCauXacNhan();
        if (XacNhan == 1)
        {
            MoThe(SoTaiKhoan);
            printf(ANSI_COLOR_GREEN "Mở thẻ thành công!\n" ANSI_COLOR_RESET);
            system("pause"); //Dừng màn hình
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