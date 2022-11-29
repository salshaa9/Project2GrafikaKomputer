
//Nama Kelompok
//Salsabhilla Anggraeni Wardani (5221600019)
//Aynur Rofiqoh (5221600029)

#define GLUT_DISABLE_ATEXIT_HACK

#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include<bits/stdc++.h>



using namespace std;
const int width = 700;
const int height = 700;

GLfloat eyeX = 31;
GLfloat eyeY = 5;
GLfloat eyeZ = -27;

GLfloat centerX = 7;
GLfloat centerY = 5;
GLfloat centerZ = 28;
double angle = 0 ;
bool l_on1 = true;
bool l_on2 = true;
bool l_on3 = true;
bool l_on4 = false;
float rot = -12;
int stop=1;
float door_angle c =.5;
float l_height =.5;
float spt_cutoff = 30;
unsigned int ID;
vector<int>v;






static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}







static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7},  //front
    {6,4,0,2},  //back
    {2,3,7,6},  //top
    {1,0,4,5},  //bottom
    {7,5,4,6},  //right
    {2,0,1,3}   //left
};


void cube(float R=0.5, float G=0.5, float B=0.5, int type=0, float val=1)
{

    GLfloat m_no[] = {0, 0, 0, 1.0};
    GLfloat m_amb[] = {R,G,B,1};
    GLfloat m_diff[] = {R,G,B,1};
    GLfloat m_spec[] = {1,1,1,1};
    GLfloat m_sh[] = {30};

    GLfloat m_em[] = {1,1,1,1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_sh);
    if(type==1)
    {

        if(l_on1)
            glMaterialfv(GL_FRONT, GL_EMISSION, m_em);
        else
            glMaterialfv(GL_FRONT, GL_EMISSION, m_no);

    }
    else if(type==2)
    {
        if(l_on3)
            glMaterialfv(GL_FRONT, GL_EMISSION, m_em);
        else
            glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, m_no);


    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);
        glTexCoord2f(0,val);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(val,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(val,val);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
    }
    glEnd();
}
class BmpLoader
{
public:
    unsigned char* textureData;
    int iWidth, iHeight;

    BmpLoader(const char*);
    ~BmpLoader();

private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
};

BmpLoader::BmpLoader(const char* filename)
{
    FILE *file=0;
    file=fopen(filename, "rb");
    if(!file)
        cout<<"File not found"<<endl;
    fread(&bfh, sizeof(BITMAPFILEHEADER),1,file);
    if(bfh.bfType != 0x4D42)
        cout<<"Not a valid bitmap"<<endl;
    fread(&bih, sizeof(BITMAPINFOHEADER),1,file);
    if(bih.biSizeImage==0)
        bih.biSizeImage=bih.biHeight*bih.biWidth*3;
    textureData = new unsigned char[bih.biSizeImage];
    fseek(file, bfh.bfOffBits, SEEK_SET);
    fread(textureData, 1, bih.biSizeImage, file);
    unsigned char temp;
    for(int i=0; i<bih.biSizeImage; i+=3)
    {
        temp = textureData[i];
        textureData[i] = textureData[i+2];
        textureData[i+2] = temp;

    }

    iWidth = bih.biWidth;
    iHeight = bih.biHeight;
    fclose(file);
}

BmpLoader::~BmpLoader()
{
    delete [] textureData;
}

void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}


static void res(int width, int height)
{
    glViewport(0, 0, width, height);
}

void axes()
{
    float length = 10;
    float width = 0.3;

    // X-axis
    glPushMatrix();
    glTranslatef(length/2,0,0);
    glScalef(length,width,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.8,0.1,0.1);
    glPopMatrix();

    // Y-axis
    glPushMatrix();
    glTranslatef(0,length/2,0);
    glScalef(width,length,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.8,0.1);
    glPopMatrix();

    // Z-axis
    glPushMatrix();
    glTranslatef(0,0,length/2);
    glScalef(width,width,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.1,0.8);
    glPopMatrix();
}


