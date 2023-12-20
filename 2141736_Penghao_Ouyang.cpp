
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <corecrt_math_defines.h>
#define MAX_CHAR 128
#define GL_MULTISAMPLE 32925
float w = 1000;
float h = 600;
float scaleCB = 1.2;
float wingLen = 120;
float scaleWings = 0.8;
float XJTLUstep = 25.0;
float  openStep = 0;
float downStep = 0;
float bookW = 10;
float bookH = 35;
float starRotate = 0.0;
float starSpeed = 1.0;  
float snowflakeY = 0.0;
float snowRotate = 0.0;
float snowSpeed = 1.0;
int snowNum = 20;
int xDown = 0, yDown = 0;
int xUp = 0, yUp = h;
int scale = 1;
bool exploded = false;
const int numFirework = 50;
struct Snowflake {
    float x;
    float y;
};
struct fireworks {
    float x;
    float y;
};
struct fireworkSpeeds {
    float x;
    float y;
};
fireworks firework[numFirework];
fireworkSpeeds fireworkSpeed[numFirework];
Snowflake snowflakes[80];

void setColor(int value) {
    if (value == 1)
        glColor3f(1.0, 1.0, 1.0);
    if (value == 2)    
        glColor3f(0.4, 0.2, 0.3);
    if (value == 3)   
        glColor3f(0.5, 0.3, 0.5);
    if (value == 4) 
        glColor3f(0.8, 0.4, 0.8);
    if (value == 5)    
        glColor3f(0.4, 0.4, 0.4);
    if (value == 6)   
        glColor3f(0.6, 0.6, 0.6);
    if (value == 7)    
        glColor3f(0.8, 0.0, 0.0);
    if (value == 8)    
        glColor3f(0.956, 0.870, 0.725);
    if (value == 9)    
        glColor3f(0.4, 0.26, 0.13);
    if (value == 10)   
        glColor3f(0.55, 0.27, 0.07);
    if (value == 11)     
        glColor3f(0.5, 0.5, 0.5);
    if (value == 12)    
        glColor3f(0.4, 0.2, 0.1);
    if (value == 13)     
        glColor3f(0.1, 0.6, 0.1);
    if (value == 14)        
        glColor3f(0.0, 0.5, 0.8);
    if (value == 15)         
        glColor3f(0.3, 0.2, 0.1);
    if (value == 16)           
        glColor3f(1.0, 0.5, 0.0);
    if (value == 17)                  
        glColor3f(0.1, 0.2, 0.4);
    if (value == 18)                     
        glColor3f(1.0, 0.8, 0.5);
    if (value == 19)                    
        glColor3f(0.7, 0.7, 0.6);
    if (value == 20)                            
        glColor3f(0.1, 0.3, 0.6);
    if (value == 21)                             
        glColor3f(0.0, 0.0, 0.0);
}
//generate snow in different place of the screen
void numSnow() {
    for (int i = 0; i < snowNum; i++) {
        snowflakes[i].x = float(rand() % int(w));
        snowflakes[i].y = h + float(rand() % int(h));
    }
}
void when_in_mainloop()
{ // idle callback function
    if (scale == 0)
        scale = 1;
    glViewport(0, 0, w / scale, h / scale);
    glutPostRedisplay(); // force OpenGL to redraw the current window
}
void keyboard_input(unsigned char key, int x, int y)
{   // keyboard interactions
    if (key == 'K' || key == 'k') // remove the cover
        openStep = 3;
    if (key == 'S' || key == 's') // remove the cover
        openStep = 0;
}
void mouse_input(int button, int state, int x, int y) {
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && !exploded) {
        firework[0] = { 100, 0 };
        fireworkSpeed[0] = { 0, 5 };
        exploded = false;
        glutPostRedisplay();
    }
    else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && exploded) {
        for (int i = 0; i < numFirework; i++) {
            firework[i] = { 0, 0 };
            fireworkSpeed[i] = { 0, 0 };
        }
        firework[0] = { 100, 0 };
        fireworkSpeed[0] = { 0, 5 };
        exploded = false;
        glutPostRedisplay();
    }
}
void drawString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;
    if (isFirstCall) {
        isFirstCall = 0;
        lists = glGenLists(MAX_CHAR);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    for (; *str != '\0'; ++str) {
        glCallList(lists + *str);
    }
}
void drawSmallString(const char* str) {
    static int isFirstCall = 1;
    static GLuint lists;
    if (isFirstCall) {
        isFirstCall = 0;
        lists = glGenLists(MAX_CHAR);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
    }
    for (; *str != '\0'; ++str) {
        glCallList(lists + *str);
    }
}
void selectFont(int size, int charset, const char* face) {
    HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0, charset, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
    HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
    DeleteObject(hOldFont);
}
void writeText(float x, float y, const char* string) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    selectFont(28, ANSI_CHARSET, "Georgia");
    glRasterPos2f(0.0, 1.0);
    drawString(string);
    glPopMatrix();
}
void writeSmallText(float x, float y, const char* string) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    selectFont(20, ANSI_CHARSET, "Times New Roman");
    glRasterPos2f(0.0, 1.0);
    drawSmallString(string);
    glPopMatrix();
}
void drawStar(float cx, float cy,float R, float G, float B) {
    glPushMatrix();   
    glTranslatef(cx, cy, 0);
    glRotatef(starRotate, 0, 0, 1);
    for (int i = 0; i < 5; i++) {
        float angle = 2.0 * M_PI * i / 5;
        float angle1 = 2.0 * M_PI * (i + 1) / 5;
        float x1 = 10 * sin(angle);
        float y1 = 10 * cos(angle);
        float x2 = 10 * sin(angle1);
        float y2 = 10 * cos(angle1);
        float x3 = 30 * sin((angle + angle1) / 2);
        float y3 = 30 * cos((angle + angle1) / 2);
        glBegin(GL_POLYGON);
        glColor3f(R + 0.6, G + 0.6, B + 0.6);
        glVertex2f(0, 0);
        glColor3f(R + 0.3, G + 0.3, B + 0.3);
        glVertex2f(x1, y1);
        glVertex2f(x3, y3);
        glColor3f(R, G, B);
        glVertex2f(x2, y2);
        glEnd();
    }

    glPopMatrix();  // 恢复坐标系
}
// (cx, cy) is the center pint of the ball, r is the radious R,G and B define the color of the ball
void drawBall(float cx, float cy, float r, float R, float G, float B) {
    glPushMatrix();
    glTranslatef(cx, cy, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float t = i * M_PI / 180.0;
        float x = r * cos(t);
        float y = r * sin(t);

        // Decrase color to make it more realistic
        float c1 = R + 0.1f * cos(t); 
        float c2 = G + 0.1f * cos(t); 
        float c3 = B + 0.1f * cos(t); 
        glColor3f(c1, c2, c3);
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
}
// Paint the hexagon outside the CB
void drawLine(void) {
    float x = w / 2 - 50, y = 2 * h / 3 - 50, line = 100;
    setColor(1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + line, y);
    glVertex2f(x + line + line * cos(60 * M_PI / 180), y + line * sin(60 * M_PI / 180));
    glVertex2f(x + line, y + 2 * line * sin(60 * M_PI / 180));
    glVertex2f(x, y + 2 * line * sin(60 * M_PI / 180));
    glVertex2f(x - line * cos(60 * M_PI / 180), y + line * sin(60 * M_PI / 180));
    glEnd();
}
void drawWings() {
    glBegin(GL_POLYGON);
    glVertex2f(xUp + 0, 0);
    glVertex2f(xUp + 0, wingLen);
    glVertex2f(xUp - wingLen * cos(20.0 * (M_PI / 180.0)) * cos(70.0 * (M_PI / 180.0)), wingLen * cos(20.0 * (M_PI / 180.0)) * sin(70.0 * (M_PI / 180.0)));
    glEnd();
}
void drawEmble(void) {
    float x = w / 2 - 50, y = 2 * h / 3 - 50, line = 100, scaleLine = line * scaleCB;

    // Draw the structure of CB
    glPushMatrix();
    glTranslatef(xUp, yUp - h, 0);
    drawLine();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scaleCB, scaleCB, 1.0);
    glTranslatef(-x - (scaleLine - line) / 2, -y - (scaleLine - line) / 2 * sqrt(3), 0);
    glTranslatef(xUp / scaleCB, (yUp - h) / scaleCB, 0);
    drawLine();
    glPopMatrix();

    setColor(1);
    glBegin(GL_LINES);
    glVertex2f(xUp + x - 20, yUp - h + y + line / 2 - 13);
    glVertex2f(xUp + x + line + 20, yUp - h + y + line / 2 - 13);
    glEnd();

    //part 1
    setColor(1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xUp + 520, yUp - h + y + line / 2 + 40);
    glVertex2f(xUp + 450, yUp - h + y + line / 2 + 20);
    glVertex2f(xUp + 450, yUp - h + y + line / 2 - 13);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 - 13);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 + 40);
    glEnd();

    // part 2
    setColor(1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xUp + 520, yUp - h + y + line / 2 + 50);
    glVertex2f(xUp + 450, yUp - h + y + line / 2 + 30);
    glVertex2f(xUp + 450, yUp - h + y + line / 2 + 60);
    glVertex2f(xUp + 470, yUp - h + y + line / 2 + 60);
    glVertex2f(xUp + 500, yUp - h + y + line / 2 + 80);
    glVertex2f(xUp + 520, yUp - h + y + line / 2 + 80);
    glVertex2f(xUp + 530, yUp - h + y + line / 2 + 60);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 + 60);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 + 50);
    glEnd();

    // part 3
    setColor(1);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xUp + 450, yUp - h + y + line / 2 + 70); 
    glVertex2f(xUp + 450, yUp - h + y + line / 2 + 100);
    glVertex2f(xUp + 470, yUp - h + y + line / 2 + 100);
    glVertex2f(xUp + 475, yUp - h + y + line / 2 + 95);
    glVertex2f(xUp + 480, yUp - h + y + line / 2 + 95);
    glVertex2f(xUp + 480, yUp - h + y + line / 2 + 100);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 + 100);
    glVertex2f(xUp + 550, yUp - h + y + line / 2 + 70);
    glVertex2f(xUp + 530, yUp - h + y + line / 2 + 70);
    glVertex2f(xUp + 520, yUp - h + y + line / 2 + 90);
    glVertex2f(xUp + 500, yUp - h + y + line / 2 + 90);
    glVertex2f(xUp + 470, yUp - h + y + line / 2 + 70);
    glEnd();


    // Call the drawWings function
    for (int i = 0; i < 3; i++)
    {
        int color = i + 2;
        glPushMatrix();
        glTranslatef(xUp + 440 - 2, yUp - h + y + 50 * sqrt(3) - 60 * sqrt(3) - 8, 0);
        glRotatef(20 * i + 30.0, 0, 0, 1); 
        glScalef(pow(scaleWings, i), pow(scaleWings, i), 0);
        setColor(color); // 设置深蓝颜色
        drawWings();
        glPopMatrix();
    }
    for (int i = 0; i < 3; i++)
    {
        int color = i + 2;
        glPushMatrix();
        glTranslatef(xUp + 560 - 2, yUp - h + y + 50 * sqrt(3) - 60 * sqrt(3) - 8, 0);
        glRotatef(-20 * i - 30.0, 0, 0, 1); 
        glScalef(-pow(scaleWings, i), pow(scaleWings, i), 0);
        setColor(color); 
        drawWings();
        glPopMatrix();
    }
}
void drawHeart() {
    glPushMatrix();
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        float angle = 2.0 * M_PI * float(i) / float(360);
        float x = 1.2 * 16 * pow(sin(angle), 3);
        float y = 1.2 * (13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle));
        float c1 = 1.0 - 0.5f * cos(angle);
        glColor3f(c1, 0.0, 0.0);  
        glVertex2f(x, y);
    }
    glEnd();
    glPopMatrix();
} 
void drawWave(void) {
    // Using Bézier curves to create curves on the hat
    setColor(1);
    glLineWidth(0.5);
    glBegin(GL_LINE_STRIP);
    float P0[] = { 16 - 18 * sqrt(3) - 3, 15 * sqrt(3) };
    float P1[] = { 16 - 18 * sqrt(3) - 3 + 5, 15 * sqrt(3) - 7 };
    float P2[] = { 16 - 18 * sqrt(3) - 3 - 5, 15 * sqrt(3) - 14 };
    float P3[] = { 16 - 18 * sqrt(3) - 3, 15 * sqrt(3) - 21 };
    for (float t = 0; t <= 1; t += 0.05) {
        float x = pow(1 - t, 3) * P0[0] + 3 * pow(1 - t, 2) * t * P1[0] + 3 * (1 - t) * pow(t, 2) * P2[0] + pow(t, 3) * P3[0];
        float y = pow(1 - t, 3) * P0[1] + 3 * pow(1 - t, 2) * t * P1[1] + 3 * (1 - t) * pow(t, 2) * P2[1] + pow(t, 3) * P3[1];
        glVertex2f(x, y);
    }
    glEnd();
}
void drawHatDown(float cx, float cy) {
    // Draw the red part of the hat
    float a = 15 + 10 * sqrt(3);
    float b = 5;
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 180; i < 360; i++) {
        float angle = 2.0 * M_PI * i / 360;
        float x = cx + a * cos(angle);
        float y = cy + b * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawHat(void) {
    setColor(5);
    glBegin(GL_POLYGON);
    glVertex2f(15 - 20 * sqrt(3), 15 * sqrt(3) + 10);
    glVertex2f(15 + 10 * sqrt(3), 15 * sqrt(3) - 10);
    glVertex2f(15 + 40 * sqrt(3), 15 * sqrt(3) + 10);
    glVertex2f(15 + 10 * sqrt(3), 15 * sqrt(3) + 30);
    glEnd();

    setColor(6);
    glBegin(GL_POLYGON);
    glVertex2f(15 + 10 * sqrt(3), 15 * sqrt(3) - 10);
    glVertex2f(15, 15 * sqrt(3) - 3);
    glVertex2f(0, 0);
    glVertex2f(30 + 20 * sqrt(3), 0);
    glVertex2f(15 + 20 * sqrt(3), 15 * sqrt(3) - 3);
    glEnd();

    setColor(7);
    glPushMatrix();
    glTranslatef(15 + 10 * sqrt(3), 0, 0);
    drawHatDown(0, 0);
    glPopMatrix();

    setColor(1);
    glLineWidth(0.5);
    glBegin(GL_LINES);
    glVertex2f(16 - 18 * sqrt(3), 20 * sqrt(3) - 0.5);
    glVertex2f(16 - 18 * sqrt(3), 15 * sqrt(3) - 0.5);
    glEnd();

    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(i * 2, -5, 0);
        drawWave();
        glPopMatrix();
    }
    drawBall(16 - 18 * sqrt(3), 15 * sqrt(3) - 3, 4, 1, 1, 1);
}
// Draw the background of the text in the inside page
void drawBookPage(void) {
    setColor(1);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, 500);
    glVertex2f(0, 500);
    glVertex2f(50, 500);
    glVertex2f(0, 0);
    glVertex2f(400, 0);
    glVertex2f(400, 0);
    glVertex2f(400, 500);
    glVertex2f(50, 50);
    glVertex2f(50, 550);
    glEnd();

    glBegin(GL_LINE_STRIP);
    float P0[] = { 400, 0 };
    float P1[] = { 300, 80 };
    float P2[] = { 50, 50 };
    float P3[] = { 400, 500 };
    float P4[] = { 300, 580 };
    float P5[] = { 50, 550 };
    for (float t = 0; t <= 1.05; t += 0.05) {  
        float x = (1 - t) * (1 - t) * P0[0] + 2 * (1 - t) * t * P1[0] + t * t * P2[0];
        float y = (1 - t) * (1 - t) * P0[1] + 2 * (1 - t) * t * P1[1] + t * t * P2[1];
        glVertex2f(x, y);
    }
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.05; t += 0.05) {  
        float x = (1 - t) * (1 - t) * P3[0] + 2 * (1 - t) * t * P4[0] + t * t * P5[0];
        float y = (1 - t) * (1 - t) * P3[1] + 2 * (1 - t) * t * P4[1] + t * t * P5[1];
        glVertex2f(x, y);
    }
    glEnd();
}
void drawBook() {
    glBegin(GL_POLYGON);
    glVertex2f(0, 0);
    glVertex2f(0, bookH);
    glVertex2f(bookW, bookH);
    glVertex2f(bookW, 0);
    glEnd();
}
void drawBookshelf(void) {
    // Draw the desk of the bookshelf
    for (int i = 0; i < 2; i++)
    {
        setColor(8);
        glBegin(GL_POLYGON);
        glVertex2f(i * 100 + 0, -10);
        glVertex2f(i * 100 + 5, -10);
        glVertex2f(i * 100 + 5, 10);
        glVertex2f(i * 100 + 0, 10);
        glEnd();
    }

    setColor(9);

 
    glBegin(GL_POLYGON);
    glVertex2f(-20, 20);
    glVertex2f(-20, 10);
    glVertex2f(120, 10);
    glVertex2f(120, 20);
    glEnd();

    setColor(10);

 
    glBegin(GL_POLYGON);
    glVertex2f(-20, 20);
    glVertex2f(-10, 35);
    glVertex2f(115, 35);
    glVertex2f(120, 20);
    glEnd();

    // Draw the book on the shelf
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        if (i == 0) {
            setColor(11);
        }
        else if (i == 1) {
            setColor(12);
        }
        else {
            setColor(13);
        }
        glTranslatef(i * 10 + 5, 20, 0);
        drawBook();
        glPopMatrix();
    }


    glPushMatrix();
    setColor(7);
    glTranslatef(35, 20, 0);
    glScalef(1.4, 1.4, 0);
    drawBook();
    drawBall(bookW / 2, bookH / 2, 3, 1.0, 0.8, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(0, bookH - 5);
    glVertex2f(0, bookH - 2);
    glVertex2f(bookW, bookH - 2);
    glVertex2f(bookW, bookH - 5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0, 5);
    glVertex2f(0, 2);
    glVertex2f(bookW, 2);
    glVertex2f(bookW, 5);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    setColor(14);
    glTranslatef(49, 20, 0);
    glScalef(0.8, 0.8, 0);
    drawBook();
    glPopMatrix();

    glPushMatrix();
    setColor(15);
    glTranslatef(57, 20, 0);
    drawBook();
    drawBall(bookW / 2, bookH / 2, 3, 1.0, 0.8, 0.5);
    glBegin(GL_POLYGON);
    glVertex2f(0, bookH - 5);
    glVertex2f(0, bookH - 2);
    glVertex2f(bookW, bookH - 2);
    glVertex2f(bookW, bookH - 5);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(0, 5);
    glVertex2f(0, 2);
    glVertex2f(bookW, 2);
    glVertex2f(bookW, 5);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    setColor(4);
    glTranslatef(67, 20, 0);
    drawBook();
    glPopMatrix();

    glPushMatrix();
    setColor(7);
    glTranslatef(94.5, 20, 0);
    glRotatef(30, 0, 0, 1);
    drawBook();
    glPopMatrix();
}
void drawBuilding(void) {
    setColor(1);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    glVertex2f(-30, 4);
    glVertex2f(-30, 60);

    glVertex2f(-20, 65);
    glVertex2f(-18, 55);
    glVertex2f(-16.5, 56);
    glVertex2f(-16, 69);

    glVertex2f(0, 80);
    glVertex2f(35, 75);
    glVertex2f(35, 45);
    glVertex2f(0, 50);
    glVertex2f(-10, 40);
    glVertex2f(-25, 37);
    glVertex2f(-25, 20);
    glVertex2f(-20, 28);
    glVertex2f(0, 34);
    glVertex2f(30, 30);
    glVertex2f(70, 10);
    glVertex2f(70, 4);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex2f(49, 73);
    glVertex2f(70, 70);
    glVertex2f(70, 30);
    glVertex2f(50, 28);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(67.5, 29.75);
    glVertex2f(67.5, 11.125);
    glVertex2f(35, 72);
    glVertex2f(49, 70);
    glVertex2f(35, 52);
    glVertex2f(49, 50);
    glVertex2f(0, 34);
    glVertex2f(0, 0);
    glVertex2f(0, 50);
    glVertex2f(0, 80);
    glEnd();
}
void drawEllipses(float cx, float cy, float a, float b) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = 2.0 * M_PI * i / 360;
        float x = cx + a * cos(angle);
        float y = cy + b * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawPlatform(void) {
    setColor(18);
    glBegin(GL_POLYGON);
    glVertex2f(130, 0);
    glVertex2f(870, 0);
    glVertex2f(870, 100);
    glVertex2f(130, 100);
    glEnd();
    setColor(19);
    drawEllipses(w / 2, 100, 370, 70);
}
// Draw color of the firework
void drawFirework(void) {
    if (!exploded) {
        glBegin(GL_POLYGON);
        setColor(7);
        float cx = firework[0].x;
        float cy = firework[0].y - 10;
        for (int i = 0; i <= 20; i++) {
            float theta = 2.0 * M_PI * i / 20; // 计算角度
            float x = 5 * cos(theta);
            float y = 5 * sin(theta);
            glVertex2f(cx + x, cy + y);
        }
        glEnd();
    }
    else {
        int color = 0;
        for (int i = 0; i < numFirework; i++) {
            float R = (float)(color % 6) / 2.0;
            float G = (float)(color % 5) / 2.0;
            float B = (float)(color % 4) / 2.0;
            glColor3f(R, G, B);
            float cx = firework[i].x;
            float cy = firework[i].y;
            glBegin(GL_POLYGON);
            for (int i = 0; i <= 20; i++) {
                float theta = 2.0 * M_PI * i / 20; // 计算角度
                float x = 3.5 * cos(theta);
                float y = 3.5 * sin(theta);
                glVertex2f(cx + x, cy + y);
            }
            glEnd();
            color = (color + 1) % 20;
        }
       
    }
}
// Draw one part of the snow
void drawShape() {
    glLineWidth(1.0);
    glBegin(GL_LINES);
    setColor(1);
    glVertex2f(0, 0);
    glVertex2f(0, 12);
    glVertex2f(0, 9);
    glVertex2f(-2, 10.5);
    glVertex2f(2, 10.5);
    glVertex2f(0, 9);
    glVertex2f(-1, 7.5);
    glVertex2f(0, 6);
    glVertex2f(1, 7.5);
    glVertex2f(0, 6);
    glVertex2f(0, 4);
    glVertex2f(3.5, 3.5 * sqrt(3));
    glEnd();
}
void drawSnow() {
    for (int i = 0; i < snowNum; i++) {
        glPushMatrix();
        glTranslatef(2 * snowflakes[i].x, 2 * snowflakes[i].y, 0);
        glRotatef(snowRotate, 0.0, 0.0, 1.0);
        glScalef(2, 2, 0);
        for (int i = 0; i < 6; i++) {
            glTranslatef(0, snowflakeY, 0);
            glRotatef(60, 0.0, 0.0, 1.0);  
            drawShape();
            glScalef(1, -1, 0); 
            drawShape();
            glScalef(1, -1, 0); 
        }
        glPopMatrix();
    }
}

