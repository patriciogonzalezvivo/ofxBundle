## What is Bundler?

[Bundler](http://www.cs.cornell.edu/~snavely/bundler/) is a structure-from-motion (SfM) system for unordered image collections (for instance, images from the Internet) written in C and C++. An earlier version of this SfM system was used in the [Photo Tourism](http://phototour.cs.washington.edu/) project. For structure-from-motion datasets, please see the [BigSFM](http://www.cs.cornell.edu/projects/bigsfm/) page.
Bundler takes a set of images, image features, and image matches as input, and produces a 3D reconstruction of camera and (sparse) scene geometry as output. The system reconstructs the scene incrementally, a few images at a time, using a modified version of the [Sparse Bundle Adjustment](http://users.ics.forth.gr/~lourakis/sba/) package of Lourakis and Argyros as the underlying optimization engine. Bundler has been successfully run on many Internet photo collections, as well as more structured collections.
The Bundler source distribution also contains potentially userful implementations of several computer vision algorithms, including: 
	•	F-matrix estimation
	•	Calibrated 5-point relative pose
	•	Triangulation of multiple rays

Bundler produces sparse point clouds. For denser points, Dr. Yasutaka Furukawa has written a beautiful software package called [PMVS2](http://www.di.ens.fr/pmvs/) for running dense multi-view stereo. A typical pipeline is to run Bundler to get camera parameters, use the provided Bundle2PMVS program to convert the results into PMVS2 input, then run PMVS2. You might also be interested in Dr. Furukawa's [CMVS](http://www.di.ens.fr/cmvs/) view clustering software, which is a helpful preprocess to running PMVS2.


## Downloading Bundler

The Bundler source code is hosted at the [bundler_sfm repository on GitHub](https://github.com/snavely/bundler_sfm). 

## Dependences

- [ofxGlmTools](https://github.com/patriciogonzalezvivo/ofxGlmTools)