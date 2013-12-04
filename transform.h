#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <string>
using namespace std;
#include <GL/glut.h>

#define toRad(a) a*3.14/180

void translate(GLfloat x, GLfloat y, GLfloat z);
void scale(GLfloat x, GLfloat y, GLfloat z);
void rotate(GLfloat angle, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
void matrixMult(GLfloat *,GLfloat *);
void setCurrent(GLfloat *);

void setCurrent_we(GLfloat *);
void matrixMult_we(GLfloat *,GLfloat *);
void worldToEye(GLfloat ex, GLfloat ey, GLfloat ez,GLfloat cx, GLfloat cy, GLfloat cz,GLfloat vix, GLfloat viy, GLfloat viz);


void translate(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat trans[] = {1,0,0,0,0,1,0,0,0,0,1,0,x,y,z,1};
	setCurrent(trans);
}

void scale(GLfloat x, GLfloat y, GLfloat z) {
	GLfloat scale_mat[] = {x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1};
	setCurrent(scale_mat);
}

void rotate_axis(GLfloat angle, GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2){
	angle = toRad(angle);
	GLfloat a = x2-x1;
	GLfloat b = y2-y1;
	GLfloat c = z2-z1;

	GLfloat V = sqrt(a*a+b*b+c*c);
	a /= V;
	b /= V;
	c /= V;
	GLfloat d = sqrt(b*b+c*c);
	GLfloat o[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,o);
	//for(int i=0;i<16;i++)
		//cout << o[i] << "  ";
	//cout << endl;
	//cout << a << b<<c<<d<<endl;
	//translate(-x1,-y1,-z1);
	GLfloat t[] = {1,0,0,0,0,1,0,0,0,0,1,0,-x1,-y1,-z1,1};
	matrixMult(t,o);
	//for(int i=0;i<16;i++)
		//cout << o[i] << "  ";
	//cout << endl;
	if(d != 0){
		GLfloat Rx[] = {1,0,0,0,0,c/d,b/d,0,0,-b/d,c/d,0,0,0,0,1};
		matrixMult(Rx,o);
	}
	GLfloat Ry[] = {d,0,a,0,0,1,0,0,-a,0,d,0,0,0,0,1};
	matrixMult(Ry,o);
	GLfloat Rz[] = {cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1};
	matrixMult(Rz,o);
	GLfloat Ryi[] = {d,0,-a,0,0,1,0,0,a,0,d,0,0,0,0,1};
	matrixMult(Ryi,o);
	if(d != 0){
		GLfloat Rxi[] = {1,0,0,0,0,c/d,-b/d,0,0,b/d,c/d,0,0,0,0,1};
		matrixMult(Rxi,o);
	}
	GLfloat ti[] = {1,0,0,0,0,1,0,0,0,0,1,0,x1,y1,z1,1};
	matrixMult(ti,o);
	/*for(int i=0;i<16;i++)
		cout << o[i] << "  ";
	cout << endl;*/
	glLoadMatrixf(o);
}

void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z){
	angle = toRad(angle);
	if(x){
		GLfloat rot[] = {1,0,0,0,0,cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1};
		setCurrent(rot);
	}
	if(y){
		GLfloat rot[] = {cos(angle),0,-sin(angle),0,0,1,0,0,sin(angle),0,cos(angle),0,0,0,0,1};
		setCurrent(rot);
	}
	if(z){
		GLfloat rot[] = {cos(angle),sin(angle),0,0,-sin(angle),cos(angle),0,0,0,0,1,0,0,0,0,1};
		setCurrent(rot);
	}
}

void shear(GLfloat xy, GLfloat xz, GLfloat yx, GLfloat yz, GLfloat zx, GLfloat zy) {
	GLfloat shear_mat[] = {1,yx,zx,0,xy,1,zy,0,xz,yz,1,0,0,0,0,1};
	setCurrent(shear_mat);
}