void cse()
{

    glPushMatrix();
    glScalef(.3,.5,1);
    glTranslatef(45,15,1);
    glPushMatrix();

    //C
    glScalef(.4,1.5,2.9);


    glTranslatef(45,3,2);
    float length=3.4;
    float width=.4;
    //ground
    float a=1,b=0,c=.5;
    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(3*length,width,width);
    glTranslatef(-.75,-0.5,-0.5);
    cube(a,b,c);
    glPopMatrix();

//right

    glPushMatrix();
    glPushMatrix();
    glTranslatef(1,length/2,1);;
    glScalef(width+3,length+.5,width);
    glTranslatef(0,-0.2,-0.5);
    cube(a,b,c);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(length*3,width,width);
    glTranslatef(-.75,8.7,-0.5);
    cube(a,b,c);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

//E


    glPushMatrix();

    glScalef(.4,1.5,2.9);

    glTranslatef(15,3,2);

    //ground
    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(3*length,width,width);
    glTranslatef(-.75,-0.5,-0.5);
    cube(a,b,c);
    glPopMatrix();

//right

    glPushMatrix();
    glPushMatrix();
    glTranslatef(1,length/2+.1,1);;
    glScalef(width+3,length,width);
    glTranslatef(0,-0.2,-0.5);
    cube(a,b,c);
    glPopMatrix();

    //top
    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(length*3,width,width);
    glTranslatef(-.75,8.7,-0.5);
    cube(a,b,c);
    glPopMatrix();
    //right
    /*
    */
    //rack4
    glPushMatrix();
    glTranslatef(length,1,1);;
    glScalef(3*length,width,width);
    glTranslatef(-.94,4,-0.5);
    cube(a,b,c);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();



//s


    glPushMatrix();

    glScalef(.4,1.5,2.9);
    glTranslatef(30,3,2);

    //ground
    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(3*length,width,width);
    glTranslatef(-.75,-0.5,-0.5);
    cube(a,b,c);
    glPopMatrix();

//right

    glPushMatrix();
    glPushMatrix();
    glTranslatef(1,length/2,1);;
    glScalef(width+3,length/2,width);
    glTranslatef(0,0.7,-0.5);
    cube(a,b,c);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1,length/2,1);;
    glScalef(width+3,length/2+.1,width);
    glTranslatef(-2,-0.5,-0.5);
    cube(a,b,c);
    glPopMatrix();


    //top
    glPushMatrix();
    glTranslatef(length/2,1,1);;
    glScalef(length*3,width,width);
    glTranslatef(-.75,8.7,-0.5);
    cube(a,b,c);
    glPopMatrix();

    //rack4
    glPushMatrix();
    glTranslatef(length,1,1);;
    glScalef(3*length,width,width);
    glTranslatef(-.9,4,-0.5);
    cube(a,b,c);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();

}



void floorWallsCeiling(float scale)
{

    glPushMatrix();
    glScalef(scale,1,scale);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.8, 0.8, 0.8);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(scale/2,scale/4,0);
    glScalef(1,scale/2,scale);
    glTranslatef(-1,-0.5,-0.5);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-scale/2+1,scale/4,0);
    glScalef(1,scale/2,scale);
    glTranslatef(-1,-0.5,-0.5);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0,scale/4,scale/2);
    glScalef(scale,scale/2,1);
    glTranslatef(-0.5,-0.5,-1);
    cube(1.000, 0.855, 0.725);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,scale/4,scale/2);
    glScalef(scale,scale/2,1);
    glTranslatef(-0.5,-0.5,-31);
    cube(1.000, 0.855, 0.725);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(0,scale/2,0);
    glScalef(scale,1,scale);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.871, 0.722, 0.529);
    glPopMatrix();



   ;
}











void flr()
{


 glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[2]);
    glPushMatrix();


    glScalef(60,1,60);
    glTranslatef(-0.5,19,-0.5);
    cube(0.690, 0.769, 0.871,0,4);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}


