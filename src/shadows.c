#include "shadow.h"

void Shadow_redraw(GLfloat lightPosition[4], void (*drawModel)()) {
  const bool RENDER_SHADOW = true;
  const bool STENCIL_SHADOW = true;
  const bool OFFSET_SHADOW = true;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  Shadow_shadowMatrix(Shadow_floorShadow, Shadow_floorPlane, lightPosition);
  glPushMatrix();
  // Your moouse control goes here at line 372

  /* Tell GL new light source position. */
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

  /* Back face culling will get used to only draw either the top or the
   bottom floor.  This let's us get a floor with two distinct
   appearances.  The top floor surface is reflective and kind of red.
   The bottom floor surface is not reflective and blue. */

  /* Draw "bottom" of floor in blue. */
  glFrontFace(GL_CW); /* Switch face orientation. */
  glColor4f(0.1, 0.1, 0.7, 1.0);
  Shadow_drawFloor();
  glFrontFace(GL_CCW);
  if (RENDER_SHADOW) {
    /* Draw the floor with stencil value 3.  This helps us only
       draw the shadow once per floor pixel (and only on the
       floor pixels). */
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 3, 0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
  }
  /* Draw "top" of floor.  Use blending to blend in reflection. */
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.7, 0.0, 0.0, 0.3);
  glColor4f(1.0, 1.0, 1.0, 0.3);
  Shadow_drawFloor();
  glDisable(GL_BLEND);

  if (drawModel != null) drawModel();

  // Draw the shadow.
  if (RENDER_SHADOW) {
    /* Render the projected shadow. */
    if (STENCIL_SHADOW) {
      /* Now, only render where stencil is set above 2 (ie, 3 where
         the top floor is).  Update stencil with 2 where the shadow
         gets drawn so we don't redraw (and accidently reblend) the
         shadow). */
      glStencilFunc(GL_LESS, 2, 0xffffffff); /* draw if ==1 */
      glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
      glEnable(GL_POLYGON_OFFSET_FILL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable(GL_LIGHTING); /* Force the 50% black. */
      glColor4f(0.0, 0.0, 0.0, 0.5);

      glPushMatrix();
      /* Project the shadow. */
      glMultMatrixf((GLfloat *)Shadow_floorShadow);
      if (drawModel != null) drawModel();
      glPopMatrix();

      glDisable(GL_BLEND);
      glEnable(GL_LIGHTING);

      // Make the shadow smooth
      glDisable(GL_POLYGON_OFFSET_FILL);
      glDisable(GL_STENCIL_TEST);
    }
  }

  glPopMatrix();
  glutSwapBuffers();
}

/* Draw a floor (possibly textured). */
void Shadow_drawFloor() {
  const bool USER_TEXTURE = false;
  glDisable(GL_LIGHTING);

  if (true) glEnable(GL_TEXTURE_2D);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0, 0.0);
  glVertex3fv(Shadow_floorVertices[0]);
  glTexCoord2f(0.0, 16.0);
  glVertex3fv(Shadow_floorVertices[1]);
  glTexCoord2f(16.0, 16.0);
  glVertex3fv(Shadow_floorVertices[2]);
  glTexCoord2f(16.0, 0.0);
  glVertex3fv(Shadow_floorVertices[3]);
  glEnd();

  if (true) glDisable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

/* Create a matrix that will project the desired shadow. */
void Shadow_shadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4],
                         GLfloat lightpos[4]) {
  GLfloat dot;

  /* Find dot product between light position vector and ground plane normal. */
  dot = groundplane[X] * lightpos[X] + groundplane[Y] * lightpos[Y] +
        groundplane[Z] * lightpos[Z] + groundplane[W] * lightpos[W];

  shadowMat[0][0] = dot - lightpos[X] * groundplane[X];
  shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y];
  shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z];
  shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W];

  shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X];
  shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y];
  shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z];
  shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W];

  shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X];
  shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y];
  shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z];
  shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W];

  shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X];
  shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y];
  shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z];
  shadowMat[3][3] = dot - lightpos[W] * groundplane[W];
}

/* Find the plane equation given 3 points. */
void Shadow_findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3],
                      GLfloat v2[3]) {
  GLfloat vec0[3], vec1[3];

  /* Need 2 vectors to find cross product. */
  vec0[X] = v1[X] - v0[X];
  vec0[Y] = v1[Y] - v0[Y];
  vec0[Z] = v1[Z] - v0[Z];

  vec1[X] = v2[X] - v0[X];
  vec1[Y] = v2[Y] - v0[Y];
  vec1[Z] = v2[Z] - v0[Z];

  /* find cross product to get A, B, and C of plane equation */
  plane[A] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
  plane[B] = -(vec0[X] * vec1[Z] - vec0[Z] * vec1[X]);
  plane[C] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];

  plane[D] = -(plane[A] * v0[X] + plane[B] * v0[Y] + plane[C] * v0[Z]);
}