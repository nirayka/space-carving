# Space Carving

This is an implementation of [A Theory of Shape by Space Carving][1] in C++.
Space Carving is a method in computer vision by which a 3D shape
is reconstructed using multiple 2D images taken at arbitrary but known viewpoints.
This is done by iteratively 'carving' out voxels from a volume that contains the
original shape. Voxels are chosen to be carved depending on their 'photo-consistency'
across the 2D images. 

## Progress Journal
*June 14:* Read paper 3x, took notes.

*June 15:* Wrote extremely detailed pseudocode for algo. Figured out
project structure. Wrote function definitions for algo and helpers.

*June 16:* Implemented algo and helpers.

*June 18:* Implemented image and metadata parsing. Implemented basic UI. Discovered bug by which cameras were being
parsed correctly but were not read correctly when running multiSweep(). Solved by dynamically allocating camera memory.

*June 19:* Discovered bug by which no voxels were being considered to be in the view plane
of any camera, even when they were in the view plane. Solved by re-doing the math in projectVoxelToImage().

*June 20:* Cleaned up project structure by adding directories but ran into a niche CMake issue which took wayyy too long
to figure out. Solved by adding a line in CMakeLists.txt.

*June 21:* Testing on basic data where the shape to be reconstructed is a cube and the viewpoints are in the six
principle directions. Trying to solve infinite loop in multiSweep(). 

## Algorithm Overview
TODO

## Project Structure
**Folders:**
`src` contains most source files and `data` contains test datasets.


**Significant Files**

`spacecarver.cpp`: Contains the functions for Multi-Sweep Algorithm and the Plane-Sweep Algorithm. Contains a helper to 
determine if a voxel is photo-consistent based on the standard deviation of its color data from
multiple images. 

`camera.cpp`: Represents an input image. Contains an RGBA array representing the image as well
as data about the camera from which it was taken. Importantly, contains a function which projects a voxel into the image
plane and a function which gets the color of the image from that projection. These are used
to determine voxel photo-consistency.

`scene.cpp`: Represents the scene to be carved using a grid of voxels. Importantly, contains a 
function which determines which cameras should be considered in voxel photo-consistency when
Plane Sweeping in a particular direction.

`mainwindow.cpp`: parses input images & metadata

## Usage
This implementation takes in as many images as desired, accompanied by a .txt file containing
metadata. Each line in the metadata represents one image and its viewpoint, and
is formatted as follows:

    imageFileName.jpg positionX,positionY,positionZ lookX,lookY,lookZ upX,upY,upZ

with spaces between the file name and coordinate triplets and commas between individual coordinates.

The following assumptions are made about the input:
* All inputted images follow the Lambertian reflectance model (allows multiple
optimizations to be made, re: Section 5.4 of *Space Carving*)
* The shape to be reconstructed subsumes the set of points (x, y, z) such that
0 <= x, y, z <= 100 (for simplicity)

The outputted 3D model is in the form of a .json file which is compatible with the Project 6
real-time renderer from Prof. Daniel Ritchie's CS1230 course at Brown University.
The output model can also be viewed in the browser by uploading the .json file
[here][2].


## Acknowledgements
Kutulakos, Kiriakos and Seitz, Steven. "A Theory of Shape by Space Carving."
*International Journal of Computer Vision*, vol. 38, no. 3, 2000, pp. 119-218.

Prof. Daniel Ritchie's course slides for CS1230 at Brown University: "Transformations & scene graphs",
"Camera models & viewing transforms", and "Raytracing I".


---
[1]: https://www.cs.toronto.edu/~kyros/pubs/00.ijcv.carve.pdf
[2]: https://scenes.cs1230.graphics/

