#include <cmath>
#include <iostream>
#include "turtle.hpp"
#include "salmon.hpp"
#include "utils.hpp"

extern int current, lightNo;

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Turtle::Turtle():
radius_(40), isAlive_(true), collided_(false) {
    
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    double window_w = viewport[2];
    double window_h = viewport[3];
    
    
    y_ = randf() * window_h;
    x_ = window_w;
    setColour();
}

Turtle::Turtle(int xCoordinate, int yCoordinate):
x_(xCoordinate), y_(yCoordinate), radius_(40),
isAlive_(true), collided_(false) {

    setColour();
}

void Turtle::setColour() {
    
    shell_[0] = 0.01;
    shell_[1] = 0.62;
    shell_[2] = 0.08;
    shell_[3] = 1.0;
}

bool Turtle::isAlive() const {
    return isAlive_;
}

void Turtle::move(double x, double y) {
    
    if (!isAlive_ || collided_) {
        return;
    }
    x_ += x;
    y_ += y;
}

void Turtle::update(Salmon &salmon) {

    if (!isAlive_ || collided_) {
        return;
    }
    
    // Move the turtle with the current.
    move(current, 0);
    
    // Get window width.
    double viewport[4];
    glGetDoublev(GL_VIEWPORT, viewport);
    double window_w = viewport[2];
    double window_h = viewport[3];
    
    // Passof the screen (the left wall) and die.
    if (x_ < 0) {
        isAlive_ = false;
    }
    
	//
    // Impact with salmon (naive).
	//
    int salmon_x_ = salmon.getXcoordinate();
    int salmon_y_ = salmon.getYcoordinate();
    
    double distance = sqrt( pow((x_ - salmon_x_), 2) + pow((y_ - salmon_y_), 2) );
    if (distance <= 2.0 * radius_) {
        collided_ = true;
        setupLight();
        salmon.setCollided(true);
        salmon.setRandomColour();
    }
}

void Turtle::setupLight() {



    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0 + lightNo);
    glEnable(GL_COLOR_MATERIAL);

    // Set the intensity of the global ambient light.
    float ambient[] = {0.3, 0.3, 0.3, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Set up the diffuse intensities of the local light source.
    float diffuse[4] = {
        0.8, 0.8, 0.8, 1
    };

    glLightfv(GL_LIGHT0 + lightNo, GL_DIFFUSE, diffuse);

    // position the light
    float light_positions[4] = {
        x_,  y_, 0, 0 // at the position of the turtle when it collided
    };

     glLightfv(GL_LIGHT0 + lightNo, GL_POSITION, light_positions);

     lightNo++;
}

void Turtle::draw() {
    

    // setup lights
    if(collided_)
        setupLight();

    // Setup transformation matrix for the turtle
    
    glPushMatrix();
    glTranslatef(x_, y_, 0);
    glNormal3f(0, 0, 1);
    
    glNormal3f(0, 0, 1);
    
    // Turtle shell.
    glColor4fv(shell_);
    glBegin(GL_POLYGON);
    for (double i = 0; i < M_PI; i += M_PI / 12)
        glVertex3f(cos(i) * radius_, sin(i) * radius_, 0.0);
    glEnd();
    
    // Turtle body, tail, legs, and head.
    float skin_[] = { 0.73, 0.59, 0.27, 1.0};
    glColor4fv(skin_);

    glBegin(GL_TRIANGLES);
    
    glVertex3f(-radius_ - 5, 10, 0);
    glVertex3f(-radius_ - 5, 20, 0);
    glVertex3f((-radius_ - 40), 15, 0);

    glVertex3f(-radius_ - 5, 10, 0);
    glVertex3f(-radius_ - 5, 0, 0);
    glVertex3f((-radius_ - 40), 5, 0);

    glVertex3f(-radius_ - 5, 10, 0);
    glVertex3f(-radius_ - 5, 0, 0);
    glVertex3f((-radius_ - 40), 5, 0);
    
    glEnd();

    glPushMatrix();
    glScalef(1, 0.15, 0.0);
    glBegin(GL_POLYGON);
    for (double i = 4.0 * M_PI / 6.0; i < 2 * M_PI; i += M_PI / 12)
        glVertex3f(cos(i) * radius_, sin(i) * radius_, 0.0);
    glEnd();
    glPopMatrix();

    glBegin(GL_TRIANGLES);
    
    glVertex3f(-radius_ - 5, 11, 0);
    glVertex3f(radius_ + 5, 0, 0);
    glVertex3f(-radius_ - 5, 0, 0);
    
    glVertex3f(-radius_ + 5, 0, 0);
    glVertex3f(-radius_ + 35, 0, 0);
    glVertex3f(-radius_ + 30, -25, 0);
    
    glVertex3f(20, 0, 0);
    glVertex3f(40, 0, 0);
    glVertex3f(40, -25, 0);
    
    glVertex3f(38, 10, 0);
    glVertex3f(40, 0, 0);
    glVertex3f(50, 15, 0);
    
    glEnd();
    
    glPushMatrix();
    glTranslatef(-radius_ - 10, 10, 0);
    glScalef(1, 0.5, 0.0);
    glBegin(GL_POLYGON);
    for (double i = -M_PI / 2.0; i < M_PI / 2.0; i += M_PI / 12)
        glVertex3f(cos(i) * 20, sin(i) * 20, 0.0);
    glEnd();
    glPopMatrix();
    
    // Turtle lowlights.
    glColor4fv(shell_);
    
    glBegin(GL_TRIANGLES);
    
    glVertex3f(-radius_ + 25, 5, 0);
    glVertex3f(-radius_ + 34, -7, 0);
    glVertex3f(-radius_ + 30, -25, 0);
    
    glVertex3f(15, -2, 0);
    glVertex3f(40, -10, 0);
    glVertex3f(40, -25, 0);
    
    glEnd();
    
    float lowlights_[] = { 0.17, 0.49, 0.27, 1.0 };
    glColor4fv(lowlights_);
    
    glBegin(GL_TRIANGLES);
    
    glVertex3f(-30, 10, 0);
    glVertex3f(-30, 5, 0);
    glVertex3f(40, 0, 0);
    
    glEnd();
    
    // Eyes and frowning eyebrows.
    glPushMatrix();
    glTranslatef(-radius_ - 10, 20, 0);
    const float white[] = {1.0, 1.0, 1.0, 1.0};
    glColor4fv(white);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 6, sin(i) * 6, 0.0);
    glEnd();
    const float black[] = {0.0, 0.0, 0.0, 1.0};
    glColor4fv(black);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 2, sin(i) * 2, 0.0);
    glEnd();
    glTranslatef(10, 0, 0);
    glColor4fv(white);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 6, sin(i) * 6, 0.0);
    glEnd();
    glColor4fv(black);
    glNormal3f(0, 0, 1);
    glBegin(GL_POLYGON);
    for (double i = 0.0; i < 2 * M_PI; i += M_PI / 6)
        glVertex3f(cos(i) * 2, sin(i) * 2, 0.0);
    glEnd();
    glPopMatrix();
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-radius_ - 5, 23, 0);
    glVertex3f(-radius_ - 15, 30, 0);
    glVertex3f(-radius_ - 15, 25, 0);
    
    glVertex3f(-radius_ - 5, 23, 0);
    glVertex3f(-radius_ + 5, 30, 0);
    glVertex3f(-radius_ + 5, 25, 0);
    glEnd();

    // Restore the transformation matrix.
    glPopMatrix();
}


