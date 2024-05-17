#define STB_IMAGE_IMPLEMENTATION    
#include<Windows.h>
#include<GL/glut.h>
#include<GL/stb_image.h>
#include <cstdlib>
#include <time.h> 
#include<iostream>
using namespace std;
char title[] = "3D Snake Game";

float snake[60][6][12]; // max 60 cube , each cube has 6 faces , each face has 4 vertices  (x1,y1 x2,y2 x3,y3 x4,y4)
float refreshMills = 1000 / 100;        // refresh interval in milliseconds [NEW]
float xpos = 0;
float ypos = 0;
float zpos[2] = { -0.25,0.25 };
float bodysize = 0.25;
int direction = 1;
float aspect = 800 / 600;
float acceleration = 0.12;
float randx = 0.2;
float randz = 0.2;
int foodX1 = 7;
int foodX2 = -7;
int score = 0;
float temp;
int maxsize = 60;
int snakelen = 10;
int tx, ty;
float dist1, dist2, dist3;
float dist1x, dist2x, dist3x;
bool gameover = false;
string gameoverText = "Game Over";
/* Initialize Texture Variables */
unsigned int texture;
int t_width, t_height, nrChannels;
unsigned char* dataa = NULL;

void check(unsigned char* dataa) {
    if (dataa)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, dataa);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(dataa);
}

