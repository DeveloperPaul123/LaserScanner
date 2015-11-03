# LaserScanner
Laser scanner control software.

Steps to build this project:
- Download OpenCV 3.0 source
- Build with cmake WITHOUT CUDA
- In the generated solution (from CMake) run build ALL_BUILD in debug and release, then also build INSTALL in debug and release.


![Image](screenshots/OpenCVBuildBuildOptions.PNG?raw=true)
![Image](screenshots/OpenCVBuildNoCude.PNG?raw=true)

Your settings for this (LaserScanner) project for Opencv should look something like this: 
![Image](screenshots/LaserScannerOpenCVSettings.PNG?raw=true)
