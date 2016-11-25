# fracture

*fracture* is an interactive audiovisual installation prompted by the accelerating rate of climate change and the contamination of water sources via fracking. This installation models virtual environments that the audience, through sensors, are able to manipulate, highlighting the agency of an individual to preserve the environment or become complicit in its destruction.

### Implementation

*fracture* uses openFrameworks and GLSL for real-time parallelized graphics processing and SuperCollider for real-time audio. The SC audio application sends OSC data to the C++ graphics application to sync events between the two mediums. OpenMP is also used to multithread the loading of media files. The application will eventually take sensor data from an Arduino (or a more powerful MCU) to form interactivity between the audience and the installation.

### Dependencies

* openFrameworks 0.9.3
* SuperCollider 3.8.0
* OpenMP 4.5