// Open to show the inner content
void UpTimer(int value)
{
    yUp += openStep;
    if (yUp <= h) openStep = 0;
    glutTimerFunc(18, UpTimer, 1);
}
void DownTimer(int value)
{
    yDown -= openStep;
    if (yDown >= 0) openStep = 0;
    glutTimerFunc(12, DownTimer, 5);
}
// Snow fall down
void SnowTimer(int value) {
    for (int i = 0; i < snowNum; i++) {
        snowflakes[i].y -= 2; 
        if (snowflakes[i].y < 0) {
            snowflakes[i].y = h + (rand() % int(h)); 
            snowflakes[i].x = rand() % int(w); 
        }
    }

    glutPostRedisplay(); // 触发重新绘制
    glutTimerFunc(30, SnowTimer, 0); // 设置16毫秒后再次调用update
}
// Snow rotate
void SnowRtimer(int value) {
    snowRotate += snowSpeed;
    glutPostRedisplay();
    glutTimerFunc(16, SnowRtimer, 0);
}
void StarTimer(int value) {
    starRotate += starSpeed;
    glutPostRedisplay(); 
    glutTimerFunc(16, StarTimer, 0);
}
void FireworkTimer(int value) {
    int weight = -1;
    if (!exploded) {
        firework[0].y += fireworkSpeed[0].y;
        if (firework[0].y >= 400.0) {
            exploded = true;
            for (int i = 1; i < numFirework; i++) {
                firework[i].x = firework[0].x;
                firework[i].y = firework[0].y;
                float angle = 2 * M_PI * (float)i / numFirework;
                weight = weight * weight;
                fireworkSpeed[i].x = weight * (1 + rand() % 5) * cos(angle);
                fireworkSpeed[i].y = weight * (1 + rand() % 5) * sin(angle);
            }        
        }
    }
    else {
        for (int i = 0; i < numFirework; i++) {
            firework[i].x += fireworkSpeed[i].x;
            firework[i].y += fireworkSpeed[i].y;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, FireworkTimer, 0);
}
void display(void)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);

    // Inside
    glBegin(GL_POLYGON);
    setColor(14);
    glVertex2f(0, h);
    glVertex2f(w, h);
    glVertex2f(w, 0);
    glVertex2f(0, 0);
    glEnd();
    setColor(1);

    // Platform
    drawPlatform();

    // BookPage 
    glPushMatrix();
    glTranslatef(200, 140, 0);
    glScalef(0.75, 0.75, 0);
    drawBookPage();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(800, 140, 0);
    glScalef(-0.75, 0.75, 0);
    drawBookPage();
    glPopMatrix();
    for (int i = 0; i < 6; i++)
    {
        glBegin(GL_LINES);
        glVertex2f(260, 220 + i * 50 - 10);
        glVertex2f(470, 220 + i * 50 - 10);
        glVertex2f(530, 220 + i * 50 - 10);
        glVertex2f(740, 220 + i * 50 - 10);
        glEnd();
    }

    // Write text
    writeSmallText(260, 470, "Dear 2024 XJTLU graduates");
    writeSmallText(540, 470, "time flies, and graduation is");
    writeSmallText(260, 420, "a significant milestone in life.");
    writeSmallText(540, 420, "You have already spread");
    writeSmallText(260, 370, "your wings. On the path to");
    writeSmallText(540, 370, "chasing your dreams, stay");
    writeSmallText(260, 320, "courageous, humble, and");
    writeSmallText(540, 320, "resilient, as you will write");
    writeSmallText(260, 270, "your own brilliant chapters");
    writeSmallText(540, 270, "and become changemakers");
    writeSmallText(260, 220, "in the world!");

    // Draw lines in the book
    setColor(7);
    glBegin(GL_POLYGON);
    glVertex2f(0, 530);
    glVertex2f(0, 440);
    glVertex2f(160, h);
    glVertex2f(70, h);
    glEnd();
    drawStar(60, 545, 0.8, 0.6, 0.3);
    setColor(7);
    glBegin(GL_POLYGON);
    glVertex2f(w, 530);
    glVertex2f(w, 440);
    glVertex2f(830, h);
    glVertex2f(920, h);
    glEnd();
    drawStar(935, 545, 0.8, 0.6, 0.3);
    

    // Draw hat
    glPushMatrix();
    glTranslatef(30, 20, 0);
    drawHat();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(970, 20, 0);
    glScalef(-1, 1, 1);
    drawHat();
    glPopMatrix();

    // Outside
    glBegin(GL_TRIANGLES);
    setColor(20);
    glVertex2f(xUp, yUp);
    glVertex2f(xUp + w / 2, yUp - h + h / 3);
    glVertex2f(xUp + w, yUp - h + h);
    glEnd();

    // Draw down-half
    glBegin(GL_POLYGON);
    setColor(20);
    glVertex2f(xDown + w / 2, yDown + h / 3);
    glVertex2f(xDown, yDown + h); 
    setColor(17);
    glVertex2f(xDown + 0, yDown + 0);
    glVertex2f(xDown + w, yDown + 0); 
    setColor(20);
    glVertex2f(xDown + w, yDown + h);
    glEnd();

    // Set line in cover
    glLineWidth(5);
    setColor(21);
    glBegin(GL_LINES);
    glVertex2f(xDown + 0, yDown + h);
    glVertex2f(xDown + w / 2, yDown + h / 3);
    glVertex2f(xDown + w / 2, yDown + h / 3);
    glVertex2f(xDown + w, yDown + h);
    glEnd();

    // Write text    
    setColor(1);
    writeText(xDown + w / 2 - 120, yDown + 100, "2024 XJTLU Graduation ");
    writeText(xDown + w / 2 - 100, yDown + 70, " Ceremony Invitation");
    writeText(xUp + w / 2 - 35, yUp - h + 360, "XJTLU");

    // Draw star
    drawStar(xUp + 110, yUp - h + 560, 0.8, 0.6, 0.3);
    drawStar(xUp + 890, yUp - h + 560, 0.8, 0.6, 0.3);
    drawStar(xUp + w / 2, yUp - h + h / 3 + 50, 0.0, 0.6, 0.5);

    // Draw cb
    drawEmble();

    // I love xjtlu
    setColor(1);
    writeText(xDown + 950, yDown + 400, "I");
    writeText(xDown + 945, yDown + 330, "X");
    writeText(xDown + 945, yDown + 300, "J");
    writeText(xDown + 945, yDown + 270, "T");
    writeText(xDown + 945, yDown + 240, "L");
    writeText(xDown + 945, yDown + 210, "U");
    glPushMatrix();
    glTranslatef(xDown + 955, yDown + 400 - XJTLUstep, 0.0);
    drawHeart();
    glPopMatrix();

    //DrawBookshelf
    glPushMatrix();
    glTranslatef(xDown + 800, yDown + 20, 0);
    glScaled(1.5, 1.5, 0);
    drawBookshelf();
    glPopMatrix();

    //Draw snow
    glPushMatrix();
    glScalef(0.5, 0.5, 0);
    drawSnow();
    glPopMatrix();
    
    //Draw Building
    glPushMatrix();
    glTranslatef(xDown + 100, yDown + 10, 0);
    glScalef(3, 3, 0);
    drawBuilding();
    glPopMatrix();

    // Set firwork
    drawFirework();
    glPushMatrix();
    glTranslatef(800, 0, 0);
    drawFirework();
    glPopMatrix();
    glutSwapBuffers();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(w, h);
    glutCreateWindow("2024 XJTLU Graduation Ceremony Invitation");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
   
    glutDisplayFunc(display);
    glutMouseFunc(mouse_input);
    glutKeyboardFunc(keyboard_input);
    glutIdleFunc(when_in_mainloop);
    glEnable(GL_MULTISAMPLE);// MSAA

    glutTimerFunc(18, UpTimer, 1); 
    glutTimerFunc(12, DownTimer, 1);
    glutTimerFunc(16, StarTimer, 0);
    glutTimerFunc(16, SnowTimer, 0);
    glutTimerFunc(16, SnowRtimer, 0); 
    glutTimerFunc(0, FireworkTimer, 0);

    glutMainLoop();
    return 0;
}
