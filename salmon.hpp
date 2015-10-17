#ifndef SALMON_H 
#define SALMON_H

class Turtle; // Forward declaration

class Salmon {

private: 
    // Location of the centre of the salmon.
    double x_, y_;

    // Rotation of the salmon in degrees (around the Z-axis).
    double rotation_;
    
    // Tracks if the salmon has collided with an object.
    bool collided_;

    // The surface colour of the salmon.
    float colour_[4];
   
    // Size of the salmon; generally constant.
    double length_, width_;
    
    // Tracks if the salmon is still alive; will be removed when not alive.
    bool isAlive_;
    
public:

    // Constructor: initializes default values.
    Salmon();   
    
    // Reset the salmon to its initial state.
    void reset();
    
    // Sets the salmon's colour.
    void setColour();
 
    // Return if the salmon is still alive.
    bool isAlive() const;

    // Return if the salmon has collided.
    bool hasCollided() const;

    // Move the salmon by the given amount in each coordinate direction.
    void move(double x, double y);
    
    // Set collision state.
    void setCollided(bool collided);

    // Rotate the salmon by the given angle (in degrees).
    void rotate(double angle);

    // Change the colour of the salmon body to a random colour.
    void setRandomColour();
    
    // Return the salmon coordinates.
    int getXcoordinate() const;
    int getYcoordinate() const;
    
    // Perform physics updates.
    void update();
    
    // Draw the salmon.
    void draw();
};


#endif
