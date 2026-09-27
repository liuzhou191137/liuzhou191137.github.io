// ============================================================
//  de_dust2 雷达风格俯视图 —— 纯 C++ / EasyX
//  只画地图本体:黑底 + 地块色块,无任何文字与图标
//  布局参照 CS2 雷达:B点左上、A点右上、警家上中、
//  匪家下中、B通道居左、A大道居右、中路居中
// ============================================================
#include <graphics.h>
#include <conio.h>

// ---------------- 配色 ----------------
const COLORREF BG     = RGB(10, 10, 12);      // 背景(墙体/虚空)
const COLORREF G1     = RGB(125, 134, 144);   // 主地面灰
const COLORREF G2     = RGB(104, 112, 122);   // 暗灰
const COLORREF G3     = RGB(148, 156, 166);   // 亮灰(平台/斜坡)
const COLORREF TAN    = RGB(139, 130, 102);   // B 点沙色
const COLORREF SPAWN_G  = RGB(112, 128, 105);   // 出生点绿
const COLORREF GARDEN = RGB(125, 132, 100);   // 后花园绿
const COLORREF DARK   = RGB(35, 38, 42);      // 暗道
const COLORREF BOX    = RGB(63, 68, 76);      // 箱子
const COLORREF BOX_DK = RGB(38, 41, 46);
const COLORREF SITE   = RGB(199, 119, 42);    // 包点橙框
const COLORREF HATCH  = RGB(60, 64, 70);      // 台阶线

// 铺一块地板
void floorRect(int l, int t, int r, int b, COLORREF c)
{
    setfillcolor(c);
    solidrectangle(l, t, r, b);
}

// 箱子:深色方块 + 描边
void crate(int l, int t, int r, int b)
{
    setfillcolor(BOX);
    solidrectangle(l, t, r, b);
    setlinecolor(BOX_DK);
    rectangle(l, t, r, b);
}

// 台阶/坡道:一组斜线
void hatch(int l, int t, int r, int b, int n)
{
    setlinecolor(HATCH);
    int gap = (b - t) / n;
    for (int i = 1; i < n; i++)
        line(l, t + i * gap, r, t + i * gap + 10);
}

void renderMap()
{
    setbkcolor(BG);
    cleardevice();

    // ---- 地块表 {左, 上, 右, 下, 颜色} ----
    // 地块相接处即为通路,留缝处即为墙
    struct Area { int l, t, r, b; COLORREF c; };
    const Area F[] = {
        // B 区(左上)
        {  20,  20, 250, 200, TAN   },   // B 点
        {  20, 200,  90, 340, G2    },   // 假门/狗位/B洞条带
        { 250, 135, 330, 215, G1    },   // B 门通道
        // 顶部走廊(警家旋转位)
        { 250,  40, 400, 120, G1    },   // 脚手架/卡车走廊
        { 470,  40, 640, 120, G1    },   // 忍者位/鹅位走廊
        { 330, 120, 470, 215, G2    },   // 卡车下沿
        { 430, 120, 570, 215, SPAWN_G },   // 警家
        { 570, 120, 655, 210, G3    },   // A 平台
        { 640,  55, 800, 230, G1    },   // A 点
        { 740,  55, 800, 130, G3    },   // A 斜坡
        { 790, 190, 835, 265, G2    },   // 蓝车位
        // 沙地与中路
        { 250, 215, 400, 290, G1    },   // 沙地
        { 385, 215, 465, 300, G2    },   // 中路凹槽
        { 340, 300, 440, 520, G1    },   // 中路
        { 440, 330, 540, 390, G1    },   // A 小
        { 540, 230, 715, 405, G1    },   // A 口交汇
        { 715, 230, 800, 505, G1    },   // A 大
        { 800, 385, 835, 460, G2    },   // 厕所
        { 620, 480, 810, 620, G1    },   // A 大坑
        { 490, 585, 660, 790, G1    },   // A 门方向车道
        // 中下区域
        { 355, 520, 475, 640, G1    },   // 中远
        { 270, 490, 355, 575, G2    },   // L 位
        { 385, 640, 475, 770, DARK  },   // 暗道
        { 230, 770, 560, 880, SPAWN_G },   // 匪家
        // B 通道(居左)
        {  55, 340, 160, 545, G2    },   // B 通
        { 160, 380, 340, 470, G2    },   // B1
        // 后花园(左下)
        {  60, 545, 225, 660, GARDEN},   // 后花园
        { 140, 660, 285, 780, GARDEN},   // 后花园平台
        {  20, 690, 140, 790, GARDEN},   // 斜坡
        {  20, 790, 120, 860, GARDEN},   // 匪家蓝车位
    };
    for (size_t i = 0; i < sizeof(F) / sizeof(F[0]); i++)
        floorRect(F[i].l, F[i].t, F[i].r, F[i].b, F[i].c);

    // ---- 箱子 ----
    const Area C[] = {
        {  45, 140, 100, 190 },           // 大箱
        { 100, 160, 135, 195 },           // 高箱
        { 100,  95, 140, 130 },           // 矮箱
        { 345, 148, 400, 196 },           // 卡车
        { 665, 140, 700, 175 },           // 二箱
        { 702, 160, 732, 190 },           // 一箱
        { 390, 348, 425, 382 },           // 叉箱
        { 592, 338, 645, 386 },           // 蓝箱
        { 700,  72, 742, 100 },           // 沙袋
        { 158, 632, 182, 656 },           // 垃圾桶
    };
    for (size_t i = 0; i < sizeof(C) / sizeof(C[0]); i++)
        crate(C[i].l, C[i].t, C[i].r, C[i].b);

    // ---- 油桶(B 点) ----
    setfillcolor(BOX_DK);
    fillcircle(48, 100, 6);
    fillcircle(64, 96, 6);
    fillcircle(56, 112, 6);

    // ---- 台阶 / 坡道 ----
    hatch(548, 252, 608, 302, 4);        // A 小楼梯
    hatch(180, 395, 250, 450, 4);        // 旋砖楼梯
    hatch( 30, 700, 130, 780, 5);        // 后花园斜坡

    // ---- 包点橙色边框 ----
    setlinecolor(SITE);
    setlinestyle(PS_SOLID, 2);
    rectangle( 95,  95, 225, 195);       // B 包点
    rectangle(655, 125, 790, 225);       // A 包点
    setlinestyle(PS_SOLID, 1);
}

int main()
{
    initgraph(850, 900);
    renderMap();
    _getch();                            // 按任意键退出
    closegraph();
    return 0;
}