void matrixMult(GLfloat *A,GLfloat *B){
	GLfloat temp;
	GLfloat res[4][4] = {0};
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			temp = 0;
			for(int k=0;k<4;k++)
				temp += A[k*4+i]*B[j*4+k];
			res[i][j] = temp;
		}
	}
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			B[4*j+i] = res[i][j];
			//cout << res[i][j] << "  ";
		}
	//cout << "\n\n";
}

void setCurrent(GLfloat *t){
	GLfloat o[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,o);
	matrixMult(t,o);
	glLoadMatrixf(o);
}

void matrixMult_we(GLfloat *A,GLfloat *B){
	GLfloat temp;
	GLfloat result[4][4] = {0};
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			temp = 0;
			for(int k=0;k<4;k++)
				temp += A[k*4+i]*B[j*4+k];
			result[i][j] = temp;
		}
	}
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++){
			B[4*j+i] = result[i][j];
		}

}

void setCurrent_we(GLfloat *t){
	GLfloat o[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,o);
	matrixMult_we(o,t);
	glLoadMatrixf(t);
}

void worldToEye(GLfloat ex, GLfloat ey, GLfloat ez,GLfloat cx, GLfloat cy, GLfloat cz,GLfloat vux, GLfloat vuy, GLfloat vuz) {
	GLfloat temp_x = ex-cx;
	GLfloat temp_y = ey-cy;
	GLfloat temp_z = ez-cz;
	GLfloat distance_eye_coi = sqrt((temp_x)*(temp_x) + (temp_y)*(temp_y) + (temp_z)*(temp_z) );
	GLfloat nx = (temp_x)/distance_eye_coi;
	GLfloat ny = (temp_y)/distance_eye_coi;
	GLfloat nz = (temp_z)/distance_eye_coi;
	temp_x = vuy*nz - vuz*ny;
	temp_y = vuz*nx - vux*nz;
	temp_z = vux*ny - vuy*nx;
	GLfloat distance_v_up_n = sqrt((temp_x)*(temp_x)+(temp_y)*(temp_y)+(temp_z)*(temp_z));
	GLfloat ux = (temp_x)/distance_v_up_n;
	GLfloat uy = (temp_y)/distance_v_up_n;
	GLfloat uz = (temp_z)/distance_v_up_n;
	GLfloat vx = ny*uz - nz*uy;
	GLfloat vy = nz*ux - nx*uz;
	GLfloat vz = nx*uy - ny*ux;
	GLfloat t1[] = {1,0,0,0,0,1,0,0,0,0,1,0,-ex,-ey,-ez,1};
	setCurrent_we(t1);
	GLfloat t[] = {ux,vx,nx,0,uy,vy,ny,0,uz,vz,nz,0,0,0,0,1};
	setCurrent_we(t);
}

void perspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
	GLfloat f = tan(toRad(fovy/2.0));
	f = 1.0/f;
	GLfloat t[] = {f/aspect,0,0,0, 0,f,0,0, 0,0,((zFar+zNear)/(zNear-zFar)),-1, 0,0,((2*zFar*zNear)/(zNear-zFar)),0};
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(t);
}

void windowToView(float vx,float vy,float width,float height) {
    float sx,sy,tx1,ty1;
	float vxl,vxr,vyt,vyb;
	float wxl,wxr,wyt,wyb;
	vxl = vx;
	vxr = vxl + width;
	vyb = vy;
	vyt = vyb + height;

	float w_width, w_height;
	w_width = glutGet(GLUT_WINDOW_WIDTH);
	w_height = glutGet(GLUT_WINDOW_HEIGHT);
	wxl = glutGet(GLUT_WINDOW_X);
	wxr = wxl + w_width;
	wyt = glutGet(GLUT_WINDOW_Y);
	wyb = wyt - w_height;
    sx = (vxr-vxl)/(wxr-wxl);
    sy = (vyt-vyb)/(wyt-wyb);
    tx1 = vxl - (sx*wxl);
    ty1 = vyb - (sy*wyb);
	GLfloat t[] = {sx,0,0,0, 0,sy,0,0, tx1,ty1,1,0, 0,0,0,1};
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(t);
}