void wall1()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[0]);

    glPushMatrix();

    glScalef(1,20,60);
    glTranslatef(-30,0,-0.5);
    cube(1,1,1,0,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


}
void wall2()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[0]);
    glPushMatrix();
   glTranslatef(0,-0.5,0);
    glScalef(1,20,60);
    glTranslatef(29,0,-0.5);
    cube(1,1,1,0,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void wall3()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[0]);
    glPushMatrix();

    glScalef(60,20,1);
    glTranslatef(-.5,0,10);
    cube(0.741, 0.718, 0.420,0,2);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


void board()
{
    glPushMatrix();

    glScalef(10,10,1);
    glTranslatef(-.7,.4,9);
    cube(1,1,1);
    glPopMatrix();


    glPushMatrix();

    glScalef(31.5,11,1);
    glTranslatef(-.601,.32,9.5);
    cube(0,0,0);
    glPopMatrix();
}
void window()
{
    glPushMatrix();
    glPushMatrix();

    glScalef(1,8,15);
    glTranslatef(28,.5,-1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[1]);
    cube(1,1,1,0,1);
    glBindTexture(GL_TEXTURE_2D,v[0]);
    glPopMatrix();
    glPushMatrix();

    glScalef(1,8,.2);
    glTranslatef(28,.5,-76);
    cube(1,.59,.0);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    glRotatef (door_angle, 0,1,0);


    glPushMatrix();

    glScalef(1,.2,15);
    glTranslatef(27.9,50,-1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();

    glScalef(1,.2,15);
    glTranslatef(27.9,30,-1);
    cube(0,0,0);
    glPopMatrix();
    for(int i=0; i>=-75; i-=2)
    {
        glPushMatrix();

        glScalef(1,8,.2);
        glTranslatef(27.9,.5,i);
        cube(0,0,0);
        glPopMatrix();
    }

    glPopMatrix();
}

void headwall()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,v[2]);
    glPushMatrix();


    glScalef(60,1,60);
    glTranslatef(-0.5,19,-0.5);
    cube(0.690, 0.769, 0.871,0,4);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
float k=0;
float angle1=0;
bool markdoor=0;
float window_val=0;
void spin()
{
    angle = (angle+k)*stop;
    k+=.001;
    if (angle >= 360)
        angle = 0;
    angle1+=.2;
    if (angle1 >= 360)
        angle1 = 0;
    bool ok=0;
    bool ok1=0;
    if(markdoor&&door_angle>-.8)
    {
        door_angle-=.001,ok=1;
        if(door_angle<.4)
            window_val+=.00085;
    
        l_on4=1;


    }
    if(!markdoor&&door_angle<.5)
    {
        door_angle+=.001,ok=1;
        window_val-=.001;

    }
    if(!ok)
        l_on4=markdoor;

}


float a=2;
float b=1;
float c= 10;
void fan()
{

    float length = 10;
    float width = 0.3;
    glPushMatrix();
    glTranslatef(1,length/2,-4);
    glScalef(width,length,width);
    glTranslatef(-20,1,20);
    cube(0.4,0.4,0.1);
    glPopMatrix();


    glPushMatrix();
    glScalef(1,.1,1);
    glTranslatef(-5,150,2);
    cube(1, 1, 1);           
    glRotatef(angle, 0, 0, 1);
    glRecti(-a, -a, a, a);
    cube(1, 0, 0);
    glRecti(-b, a, b, c);
    glRecti(-c, -b, -a, b);
    glRecti(-b, -c, b, -a);
    glRecti(a, -b, c, b);
    glPopMatrix();
}

