#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iostream>
#include <list>
#include "a1.hpp"
#include "salmon.hpp"
#include "turtle.hpp"
#include "utils.hpp"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
//sfads
// Positioning state.
int window_w, window_h;
//asdfa
// The salmon, turtles, and river current speed.
Salmon salmon;
std::list<Turtle *> turtles;
int current =  -5;
int lightNo = 2;

// state variables to keep track of arrow key presses
int specialKey;
bool specialKeyState;


// Put any other global state variables you need here.
// e.g., keyboard state, mouse state....

// state variables to keep track of rotation based on mouse movement

int displacement;
int prev_mouse_pos;

void setup_lighting(void) {
    
    // Turn on lighting, and two local lights.
	
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    
    // Set the intensity of the global ambient light.
    float ambient[] = {0.3, 0.3, 0.3, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    
    // Set up the diffuse intensities of the local light source.
    float diffuse[][4] = {
        0.8, 0.8, 0.8, 1,
        0.2, 0.2, 0.2, 1,
    };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse[0]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse[1]);
    
    // Move the light near the top corner of the window.
    float light_positions[][4] = {
        0,  1, 2, 0, // From above-left
           0, -5, 0, 0, // From below
    };
    glLightfv(GL_LIGHT0, GL_POSITION, light_positions[0]);
    glLightfv(GL_LIGHT1, GL_POSITION, light_positions[1]);
}

// Called by GLUT at the start of the session, and when the window is reshaped.
void reshape(int w, int h) {
    
    window_w = w;
    window_h = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Z range must be big enough to encompass salmon and turtles.
    glOrtho(0, w, 0, h, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    setup_lighting();

    salmon.reset();
}

// Called by GLUT when you move the mouse.
void mouse_move(int x, int y) {
    
    // @@@@@@@@@@
    // Use mouse movements to rotate the salmon.
    // Compare the current and previous mouse coordinates and use
    // the displacement to set the amount of rotation.
    displacement = x - prev_mouse_pos;
    prev_mouse_pos = x;

    // @@@@@@@@@@
}


// Called by GLUT when you press a key on the keyboard.
void keyboard(unsigned char key, int x, int y) {

    // Reset the salmon by pressing 'r'.
    // Increase and decrease the current with the '>' and '<' keys.
    switch(key) {
        case 'r':
            // @@@@@@@@@@
            // Do something or call a function here
            // @@@@@@@@@@
            break;
        case '>':
            changeCurrent(true);
            break;
        case '<':
            changeCurrent(false);
            break;

		// @@@@@@@@@@
		// Toggle between advanced mode and basic mode here by detecting
		// non-arrow key presses here.  You can detect any other key press
        // events here, and use them however you like.


        // @@@@@@@@@@
    }
}


// Change the current.
void changeCurrent(bool increase) {

    // @@@@@@@@@@
}


// Called by GLUT when you release a key on the keyboard.
void keyboardup(unsigned char key, int x, int y) {
    
    // @@@@@@@@@@
    // Detect non-arrow key releases here.

    // @@@@@@@@@@
}

// Called by GLUT when you press a special key on the keyboard.
void keyboardSpecial(int key, int x, int y) {
    
    // @@@@@@@@@@
    // Detect arrow key presses here.
 	specialKey= key;
	specialKeyState = true; 
    // @@@@@@@@@@
}

// Called by GLUT when you release a special key on the keyboard.
void keyboardSpecialUp(int key, int x, int y) {

    specialKey = key;
    specialKeyState = false;
    // @@@@@@@@@@
}

// Called by GLUT when we need to redraw the screen.
void display(void) {

    
    // Clear the buffer we will draw into.
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Initialize the modelview matrix.
    glLoadIdentity();

    // Disable all remaining lights.
    for (int i = lightNo; i < GL_MAX_LIGHTS; ++i) {
        glDisable(GL_LIGHT0 + i);
    }

    // Draw a blue river background grid.
    glNormal3f(0, 0, 1);
    const int res = 40;
    for (int x = 0; x <= window_w; x += res) {
        glBegin(GL_QUAD_STRIP);
        for (int y = 0; y <= window_h; y+= res) {
            double gradient = (double)y/(double)window_h;
            glColor3f(gradient * 0.74, 0.69 + gradient * 0.25, 0.9 + gradient * 0.08);
            glVertex3f(x, y, -20);
            glVertex3f(x+res, y, -20);
        }
        glEnd();
    }
    
    // Draw the salmon.
    salmon.draw();

    // Draw the turtles.
    std::list<Turtle *>::iterator turtle;
    for (turtle = turtles.begin(); turtle != turtles.end(); turtle++) {
        (*turtle)->draw();
    }
    
    // Make the buffer we just drew into visible.
    glutSwapBuffers();
}


// Called by GLUT on a timer for every frame of animation.
// We are responsible for setting the argument that is passed to this function.
void animate(int last_frame = 0) {
    
    // @@@@@@@@@@
    // Control the movement of the salmon here based on arrow key states.
    if(specialKeyState){
        switch(specialKey){
        case GLUT_KEY_DOWN:
            salmon.move(0,-7);
            break;
        case GLUT_KEY_UP:
            salmon.move(0,7);
            break;
        case GLUT_KEY_RIGHT:
            salmon.move(7,0);
            break;
        case GLUT_KEY_LEFT:
            salmon.move(-7,0);
            break;
        default:
            //cout << "some other special key was pressed";
        ;
        }

    }

    // make salmon rotate if mouse moved horizaontally
    if(displacement!= 0){
        salmon.rotate(displacement*2);
        displacement = 0;
    }

    // Find collisions between turtles and the salmon (naive).
    std::list<Turtle *>::iterator turtle;
    for (turtle = turtles.begin(); turtle != turtles.end(); turtle++) {
        (*turtle)->update(salmon);
    }
    
    // Randomly generate some new turtles upstream.
    if (randf() < 0.01) {
        turtles.push_back(new Turtle(window_w, randf() * window_h));
    }
    
    // Delete turtles who have swum downstream out of the window.
    turtle = turtles.begin();
    while (turtle != turtles.end()) { 
        if (!(*turtle)->isAlive()) {
            delete *turtle;
            turtle = turtles.erase(turtle);
        } else {
            turtle++;
        }
    }
    
    // Update the salmon's properties.
    salmon.update();

    // Signal that the window should be redrawn.
    glutPostRedisplay();
    
    // Schedule the next frame.
    int current_time = glutGet(GLUT_ELAPSED_TIME);
    int next_frame = last_frame + 1000 / 30;
    glutTimerFunc(MAX(0, next_frame - current_time), animate, current_time);
}


int main(int argc, char *argv[]) {
    
    // Initialize random seed.
    srand(time(NULL));
    
    // Initialize GLUT and open a window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(900, 600);
    glutCreateWindow(argv[0]);
    
    // Register callbacks for GLUT events.
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMotionFunc(mouse_move);
    glutPassiveMotionFunc(mouse_move);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
        

    // @@@@@@@@@@
    // Register your callbacks here.
    glutSpecialFunc(keyboardSpecial);
    glutSpecialUpFunc(keyboardSpecialUp);
	

    // @@@@@@@@@@
    
    // Schedule the first animation callback ASAP.
    glutTimerFunc(0, animate, 0);

    // Pass control to GLUT.
    glutMainLoop();
    
    return 0;
}

