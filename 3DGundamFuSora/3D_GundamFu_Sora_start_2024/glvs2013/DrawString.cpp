//#include "glew.h"
#include "glut.h"
#include "string.h"

void DrawString(char* str, int w, int h, int x0, int y0)
{
    glDisable(GL_LIGHTING);
    // ���s���e�ɂ���
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	    glLoadIdentity();
	    gluOrtho2D(0, w, h, 0);
	    glMatrixMode(GL_MODELVIEW);
	    glPushMatrix();
		    glLoadIdentity();
 
		    // ��ʏ�Ƀe�L�X�g�`��
			glRasterPos2f(x0, y0);
//		    int size = (int)str.size();
			int size = (int)strlen(str);
			for(int i = 0; i < size; ++i){
				char ic = str[i];
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
			}
	    glPopMatrix();
		glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHTING);
}
