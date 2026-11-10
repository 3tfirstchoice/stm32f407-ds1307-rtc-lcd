# Kiến trúc chương trình đồng hồ I2C

Chương trình chia thành 4 tầng. Tầng trên được phép gọi tầng dưới, tầng dưới không bao giờ gọi ngược lên.

```
main.c                      chỉ gọi clock_app_init() và clock_app_run()
  |
Tầng 3  clock_app           chính sách: khi nào quét bus, khi nào đặt giờ, khi nào làm mới màn hình
  |
Tầng 2  clock_ui            trình bày: viết gì lên LCD, ở dòng nào
  |
Tầng 1  lcd                 driver LCD 1602, chuẩn HD44780, bus 4 bit
        ds1307              driver chip RTC DS1307, địa chỉ I2C 0x68
        i2c_scan            tiện ích dò thiết bị trên bus I2C
        status_led          LED xanh LD4 trên board
  |
Tầng 0  datetime            kiểu ngày giờ và chuyển đổi sang chuỗi, không đụng phần cứng
  |
HAL + CubeMX                cấu hình chân và ngoại vi, do CubeMX sinh ra
```

## Trách nhiệm của từng module

| Module | Biết gì | Không biết gì |
|---|---|---|
| datetime | Ngày giờ, tính thứ trong tuần, định dạng chuỗi | Phần cứng nào cả |
| ds1307 | Bản đồ thanh ghi, BCD, bit CH | LCD, ứng dụng |
| lcd | Lệnh HD44780, cách gửi 4 bit, thời gian chờ | Nội dung hiển thị |
| i2c_scan | Cách dò địa chỉ 7 bit | Thiết bị nào nằm ở địa chỉ nào |
| status_led | Chân của LED LD4 | Khi nào nên nháy |
| clock_ui | Chữ nào ở dòng nào của LCD | Cách đọc RTC |
| clock_app | Thứ tự khởi động, chu kỳ làm mới | Chi tiết thanh ghi hay lệnh LCD |

## Thay đổi thường gặp nằm ở đâu

| Muốn làm gì | Sửa ở đâu |
|---|---|
| Đổi chu kỳ làm mới, thời gian hiện màn hình quét, ép đặt giờ | app_config.h |
| Đổi chân LCD hoặc chân I2C | file .ioc trong CubeMX, giữ nguyên các label LCD_RS, LCD_E, LCD_D4 đến LCD_D7 |
| Đổi chữ hiển thị hoặc bố cục hai dòng | clock_ui.c |
| Thay DS1307 bằng DS3231 | viết ds3231.c với cùng giao diện get_time và set_time, rồi đổi lời gọi trong clock_app.c |
| Thêm phím bấm chỉnh giờ | thêm module button, gọi từ clock_app.c |

## Đặt giờ cho RTC

Giờ được lấy từ lúc biên dịch clock_app.c. Vì vậy trước khi đặt giờ, dùng Project, Clean, rồi Build, để chắc chắn clock_app.c được biên dịch lại.