void t_load(int imgnum) {
    if (imgnum == 1) {

        dataa = stbi_load("soil.jpg", &t_width, &t_height, &nrChannels, 0);
        check(dataa);
    }
    else if (imgnum == 2) {

        dataa = stbi_load("head.jpg", &t_width, &t_height, &nrChannels, 0);
        check(dataa);
    }
    else if (imgnum == 3) {

        dataa = stbi_load("downloadbody.jpg", &t_width, &t_height, &nrChannels, 0);
        check(dataa);
    }
    else if (imgnum == 4) {

        dataa = stbi_load("images.jpg", &t_width, &t_height, &nrChannels, 0);
        check(dataa);
    }
}
/* Initialize OpenGL Graphics */
void drawFood();
void randomPos();
void initPos();
void swapPos();
void check(unsigned char*);
void t_load(int);
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightAmbient[] = { 0.2, 0.2, 0.2, 1.0 }; // Ambient light
    GLfloat lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Diffuse light
    GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 }; // Specular light
    GLfloat lightPosition[] = { 5.0, 5.0, 5.0, 1.0 }; // Light position (directional light)
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void initHead() {
    //top face
    snake[0][0][0] = xpos + bodysize; //x1
    snake[0][0][1] = ypos + bodysize; //y1
    snake[0][0][2] = zpos[0]; //z1

    snake[0][0][3] = xpos - bodysize; //x2
    snake[0][0][4] = ypos + bodysize; //y2 
    snake[0][0][5] = zpos[0]; //z2

    snake[0][0][6] = xpos - bodysize; //x3
    snake[0][0][7] = ypos + bodysize; //y3 
    snake[0][0][8] = zpos[1]; //z3

    snake[0][0][9] = xpos + bodysize; //x4
    snake[0][0][10] = ypos + bodysize; //y4
    snake[0][0][11] = zpos[1]; //z4


    // bottom face
    snake[0][1][0] = xpos + bodysize; //x1
    snake[0][1][1] = ypos - bodysize; //y1
    snake[0][1][2] = zpos[0]; //z1

    snake[0][1][3] = xpos - bodysize; //x2
    snake[0][1][4] = ypos - bodysize; //y2 
    snake[0][1][5] = zpos[0]; //z2

    snake[0][1][6] = xpos - bodysize; //x3
    snake[0][1][7] = ypos - bodysize; //y3 
    snake[0][1][8] = zpos[1]; //z3

    snake[0][1][9] = xpos + bodysize; //x4
    snake[0][1][10] = ypos - bodysize; //y4
    snake[0][1][11] = zpos[1]; //z4

    // front face
    snake[0][2][0] = xpos + bodysize; //x1
    snake[0][2][1] = ypos + bodysize; //y1
    snake[0][2][2] = zpos[1]; //z1

    snake[0][2][3] = xpos - bodysize; //x2
    snake[0][2][4] = ypos + bodysize; //y2 
    snake[0][2][5] = zpos[1]; //z2

    snake[0][2][6] = xpos - bodysize; //x3
    snake[0][2][7] = ypos - bodysize; //y3 
    snake[0][2][8] = zpos[1]; //z3

    snake[0][2][9] = xpos + bodysize; //x4
    snake[0][2][10] = ypos - bodysize; //y4
    snake[0][2][11] = zpos[1]; //z4

    // back face
    snake[0][3][0] = xpos + bodysize; //x1
    snake[0][3][1] = ypos - bodysize; //y1
    snake[0][3][2] = zpos[0]; //z1

    snake[0][3][3] = xpos - bodysize; //x2
    snake[0][3][4] = ypos - bodysize; //y2 
    snake[0][3][5] = zpos[0]; //z2

    snake[0][3][6] = xpos - bodysize; //x3
    snake[0][3][7] = ypos + bodysize; //y3 
    snake[0][3][8] = zpos[0]; //z3

    snake[0][3][9] = xpos + bodysize; //x4
    snake[0][3][10] = ypos + bodysize; //y4
    snake[0][3][11] = zpos[0]; //z4

    // left face
    snake[0][4][0] = xpos - bodysize; //x1
    snake[0][4][1] = ypos + bodysize; //y1
    snake[0][4][2] = zpos[0]; //z1

    snake[0][4][3] = xpos - bodysize; //x2
    snake[0][4][4] = ypos + bodysize; //y2 
    snake[0][4][5] = zpos[1]; //z2

    snake[0][4][6] = xpos - bodysize; //x3
    snake[0][4][7] = ypos - bodysize; //y3 
    snake[0][4][8] = zpos[1]; //z3

    snake[0][4][9] = xpos - bodysize; //x4
    snake[0][4][10] = ypos - bodysize; //y4
    snake[0][4][11] = zpos[0]; //z4

    // right face
    snake[0][5][0] = xpos + bodysize; //x1
    snake[0][5][1] = ypos + bodysize; //y1
    snake[0][5][2] = zpos[0]; //z1

    snake[0][5][3] = xpos + bodysize; //x2
    snake[0][5][4] = ypos + bodysize; //y2 
    snake[0][5][5] = zpos[1]; //z2

    snake[0][5][6] = xpos + bodysize; //x3
    snake[0][5][7] = ypos - bodysize; //y3 
    snake[0][5][8] = zpos[1]; //z3

    snake[0][5][9] = xpos + bodysize; //x4
    snake[0][5][10] = ypos - bodysize; //y4
    snake[0][5][11] = zpos[0]; //z4
}
void collision();
void drawPlane() {
    glColor3f(0.5f, 0.5f, 0.5f); // Gray color
    t_load(1);
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex3f(-10.0f, -1.0f, 10.0f); // Bottom left
    glTexCoord2d(1, 0);
    glVertex3f(10.0f, -1.0f, 10.0f);  // Bottom right
    glTexCoord2d(1, 1);
    glVertex3f(10.0f, -1.0f, -10.0f); // Top right
    glTexCoord2d(0, 1);
    glVertex3f(-10.0f, -1.0f, -10.0f); // Top left
    glEnd();
}
/* setup and initialize display function */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat sunAmbient[] = { 1.0, 1.0, 1.0, 1.0 }; // Ambient color (red)
    GLfloat sunDiffuse[] = { 1.0, 1.0, 0.0, 1.0 }; // Diffuse color (red)
    GLfloat sunSpecular[] = { 1.0, 1.0, 1.0, 1.0 }; // Specular color (white)
    GLfloat sunShininess = 50.0; // Shininess factor
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sunAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, sunDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sunSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, sunShininess);
    //first draw plane
    drawPlane();
    // second draw head
    initHead();
    t_load(2);
    glBegin(GL_QUADS);
    for (int i = 0; i < 6; i++) {
        //glColor3f(0.99, 0.85, 0.05);
        for (int j = 0; j < 12; j += 3) {
            if (i == 2) {
                switch (j) {
                case 0:
                    tx = 0; ty = 0;
                    break;
                case 3:
                    tx = 1; ty = 0;
                    break;
                case 6:
                    tx = 1; ty = 1;
                    break;
                case 9:
                    tx = 0; ty = 1;
                    break;
                }
                glTexCoord2d(tx, ty);
            }
            glVertex3f(snake[0][i][j], snake[0][i][j + 1], snake[0][i][j + 2]);
        }
    }
    glEnd();

    t_load(3);
    glBegin(GL_QUADS);
    // draw snake body
    for (int i = 1; i < snakelen; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 12; k += 3) {
                    switch (k) {
                    case 0:
                        tx = 0; ty = 0;
                        break;
                    case 3:
                        tx = 1; ty = 0;
                        break;
                    case 6:
                        tx = 1; ty = 1;
                        break;
                    case 9:
                        tx = 0; ty = 1;
                        break;
                    }
                    glTexCoord2d(tx, ty);
                glVertex3f(snake[i][j][k], snake[i][j][k + 1], snake[i][j][k + 2]);
            }
        }
    }
    glEnd();
    if (gameover) {
        // Render "Game Over" message
        glColor3f(1.0, 0.0, 0.0); // Red color
        glRasterPos3f(3.0, 5.0, -1); // Position of the text
        for (char c : gameoverText) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c); // Render each character
        }
        cout << "Damn!!! You almost ate yourself you must be hungry" << endl;
        cout << "Final score is " << score << endl;
        system("pause");
    }
    drawFood();
    collision();
    glutSwapBuffers();
}
void drawFood() {

    //define coordinates and grow snake according to snake collision
    switch (direction) {
    case 1: //right
        if (round(snake[0][5][3] - (randx - bodysize)) == 0
            && round(0.5 * snake[0][5][4] - 0.5 * 0.15) == 0
            && round(1.5 * snake[0][5][5] - 1.5 * (-randz + bodysize)) == 0)
        {
            randomPos();
            if (maxsize > snakelen)
                snakelen++;
            initPos();
            score++;
            break;
        }
        break;
    case -1: //left
        if (round(snake[0][4][3] - (randx)) == 0
            && round(0.5 * snake[0][4][4] - 0.5 * 0.15) == 0
            && round(1.5 * snake[0][4][5] - 1.5 * (-randz + bodysize)) == 0)
        {
            randomPos();
            if (maxsize > snakelen)
                snakelen++;
            initPos();
            score++;
            break;
        }
        break;
    case 2: //up
        if (round(snake[0][3][3] - (randx - bodysize)) == 0
            && round(0.5 * snake[0][3][4] - 0.5 * 0.15) == 0
            && round(1.5 * snake[0][3][5] - 1.5 * (-randz + bodysize)) == 0)
        {
            randomPos();
            if (maxsize > snakelen)
                snakelen++;
            initPos();
            score++;
            break;
        }
        break;
    case -2: //down
        if (round(snake[0][2][3] - (randx - bodysize)) == 0
            && round(0.5 * snake[0][2][4] - 0.5 * 0.15) == 0
            && round(1.5 * snake[0][2][5] - 1.5 * (-randz)) == 0)
        {
            randomPos();
            if (maxsize > snakelen)
                snakelen++;
            initPos();
            score++;
            break;
        }
        break;
    }
    t_load(4);
    glBegin(GL_QUADS);
    // Draw the Top face (y = 1.0f)
    //glColor3f(0.0f, 1.0f, 0.0f); // Green
    glTexCoord2d(0,0);
    glVertex3f(randx, 0.15, -randz); //inner right vertex
    glTexCoord2d(1,0);
    glVertex3f(randx - bodysize, 0.15, -randz); //inner left vertex
    glTexCoord2d(1,1);
    glVertex3f(randx - bodysize, 0.15, -randz + bodysize); //outer left vertex
    glTexCoord2d(0,1);
    glVertex3f(randx, 0.15, -randz + bodysize); //outer right vertex

    // Draw the Bottom face (y = -randx)
    //glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glTexCoord2d(0, 0);
    glVertex3f(randx, -0.15, -randz); //inner right vertex
    glTexCoord2d(1, 0);
    glVertex3f(randx - bodysize, -0.15, -randz); //inner left vertex
    glTexCoord2d(1, 1);
    glVertex3f(randx - bodysize, -0.15, -randz + bodysize); //outer left vertex
    glTexCoord2d(0, 1);
    glVertex3f(randx, -0.15, -randz + bodysize); //outer right vertex

    // Draw the Front face  (z = randx)
    //glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glTexCoord2d(0, 0);
    glVertex3f(randx, 0.15, -randz + bodysize); // outer top right vertex
    glTexCoord2d(1, 0);
    glVertex3f(randx - bodysize, 0.15, -randz + bodysize); // outer top left vertex
    glTexCoord2d(1, 1);
    glVertex3f(randx - bodysize, -0.15, -randz + bodysize); // outer bottom left vertex
    glTexCoord2d(0, 1);
    glVertex3f(randx, -0.15, -randz + bodysize); // outer bottom right vertex

    // Draw the Back face (z = -randx)
   // glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glTexCoord2d(0, 0);
    glVertex3f(randx, -0.15, -randz);
    glTexCoord2d(1, 0);
    glVertex3f(randx - bodysize, -0.15, -randz);
    glTexCoord2d(1, 1);
    glVertex3f(randx - bodysize, 0.15, -randz);
    glTexCoord2d(0, 1);
    glVertex3f(randx, 0.15, -randz);

    // Draw the Left face (x = -randx)
   // glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glTexCoord2d(0, 0);
    glVertex3f(randx - bodysize, 0.15, -randz); //inner top left
    glTexCoord2d(1, 0);
    glVertex3f(randx - bodysize, 0.15, -randz + bodysize); //outer top left
    glTexCoord2d(1, 1);
    glVertex3f(randx - bodysize, -0.15, -randz + bodysize); //outer bottom left
    glTexCoord2d(0,1);
    glVertex3f(randx - bodysize, -0.15, -randz); //inner bottom left

    // Draw the Right face (x = randx)
   // glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glTexCoord2d(1,1);
    glVertex3f(randx, 0.15, -randz);
    glTexCoord2d(0,1);
    glVertex3f(randx, 0.15, -randz + bodysize);
    glTexCoord2d(0,0);
    glVertex3f(randx, -0.15, -randz + bodysize);
    glTexCoord2d(1,0);
    glVertex3f(randx, -0.15, -randz);
    glEnd();
}

