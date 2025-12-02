# Cinematic Ray Tracer - Showcase Piece

## Personal Information

- **Full Name**: Alex Yuan
- **UtorID**: yuanale4
- **Student Number**: 1007896305
- **Assignment Augmented**: Computer Graphics - Ray Tracing

## Instructions

To compile and run the code to reproduce the piece:

1.  **Build the project**:

    ```bash
    mkdir build-release
    cd build-release
    cmake ../ -DCMAKE_BUILD_TYPE=Release
    make
    ```

2.  **Run the executable**:

    ```bash
    ./raytracing ../data/sphere-packing.json
    ```

    _Note: The rendering process includes 50 samples per pixel for high quality, so it may take a minute or two. A progress bar will indicate the status._

3.  **Output**:
    The rendered image will be saved as `piece.ppm` in the `build` directory. You can also find `piece.png` in the root of this folder that shows the output.

## Description

For this showcase, I augmented the standard Ray Tracer to produce a "Cinematic" rendering effect. The goal was to move away from the perfect, sterile look of a pinhole camera and simulate a real-world camera lens.

**Features Added:**

1.  **Depth of Field (Defocus Blur)**

    - _Implementation_: In `main.cpp`, I modified the ray generation logic. Instead of all rays originating from a single point (pinhole), I simulate a lens aperture. Rays are jittered on a disk around the camera origin (`camera.e`) and directed towards a focal point on the focal plane.
    - _Effect_: Objects at the focal distance remain sharp, while objects closer or further away become realistically blurry.

2.  **Anti-Aliasing (Super-Sampling)**

    - _Implementation_: In `main.cpp`, I implemented a loop to cast multiple rays (50 samples) per pixel. Each ray's screen coordinate is slightly jittered by a random amount. The final pixel color is the average of these samples.
    - _Effect_: This eliminates jagged edges ("jaggies") on geometry and shadows, producing a much smoother, high-quality image.

3.  **Auto-Focus System**
    - _Implementation_: In `main.cpp`, before the main render loop, I cast a single probe ray through the center of the screen to determine the distance to the nearest object.
    - _Effect_: This automatically sets the focal distance (`t_focus`) so the main subject of the scene is always in sharp focus without manual tuning.

## Acknowledgements

- **Base Code**: CSC317 Course Material (Ray Tracing Assignment).
- **External Libraries**:
  - `Eigen`: For linear algebra operations.
  - `nlohmann/json`: For parsing scene files.
- **Scene Data**: `sphere-packing.json` from the course data folder.
