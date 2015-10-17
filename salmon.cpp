#include <cmath>
#include <iostream>
#include "salmon.hpp"
#include "utils.hpp"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Salmon::Salmon():
x_(100.0), y_(100.0), rotation_(0.0),
collided_(false), length_(70.0), width_(20.0), isAlive_(true) {
    
    setColour();

}

void Salmon::reset() {
    
    isAlive_ = true;
    collided_ = false;
    rotation_ = 0.0;
    setColour();

    // Position salmon slightly left of centre of viewport.
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    double window_w = viewport[2];
    double window_h = viewport[3];

    x_ = window_w / 3;
    y_ = window_h / 2;

}

void Salmon::setColour() {
    
    // Salmon colour defaults to 'salmon-ish.'
    colour_[0] = 0.98;
    colour_[1] = 0.20;
    colour_[2] = 0.04;
    colour_[3] = 1.0;
}

bool Salmon::isAlive() const {
    return isAlive_;
}

bool Salmon::hasCollided() const {
    return collided_;
}

void Salmon::move(double x, double y) {
    
    if (!isAlive_ || collided_) {
        return;
    }
    x_ += x;
    y_ += y;
}

int Salmon::getXcoordinate() const {
    return x_;
}

int Salmon::getYcoordinate() const {
    return y_;
}



void Salmon::setCollided(bool collided)
{
    collided_ = collided;
    // @@@@@@@@@@
    //
    // @@@@@@@@@@
}

void Salmon::rotate(double angle) {
    
    if (isAlive_ && !collided_) {
    	rotation_ += angle;
    }
}

void Salmon::setRandomColour() {
    
    // @@@@@@@@@@
    // Give the salmon a random colour here.
    colour_[0] = randf();
    colour_[1] = randf();
    colour_[2] = randf();
    colour_[3] = 1.0;

}

void Salmon::update() {

    if (collided_ || !isAlive_) {
        return;
    }
 
    // Get window width.
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    double window_w = viewport[2];
    double window_h = viewport[3];
    
    // Evaluate trig quantities based on salmon's rotation.
    double sinR = sin(rotation_*M_PI/180.0);
    double cosR = cos(rotation_*M_PI/180.0);
    double offset = length_/4.0;
    double vRadius = width_/2.0 + fabs(cosR)*offset;
    double hRadius = width_/2.0 + fabs(sinR)*offset;  
}