void Chair12()
{
    float height=2;
    float width=4;
    float length=2;

    float base_height=.5;
    float leg_height=height-base_height;
    float leg_width=.4;

 
    glPushMatrix();
    glTranslatef(0,leg_height,0);

    glPushMatrix();
    glScalef(width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.4f, 0.302f, 0.0f);
    glPopMatrix();

  
    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.4f, 0.302f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.4f, 0.302f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.4f, 0.302f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.4f, 0.302f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glScalef(3,2,.5);
    glTranslatef(-0.5,.5,-1);
    cube(1,1,0);
    glPopMatrix();

    
    glPushMatrix();
    glScalef(.2,1,.2);
    glTranslatef(3,0,-2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glScalef(.2,1,.2);
    glTranslatef(-4,0,-2);
    cube(0,0,0);
    glPopMatrix();



    glPopMatrix();

}

void Table12(float height=2, float width=4,float length=2,float base_height=.5,float leg_width=.2)
{

    float leg_height=height-base_height;


    glPushMatrix();
    glTranslatef(0,leg_height,0);


    glPushMatrix();
    glScalef(width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(.6,.2,0);
    glPopMatrix();


    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(.8f, .6f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(.8f, .6f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(.8f, .6f, 0.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(.8f, .6f, 0.0f);
    glPopMatrix();
    glPopMatrix();
}
void test()
{

    glPushMatrix();
    glTranslatef(10.0, 1, -14+9 * 2.0 + 2.2f);
    glScalef(0.8f, 0.8f, 0.8f);
    glRotatef(180.0, 0.0, 1.0, 0.0);
    Chair12();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10.0, 1, -16+9 * 2.0 + 2.2f);
    glScalef(1.2f, 1.3f, 0.8f);
    Table12(2,4,2,.6,.3);
    glPopMatrix();

    for (int i = -8; i <= 7; i+=2)
    {
        for (int j = -4; j <= 6; j+=2)
        {
            glPushMatrix();
            glTranslatef(i*2.0, 0.6f, -16+j * 2.0 + 2.2f);
            glScalef(0.75f, 0.75f, 0.75f);
            glRotatef(0.0, 0.0, 1.0, 0.0);
            glPopMatrix();
        }
    }


    for (int i = -8; i <= 7; i+=2)
    {
        for (int j = -4; j <= 6; j+=2)
        {
            glPushMatrix();
            glTranslatef(i*2.0 + 0.3f, 1.2f,-14.2+ j * 2.0 + 1.2f);
            glScalef(1.0f, 1.0f, 1.0f);
            Table12();
            glPopMatrix();
        }
    }

}



void table()
{
    float height=1;
    float width=30;
    float length=5;

    float base_height=.5;
    float leg_height=height-base_height;
    float leg_width=1;


    glPushMatrix();
    glTranslatef(0,0,5);
    glPushMatrix();
    glTranslatef(0,leg_height,0);


    glPushMatrix();
    glScalef(width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();


    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}
void drop()
{

    glPushMatrix();

    cube(113/255.0,74/255.0,44/255.0);
 
    glBegin(GL_POLYGON);
    glVertex3f (460, 660, 0);
    glVertex3f (470, 670, 0);
    glVertex3f (330, 670,0);
    glVertex3f (340, 660, 0);
    glVertex3f (340, 320, 0);
    glVertex3f (460, 320, 0);
    glEnd();




    glPopMatrix();

    glPushMatrix();


    glTranslatef(0,0,0);
    glScalef(105,95,10);

    glPushMatrix();
    glTranslatef(3.32,4.6,-.49);
    cube(62/255.0,43/255.0,45/255.0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(3.32,5.7,-.49);
    cube(62/255.0,43/255.0,45/255.0);
    glPopMatrix();


    glPushMatrix();
    glTranslatef(3.32,3.5,-.49);
    cube(62/255.0,43/255.0,45/255.0);
    glPopMatrix();
    glPopMatrix();




    glPushMatrix();
    glTranslatef(1,0,0);
    glScalef(20,10,10);
    glPushMatrix();
    glTranslatef(19.5,38,-.5);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(19.5,58,-.5);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(19.5,48,-.5);
    cube(1,1,1);
    glPopMatrix();

    glPopMatrix();


    cube(62/255.0,43/255.0,35/255.0);
    glColor3ub (62,43,35);
    glBegin(GL_POLYGON);
    glVertex3f (470, 670, 0);
    glVertex3f (500, 667, 0);
    glVertex3f (492, 657, 0);
    glVertex3f (492, 330, 0);
    glVertex3f (460, 320, 0);
    glVertex3f (460, 660, 0);
    glEnd();

    glPopMatrix();

}
void weardrop()
{
    glPushMatrix();

    glTranslatef(9,-7,8);

    glScalef(.04,.022,.03);
    drop();
    glPopMatrix();
}


void weardrop2()
{
    glPushMatrix();
    glTranslatef(-42,-7,6.5);



    glScalef(.04,.022,.03);
    drop();
    glPopMatrix();
}

void projector()
{
     glPushMatrix();
    glScalef(2,.7,1.8);
    glTranslatef(-0.43,15,-.4);
    cube(1, 0, 0);
    glPopMatrix();

    glPushMatrix();
    glScalef(.5,3.4,.5);
    glTranslatef(0,3.2,0);
    cube(0.392, 0.3, 0.929);
    glPopMatrix();

    glPushMatrix();
    glScalef(.68,.68,.68);
    glTranslatef(0,15.45,1);
    cube(1,0,1);
    glPopMatrix();

    glPushMatrix();
    glScalef(.60,.60,.60);
    glTranslatef(0.05,17.55,1.4);
    cube(0,0,0);
    glPopMatrix();
}

void projector_board()
{
    glPushMatrix();
    glScalef(6,6,0.1);
    glTranslatef(-0.2,.8,130);
    cube(1, 1, 1);
    glPopMatrix();

}


void AC()
{
    glPushMatrix();
    glScalef(6,2,2);
    glTranslatef(-1.5,6,-10.5);
    cube(.8, .8,.8);
    glPopMatrix();
}

void AC2()
{
    glPushMatrix();
    glScalef(6,2,2);
    glTranslatef(1.5,6,-10.5);
    cube(.8, .8,.8);
    glPopMatrix();



}
void clock1()
{

    glPushMatrix();
//clock outer
    glPushMatrix();
    glScalef(1.5,1.3,1);
    glTranslatef(-220,-200,1);
    cube(0,0,0);
    glBegin(GL_QUADS);
    glVertex3f (680, 800, 0);
    glVertex3f (630, 800, 0);
    glVertex3f (630, 900, 0);
    glVertex3f (680, 900, 0);
    glEnd();


    glPushMatrix();
    glTranslatef(0,0,-.98);
    cube(0.847, 0.749, 0.847);
    glBegin(GL_QUADS);
    glVertex3f (675, 805, 0);
    glVertex3f (635, 805, 0);
    glVertex3f (635, 895, 0);
    glVertex3f (675, 895, 0);
    glEnd();
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-2);
////////////////////////////
    glPushMatrix();
    glTranslatef(653,847,1);
    glRotatef(angle1, 0, 0, 1);

    cube(0, 0, 0);
    glRecti(-b, a, b, 3*c);
    glPopMatrix();
    /////////////////////////////
//clock second
    cube(0,0,0);

    glBegin(GL_LINES);
    glVertex3f (655, 850, 0);
    glVertex3f (655, 870, 0);
    glEnd();





//clock hour
    cube(0,0,0);
    glBegin(GL_LINES);

//glBegin(GL_POINT_SIZE);
    glVertex3f (655, 850, 0);
    glVertex3f (645, 810, 0);
    glEnd();
    glPopMatrix();
    glPopMatrix();
}
void clock2()
{


    glPushMatrix();
//    glTranslatef(0,-0.5,0);

    glTranslatef(-75,-30,9);
//glRotatef(rot,0,1,1);
    glScalef(.08,.05,.05);
    clock1();
    glPopMatrix();
}

float al1,dl1,sl1;
float al2,dl2,sl2;
float al3,dl3,sl3;
void light1(float a, float b, float c)
{
    glEnable(GL_LIGHT0);

    //light
    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {0.4f+al1, 0.4f+al1, 0.4f+al1, 1.0f};
    GLfloat l_dif[] = {.4f+dl1,.4f+dl1,.4f+dl1,1};
    GLfloat l_spec[] = {.4f+sl1,.4f+sl1,.4f+sl1,1};
    GLfloat l_pos[] = {a,b,c,1.0};

    if(l_on1)
        glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
    else
        glLightfv(GL_LIGHT0, GL_AMBIENT, l_no);
    if(l_on1)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l_dif);
    else
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l_no);
    if(l_on1)
        glLightfv(GL_LIGHT0, GL_SPECULAR, l_spec);
    else
        glLightfv(GL_LIGHT0, GL_SPECULAR, l_no);

    glLightfv(GL_LIGHT0, GL_POSITION, l_pos);


}

void light2(float a,float b,float c)
{
    glEnable(GL_LIGHT2);
    //light
    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {0.3f+al2, 0.3f+al2, 0.3f+al2, 1.0f};
    GLfloat l_dif[] = {.6f+dl2,.6f+dl2,.6f+dl2,1};
    GLfloat l_spec[] = {.5f+sl2,.5f+sl2,.5f+sl2,1};
    GLfloat l_pos[] = {a,b,c,1.0};

    if(l_on3)
        glLightfv(GL_LIGHT2, GL_AMBIENT, l_amb);
    else
        glLightfv(GL_LIGHT2, GL_AMBIENT, l_no);
    if(l_on3)
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l_dif);
    else
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l_no);
    if(l_on3)
        glLightfv(GL_LIGHT2, GL_SPECULAR, l_spec);
    else
        glLightfv(GL_LIGHT2, GL_SPECULAR, l_no);

    glLightfv(GL_LIGHT2, GL_POSITION, l_pos);



}




void spot_light(float a,float b,float c)
{
    //light
    glEnable(GL_LIGHT1);

    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {1+al3, 0+al3, 0+al3, 1.0};
    GLfloat l_dif[] = {1+dl3,1+dl3,1+dl3,1};
    GLfloat l_spec[] = {1+sl3,1+sl3,1+sl3,1};
    GLfloat l_pos[] = {a,b,c,1.0};

    if(l_on2)
        glLightfv(GL_LIGHT1, GL_AMBIENT, l_amb);
    else
        glLightfv(GL_LIGHT1, GL_AMBIENT, l_no);
    if(l_on2)
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l_dif);
    else
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l_no);
    if(l_on2)
        glLightfv(GL_LIGHT1, GL_SPECULAR, l_spec);
    else
        glLightfv(GL_LIGHT1, GL_SPECULAR, l_no);

    glLightfv(GL_LIGHT1, GL_POSITION, l_pos);
    GLfloat l_spt[] = {0,-1,0,1};
    GLfloat spt_ct[] = {30};
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l_spt);
    glLightfv(GL_LIGHT1, GL_SPOT_CUTOFF, spt_ct);

}