void randomPos() {
    temp = randx;
    srand(time(NULL));
    randx = rand() % (foodX1 - foodX2 - 1) + foodX2;
    while (temp == randx)
        randx = rand() % (foodX1 - foodX2 - 1) + foodX2;
    randz = randx;
}

void initPos() {
    for (int i = snakelen - 1; i > 0; i--) {
        for (int j = 0; j < 6; j++) {
            glColor3f(1, 0.55, 0);
            for (int k = 0; k < 12; k += 3) {
                snake[i][j][k] = snake[i - 1][j][k] - bodysize * 2;
                snake[i][j][k + 1] = snake[i - 1][j][k + 1];
                snake[i][j][k + 2] = snake[i - 1][j][k + 2];
            }
        }
    }
}

void swapPos() {
    switch (direction) {
    case 1: //right
        for (int i = snakelen - 1; i > 0; i--) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 12; k += 3) {
                    snake[i][j][k] = snake[i - 1][j][k] - bodysize - acceleration;
                    snake[i][j][k + 1] = snake[i - 1][j][k + 1];
                    snake[i][j][k + 2] = snake[i - 1][j][k + 2];
                }
            }
        }
        break;
    case -1: //left
        for (int i = snakelen - 1; i > 0; i--) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 12; k += 3) {
                    snake[i][j][k] = snake[i - 1][j][k] + bodysize + acceleration;
                    snake[i][j][k + 1] = snake[i - 1][j][k + 1];
                    snake[i][j][k + 2] = snake[i - 1][j][k + 2];
                }
            }
        }
        break;
    case 2: //up
        for (int i = snakelen - 1; i > 0; i--) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 12; k += 3) {
                    snake[i][j][k] = snake[i - 1][j][k];
                    snake[i][j][k + 1] = snake[i - 1][j][k + 1];
                    snake[i][j][k + 2] = snake[i - 1][j][k + 2] + bodysize * 2;
                }
            }
        }
        break;
    case -2: //down
        for (int i = snakelen - 1; i > 0; i--) {
            for (int j = 0; j < 6; j++) {
                for (int k = 0; k < 12; k += 3) {
                    snake[i][j][k] = snake[i - 1][j][k];
                    snake[i][j][k + 1] = snake[i - 1][j][k + 1];
                    snake[i][j][k + 2] = snake[i - 1][j][k + 2] - bodysize * 2;
                }
            }
        }
        break;
    }
}


