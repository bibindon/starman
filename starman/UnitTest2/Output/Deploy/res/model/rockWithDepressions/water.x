xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Plane {
    FrameTransformMatrix {
       1.000000, 0.000000, 0.000000, 0.000000,
       0.000000, 1.000000, 0.000000, 0.000000,
       0.000000, 0.000000, 1.000000, 0.000000,
       0.000000,-0.523333, 0.408652, 1.000000;;
    }
    Mesh { // Plane mesh
      4;
      -0.562846;-0.564140; 0.000000;,
       0.562846;-0.564140; 0.000000;,
      -0.562846; 0.564140; 0.000000;,
       0.562846; 0.564140; 0.000000;;
      1;
      4;2,3,1,0;;
      MeshNormals { // Plane normals
        1;
         0.000000; 0.000000; 1.000000;;
        1;
        4;0,0,0,0;;
      } // End of Plane normals
      MeshMaterialList { // Plane material list
        1;
        1;
        0;
        Material Material_001 {
           0.179662; 0.420578; 0.640000; 1.000000;;
           96.078431;
           0.500000; 0.500000; 0.500000;;
           0.000000; 0.000000; 0.000000;;
        }
      } // End of Plane material list
    } // End of Plane mesh
  } // End of Plane
} // End of Root