void window_light(float a,float b,float c)
{
    //light
    glEnable(GL_LIGHT3);

    GLfloat l_no[] = {0, 0, 0, 1.0};
    GLfloat l_amb[] = {.5f * window_val, .5f * window_val, .5f * window_val, 1.0};
    GLfloat l_dif[] = {.1f * window_val,.1f * window_val,.1f * window_val,1};
    GLfloat l_spec[] = {.1f * window_val,.1f * window_val,.1f * window_val,1};
    GLfloat l_pos[] = {a,b,c,1.0};

    if(l_on4)
        glLightfv(GL_LIGHT3, GL_AMBIENT, l_amb);
    else
        glLightfv(GL_LIGHT3, GL_AMBIENT, l_no);
    if(l_on4)
        glLightfv(GL_LIGHT3, GL_DIFFUSE, l_dif);
    else
        glLightfv(GL_LIGHT3, GL_DIFFUSE, l_no);
    if(l_on4)
        glLightfv(GL_LIGHT3, GL_SPECULAR, l_spec);
    else
        glLightfv(GL_LIGHT3, GL_SPECULAR, l_no);

    glLightfv(GL_LIGHT3, GL_POSITION, l_pos);
    GLfloat l_spt[] = {0,-1,0,1};
    GLfloat spt_ct[] = {84};
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, l_spt);
    glLightfv(GL_LIGHT3, GL_SPOT_CUTOFF, spt_ct);

}
void light()
{


    glPushMatrix();
    glPushMatrix();

    glRotatef(200, 0,1,0);
    float a=-5,b=16,c=9;
    light1(a,b,c);
    glPopMatrix();
    glTranslatef(a,b+1,c);
    glScalef(15,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1,1);
    glPopMatrix();


    glPushMatrix();
    glPushMatrix();

    glRotatef(200, 0,1,0);
    a=15,b=17,c=-15;
    light2(a,b,c);
    glPopMatrix();
    glTranslatef(a,b+1,c);
    glScalef(15,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1,2);
    glPopMatrix();


    //spot light
    glPushMatrix();
    glPushMatrix();
    glRotatef(-95, 0,1,0);
    a=15,b=30,c=-15;
    spot_light(a,b,c);

    glPopMatrix();
    glTranslatef(a,b+1,c);
    glScalef(15,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube(1,0,0,true);
    glPopMatrix();


    //window light
    glPushMatrix();
    glPushMatrix();
    glRotatef(0, 0,1,0);
    a=35,b=15,c=-10;
    window_light(a,b,c);

    glPopMatrix();
    glTranslatef(a,b+1,c);
    glScalef(15,1,1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube(1,0,0,true);
    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 2.0, 100.0);
    gluPerspective(0,0,0,0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, centerX,centerY,centerZ, 0,1,0);


    glRotatef(rot, 0,1,0);
    table();
    floorWallsCeiling(100);
    wall1();
    wall2();
    wall3();
    board();
    headwall();
    clock2();
    weardrop();
    weardrop2();
    light();
projector();
AC();
AC2();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y);

float l_val=.1;

static void light14(unsigned char light1, int x, int y)
{

    switch (light1)
    {

    case 27:
        glutKeyboardFunc(key);
        break;
    case '1':
        al1+=l_val;
        break;
    case '2':
        al1-=l_val;
        break;
    case '3':
        dl1+=l_val;
        break;
    case '4':
        dl1-=l_val;
        break;
    case '5':
        sl1+=l_val;
        break;
    case '6':
        sl1-=l_val;
        break;
    }
read:
    return;
}

static void light24(unsigned char light2, int x, int y)
{
    switch (light2)
    {

    case 27:
        glutKeyboardFunc(key);

        break;
    case '1':
        al2+=l_val;
        break;
    case '2':
        al2-=l_val;
        break;
    case '3':
        dl2+=l_val;
        break;
    case '4':
        dl2-=l_val;
        break;
    case '5':
        sl2+=l_val;
        break;
    case '6':
        sl2-=l_val;
        break;
    }
}

static void spot_light14(unsigned char spot_light, int x, int y)
{
    switch (spot_light)
    {

    case 27:

        glutKeyboardFunc(key);
        break;
    case '1':
        al3+=l_val;
        break;
    case '2':
        al3-=l_val;
        break;
    case '3':
        dl3+=l_val;
        break;
    case '4':
        dl3-=l_val;
        break;
    case '5':
        sl3+=l_val;
        break;
    case '6':
        sl3-=l_val;
        break;
    }
}
static void key(unsigned char key, int x, int y)
{
    switch (key)
    {

    case 27:
        exit(0);
        break;
    case '+':
        eyeZ++;
        //eyeY++;
        break;
    case '-':
        eyeZ--;
        // eyeY--;
        break;

    case 'a':
        eyeX++;

        //eyeY++;
        break;
    case 's':
        eyeX--;
        // eyeY--;
        break;

    case 'g':
        eyeY++;
        //eyeY++;
        break;
    case 'h':
        eyeY--;
        // eyeY--;
        break;

    case 'w':
        //centerX++;
        centerY++;
        //  centerZ++;
        break;
    case 'e':
        //  centerX--;
        centerY--;
        //  centerZ--;
        break;

    case 'o':
        centerX++;

        //centerY++;
        //  centerZ++;
        break;
    case 'p':
        centerX--;
        //centerY--;
        //  centerZ--;
        break;

    case 'k':
        centerZ++;
        //centerY++;
        //  centerZ++;
        break;
    case 'l':
        centerZ--;
        //centerY--;
        //  centerZ--;
        break;


    case ',':
        rot++;
        break;
    case '.':
        rot--;
        break;

    case '*':
        stop=0;
        break;

    case ';':
        angle=0,stop=1,k=0;
        break;

    case '1':
        l_height+=.1;
        break;
    case '2':
        l_height-=.1;
        break;

    case '3':
        spt_cutoff++;
        break;
    case '4':
        spt_cutoff--;
        break;
    case 't':
        l_on1=1-l_on1;
        break;

    case 'u':
        l_on2=1-l_on2;
        break;

    case 'y':
        l_on3=1-l_on3;
        break;
    case 'd':
        markdoor=1-markdoor;
        break;
    case 'b':
        glutKeyboardFunc(light14);
        break;
    case 'n':
        glutKeyboardFunc(light24);
        break;
    case 'm':
        glutKeyboardFunc(spot_light14);
        break;

    }
    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
    spin();
}

/* Program entry point */
void texture_image()
{
    LoadTexture("C:\\Users\\Sourav\\Desktop\\ui\\figures\\wall1.bmp");
    v.push_back(ID);
    LoadTexture("C:\\Users\\Sourav\\Desktop\\ui\\figures\\head.bmp");
    v.push_back(ID);
    LoadTexture("C:\\Users\\Sourav\\Desktop\\ui\\figures\\top.bmp");
    v.push_back(ID);
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(50,50);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Yoshe UwU LopeLope");
    glutReshapeFunc(res);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);


    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);
    GLfloat globalAmbient[] = {0.0,0.0,0.0,1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,globalAmbient);
    texture_image();
    int t=1;
    printf("MATIKAN CAPSLOCK DAN GUNAKAN SHIFT KEY SEBELUM * DAN + \n");
    printf("%d. klik 't' untuk matikan light1.\n",t++);
    printf("%d. klik 'y' untuk matikan light2.\n",t++);
    printf("%d. klik 'u' untuk matikan cse light.\n",t++);

    printf("%d. Klik 'w' untuk ke atas.\n%d. klik 'e' untuk ke bawah.\n%d. klik 's' untuk ke kanan.\n%d. klik 'a' untuk ke kiri.\n",t+1,t+2,t+3,t+4);
    t+=4;
    printf("%d. klik 'o' untuk memindah kamera ke kiri.\n%d. klik 'p' untuk memindah kamera ke kanan.\n",t+1,t+2);
    t+=2;
    printf("%d. klik '+' untuk zoom in.\n%d. klik '-' untuk zoom out.\n",t+1,t+2);
    t+=2;
    printf("%d. klik b,n,m for operate with light1,light2 and CSE light respectively.\n",t++);
    printf("   1. klik '1' untuk add ambient.\n");
    printf("   2. klik '2' untuk reduce ambient.\n");
    printf("   3. klik '3' untuk add diffuse.\n");
    printf("   4. klik '4' untuk reduce diffuse.\n");

    printf("   5. klik '5' untuk add specular light.\n");
    printf("   6. klik '6' untuk reduce specular light.\n");
    printf("   7. klik ESC key untuk keluar dari operasi ini dan kembali ke main program\n");

    printf("%d. klik ESC untuk kembali ke main program.\n",t++);

    glutMainLoop();

    return EXIT_SUCCESS;
}