void Salmon::draw() {

    if (!isAlive_) {
        return;
    }   
 
    // Setup transformation matrix for the salmon.
    glPushMatrix();

    // @@@@@@@@@@
	// There are several different tasks to complete here:
    // - Use glTranslate to set the position of the salmon.
    // - Use glRotate to set the rotation of the salmon.
	// - Set the colour of the salmon.
    // @@@@@@@@@@
    
    // We are now in a coordinate system with 0,0,0 at the centre of the salmon.
    // DRAW IT!
    glTranslatef(x_, y_, 0);
    
    glRotatef(rotation_, 0, 0, 1);

    // Salmon body (main body colour).
    glNormal3f(0, 0, 1);
    glColor4fv(colour_);
    glBegin(GL_POLYGON); //Naive.  Why are triangles better?
    glVertex3f(53.0, -20.0, 0);
    glVertex3f(53.0, 18.0, 0);
    glVertex3f(25.0, 40.0, 0);
    glVertex3f(-10.0, 40, 0);
    glVertex3f(-30.0, 35, 0);
    glVertex3f(-60.0, 20, 0);
    glVertex3f(-70.0, 15, 0);
    glVertex3f(-80.0, 12, 0);
    glVertex3f(-80.0, -10, 0);
    glVertex3f(-60.0, -15, 0);
    glEnd();
    
    // Salmon tail (sans fins).
    glBegin(GL_TRIANGLES);
    
    glVertex3f(-80.0, 12, 0);
    glVertex3f(-130.0, 30, 0);
    glVertex3f(-80.0, -10, 0);
    
    glVertex3f(-80.0, 12, 0);
    glVertex3f(-130.0, -30, 0);
    glVertex3f(-80.0, -10, 0);
    glEnd();
    
    // Salmon belly, dorsal fins, and contours (lighter).
    float belly_[] = { 0.98, 0.45, 0.04, 1.0 };
    glColor4fv(belly_);
    glPushMatrix();
    glScalef(1, 0.4, 0.0);
    glBegin(GL_POLYGON);
    for (double i = (6.8 / 6 ) * M_PI; i < 1.9 * M_PI; i += M_PI / 6) {
        glVertex3f(cos(i) * (1.2 * length_), sin(i) * length_, 0.0);
    }
    glEnd();
    glPopMatrix();
    
    glBegin(GL_TRIANGLES);
    
    glVertex3f(-40.0, 25, 0);
    glVertex3f(-70.0, 10, 0);
    glVertex3f(-80.0, 10, 0);
    
    glVertex3f(25.0, -5.0, 0);
    glVertex3f(45.0, 20.0, 0);
    glVertex3f(35.0, 15.0, 0);
    
    glVertex3f(-90.0, 0, 0);
    glVertex3f(-130.0, 30, 0);
    glVertex3f(-110.0, 0, 0);
    
    glVertex3f(-90.0, 0, 0);
    glVertex3f(-130.0, -30, 0);
    glVertex3f(-110.0, 0, 0);
    
    glVertex3f(-60.0, 20, 0);
    glVertex3f(-70.0, 15, 0);
    glVertex3f(-75.0, 22, 0);

    glVertex3f(-10.0, 40, 0);
    glVertex3f(-30.0, 35, 0);
    glVertex3f(-30.0, 60, 0);
    
    glVertex3f(-30.0, 35, 0);
    glVertex3f(-30.0, 60, 0);
    glVertex3f(-50.0, 35, 0);
    
    glVertex3f(-30.0, 35, 0);
    glVertex3f(-40.0, 30, 0);
    glVertex3f(-50.0, 35, 0);
    
    glEnd();
    
    // Green lowlights.
    float lowlights_[] = { 0.17, 0.49, 0.27, 1.0 };
    glColor4fv(lowlights_);
    glBegin(GL_TRIANGLES);
    
    glVertex3f(-85.0, 0, 0);
    glVertex3f(-125.0, -28, 0);
    glVertex3f(-90.0, 0, 0);
    
    glEnd();
    
    // Green highlights.
    float highlights_[] = { 0.37, 0.67, 0.35, 1.0 };
    glColor4fv(highlights_);
    glBegin(GL_TRIANGLES);

    glVertex3f(-90.0, 0, 0);
    glVertex3f(-130.0, 30, 0);
    glVertex3f(-110.0, 0, 0);
    
    glVertex3f(-90.0, 0, 0);
    glVertex3f(-130.0, -30, 0);
    glVertex3f(-110.0, 0, 0);
    
    glEnd();
    
    // Salmon fins (same colour as body to contrast with belly).
    glColor4fv(colour_);
    glBegin(GL_TRIANGLES);
    
    glVertex3f(40.0, -15.0, 0);
    glVertex3f(15.0, -20.0, 0);
    glVertex3f(30.0, -40, 0);
    
    glVertex3f(0.0, -25.0, 0);
    glVertex3f(-20.0, -25.0, 0);
    glVertex3f(-25.0, -35, 0);
    
    glVertex3f(-60.0, -20, 0);
    glVertex3f(-70.0, -15, 0);
    glVertex3f(-75.0, -22, 0);
    
    glEnd();
    
    // Face and top mandible (greenish).
    float face_[] = { 0.37, 0.67, 0.35, 1.0 };
    glColor4fv(face_);
    glNormal3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    
    glVertex3f(35.0, -5.0, 0);
    glVertex3f(55.0, 17.0, 0);
    glVertex3f(100.0, 10, 0);
    
    glVertex3f(35.0, -5.0, 0);
    glVertex3f(100.0, 10, 0);
    glVertex3f(60.0, -17, 0);

    glVertex3f(100.0, 10, 0);
    glVertex3f(93.0, -5, 0);
    glVertex3f(86.0, 10, 0);
    
    glEnd();
    
    // Lower mandible (darker green).
    float jaw_[] = { 0.17, 0.49, 0.27, 1.0 };
    glColor4fv(jaw_);
    glBegin(GL_TRIANGLES);
    
    glVertex3f(37.0, -7.0, 0);
    glVertex3f(37.0, -25.0, 0);
    glVertex3f(75.0, -22, 0);
    
    glVertex3f(55.0, -20.0, 0);
    glVertex3f(100.0, -10.0, 0);
    glVertex3f(75.0, -22, 0);
    
    glEnd();

    // Eye
    glPushMatrix();
    glTranslatef(50, 0, 0);
    const float white[] = {1.0, 1.0, 1.0, 1.0};
    glColor4fv(white);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 4, sin(i) * 4, 0.0);
    glEnd();
    glTranslatef(1, 0, 0);
    const float black[] = {0.0, 0.0, 0.0, 1.0};
    glColor4fv(black);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 2, sin(i) * 2, 0.0);
    glEnd();
    glPopMatrix();

    // Restore the transformation matrix.
    glPopMatrix();
}