/* Called back when timer expired [NEW] */
void timer(int value) {
    glutPostRedisplay();      // Post re-paint request to activate display()
    glutTimerFunc(refreshMills, timer, 0); // next timer call milliseconds later
    // Code direction
    switch (direction) {
    case 1: //right
        if (xpos < 10 - aspect * bodysize - bodysize - acceleration) {
            xpos += acceleration;
        }
        else {
            cout << "Final score is " << score << endl;
            exit(0);
        }
        break;
    case -1: //left
        if (xpos > -10 + aspect * bodysize + bodysize + acceleration) {
            xpos -= acceleration;
        }
        else {
            cout << "Final score is " << score << endl;
            exit(0);
        }
        break;
    case 2: //up
        if (zpos[0] > -10 + (aspect * bodysize + 2 * bodysize + acceleration * aspect)) {
            zpos[0] -= acceleration;
            zpos[1] -= acceleration;
        }
        else {
            cout << "Final score is " << score << endl;
            exit(0);
        }
        break;
    case -2: //down
        if (zpos[1] < 10 + aspect * bodysize - acceleration * aspect) {
            zpos[0] += acceleration;
            zpos[1] += acceleration;
        }
        else {
            cout << "Final score is " << score << endl;
            exit(0);
        }
        break;
    }
    swapPos();
}
void collision() {

    switch (direction) {
    case 1:
        dist1 = (snake[0][5][3]);
        dist2 = (snake[0][5][4]);
        dist3 = (snake[0][5][5]);
        for (int i = 2; i < snakelen; i++) {
            dist1x = (snake[i][4][3]);
            dist2x = (snake[i][4][4]);
            dist3x = (snake[i][4][5]);
            if (abs(dist1x - dist1) <= 0.1 && abs(dist2x - dist2) <= 0.1 && abs(dist3x - dist3) <= 0.1) {
                acceleration = 0;
                gameover = true;

            }
        }
        break;
    case -1:
        dist1 = (snake[0][4][3]);
        dist2 = (snake[0][4][4]);
        dist3 = (snake[0][4][5]);
        for (int i = 2; i < snakelen; i++) {
            dist1x = (snake[i][5][3]);
            dist2x = (snake[i][5][4]);
            dist3x = (snake[i][5][5]);
            if (abs(dist1x - dist1) <= 0.1 && abs(dist2x - dist2) <= 0.1 && abs(dist3x - dist3) <= 0.1) {
                acceleration = 0;
                gameover = true;

 
            }
        }
        break;
    case 2:
        dist1 = (snake[0][3][3]);
        dist2 = (snake[0][3][4]);
        dist3 = (snake[0][3][5]);
        for (int i = 2; i < snakelen; i++) {
            dist1x = (snake[i][2][6]);
            dist2x = (snake[i][2][7]);
            dist3x = (snake[i][2][8]);
            if (abs(dist1x - dist1) <= 0.2 && abs(dist2x - dist2) <= 0.2 && abs(dist3x - dist3) <= 0.2) {
                acceleration = 0;
                gameover = true;

            }
        }
        break;
    case -2:
        dist1 = (snake[0][2][3]);
        dist2 = (snake[0][2][4]);
        dist3 = (snake[0][2][5]);
        for (int i = 2; i < snakelen; i++) {
            dist1x = (snake[i][3][6]);
            dist2x = (snake[i][3][7]);
            dist3x = (snake[i][3][8]);
            if (abs(dist1x - dist1) <= 0.2 && abs(dist2x - dist2) <= 0.2 && abs(dist3x - dist3) <= 0.2) {
                acceleration = 0;
                gameover = true;
            }
        }
        break;
    }

}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
   // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    gluLookAt(
        0, 10, 20,
        0, -2, 0,
        0, 1, 0
    );
}

void keyboard(unsigned char key, int x, int y) {
    key = tolower(key);
    switch (key) {
    case 119: // w
        if (direction != -2)
            direction = 2;
        break;
    case 115: // s
        if (direction != 2)
            direction = -2;
        break;
    case 97: // a
        if (direction != 1)
            direction = -1;
        break;
    case 100: // d
        if (direction != -1)
            direction = 1;
        break;
    }
}
/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(800, 600);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    initGL();                       // Our own OpenGL initialization
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);     // First timer call immediately [NEW]
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}